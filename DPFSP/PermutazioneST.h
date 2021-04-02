#pragma once
#include "Permutazione.h"
#include <cstring>
#include "Random.h"


class PermutazioneST : public Permutazione {

	public:
		PermutazioneST(unsigned short);
		PermutazioneST(unsigned short*, unsigned short);
		PermutazioneST(const PermutazioneST& p);

		void prodotto(double);

	private:
		void randomBS(PermutazioneST&, unsigned int, unsigned short*);
		unsigned int numeroInversioni(PermutazioneST&);		
};