#pragma once
#include "Permutazione.h"

class PermutazioneI : public Permutazione {

	public:
		PermutazioneI(unsigned short);
		PermutazioneI(unsigned short, unsigned int);
		PermutazioneI(unsigned short*, unsigned short, unsigned int);
		PermutazioneI(unsigned short*, unsigned short);
		PermutazioneI(const PermutazioneI& p);

		void prodotto(double);

	private:

		struct Coppia {
			unsigned short x;
			unsigned short y;
		};

		void PermutazioneI::inserisci(PermutazioneI*, Coppia&);
		void randomIS(PermutazioneI*, unsigned short&, double, Coppia*, int);
		void createLengthsArray(PermutazioneI*, unsigned short*, unsigned short*, unsigned short&, int&);
		void insertionSortWithValues(unsigned short*, unsigned short, PermutazioneI*);
		unsigned short lisLength(PermutazioneI*);
		unsigned short insWeight(PermutazioneI*);
		unsigned short PermutazioneI::ricercaBinaria(unsigned short*, unsigned short, unsigned short);
};