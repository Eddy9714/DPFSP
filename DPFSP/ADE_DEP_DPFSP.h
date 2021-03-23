#pragma once

#include "ADE.h"
#include "Istanza.h"
#include "Permutazione.h"
#include <string>

using namespace std;

class ADE_DEP_DPFSP : public ADE<Permutazione> {

	public:
		ADE_DEP_DPFSP(string);
		Permutazione esegui(unsigned short, unsigned short, double, double, double, bool, unsigned long long);
		const Istanza istanza;

	public:
		struct InfoInserzione {
			unsigned int makeSpan;
			unsigned short posizione;
		};

		void creaPopolazione(Permutazione**, unsigned short);
		void inizializzaPopolazione(Permutazione**, unsigned short, bool);
		void selezionaPopolazione(Permutazione**, Permutazione**, unsigned short, double, unsigned short&, bool);
		void ordinaFabbriche(Permutazione*);
		void normalizza(Permutazione*);
		void NEH2(Permutazione*);
		void crossover(Permutazione*, Permutazione*);
		void subCrossover(Permutazione*, Permutazione*, Permutazione*);
		void ricercaLocale(Permutazione*);
		void ricercaLocaleRandomizzata(Permutazione**, unsigned short);
		void VND(Permutazione*, bool);
		void LS1(Permutazione*, unsigned short, unsigned short);
		int LS2(Permutazione*, unsigned int, int*, unsigned int*, bool);
		void LS3(Permutazione*);
		InfoInserzione miglioreInserzione(unsigned short*, unsigned short, unsigned short);
		unsigned int valutaIndividuoParziale(unsigned short*, unsigned short);
		unsigned int valutaIndividuo(Permutazione*);
		void stampa(Permutazione**, unsigned short);

};