#include "ADE_DEP_DPFSP.h"
#include <iostream>

using namespace std;

ADE_DEP_DPFSP::ADE_DEP_DPFSP(string percorso) : istanza(Istanza(percorso)) {}

void ADE_DEP_DPFSP::creaPopolazione(Permutazione** popolazione, unsigned short nIndividui, unsigned long long seed) {

	for (unsigned int k = 0; k < nIndividui; k++) {
		if(seed > 0)
			popolazione[k] = new Permutazione(istanza.lavori + istanza.fabbriche - 1, max(seed + 2ULL*k + 1746184ULL, 1ULL));
		else 
			popolazione[k] = new Permutazione(istanza.lavori + istanza.fabbriche - 1);
	}
}

void ADE_DEP_DPFSP::inizializzaPopolazione(Permutazione** popolazione, unsigned short nIndividui, unsigned long long seed) {

	Random r;

	if (seed > 0) r.impostaSeed(seed + 7483984741ULL);

	for (int i = 0; i < nIndividui; i++) {

		for (int j = 0; j < istanza.lavori + istanza.fabbriche - 1; j++) {
			popolazione[i]->individuo[j] = j;
		}

		for (int k = istanza.lavori + istanza.fabbriche - 2; k > 0; k--) {
			int valoreRandom = r.randIntU(0, k);

			unsigned short tmp = popolazione[i]->individuo[valoreRandom];
			popolazione[i]->individuo[valoreRandom] = popolazione[i]->individuo[k];
			popolazione[i]->individuo[k] = tmp;
		}

		VND(popolazione[i]);
	}
}

unsigned int ADE_DEP_DPFSP::valutaIndividuo(Permutazione* p) {

	auto Cm = make_unique<unsigned int[]>(istanza.macchine);

	unsigned int Cmax = 0;
	short c = 0;

	for (unsigned short k = 0; k < istanza.lavori + istanza.fabbriche - 1; k++, c++) {

		if (p->individuo[k] >= istanza.lavori) {
			Cmax = max(Cmax, Cm[istanza.macchine - 1]);
			c = -1;
		}
		else {
			for (unsigned short j = 0; j < istanza.macchine; j++) {

				if (c == 0 && j == 0) {
					Cm[j] = istanza.p[p->individuo[k]][0];
				}
				else if (c != 0 && j != 0) {
					Cm[j] = istanza.p[p->individuo[k]][j] + max(Cm[j - 1], Cm[j]);
				}
				else if (c == 0 && j != 0) {
					Cm[j] = istanza.p[p->individuo[k]][j] + Cm[j - 1];
				}
				else {
					Cm[0] = istanza.p[p->individuo[k]][0] + Cm[0];
				}
			}
		}
	}

	return max(Cmax, Cm[istanza.macchine - 1]);
}

void ADE_DEP_DPFSP::crossover(Permutazione* i1, Permutazione* i2, unsigned long long seed) {

	Permutazione p1(i1->dimensione, i1->seed);
	Permutazione p2(i2->dimensione, i2->seed);

	subCrossover(i1, i2, &p1);
	subCrossover(i2, i1, &p2);

	p1.score = valutaIndividuo(&p1);
	p2.score = valutaIndividuo(&p2);

	if (p1.score >= p2.score) i2->scambia(&p1);
	else i2->scambia(&p2);
}

