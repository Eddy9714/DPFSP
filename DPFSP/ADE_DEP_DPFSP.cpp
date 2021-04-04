#pragma once
#include "ADE_DEP_DPFSP.h"

using namespace std;

ADE_DEP_DPFSP::ADE_DEP_DPFSP(string percorso) : istanza(Istanza(percorso)) {}

void ADE_DEP_DPFSP::creaPopolazione(Permutazione** popolazione, unsigned short nIndividui) {

	for (unsigned int k = 0; k < nIndividui; k++) {
		popolazione[k] = new PermutazioneI(istanza.lavori + istanza.fabbriche - 1);
	}
}

void ADE_DEP_DPFSP::inizializzaPopolazione(Permutazione** popolazione, unsigned short nIndividui, bool normalizzazione) {
	//NEH2(popolazione[0]);
	//popolazione[0]->score = valutaIndividuo(popolazione[0]);
	//if (normalizzazione) normalizza(popolazione[0]);

	for (unsigned short i = 0; i < nIndividui; i++) {
		popolazione[i]->random();
		popolazione[i]->score = valutaIndividuo(popolazione[i]);
		if(normalizzazione) normalizza(popolazione[i]);
	}
}

void ADE_DEP_DPFSP::selezionaPopolazione(Permutazione** popolazione, Permutazione** popolazioneAlternativa,
	unsigned short nIndividui, double theta, bool normalizzazione, bool* vettoreSuccessi) {

	double delta;

	for (unsigned short i = 0; i < nIndividui; i++) {

		popolazioneAlternativa[i]->score = valutaIndividuo(popolazioneAlternativa[i]);

		//Seleziona individui
		delta = ((int)popolazioneAlternativa[i]->score - (int)popolazione[i]->score) / (double)popolazione[i]->score;

		if (popolazioneAlternativa[i]->score < popolazione[i]->score ||
			(i != 0 && genRand.randDouble(0, 1) < max(theta - delta, 0.))) {

			popolazione[i]->scambia(popolazioneAlternativa[i]);
			if (normalizzazione) normalizza(popolazione[i]);

			vettoreSuccessi[i] = true;
		}
		else vettoreSuccessi[i] = false;

		scambiaFabbricheRandom(popolazione[i]);
	}
};

