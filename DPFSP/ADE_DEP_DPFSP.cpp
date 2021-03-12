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