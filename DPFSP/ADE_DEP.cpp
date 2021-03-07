#pragma once
#include "ADE_DEP.h"
#include "IndiciRandom.h"

#include <iostream>

using namespace std;

unsigned int ADE_DEP::esegui(unsigned short nIndividui, unsigned short nGenerazioni) {
	
	// Crea e inizializza popolazione
	Permutazione** popolazione = creaPopolazione(nIndividui);
	Permutazione** popolazioneAlternativa = creaPopolazione(nIndividui);

	inizializzaPopolazione(popolazione, nIndividui);

	IndiciRandom r(nIndividui);
	unique_ptr<unsigned short[]> treIndici = unique_ptr<unsigned short[]>(new unsigned short[3]);

	for (unsigned short i = 0; i < nIndividui; i++) {
		popolazione[i]->score = valutaIndividuo(popolazione[i]);
	}

	for (unsigned short g = 0; g < nGenerazioni; g++) {

		cout << "Generazione: " << g << endl;
		cout << endl;

		stampa(popolazione, nIndividui);

		for (unsigned short i = 0; i < nIndividui; i++) {

			Random ran;
			double F = ran.randDouble(0.0, 1.0);

			r.generaIndici(treIndici, 3);

			Permutazione ris = *(popolazione[treIndici[0]]) + 
				((*(popolazione[treIndici[1]]) - *(popolazione[treIndici[2]])) * F);

			*(popolazioneAlternativa[i]) = ris;
			//Crossover

			//Valutazione nuovi individui
			popolazioneAlternativa[i]->score = valutaIndividuo(popolazioneAlternativa[i]);
		}

		for (unsigned short i = 0; i < nIndividui; i++) {
			//Seleziona individui
			if (popolazioneAlternativa[i]->score < popolazione[i]->score) {
				*(popolazione[i]) = *(popolazioneAlternativa[i]);
			}
		}

	}

	eliminaPopolazione(popolazione, nIndividui);
	eliminaPopolazione(popolazioneAlternativa, nIndividui);

	return 0;

}