void ADE_DEP_DPFSP::subCrossover(Permutazione* i1, Permutazione* i2, Permutazione* ris) {

	Random r;
	if (ris->seed > 0) r.impostaSeed(ris->seed + 9471032902612ULL);

	bool* daEliminare = new bool[i1->dimensione];
	unsigned short* indiceTagli = new unsigned short[istanza.fabbriche * 2];

	for (unsigned short i = 0; i < i1->dimensione; i++) {
		daEliminare[i] = false; 
	}

	unsigned short f = 0;

	for (unsigned short i = 0; i <= i1->dimensione; i++) {

		if (i1->individuo[i] >= istanza.lavori || i == i1->dimensione) {

			if (f == 0) indiceTagli[0] = r.randIntU(0, i);
			else indiceTagli[f] = r.randIntU(indiceTagli[f - 1] + 1, i);

			indiceTagli[f + 1] = i;

			for (unsigned short k = indiceTagli[f]; k < indiceTagli[f + 1]; k++) {
				daEliminare[i1->individuo[k]] = true;
			}

			f += 2;
		}
	}

	unsigned short scostamento = 0;

	f = 0;

	for (unsigned short i = 0; i <= i2->dimensione; i++) {

		if (i2->individuo[i] >= istanza.lavori || i == i2->dimensione) {
			unsigned short lunghezzaSegmento = indiceTagli[f + 1] - indiceTagli[f];
			if (lunghezzaSegmento > 0) {
				memcpy(&(ris->individuo[scostamento]), &(i1->individuo[indiceTagli[f]]),
					lunghezzaSegmento*sizeof(unsigned short));
				scostamento += lunghezzaSegmento;
			}

			if(i != i2->dimensione) ris->individuo[scostamento++] = i2->individuo[i];

			f += 2;
		}
		else if (!daEliminare[i2->individuo[i]]) {
			ris->individuo[scostamento++] = i2->individuo[i];
		}
	}

	delete[] daEliminare;
	delete[] indiceTagli;
}

void ADE_DEP_DPFSP::ricercaLocale(Permutazione** popolazione, unsigned short nIndividui) {
	
	//Scegli criterio per effettuare ricerca locale
	//Chiama VND sugli elementi selezionati

	for (unsigned short i = 0; i < nIndividui; i++) {
		this->VND(popolazione[i]);
	}
}

void ADE_DEP_DPFSP::VND(Permutazione* p) {

	bool miglioramento = true;
	bool* ricercaNecessaria = new bool[istanza.fabbriche];
	unsigned int* cFabbriche = new unsigned int[istanza.fabbriche];

	int* indiciFabbriche = new int[istanza.fabbriche * 2];

	for (unsigned short f = 0; f < istanza.fabbriche; f++) {
		ricercaNecessaria[f] = true;
	}

	indiciFabbriche[0] = 0;
	indiciFabbriche[istanza.fabbriche * 2 - 1] = p->dimensione - 1;

	unsigned short incremento = 0;

	for (int i = 0; i < p->dimensione; i++) {
		if (p->individuo[i] >= istanza.lavori) {
			indiciFabbriche[++incremento] = i - 1;
			indiciFabbriche[++incremento] = i + 1;
		}
	}

	while (miglioramento) {
		for (unsigned short f = 0; f < istanza.fabbriche; f++) {
			if (ricercaNecessaria[f]) {

				int inizioFabbrica = indiciFabbriche[2 * f];
				int fineFabbrica = indiciFabbriche[2 * f + 1];

				if (inizioFabbrica < fineFabbrica)
					// Effettua ricerca LS1 su fabbrica Ff
					LS1(p, inizioFabbrica, fineFabbrica);
				ricercaNecessaria[f] = false;
			}
		}

		//Trova fabbrica Fy con Cmax peggiore
		unsigned int cPeggiore = 0;
		unsigned int kPeggiore = 0;

		for (unsigned int k = 0, i = 0; k < istanza.fabbriche * 2; k += 2, i++) {

			if (indiciFabbriche[k] < indiciFabbriche[k + 1]) {
				cFabbriche[i] = valutaIndividuoParziale(&(p->individuo[indiciFabbriche[k]]), 
					indiciFabbriche[k + 1] - indiciFabbriche[k] + 1);
				if (cFabbriche[i] > cPeggiore) {
					cPeggiore = cFabbriche[i];
					kPeggiore = k;
				}
			}
			else cFabbriche[i] = 0;
		}

		//Effettua L2 su Fy
		int indiceFCambiata = -1;

		if ((indiceFCambiata = LS2(p, kPeggiore, indiciFabbriche, cFabbriche)) != -1) {
			//Se cambiamento (Fx, Fy cambiati) effettua L1 su Fx, Fy
			ricercaNecessaria[indiceFCambiata] = true;
			ricercaNecessaria[kPeggiore / 2] = true;
		}
		else miglioramento = false;
	}

	delete[] indiciFabbriche;
	delete[] ricercaNecessaria;
	delete[] cFabbriche;
}

