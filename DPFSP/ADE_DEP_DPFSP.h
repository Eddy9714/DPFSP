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
		struct InfoInserzione {
			unsigned int makeSpan;
			unsigned short posizione;
		};

		void creaPopolazione(Permutazione**, unsigned short, unsigned long long);
		void inizializzaPopolazione(Permutazione**, unsigned short, unsigned long long);
		void crossover(Permutazione*, Permutazione*, unsigned long long);
		void subCrossover(Permutazione*, Permutazione*, Permutazione*);
		void ricercaLocale(Permutazione**);
		bool VND(Permutazione*);
		bool LS1(Permutazione*, unsigned short, unsigned short);
		int LS2(Permutazione*, unsigned int, unsigned short*, unsigned int*);
		InfoInserzione miglioreInserzione(unsigned short*, unsigned short, unsigned short);
		unsigned int valutaIndividuoParziale(unsigned short*, unsigned short);
		unsigned int valutaIndividuo(Permutazione*);
		void stampa(Permutazione**, unsigned short);

};