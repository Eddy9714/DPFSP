#pragma once
#include "Permutazione.h"
#include <vector>
#include <memory>

using namespace std;

class ADE_DEP {

	protected:
		virtual Permutazione** creaPopolazione(unsigned short) = 0;
		virtual void inizializzaPopolazione(Permutazione**, unsigned short) = 0;
		virtual void eliminaPopolazione(Permutazione**, unsigned short) = 0;
		virtual void crossover(Permutazione*, Permutazione*);
		virtual void selezionaPopolazione(Permutazione**, Permutazione**, unsigned short, double);
		virtual unsigned int valutaIndividuo(Permutazione*) = 0;
		virtual void stampa(Permutazione**, unsigned short) = 0;
		unsigned int esegui(unsigned short, unsigned short, double);

};