void ADE_DEP_DPFSP::LS1(Permutazione* p, unsigned short inizioFabbrica, unsigned short fineFabbrica) {
	bool miglioramento;
	
	unsigned short nLavoriFabbrica = fineFabbrica - inizioFabbrica + 1;

	unsigned short* lavoriFabbrica = new unsigned short[nLavoriFabbrica - 1];

	unsigned int migliorPunteggio = valutaIndividuoParziale(&(p->individuo[inizioFabbrica]), nLavoriFabbrica);

	do {
		miglioramento = false;

		memcpy(lavoriFabbrica, &(p->individuo[inizioFabbrica + 1]), (nLavoriFabbrica - 1) * sizeof(unsigned short));

		for (unsigned short i = 0; i < nLavoriFabbrica; i++) {
			if (i != 0) {
				lavoriFabbrica[i-1] = p->individuo[inizioFabbrica + i - 1];
			}

			InfoInserzione risultato = miglioreInserzione(lavoriFabbrica, nLavoriFabbrica-1, 
				p->individuo[inizioFabbrica + i]);

			if (risultato.makeSpan < migliorPunteggio) {

				migliorPunteggio = risultato.makeSpan;
				miglioramento = true;

				//aggiorna permutazione spostando solo gli elementi toccati
				unsigned short posizionePrecedente = inizioFabbrica + i;
				unsigned short nuovaPosizione = inizioFabbrica + risultato.posizione;


				unsigned short tmp = p->individuo[posizionePrecedente];

				if (nuovaPosizione > posizionePrecedente) {
					for (unsigned short k = posizionePrecedente; k < nuovaPosizione; k++) {
						p->individuo[k] = p->individuo[k + 1];
					}
				}
				else {
					for (unsigned short k = posizionePrecedente; k > nuovaPosizione; k--) {
						p->individuo[k] = p->individuo[k - 1];
					}
				}

				p->individuo[nuovaPosizione] = tmp;
				break;
			}		
		}

	} while (miglioramento);

	delete[] lavoriFabbrica;
}

