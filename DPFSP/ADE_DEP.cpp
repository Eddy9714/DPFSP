#include "ADE_DEP.h"
#include "Permutazione.h"
#include "Random.h"

#include <memory>

#include <iostream>

using namespace std;

ADE_DEP::ADE_DEP(Istanza* i) : ADE::ADE() {
	istanza = i;
}

void ADE_DEP::inizializzaPopolazione(Individuo** popolazione, unsigned short nIndividui) {
	for (int i = 0; i < nIndividui; i++) {

		for (int j = 0; j < istanza->lavori + istanza->fabbriche - 1; j++) {
			popolazione[i]->individuo[j] = j;
		}

		unique_ptr<Random> r(new Random());

		for (int k = istanza->lavori + istanza->fabbriche - 2; k > 0; k--) {
			int valoreRandom = r->randIntU(0, k);

			unsigned short tmp = popolazione[i]->individuo[valoreRandom];
			popolazione[i]->individuo[valoreRandom] = popolazione[i]->individuo[k];
			popolazione[i]->individuo[k] = tmp;
		}

		for (int j = 0; j < istanza->lavori + istanza->fabbriche - 1; j++) {
			cout << popolazione[i]->individuo[j] << "\t";
		}


		cout << ":" << "\t" << popolazione[i]->makeSpan() << endl;
	}
}

unsigned int valutaIndividuo(Individuo* i) {
	Permutazione* p = (Permutazione*)i;
	return 0;
}

unsigned int ADE_DEP::esegui(unsigned short nIndividui, unsigned short nGenerazioni) {
	return ADE::esegui(nIndividui, nGenerazioni);
}
