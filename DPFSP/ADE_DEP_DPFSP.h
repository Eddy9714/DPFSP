#pragma once
#include "ADE.h"
#include "Globali.h"
#include "Istanza.h"
#include "Permutazione.h"
#include "PermutazioneI.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <cstring>

using namespace std;

class ADE_DEP_DPFSP : public ADE<Permutazione> {

	public:
		ADE_DEP_DPFSP(string);
		Permutazione esegui(unsigned short, unsigned short, double, double, double, bool, unsigned int);
		const Istanza istanza;

	public:
		struct InfoInserzione {
			unsigned int makeSpan;
			unsigned short posizione;
		};

		void creaPopolazione(Permutazione**, unsigned short);
		void inizializzaPopolazione(Permutazione**, unsigned short, bool);
		void selezionaPopolazione(Permutazione**, Permutazione**, unsigned short, double, bool, bool*);
		void normalizza(Permutazione*);
		void NEH2(Permutazione*);
		void crossover(Permutazione*, Permutazione*);
		void subCrossover(Permutazione*, Permutazione*, Permutazione*);
		void ricercaLocale(Permutazione*);
		void ricercaLocaleRandomizzata(Permutazione**, unsigned short);
		void scambiaFabbricheRandom(Permutazione*);

		void VND(Permutazione*, bool);
		void LS1(Permutazione*, unsigned short, unsigned short);
		int LS2(Permutazione*, unsigned int, int*, unsigned int*, bool);
		
		void VNDEXC(Permutazione* p);
		void LS1EXC(Permutazione* p, unsigned int, int*, unsigned int*);
		bool LS2EXC(Permutazione*, unsigned int, unsigned int,
			int*, unsigned int*);

		InfoInserzione miglioreInserzione(unsigned short*, unsigned short, unsigned short);
		unsigned int valutaIndividuoParziale(unsigned short*, unsigned short);
		unsigned int valutaIndividuo(Permutazione*);
		void stampa(Permutazione**, unsigned short);

};