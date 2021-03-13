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
		void ricercaLocale(Permutazione**);
		bool VND(Permutazione*);
		bool LS1(Permutazione*, unsigned short, unsigned short);
		bool LS2(Permutazione*, unsigned short, unsigned short);
		unsigned short miglioreInserzione(unsigned short*, unsigned short, unsigned short);
		void stampa(Permutazione**, unsigned short);
		unsigned int valutaIndividuo(Permutazione*);
};