int ADE_DEP_DPFSP::LS2(Permutazione* p, unsigned int indiceFabbrica, int* indiciFabbriche,
	unsigned int* cFabbriche) {

	unsigned short inizioFabbrica = indiciFabbriche[indiceFabbrica];
	unsigned short fineFabbrica = indiciFabbriche[indiceFabbrica + 1];
	unsigned short nLavoriFabbrica = fineFabbrica - inizioFabbrica + 1;

	//Non bisogna istanziarlo se si ha un solo lavoro
	unsigned short* lavoriFabbrica;

	if (nLavoriFabbrica > 1) {
		lavoriFabbrica = new unsigned short[nLavoriFabbrica - 1];
		memcpy(lavoriFabbrica, &(p->individuo[inizioFabbrica + 1]), (nLavoriFabbrica - 1) * sizeof(unsigned short));
	}

	int migliorGuadagno = -1;
	unsigned int posizioneFabbricaCambiata = 0;
	unsigned short posizioneLavoroRimosso = 0;
	unsigned short posizioneLavoroInserito = 0; 
	unsigned int nuovoCostoCy = 0;
	unsigned int nuovoCostoCx = 0;

	//cambiare gli indici dove necessario
	for (unsigned short i = 0; i < nLavoriFabbrica; i++) {

		// rimuovi il lavoro k in posizione i
		if (i != 0) {
			lavoriFabbrica[i - 1] = p->individuo[inizioFabbrica + i - 1];
		}

		//valuta nuovo individuo
		unsigned int nuovoCY = valutaIndividuoParziale(lavoriFabbrica, nLavoriFabbrica - 1);

		// prova k in ogni posizione possibile dalle altre fabbriche
		for (unsigned int k = 0; k < istanza.fabbriche * 2; k+=2) {
			if (k != indiceFabbrica) {
				InfoInserzione risultato = miglioreInserzione(&(p->individuo[indiciFabbriche[k]]),
					indiciFabbriche[k + 1] - indiciFabbriche[k] + 1, p->individuo[inizioFabbrica + i]);

				int guadagno = cFabbriche[indiceFabbrica / 2] - nuovoCY + cFabbriche[k / 2] - (int)risultato.makeSpan;

				if (guadagno > 0 && guadagno > migliorGuadagno) {
					migliorGuadagno = guadagno;
					posizioneFabbricaCambiata = k;
					posizioneLavoroRimosso = inizioFabbrica + i;
					posizioneLavoroInserito = indiciFabbriche[k] + risultato.posizione;
					nuovoCostoCx = risultato.makeSpan;
					nuovoCostoCy = nuovoCY;
				}
			}
		}
	}

	//Se c'è stato un miglioramento, aggiorniamo la permutazione 
	if (migliorGuadagno != -1) {

		if (posizioneLavoroRimosso < posizioneLavoroInserito) {
			unsigned short lavoroRimosso = p->individuo[posizioneLavoroRimosso];

			for (unsigned short i = posizioneLavoroRimosso; i < posizioneLavoroInserito - 1; i++) {
				p->individuo[i] = p->individuo[i + 1];
			}

			p->individuo[posizioneLavoroInserito - 1] = lavoroRimosso;


			//Aggiorniamo gli indici delle fabbriche, scalando di 1 le posizioni
			for (unsigned int k = indiceFabbrica + 1; k <= posizioneFabbricaCambiata; k++) {
				indiciFabbriche[k]--;
			}

		}
		else {
			unsigned short lavoroRimosso = p->individuo[posizioneLavoroRimosso];

			for (unsigned short i = posizioneLavoroRimosso; i > posizioneLavoroInserito; i--) {
				p->individuo[i] = p->individuo[i - 1];
			}

			//Aggiorniamo gli indici delle fabbriche, scalando di 1 le posizioni
			for (unsigned int k = posizioneFabbricaCambiata + 1; k <= indiceFabbrica; k++) {
				indiciFabbriche[k]++;
			}

			p->individuo[posizioneLavoroInserito] = lavoroRimosso;
		}

		//Aggiorniamo i costi delle due fabbriche cambiate
		cFabbriche[indiceFabbrica / 2] = nuovoCostoCy;
		cFabbriche[posizioneFabbricaCambiata / 2] = nuovoCostoCx;
	}

	if(nLavoriFabbrica > 1) delete[] lavoriFabbrica;

	return migliorGuadagno != -1 ? posizioneFabbricaCambiata / 2 : -1;
}

