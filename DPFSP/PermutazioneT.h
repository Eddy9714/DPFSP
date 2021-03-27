#pragma once
#include "Permutazione.h"

class PermutazioneT : public Permutazione {

public:
	PermutazioneT(unsigned short);
	PermutazioneT(unsigned short, unsigned int);
	PermutazioneT(unsigned short*, unsigned short, unsigned int);
	PermutazioneT(unsigned short*, unsigned short);
	PermutazioneT(const PermutazioneT& p);

	void prodotto(double);

private:

	struct Coppia {
		unsigned short x;
		unsigned short y;
	};

	void randomSS(PermutazioneT*, unsigned short&, double, Coppia*);
	void randomMergeSS(PermutazioneT*, unsigned short&, double, Coppia*);
};