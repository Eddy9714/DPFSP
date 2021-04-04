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

			double* vettoreF = new double[nIndividui];
			bool* vettoreSuccessi = new bool[nIndividui];

			unsigned short h = nIndividui;
			double* vettoreM = new double[h];

			inizializzaPopolazione(popolazione, nIndividui, normalizzazione);

			//stampa(popolazione, nIndividui);

			indiciRandom = new IndiciRandom(&genRand, nIndividui);

			for (unsigned short i = 0; i < h; i++) {
				vettoreM[i] = 0.5;
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
						valoreRandom = genRand.cauchy(vettoreM[indiceRandom], 0.1);
					} while (valoreRandom < 0);
	
					vettoreF[i] = min(Fmax, valoreRandom);
				}

				for (unsigned short i = 0; i < nIndividui; i++) {
					indiciRandom->generaIndici(treIndici, 3);

					unsigned short indiceMiglioreRand = genRand.randIntU(0, ceil(genRand.randDouble(0.05, 0.2) * nIndividui) - 1);

					*popolazioneAlternativa[i] = *(popolazione[indiceMiglioreRand]);
					popolazioneAlternativa[i]->differenza(popolazione[treIndici[1]]);
					popolazioneAlternativa[i]->prodotto(vettoreF[i]);

					//*popolazioneAlternativa[i + 1] = *(popolazione[indiceMiglioreRand]);
					//popolazioneAlternativa[i + 1]->differenza(popolazione[i]);
					//popolazioneAlternativa[i + 1]->prodotto(vettoreF[i]);

					//popolazioneAlternativa[i]->somma(popolazioneAlternativa[i + 1]);
					popolazioneAlternativa[i]->somma(popolazione[i]);
				}

				//ricercaLocaleRandomizzata(popolazioneAlternativa, nIndividui);
				ricercaLocale(popolazioneAlternativa[0]);

				selezionaPopolazione(popolazione, popolazioneAlternativa, nIndividui, theta, normalizzazione, vettoreSuccessi);

				double sQ = 0, s = 0;
				unsigned short contatore = 0;
				for (unsigned short i = 0; i < nIndividui; i++) {
					if (vettoreSuccessi[i]) {
						sQ += pow(vettoreF[i], 2);
						s += vettoreF[i];
						contatore++;
					}
				}

				if (contatore != 0) {
					double mediaP = sQ / s;
					vettoreM[posizione] = mediaP;
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

			delete[] vettoreF;
			delete[] vettoreM;
			delete[] vettoreSuccessi;

			delete indiciRandom;

			return migliorIndividuo;
		};
};
