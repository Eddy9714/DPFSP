#pragma once
#include "ADE_DEP.h"
#include "IndiciRandom.h"

#include <iostream>

using namespace std;

unsigned int ADE_DEP::esegui(unsigned short nIndividui, unsigned short nGenerazioni, double theta, double Fmin, double Fmax) {
	
	// Crea e inizializza popolazione
	Permutazione** popolazione = creaPopolazione(nIndividui);
	Permutazione** popolazioneAlternativa = creaPopolazione(nIndividui);

	double* vettoreF = new double[nIndividui];

	inizializzaPopolazione(popolazione, nIndividui);

	IndiciRandom r(nIndividui);
	Random ran;

	for (unsigned short i = 0; i < nIndividui; i++) {
		vettoreF[i] = ran.randDouble(Fmin, Fmax);
	}

	unsigned short treIndici[3];

	
	for (unsigned short i = 0; i < nIndividui; i++) {
		popolazione[i]->score = valutaIndividuo(popolazione[i]);
	}

	for (unsigned short g = 0; g < nGenerazioni; g++) {

		cout << "Generazione: " << g << endl;
		cout << endl;

		stampa(popolazione, nIndividui);

		for (unsigned short i = 0; i < nIndividui; i++) {

			r.generaIndici(treIndici, 3);

			
			Permutazione ris = *(popolazione[treIndici[0]]) + 
				((*(popolazione[treIndici[1]]) - *(popolazione[treIndici[2]])) * vettoreF[i]);

			*(popolazioneAlternativa[i]) = ris;
			//Crossover
			crossover(popolazione[i], popolazioneAlternativa[i]);

			//Valutazione nuovi individui
			if(popolazioneAlternativa[i]->score == 0)
				popolazioneAlternativa[i]->score = valutaIndividuo(popolazioneAlternativa[i]);			
		}

		selezionaPopolazione(popolazione, popolazioneAlternativa, nIndividui, theta);

		for (unsigned short i = 0; i < nIndividui; i++) {

			if (ran.randDouble(0, 1) < 0.1) {
				double pesoRandom = ran.randDouble(0, 1);
				vettoreF[i] = Fmin + pesoRandom * Fmax;
			}

		}
	}

	eliminaPopolazione(popolazione, nIndividui);
	eliminaPopolazione(popolazioneAlternativa, nIndividui);

	delete[] vettoreF;

	return 0;
}

void ADE_DEP::crossover(Permutazione* pi, Permutazione* vi) {
	Permutazione p1(pi->dimensione);
	Permutazione p2(pi->dimensione);

	unsigned short j, k, jmin, kmax, cursore1, cursore2 = 0;

	Random r;

	bool* posizioni1 = new bool[pi->dimensione];
	bool* posizioni2 = new bool[pi->dimensione];

	j = r.randIntU(0, pi->dimensione - 1);
	k = r.randIntU(0, pi->dimensione - 1);

	jmin = min(j, k);
	kmax = max(j, k);

	if (j != k) {

		for (unsigned short s = 0; s < pi->dimensione; s++) {
			posizioni1[s] = false;
			posizioni2[s] = false;
		}

		for (unsigned short s = jmin; s <= kmax; s++) {
			p1.individuo[s] = pi->individuo[s];
			posizioni1[p1.individuo[s]] = true;

			p2.individuo[s] = vi->individuo[s];
			posizioni2[p2.individuo[s]] = true;
		}

		for (unsigned short s = 0, cursore1 = 0, cursore2=0; s < pi->dimensione; s++) {

			if (cursore1 == jmin) cursore1 = kmax + 1;
			if (cursore2 == jmin) cursore2 = kmax + 1;

			if (!posizioni1[vi->individuo[s]]){
				p1.individuo[cursore1] = vi->individuo[s];
				cursore1++;
			}

			if (!posizioni2[pi->individuo[s]]){
				p2.individuo[cursore2] = pi->individuo[s];
				cursore2++;
			}
				
		}

		p1.score = valutaIndividuo(&p1);
		p2.score = valutaIndividuo(&p2);

		if (p1.score > p2.score) *(vi) = p1;
		else *(vi) = p2;
	}
	
	delete[] posizioni1;
	delete[] posizioni2;
}

void ADE_DEP::selezionaPopolazione(Permutazione** popolazione, Permutazione** popolazioneAlternativa, 
	unsigned short nIndividui, double theta) {

	Random r;
	double delta;
	
	for (unsigned short i = 0; i < nIndividui; i++) {
		//Seleziona individui
		delta = (popolazioneAlternativa[i]->score - popolazione[i]->score) / popolazione[i]->score;

		if (popolazioneAlternativa[i]->score < popolazione[i]->score || r.randDouble(0,1) < max(theta - delta, 0.)) {
			Permutazione* tmp = popolazione[i];
			popolazione[i] = popolazioneAlternativa[i];
			popolazioneAlternativa[i] = tmp;
		}
	}
}
