#pragma once
#include "Permutazione.h"

class PermutazioneST : public Permutazione {

	public:
		PermutazioneST(unsigned short);
		PermutazioneST(unsigned short, unsigned int);
		PermutazioneST(unsigned short*, unsigned short, unsigned int);
		PermutazioneST(unsigned short*, unsigned short);
		PermutazioneST(const PermutazioneST& p);

		void prodotto(double);

	private:
		void randomBS(PermutazioneST&, unsigned int, unsigned short*);
		unsigned int numeroInversioni(PermutazioneST&);		
};