void ADE_DEP_DPFSP::NEH2(Permutazione* individuo) {
	unsigned int* p = new unsigned int[istanza.lavori];
	unsigned short* ordinamento = new unsigned short[istanza.lavori];

	//Sommiamo i pij, per i=1...lavori, j = 1...macchine
	for (unsigned short i = 0; i < istanza.lavori; i++) {
		p[i] = 0;
		ordinamento[i] = i;
		for (unsigned short j = 0; j < istanza.macchine; j++) {
			p[i] += istanza.p[i][j];
		}
	}

	//Ordiniamo i job in ordine decrescente (rispetto ai pi)
	sort(ordinamento, ordinamento + istanza.lavori, [p](unsigned short i, unsigned short j) {
		return p[i] > p[j];
	});

	//Inizializziamo l'individuo che contiene solo le fabbriche
	for (unsigned short f = 0; f < istanza.fabbriche - 1; f++) {
		individuo->individuo[f] = istanza.lavori + f;
	}
	unsigned short lunghezzaIndividuo = istanza.fabbriche - 1;

	//Inizializziamo gli indici delle fabbriche
	int* indiciFabbriche = new int[istanza.fabbriche * 2];
	for (unsigned int k = 0; k < istanza.fabbriche * 2; k += 2) {
		indiciFabbriche[k] = k/2;
		indiciFabbriche[k + 1] = k/2 - 1;
	}

	for (unsigned short i = 0; i < istanza.lavori; i++) {
		int fMigliore = -1;
		InfoInserzione migliore = {UINT32_MAX, 0};

		for (unsigned int f = 0; f < istanza.fabbriche * 2; f+=2) {
			//inserisci ordinamento[i] in ogni posizione della fabbrica ff
			InfoInserzione info = miglioreInserzione(&(individuo->individuo[indiciFabbriche[f]]), 
				indiciFabbriche[f+1] - indiciFabbriche[f] + 1, ordinamento[i]);

			//valuta se è stata trovata la migliore inserzione possibile fino ad ora ed eventualmente
			//registra fabbrica e posizione migliore
			if (info.makeSpan < migliore.makeSpan) {
				migliore = info;
				fMigliore = f;
			}
		}

		//Inserisci ordinamento[i] nella posizione migliore della fabbrica migliore mai trovata
		for (unsigned short i = lunghezzaIndividuo++; i > indiciFabbriche[fMigliore] + migliore.posizione; i--) {
			individuo->individuo[i] = individuo->individuo[i-1];
		}
		individuo->individuo[indiciFabbriche[fMigliore] + migliore.posizione] = ordinamento[i];

		//Aggiorna indici fabbriche
		for (unsigned int f = fMigliore + 1; f < istanza.fabbriche * 2; f++) {
			indiciFabbriche[f]++;
		}
	}
	
	delete[] p;
	delete[] ordinamento;
	delete[] indiciFabbriche;
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

void ADE_DEP_DPFSP::crossover(Permutazione* i1, Permutazione* i2) {

	PermutazioneI p1(i1->dimensione);
	PermutazioneI p2(i2->dimensione);

	subCrossover(i1, i2, &p1);
	subCrossover(i2, i1, &p2);

	p1.score = valutaIndividuo(&p1);
	p2.score = valutaIndividuo(&p2);

	if (p1.score >= p2.score) i2->scambia(&p1);
	else i2->scambia(&p2);
}

void ADE_DEP_DPFSP::subCrossover(Permutazione* i1, Permutazione* i2, Permutazione* ris) {

	bool* daEliminare = new bool[i1->dimensione];
	unsigned short* indiceTagli = new unsigned short[istanza.fabbriche * 2];

	for (unsigned short i = 0; i < i1->dimensione; i++) {
		daEliminare[i] = false; 
	}

	unsigned short f = 0;

	for (unsigned short i = 0; i <= i1->dimensione; i++) {

		if (i1->individuo[i] >= istanza.lavori || i == i1->dimensione) {

			if (f == 0) indiceTagli[0] = genRand.randIntU(0, i);
			else indiceTagli[f] = genRand.randIntU(indiceTagli[f - 1] + 1, i);

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

void ADE_DEP_DPFSP::ricercaLocale(Permutazione* p) {
	VNDEXC(p);
	VND(p, false);
	p->score = valutaIndividuo(p);
}

void ADE_DEP_DPFSP::ricercaLocaleRandomizzata(Permutazione** popolazione, unsigned short nIndividui) {
	
	//Scegli criterio per effettuare ricerca locale
	//Chiama VND sugli elementi selezionati
	unsigned short posizioni[1];
	indiciRandom->generaIndici(posizioni, 1);
	//VNDEXC(popolazione[posizioni[0]]);
	VND(popolazione[posizioni[0]], false);
}

void ADE_DEP_DPFSP::VNDEXC(Permutazione* p) {

	bool miglioramento = true;
	unsigned int* cFabbriche = new unsigned int[istanza.fabbriche];

	int* indiciFabbriche = new int[istanza.fabbriche * 2];

	indiciFabbriche[0] = 0;
	indiciFabbriche[istanza.fabbriche * 2 - 1] = p->dimensione - 1;

	unsigned short incremento = 0;

	for (int i = 0; i < p->dimensione; i++) {
		if (p->individuo[i] >= istanza.lavori) {
			indiciFabbriche[++incremento] = i - 1;
			indiciFabbriche[++incremento] = i + 1;
		}
	}

	for (unsigned int k = 0, i = 0; k < istanza.fabbriche * 2; k += 2, i++) {
		if (indiciFabbriche[k] < indiciFabbriche[k + 1]) {
			cFabbriche[i] = valutaIndividuoParziale(&(p->individuo[indiciFabbriche[k]]),
				indiciFabbriche[k + 1] - indiciFabbriche[k] + 1);
		}
		else cFabbriche[i] = 0;
	}

	unsigned int cPeggiore = 0, kPeggiore = 0, kMigliore, kSecondoPeggiore, cMigliore = UINT32_MAX;

	//Trova fabbrica Fy con Cmax peggiore
	for (unsigned int k = 0, i = 0; k < istanza.fabbriche * 2; k += 2, i++) {

		if (indiciFabbriche[k] < indiciFabbriche[k + 1]) {
			if (cFabbriche[i] > cPeggiore) {
				kSecondoPeggiore = kPeggiore;
				cPeggiore = cFabbriche[i];
				kPeggiore = k;
			}

			if (cFabbriche[i] < cMigliore) {
				cMigliore = cFabbriche[i];
				kMigliore = k;
			}
		}
		else cFabbriche[i] = 0;
	}

	while (miglioramento) {
		//Ricerca locale sul peggiore
		LS1EXC(p, kPeggiore, indiciFabbriche, cFabbriche);

		cPeggiore = 0, kPeggiore = 0, kMigliore, kSecondoPeggiore, cMigliore = UINT32_MAX;
		//Ricalcolo i peggiori, secondi peggiori, migliori
		for (unsigned int k = 0, i = 0; k < istanza.fabbriche * 2; k += 2, i++) {

			if (indiciFabbriche[k] < indiciFabbriche[k + 1]) {
				if (cFabbriche[i] > cPeggiore) {
					kSecondoPeggiore = kPeggiore;
					cPeggiore = cFabbriche[i];
					kPeggiore = k;
				}

				if (cFabbriche[i] < cMigliore) {
					cMigliore = cFabbriche[i];
					kMigliore = k;
				}
			}
			else cFabbriche[i] = 0;
		}

		//Prendi fabbrica migliore e peggiore
		//Scambia ciascun elemento della fabbrica migliore con quella peggiore
		//Se scambio effettuato, applica scambi locali su fabbrica migliore e peggiore
		
		//Effettua L2 su Fy
		int indiceFCambiata = -1;

		if (LS2EXC(p, kPeggiore, kMigliore, indiciFabbriche, cFabbriche)) {
			if (cFabbriche[kPeggiore / 2] < cFabbriche[kMigliore / 2]) {
				
				if (cFabbriche[kSecondoPeggiore / 2] < cFabbriche[kMigliore / 2]) {
					kPeggiore = kMigliore;
				}	
				else kPeggiore = kSecondoPeggiore;
			}
			else if (cFabbriche[kPeggiore / 2] < cFabbriche[kSecondoPeggiore / 2]) {
				kPeggiore = kSecondoPeggiore;
			}

		}
		else miglioramento = false;
	}

	delete[] indiciFabbriche;
	delete[] cFabbriche;
}

void ADE_DEP_DPFSP::LS1EXC(Permutazione* p, unsigned int iF, int* indiciFabbriche, unsigned int* cFabbriche) {
	
	unsigned short dimensioneFabbrica = indiciFabbriche[iF + 1] - indiciFabbriche[iF] + 1;

	unsigned short  posizioniDaSelezionare = min(dimensioneFabbrica, (unsigned short)genRand.randIntU(2, 50));

	unsigned short* indici = new unsigned short[posizioniDaSelezionare];
	
	IndiciRandom indRand(&genRand, dimensioneFabbrica);

	indRand.generaIndici(indici, posizioniDaSelezionare);

	bool miglioramento = false;
	unsigned short miglioreIndice1, miglioreIndice2, nuovoCosto, miglioreCosto = cFabbriche[iF / 2],
		indice1, indice2, tmp;

	for (unsigned short i = 0; i < posizioniDaSelezionare - 1; i++) {
		indice1 = indiciFabbriche[iF] + indici[i];

		for (unsigned short j = i + 1; j < posizioniDaSelezionare; j++) {
			indice2 = indiciFabbriche[iF] + indici[j];

			tmp = p->individuo[indice1];
			p->individuo[indice1] = p->individuo[indice2];
			p->individuo[indice2] = tmp;

			nuovoCosto = valutaIndividuoParziale(&p->individuo[indiciFabbriche[iF]], dimensioneFabbrica);
			if (nuovoCosto < miglioreCosto) {
				miglioreCosto = nuovoCosto;
				miglioreIndice1 = indice1;
				miglioreIndice2 = indice2;
				miglioramento = true;
			}

			tmp = p->individuo[indice1];
			p->individuo[indice1] = p->individuo[indice2];
			p->individuo[indice2] = tmp;
		}
	}

	if (miglioramento) {
		tmp = p->individuo[miglioreIndice1];
		p->individuo[miglioreIndice1] = p->individuo[miglioreIndice2];
		p->individuo[miglioreIndice2] = tmp;

		cFabbriche[iF / 2] = miglioreCosto;
	}

	delete[] indici;
}

bool ADE_DEP_DPFSP::LS2EXC(Permutazione* p, unsigned int iFPeggiore, unsigned int iFMigliore,
	int* indiciFabbriche, unsigned int* cFabbriche) {

	unsigned short dimensioneFabbricaPeggiore = indiciFabbriche[iFPeggiore + 1] - indiciFabbriche[iFPeggiore] + 1;
	unsigned short dimensioneFabbricaMigliore = indiciFabbriche[iFMigliore + 1] - indiciFabbriche[iFMigliore] + 1;

	bool miglioramento = false;
	unsigned short miglioreIndice1, miglioreIndice2, nuovoCosto1, nuovoCosto2, 
		posizioneFPeggioreCorrente, posizioneFMiglioreCorrente, tmp;

	unsigned int nuovoPunteggioPeggiore, nuovoPunteggioMigliore, guadagnoNET, migliorGuadagnoNET;

	unsigned short posizioniDaSelezionareP = min(dimensioneFabbricaPeggiore, (unsigned short)genRand.randIntU(2, 50));
	unsigned short posizioniDaSelezionareM = min(dimensioneFabbricaMigliore, (unsigned short)genRand.randIntU(2, 50));

	IndiciRandom iP(&genRand, dimensioneFabbricaPeggiore);
	IndiciRandom iM(&genRand, dimensioneFabbricaMigliore);

	unsigned short* indici1 = new unsigned short[posizioniDaSelezionareP];
	unsigned short* indici2 = new unsigned short[posizioniDaSelezionareM];
		
	iP.generaIndici(indici1, posizioniDaSelezionareP);
	iM.generaIndici(indici2, posizioniDaSelezionareM);
	
	for (unsigned short i = 0; i < posizioniDaSelezionareP; i++) {

		posizioneFPeggioreCorrente = indiciFabbriche[iFPeggiore] + indici1[i];

		for (unsigned short j = 0; j < posizioniDaSelezionareM; j++) {

			posizioneFMiglioreCorrente = indiciFabbriche[iFMigliore] + indici2[j];

			tmp = p->individuo[posizioneFPeggioreCorrente];
			p->individuo[posizioneFPeggioreCorrente] = p->individuo[posizioneFMiglioreCorrente];
			p->individuo[posizioneFMiglioreCorrente] = tmp;
			
			nuovoPunteggioPeggiore = valutaIndividuoParziale(&p->individuo[indiciFabbriche[iFPeggiore]], dimensioneFabbricaPeggiore);
			nuovoPunteggioMigliore = valutaIndividuoParziale(&p->individuo[indiciFabbriche[iFMigliore]], dimensioneFabbricaMigliore);

			guadagnoNET = max(nuovoPunteggioMigliore, nuovoPunteggioPeggiore);

			if (guadagnoNET < cFabbriche[iFPeggiore / 2]) {
					migliorGuadagnoNET = guadagnoNET;
					miglioreIndice1 = posizioneFPeggioreCorrente;
					miglioreIndice2 = posizioneFMiglioreCorrente;
					nuovoCosto1 = nuovoPunteggioPeggiore;
					nuovoCosto2 = nuovoPunteggioMigliore;
					miglioramento = true;
			}

			tmp = p->individuo[posizioneFPeggioreCorrente];
			p->individuo[posizioneFPeggioreCorrente] = p->individuo[posizioneFMiglioreCorrente];
			p->individuo[posizioneFMiglioreCorrente] = tmp;
		}
	}

	if (miglioramento) {
		tmp = p->individuo[miglioreIndice1];
		p->individuo[miglioreIndice1] = p->individuo[miglioreIndice2];
		p->individuo[miglioreIndice2] = tmp;

		cFabbriche[iFPeggiore / 2] = nuovoCosto1;
		cFabbriche[iFMigliore / 2] = nuovoCosto2;
	}

	delete[] indici1;
	delete[] indici2;

	return miglioramento;
}

void ADE_DEP_DPFSP::VND(Permutazione* p, bool A) {

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

		if ((indiceFCambiata = LS2(p, kPeggiore, indiciFabbriche, cFabbriche, A)) != -1) {
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
	unsigned int* cFabbriche, bool A) {

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

		if (A) {
			if (nuovoCY < cFabbriche[indiceFabbrica / 2]) {
				for (unsigned int k = 0; k < istanza.fabbriche * 2; k += 2) {
					if (k != indiceFabbrica) {
						InfoInserzione risultato = miglioreInserzione(&(p->individuo[indiciFabbriche[k]]),
							indiciFabbriche[k + 1] - indiciFabbriche[k] + 1, p->individuo[inizioFabbrica + i]);

						if ((int)risultato.makeSpan < cFabbriche[k / 2]) {
							migliorGuadagno = (int)risultato.makeSpan;
							posizioneFabbricaCambiata = k;
							posizioneLavoroRimosso = inizioFabbrica + i;
							posizioneLavoroInserito = indiciFabbriche[k] + risultato.posizione;
							nuovoCostoCx = risultato.makeSpan;
							nuovoCostoCy = nuovoCY;
						}
					}
				}
			}
		}
		else {
			for (unsigned int k = 0; k < istanza.fabbriche * 2; k += 2) {
				if (k != indiceFabbrica && (cFabbriche[k / 2] + istanza.minp[p->individuo[inizioFabbrica + i]] < cFabbriche[indiceFabbrica / 2])) {
					InfoInserzione risultato = miglioreInserzione(&(p->individuo[indiciFabbriche[k]]),
						indiciFabbriche[k + 1] - indiciFabbriche[k] + 1, p->individuo[inizioFabbrica + i]);

					//Cmax può peggiorare!
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

/*
void ADE_DEP_DPFSP::normalizza(Permutazione* p) {
	unsigned short contatore = istanza.lavori;
	for (unsigned short i = 0; i < p->dimensione; i++) {
		if (p->individuo[i] >= istanza.lavori) {
			p->individuo[i] = contatore++;
		}
	}
}
*/

void ADE_DEP_DPFSP::normalizza(Permutazione* p) {
	unsigned short* valoriMinimiF = new unsigned short[istanza.fabbriche];
	unsigned short* indiciFabbriche = new unsigned short[istanza.fabbriche * 2];

	indiciFabbriche[0] = 0;
	indiciFabbriche[istanza.fabbriche * 2 - 1] = p->dimensione - 1;

	unsigned short incremento = 0;

	unsigned short valoreMinimo = UINT16_MAX;
	unsigned short f = 0;

	for (unsigned short i = 0; i < p->dimensione; i++) {
		if (p->individuo[i] >= istanza.lavori) {
			valoriMinimiF[f++] = valoreMinimo;
			valoreMinimo = UINT16_MAX;
			indiciFabbriche[++incremento] = i - 1;
			indiciFabbriche[++incremento] = i + 1;
		}
		else {
			valoreMinimo = min(valoreMinimo, p->individuo[i]);
		}
	}
	valoriMinimiF[f] = valoreMinimo;

	bool cambiamento = false;
	for (unsigned short i = 0; i < istanza.fabbriche - 1; i++) {
		for (unsigned short j = i + 1; j < istanza.fabbriche; j++) {
			if (valoriMinimiF[i] > valoriMinimiF[j]) {

				cambiamento = true;

				unsigned short ind1 = 2 * i;
				unsigned short ind2 = 2 * j;

				//scambia indici fabbriche
				unsigned short tmp = indiciFabbriche[ind1];
				indiciFabbriche[ind1] = indiciFabbriche[ind2];
				indiciFabbriche[ind2] = tmp;

				ind1++, ind2++;
				tmp = indiciFabbriche[ind1];
				indiciFabbriche[ind1] = indiciFabbriche[ind2];
				indiciFabbriche[ind2] = tmp;

				//scambia indici minimi
				tmp = valoriMinimiF[i];
				valoriMinimiF[i] = valoriMinimiF[j];
				valoriMinimiF[j] = tmp;
			}
		}
	}

	if (cambiamento) {
		unsigned short* nuovoIndividuo = new unsigned short[istanza.lavori + istanza.fabbriche - 1];

		unsigned short posizione = 0;

		for (unsigned short i = 0; i < istanza.fabbriche; i++) {

			unsigned short dimensioneFabbrica = indiciFabbriche[2 * i + 1] - indiciFabbriche[2 * i] + 1;
			if (dimensioneFabbrica > 0) {
				memcpy(&nuovoIndividuo[posizione], &p->individuo[indiciFabbriche[2 * i]], 
					dimensioneFabbrica * sizeof(unsigned short));

				posizione += dimensioneFabbrica;
			}

			if(i!=istanza.fabbriche - 1)
				nuovoIndividuo[posizione++] = istanza.lavori + i;
		}

		delete[] p->individuo;
		p->individuo = nuovoIndividuo;
	}

	delete[] valoriMinimiF;
	delete[] indiciFabbriche;
}

void ADE_DEP_DPFSP::scambiaFabbricheRandom(Permutazione* p) {
	Coppia* indiciFabbriche = new Coppia[istanza.fabbriche];
	unsigned short* valoriFabbriche = new unsigned short[istanza.fabbriche - 1];

	indiciFabbriche[0] = {0, 0};
	indiciFabbriche[istanza.fabbriche - 1] = { 0, p->dimensione - 1 };

	unsigned short incremento = 0;

	for (unsigned short i = 0; i < p->dimensione; i++) {
		if (p->individuo[i] >= istanza.lavori) {
			valoriFabbriche[incremento] = p->individuo[i];

			indiciFabbriche[incremento].y = i - 1;
			indiciFabbriche[++incremento].x = i + 1;
		}
	}

	unsigned short* individuo = new unsigned short[p->dimensione];
	//Scambia fabbriche
	for (unsigned short k = istanza.fabbriche - 1; k > 0; k--) {
		unsigned short valoreRandom = genRand.randIntU(0, k);
		Coppia tmp = indiciFabbriche[k];
		indiciFabbriche[k] = indiciFabbriche[valoreRandom];
		indiciFabbriche[valoreRandom] = tmp;
	}

	//Copia valori fabbriche su individuo
	unsigned short posizione = 0;
	unsigned short dimensione = 0;
	incremento = 0;

	for (unsigned short i = 0; i < istanza.fabbriche; i++) {
		dimensione = indiciFabbriche[i].y - indiciFabbriche[i].x + 1;

		if (dimensione != 0) {
			memcpy(individuo + posizione, p->individuo + indiciFabbriche[i].x,
				sizeof(unsigned short) * dimensione);
			posizione += dimensione;
		}
		
		if(i != istanza.fabbriche - 1)
			individuo[posizione++] = valoriFabbriche[i];
	}

	delete[] p->individuo;
	p->individuo = individuo;

	delete[] indiciFabbriche;
	delete[] valoriFabbriche;
}

Permutazione ADE_DEP_DPFSP::esegui(unsigned short nIndividui, unsigned short scalaElaborazione, double theta,
	double Fmin, double Fmax, bool normalizzazione, unsigned int seed) {

	unsigned long long tempoDisponibileMs = scalaElaborazione * istanza.lavori * istanza.macchine;

	return ADE::esegui(nIndividui, tempoDisponibileMs, theta, Fmin, Fmax, normalizzazione, seed);
}

void ADE_DEP_DPFSP::stampa(Permutazione** popolazione, unsigned short nIndividui) {

	for (int i = 0; i < nIndividui; i++) {

		for (int j = 0; j < istanza.lavori + istanza.fabbriche - 1; j++) {
			cout << popolazione[i]->individuo[j] << " ";
		}

		cout << ":" << "\t" << valutaIndividuo(popolazione[i]) << endl;
	}

	cout << endl;
}