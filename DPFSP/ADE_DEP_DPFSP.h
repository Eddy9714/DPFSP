#pragma once

#include "ADE.h"
#include "Istanza.h"
#include <string>

using namespace std;

class ADE_DEP_DPFSP : public ADE<Permutazione> {

	public:
		ADE_DEP_DPFSP(string);
		unsigned int esegui(unsigned short, unsigned short, double, double, double);

	private:
		void creaPopolazione(Permutazione**, unsigned short);
		void inizializzaPopolazione(Permutazione**, unsigned short);
		void crossover(Permutazione*, Permutazione*);
		void stampa(Permutazione**, unsigned short);
		unsigned int valutaIndividuo(Permutazione*);

		const Istanza istanza;
};