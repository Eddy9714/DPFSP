#pragma once
#include "IndiciRandom.h"
#include "Permutazione.h"
#include <memory>

using namespace std;

template <class T> class ADE {

	protected:
		virtual void creaPopolazione(T**, unsigned short) = 0;
		virtual void inizializzaPopolazione(T**, unsigned short) = 0;
		virtual void crossover(T*, T*) = 0;
		virtual unsigned int valutaIndividuo(T*) = 0;
		virtual void stampa(T**, unsigned short) = 0;

		unsigned int esegui(unsigned short nIndividui, unsigned short nGenerazioni, double theta, double Fmin, double Fmax) {

			T** popolazione = new T * [nIndividui];
			T** popolazioneAlternativa = new T * [nIndividui];

			creaPopolazione(popolazione, nIndividui);
			creaPopolazione(popolazioneAlternativa, nIndividui);

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

				//stampa(popolazione, nIndividui);


				for (unsigned short i = 0; i < nIndividui; i++) {

					r.generaIndici(treIndici, 3);


					*(popolazioneAlternativa[i]) = *(popolazione[treIndici[0]]) +
						((*(popolazione[treIndici[1]]) - *(popolazione[treIndici[2]])) * vettoreF[i]);

					//Crossover
					crossover(popolazione[i], popolazioneAlternativa[i]);

					//Valutazione nuovi individui
					if (popolazioneAlternativa[i]->score == 0)
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

			unsigned int migliorPunteggio = UINT32_MAX;

			for (unsigned int i = 0; i < nIndividui; i++) {
				migliorPunteggio = min(migliorPunteggio, popolazione[i]->score);
			}

			for (unsigned int i = 0; i < nIndividui; i++) {
				delete popolazione[i];
				delete popolazioneAlternativa[i];
			}

			delete[] popolazione;
			delete[] popolazioneAlternativa;

			delete[] vettoreF;

			return migliorPunteggio;
		};

		virtual void selezionaPopolazione(T** popolazione, T** popolazioneAlternativa,
			unsigned short nIndividui, double theta) {

			Random r;
			double delta;

			for (unsigned short i = 0; i < nIndividui; i++) {
				//Seleziona individui
				delta = (popolazioneAlternativa[i]->score - popolazione[i]->score) / popolazione[i]->score;

				if (popolazioneAlternativa[i]->score < popolazione[i]->score || r.randDouble(0, 1) < max(theta - delta, 0.)) {
					T* tmp = popolazione[i];
					popolazione[i] = popolazioneAlternativa[i];
					popolazioneAlternativa[i] = tmp;
				}
			}
		};
};