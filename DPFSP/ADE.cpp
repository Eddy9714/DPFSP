#include "ADE.h";
#include "Random.h";
#include <memory>
#include <iostream>

using namespace std;


unsigned int ADE::esegui(unsigned short nIndividui, unsigned short nGenerazioni) {

	// Crea e inizializza popolazione
	Individuo** popolazione = creaPopolazione(nIndividui);
	Individuo** popolazioneMutata = creaPopolazione(nIndividui);

	inizializzaPopolazione(popolazione, nIndividui);

	for (unsigned short g = 0; g < nGenerazioni; g++) {

		for (unsigned short i = 0; i < nIndividui; i++) {
			//Seleziona tre indici a caso 
			//Calcola inversa xr2
			//Effettua la composizione inversa xr2 e xr1
			//Decomponi la composizione tramite randombs dato F
			// Somma xr0 e il risultato della decomposizione

			//Crossover
			//Valutazione individui
		}

		for (unsigned short i = 0; i < nIndividui; i++) {
			//Seleziona individui
		}

	}

	eliminaPopolazione(popolazione, nIndividui);
	eliminaPopolazione(popolazioneMutata, nIndividui);

	return 0;
}