ADE_DEP_DPFSP::InfoInserzione ADE_DEP_DPFSP::miglioreInserzione(unsigned short* vettoreFabbrica, 
	unsigned short dimensioneVettoreFabbrica,
	unsigned short lavoroDaInserire) {

	if (dimensioneVettoreFabbrica == 0) {
		unsigned short fabbrica[1] = { lavoroDaInserire };
		return { valutaIndividuoParziale(fabbrica, 1) , 0};
	}
		
	//Accelerazione di Taillard, guardare l'articolo relativo
	unsigned int* e = new unsigned int[istanza.macchine];
	unsigned int* f = new unsigned int[istanza.macchine];
	unsigned int** q = new unsigned int* [istanza.lavori];

	unsigned int miglioreMakespan = UINT32_MAX;
	unsigned short migliorePosizione = 0;

	for (int i = dimensioneVettoreFabbrica - 1; i >= 0; i--) {
		q[i] = new unsigned int[istanza.macchine];

		for (int j = istanza.macchine - 1; j >= 0; j--) {

			if (i != dimensioneVettoreFabbrica - 1 && j != istanza.macchine - 1) {
				q[i][j] = max(q[i][j + 1], q[i + 1][j]) + istanza.p[vettoreFabbrica[i]][j];
			}
			else if (i == dimensioneVettoreFabbrica - 1 && j != istanza.macchine - 1) {
				q[i][j] = q[i][j + 1] + istanza.p[vettoreFabbrica[i]][j];
			}
			else if (i != dimensioneVettoreFabbrica - 1 && j == istanza.macchine - 1) {
				q[i][j] = q[i + 1][j] + istanza.p[vettoreFabbrica[i]][j];
			}
			else {
				q[i][j] = istanza.p[vettoreFabbrica[i]][j];
			}	
		}
	}

	for (unsigned short i = 0; i <= dimensioneVettoreFabbrica; i++) {

		for (unsigned short j = 0; j < istanza.macchine; j++) {

			if (i == 0 && j == 0) {
				f[0] = istanza.p[lavoroDaInserire][0];
				e[0] = istanza.p[vettoreFabbrica[0]][0];
			}
			else if (i != 0 && j == 0) {
				f[0] = e[0] + istanza.p[lavoroDaInserire][0];

				if (i != dimensioneVettoreFabbrica)
					e[0] = e[0] + istanza.p[vettoreFabbrica[i]][0];
			}
			else if (i == 0 && j != 0) {
				f[j] = f[j - 1] + istanza.p[lavoroDaInserire][j];
				e[j] = e[j - 1] + istanza.p[vettoreFabbrica[0]][j];
			}
			else {
				f[j] = max(f[j - 1], e[j]) + istanza.p[lavoroDaInserire][j];
				if (i != dimensioneVettoreFabbrica)
					e[j] = max(e[j - 1], e[j]) + istanza.p[vettoreFabbrica[i]][j];
			}
		}

		unsigned int makeSpanParziale = 0;

		for (unsigned short j = 0; j < istanza.macchine; j++) {

			if (i != dimensioneVettoreFabbrica)
				makeSpanParziale = max(makeSpanParziale, f[j] + q[i][j]);
			else
				makeSpanParziale = max(makeSpanParziale, f[j]);
		}

		if (makeSpanParziale < miglioreMakespan) {
			miglioreMakespan = makeSpanParziale;
			migliorePosizione = i;
		}

	}

	delete[] e;
	delete[] f;

	for (unsigned short i = 0; i < dimensioneVettoreFabbrica; i++) {
		delete[] q[i];
	}

	delete[] q;

	return { miglioreMakespan, migliorePosizione };
}

unsigned int ADE_DEP_DPFSP::valutaIndividuoParziale(unsigned short* fabbrica, unsigned short lunghezza) {
	if (lunghezza == 0) return 0;

	unsigned int* c = new unsigned int[istanza.macchine];

	for (unsigned short i = 0; i < lunghezza ; i++) {

		for (unsigned short j = 0; j < istanza.macchine; j++) {

			if (i == 0 && j == 0) {
				c[j] = istanza.p[fabbrica[i]][0];
			}
			else if (i != 0 && j != 0) {
				c[j] = istanza.p[fabbrica[i]][j] + max(c[j - 1], c[j]);
			}
			else if (i == 0 && j != 0) {
				c[j] = istanza.p[fabbrica[i]][j] + c[j - 1];
			}
			else {
				c[0] = istanza.p[fabbrica[i]][0] + c[0];
			}
		}
	}

	unsigned int makeSpan = c[istanza.macchine - 1];

	delete[] c;

	return makeSpan;
;}

Permutazione ADE_DEP_DPFSP::esegui(unsigned short nIndividui, unsigned short nGenerazioni, double theta, 
	double Fmin, double Fmax, unsigned long long seed) {
	return ADE::esegui(nIndividui, nGenerazioni, theta, Fmin, Fmax, seed);
}

void ADE_DEP_DPFSP::stampa(Permutazione** popolazione, unsigned short nIndividui) {

	for (int i = 0; i < nIndividui; i++) {

		for (int j = 0; j < istanza.lavori + istanza.fabbriche - 1; j++) {
			cout << popolazione[i]->individuo[j] << " ";
		}

		cout << ":" << "\t" << popolazione[i]->score << endl;
	}

	cout << endl;
}