#pragma once
#include "IndiciRandom.h"

using namespace std;

template <class T> class ADE {

	protected:
		virtual void creaPopolazione(T**, unsigned short, unsigned long long) = 0;
		virtual void inizializzaPopolazione(T**, unsigned short, unsigned long long) = 0;
		virtual void crossover(T*, T*, unsigned long long) = 0;
		virtual void ricercaLocale(T**) = 0;
		virtual unsigned int valutaIndividuo(T*) = 0;
		virtual void stampa(T**, unsigned short) = 0;

		T esegui(unsigned short nIndividui, unsigned short nGenerazioni, double theta, double Fmin, double Fmax, unsigned long long seed) {

			T** popolazione = new T * [nIndividui];
			T** popolazioneAlternativa = new T * [nIndividui];

			creaPopolazione(popolazione, nIndividui, seed);
			creaPopolazione(popolazioneAlternativa, nIndividui, seed);

			double* vettoreF = new double[nIndividui];

			inizializzaPopolazione(popolazione, nIndividui, seed);

			IndiciRandom r(nIndividui);

			if (seed > 0) r.impostaSeed(seed + 19048475920238ULL);

			Random ran;

			if (seed > 0) ran.impostaSeed(seed + 841094662892ULL);

			for (unsigned short i = 0; i < nIndividui; i++) {
				vettoreF[i] = ran.randDouble(Fmin, Fmax);
			}

			unsigned short treIndici[3];


			for (unsigned short i = 0; i < nIndividui; i++) {
				popolazione[i]->score = valutaIndividuo(popolazione[i]);
			}

			for (unsigned short g = 0; g < nGenerazioni; g++) {

				if (g > 0) cout << "\r";
				cout << "Completamento: " << (int)(((g+1) / (double)nGenerazioni) * 100) << "%";

				//stampa(popolazione, nIndividui);

				for (unsigned short i = 0; i < nIndividui; i++) {

					r.generaIndici(treIndici, 3);

					if (seed > 0) {
						popolazione[treIndici[0]]->seed = max(1ULL, popolazione[treIndici[0]]->seed + g);
						popolazione[treIndici[1]]->seed = max(1ULL, popolazione[treIndici[1]]->seed + g);
						popolazione[treIndici[2]]->seed = max(1ULL, popolazione[treIndici[2]]->seed + g);
					}

					T temp = *(popolazione[treIndici[1]]);

					temp.differenza(popolazione[treIndici[2]]);
					temp.prodotto(vettoreF[i]);
					temp.somma(popolazione[treIndici[0]]);

					popolazioneAlternativa[i]->scambia(&temp);

					//Crossover
					crossover(popolazione[i], popolazioneAlternativa[i], seed);
					ricercaLocale(popolazioneAlternativa);

					//Valutazione nuovi individui
					if (popolazioneAlternativa[i]->score == 0)
						popolazioneAlternativa[i]->score = valutaIndividuo(popolazioneAlternativa[i]);
				}

				selezionaPopolazione(popolazione, popolazioneAlternativa, nIndividui, theta, seed);

				for (unsigned short i = 0; i < nIndividui; i++) {

					if (ran.randDouble(0, 1) < 0.1) {
						double pesoRandom = ran.randDouble(0, 1);
						vettoreF[i] = Fmin + pesoRandom * Fmax;
					}

				}

			}

			cout << endl << endl;

			unsigned int migliorPunteggio = UINT32_MAX;
			unsigned int indice = -1;

			for (unsigned int i = 0; i < nIndividui; i++) {

				if (migliorPunteggio > popolazione[i]->score) {
					migliorPunteggio = popolazione[i]->score;
					indice = i;
				}
					
			}

			T migliorIndividuo = *(popolazione[indice]);

			for (unsigned int i = 0; i < nIndividui; i++) {
				delete popolazione[i];
				delete popolazioneAlternativa[i];
			}

			delete[] popolazione;
			delete[] popolazioneAlternativa;

			delete[] vettoreF;

			return migliorIndividuo;
		};

		virtual void selezionaPopolazione(T** popolazione, T** popolazioneAlternativa,
			unsigned short nIndividui, double theta, unsigned long long seed) {

			Random r;

			if(seed > 0)
				r.impostaSeed(seed + 4958152750925591ULL);

			double delta;

			for (unsigned short i = 0; i < nIndividui; i++) {
				//Seleziona individui
				delta = (popolazioneAlternativa[i]->score - popolazione[i]->score) / popolazione[i]->score;

				if (popolazioneAlternativa[i]->score < popolazione[i]->score || r.randDouble(0, 1) < max(theta - delta, 0.)) {
					popolazione[i]->scambia(popolazioneAlternativa[i]);
				}
			}
		};
};
