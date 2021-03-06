#pragma once
#include "Individuo.h"
#include "Istanza.h";
#include "ADE.h"

class ADE_DEP : public ADE{
	public:
		ADE_DEP(Istanza*);

		void inizializzaPopolazione(Individuo**, unsigned short);
		unsigned int esegui(unsigned short, unsigned short);
	
	protected:
		Istanza* istanza;
};
