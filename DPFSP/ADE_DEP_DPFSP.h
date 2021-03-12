#pragma once

#include "ADE.h"
#include "Istanza.h"
#include "Permutazione.h"
#include <string>

using namespace std;

class ADE_DEP_DPFSP : public ADE<Permutazione> {

	public:
		ADE_DEP_DPFSP(string);
		Permutazione esegui(unsigned short, unsigned short, double, double, double, unsigned long long);
		const Istanza istanza;

	private:
		void creaPopolazione(Permutazione**, unsigned short, unsigned long long);
		void inizializzaPopolazione(Permutazione**, unsigned short, unsigned long long);
		void crossover(Permutazione*, Permutazione*, unsigned long long);
		void subCrossover(Permutazione*, Permutazione*, Permutazione*);
		void stampa(Permutazione**, unsigned short);
		unsigned int valutaIndividuo(Permutazione*);
};