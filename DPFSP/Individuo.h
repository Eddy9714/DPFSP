#pragma once
#include "Istanza.h"
class Individuo {
	private:
		Istanza* istanza;

	public:
		unsigned short* individuo;

		Individuo(unsigned short, Istanza* i);
		~Individuo();
		unsigned int makeSpan();
};