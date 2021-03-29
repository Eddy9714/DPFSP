#pragma once
#include "IndiciRandom.h"
#include <chrono>
#include <iostream>

using namespace std;

template <class T> class ADE {

	protected:
		IndiciRandom* indiciRandom;
		unsigned int seed = 0;

		virtual void creaPopolazione(T**, unsigned short) = 0;
		virtual void inizializzaPopolazione(T**, unsigned short, bool) = 0;
		virtual void crossover(T*, T*) = 0;
		virtual void normalizza(T*) = 0;
		virtual void selezionaPopolazione(T**, T**, unsigned short, double, unsigned short&, bool) = 0;
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

			T** popolazione = new T * [nIndividui];
			T** popolazioneAlternativa = new T * [nIndividui];

			creaPopolazione(popolazione, nIndividui);
			creaPopolazione(popolazioneAlternativa, nIndividui);

			double* vettoreF = new double[nIndividui];

			inizializzaPopolazione(popolazione, nIndividui, normalizzazione);

			//stampa(popolazione, nIndividui);

			indiciRandom = new IndiciRandom(nIndividui);

			if (seed > 0) indiciRandom->impostaSeed(seed + 1900238);

			Random ran;

			if (seed > 0) ran.impostaSeed(seed + 541892);

			for (unsigned short i = 0; i < nIndividui; i++) {
				vettoreF[i] = ran.randDouble(Fmin, Fmax);
			}

			unsigned short treIndici[3];


			//Troviamo l'individuo migliore
			unsigned int migliorPunteggioIniziale = UINT32_MAX;
			unsigned short migliore = 0;

			for (unsigned short i = 0; i < nIndividui; i++) {
				if (popolazione[i]->score < migliorPunteggioIniziale) {
					migliorPunteggioIniziale = popolazione[i]->score;
					migliore = i;
				}
			}

			unsigned int contatore = 0;

			while (true) {
				sec tempoRimasto = tempoFinale - orologio::now();
				auto count = tempoRimasto.count();
				if (count <= 0.)
					break;

				//cout << "Tempo rimasto: " << (unsigned int)count << " secondi \t\r";

				for (unsigned short i = 0; i < nIndividui; i++) {
					indiciRandom->generaIndici(treIndici, 3);

					if (seed > 0) {
						popolazione[treIndici[0]]->seed = max(1U, popolazione[treIndici[0]]->seed + contatore);
						popolazione[treIndici[1]]->seed = max(1U, popolazione[treIndici[1]]->seed + contatore);
						popolazione[treIndici[2]]->seed = max(1U, popolazione[treIndici[1]]->seed + contatore);
						++contatore;
					}


					*popolazioneAlternativa[i] = *(popolazione[treIndici[1]]);
					popolazioneAlternativa[i]->differenza(popolazione[treIndici[2]]);
					popolazioneAlternativa[i]->prodotto(vettoreF[i]);
					popolazioneAlternativa[i]->somma(popolazione[treIndici[0]]);
				}

				ricercaLocaleRandomizzata(popolazioneAlternativa, nIndividui);

				selezionaPopolazione(popolazione, popolazioneAlternativa, nIndividui, theta, migliore, normalizzazione);

				for (unsigned short i = 0; i < nIndividui; i++) {

					if (ran.randDouble(0, 1) < 0.1) {
						vettoreF[i] = Fmin + ran.randDouble(0, 1) * (Fmax - Fmin);
					}

				}
			}

			//cout << endl << endl;

			ricercaLocale(popolazione[migliore]);

			T migliorIndividuo = *(popolazione[migliore]);

			for (unsigned int i = 0; i < nIndividui; i++) {
				delete popolazione[i];
				delete popolazioneAlternativa[i];
			}

			delete[] popolazione;
			delete[] popolazioneAlternativa;

			delete[] vettoreF;
			delete indiciRandom;

			return migliorIndividuo;
		};
};
