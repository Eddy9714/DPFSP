#pragma once

#include "ADE_DEP.h"
#include "Istanza.h"
#include <string>

using namespace std;

class ADE_DEP_DPFSP : public ADE_DEP {

	public:
		ADE_DEP_DPFSP(string);
		unsigned int esegui(unsigned short, unsigned short, double);

	private:
		Permutazione** creaPopolazione(unsigned short);
		void inizializzaPopolazione(Permutazione**, unsigned short);
		void eliminaPopolazione(Permutazione**, unsigned short);
		void stampa(Permutazione**, unsigned short);
		unsigned int valutaIndividuo(Permutazione*);
		const Istanza istanza;
};