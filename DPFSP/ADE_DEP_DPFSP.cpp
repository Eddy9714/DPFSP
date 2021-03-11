#include "ADE_DEP_DPFSP.h"
#include <iostream>

using namespace std;

ADE_DEP_DPFSP::ADE_DEP_DPFSP(string percorso) : istanza(Istanza(percorso)) {}

void ADE_DEP_DPFSP::creaPopolazione(Permutazione** popolazione, unsigned short nIndividui) {

	for (unsigned int k = 0; k < nIndividui; k++) {
		popolazione[k] = new Permutazione(istanza.lavori + istanza.fabbriche - 1);
	}
}

void ADE_DEP_DPFSP::inizializzaPopolazione(Permutazione** popolazione, unsigned short nIndividui) {

	Random r;

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

void ADE_DEP_DPFSP::crossover(Permutazione* i1, Permutazione* i2) {

}

unsigned int ADE_DEP_DPFSP::esegui(unsigned short nIndividui, unsigned short nGenerazioni, double theta, 
	double Fmin, double Fmax) {
	return ADE::esegui(nIndividui, nGenerazioni, theta, Fmin, Fmax);
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
