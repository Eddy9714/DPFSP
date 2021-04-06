#pragma once
#include "IndiciRandom.h"
#include "Globali.h"
#include <chrono>
#include <iostream>
#include <algorithm>

using namespace std;

template <class T> class ADE {

	protected:
		IndiciRandom* indiciRandom;
		unsigned int seed;

		virtual void creaPopolazione(T**, unsigned short) = 0;
		virtual void inizializzaPopolazione(T**, unsigned short, bool) = 0;
		virtual void crossover(T*, T*) = 0;
		virtual void normalizza(T*) = 0;
		virtual void selezionaPopolazione(T**, T**, unsigned short, double, bool, bool*) = 0;
		virtual void ricercaLocaleRandomizzata(T**, unsigned short) = 0;
		virtual void ricercaLocale(T*) = 0;
		virtual unsigned int valutaIndividuo(T*) = 0;
		virtual void stampa(T**, unsigned short) = 0;

		T esegui(unsigned short nIndividui, unsigned long long tempoDisponibileMs, double theta, double Fmin, 
			double Fmax, bool normalizzazione, unsigned int s) {

			using orologio = std::chrono::system_clock;
			auto tempoFinale = orologio::now() + chrono::milliseconds(tempoDisponibileMs);
			using sec = std::chrono::duration<double>;

			seed = s;
			if(seed > 0) genRand.impostaSeed(seed);

			T** popolazione = new T * [nIndividui];
			T** popolazioneAlternativa = new T * [nIndividui + 1];

			creaPopolazione(popolazione, nIndividui);

			//L'ultimo elemento è di appoggio
			creaPopolazione(popolazioneAlternativa, nIndividui + 1);

			double* vettoreF1 = new double[nIndividui];
			double* vettoreF2 = new double[nIndividui];

			bool* vettoreSuccessi = new bool[nIndividui];

			unsigned short h = 25;
			double* vettoreM1 = new double[h];
			double* vettoreM2 = new double[h];

			inizializzaPopolazione(popolazione, nIndividui, normalizzazione);

			//stampa(popolazione, nIndividui);

			indiciRandom = new IndiciRandom(&genRand, nIndividui);

			for (unsigned short i = 0; i < h; i++) {
				vettoreM1[i] = 0.5;
				vettoreM2[i] = 0.5;
			}

			unsigned short treIndici[3];

			unsigned int contatore = 0;
			sec tempoDisponibile = tempoFinale - orologio::now();

			unsigned short posizione = 0;

			while (true) {

				sort(popolazione, popolazione + nIndividui, [](T* p1, T* p2) -> bool {
					return p1->score < p2->score;
				});

				sec tempoRimasto = tempoFinale - orologio::now();
				auto count = tempoRimasto.count();
				if (count <= 0.)
					break;

				cout << "Tempo rimasto: " << (unsigned int)count << " secondi \t\r";
				//stampa(popolazione, nIndividui);

				unsigned short indiceRandom;
				double valoreRandom;

				for (unsigned short i = 0; i < nIndividui; i++) {

					indiceRandom = genRand.randIntU(0, h - 1);

					do {
						valoreRandom = genRand.cauchy(vettoreM1[indiceRandom], 0.1);
					} while (valoreRandom < 0);

					vettoreF1[i] = min(Fmax, valoreRandom);

					indiceRandom = genRand.randIntU(0, h - 1);

					do {
						valoreRandom = genRand.cauchy(vettoreM2[indiceRandom], 0.1);
					} while (valoreRandom < 0);
	
					vettoreF2[i] = min(Fmax, valoreRandom);
				}

				for (unsigned short i = 0; i < nIndividui; i++) {
					indiciRandom->generaIndici(treIndici, 3);

					unsigned short indiceMiglioreRand = genRand.randIntU(0, ceil(genRand.randDouble(0.05, 0.1) * nIndividui) - 1);

					*popolazioneAlternativa[i] = *(popolazione[treIndici[0]]);
					popolazioneAlternativa[i]->differenza(popolazione[treIndici[1]]);
					popolazioneAlternativa[i]->prodotto(vettoreF2[i]);

					*popolazioneAlternativa[i + 1] = *(popolazione[indiceMiglioreRand]);
					popolazioneAlternativa[i + 1]->differenza(popolazione[i]);
					popolazioneAlternativa[i + 1]->prodotto(vettoreF1[i]);

					popolazioneAlternativa[i]->somma(popolazioneAlternativa[i + 1]);
					popolazioneAlternativa[i]->somma(popolazione[i]);
				}

				unsigned short indiceMiglioreRand = genRand.randIntU(0, 0.1 * nIndividui - 1);
				ricercaLocale(popolazioneAlternativa[indiceMiglioreRand]);

				selezionaPopolazione(popolazione, popolazioneAlternativa, nIndividui, theta, normalizzazione, vettoreSuccessi);

				double sQ1 = 0, s1 = 0, sQ2 = 0, s2 = 0;

				unsigned short contatore = 0;
				for (unsigned short i = 0; i < nIndividui; i++) {
					if (vettoreSuccessi[i]) {
						sQ1 += pow(vettoreF1[i], 2);
						s1 += vettoreF1[i];
						sQ2 += pow(vettoreF2[i], 2);
						s2 += vettoreF2[i];
						contatore++;
					}
				}

				if (contatore != 0) {
					double mediaP = sQ1 / s1;
					vettoreM1[posizione] = mediaP;

					mediaP = sQ2 / s2;
					vettoreM2[posizione] = mediaP;

					posizione = (posizione + 1) % h;
				}
				
			}

			cout << endl << endl;

			//Troviamo l'individuo migliore
			unsigned int migliorPunteggioIniziale = UINT32_MAX;
			unsigned short migliore = 0;

			for (unsigned short i = 0; i < nIndividui; i++) {
				if (popolazione[i]->score < migliorPunteggioIniziale) {
					migliorPunteggioIniziale = popolazione[i]->score;
					migliore = i;
				}
			}

			ricercaLocale(popolazione[migliore]);

			stampa(popolazione, nIndividui);

			T migliorIndividuo = *(popolazione[migliore]);

			for (unsigned int i = 0; i < nIndividui; i++) {
				delete popolazione[i];
				delete popolazioneAlternativa[i];
			}
			delete popolazioneAlternativa[nIndividui];

			delete[] popolazione;
			delete[] popolazioneAlternativa;

			delete[] vettoreF1;
			delete[] vettoreF2;
			delete[] vettoreM1;
			delete[] vettoreM2;
			delete[] vettoreSuccessi;

			delete indiciRandom;

			return migliorIndividuo;
		};
};
