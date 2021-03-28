#include "PermutazioneST.h"

using namespace std;

PermutazioneST::PermutazioneST(const PermutazioneST& p) : Permutazione(p) {}

PermutazioneST::PermutazioneST(unsigned short d, unsigned int seed) : Permutazione(d, seed) {}

PermutazioneST::PermutazioneST(unsigned short d) : Permutazione(d) {}

PermutazioneST::PermutazioneST(unsigned short* p, unsigned short d, unsigned int seed) : Permutazione(p, d, seed) {};

PermutazioneST::PermutazioneST(unsigned short* p, unsigned short d) : Permutazione(p, d) {}

void PermutazioneST::prodotto(double F) {
	seed = max(1ULL, (unsigned long long)((1 + F) * seed));

	if (F >= 0) {
		unsigned int numeroInversioniMassime = dimensione * (dimensione - 1) / 2;

		unsigned int numeroInversioniP = numeroInversioni(*this);

		unsigned int nInvApplicabili = (unsigned int)ceil(F * numeroInversioniP);

		unsigned short* arrayInversioni;

		if (F < 1.) {
			PermutazioneST copia(*this);

			copia.inversa();

			arrayInversioni = new unsigned short[nInvApplicabili];

			randomBS(copia, nInvApplicabili, arrayInversioni);
			this->identita();
		}
		else {
			nInvApplicabili = min(numeroInversioniMassime, nInvApplicabili);
			nInvApplicabili -= numeroInversioniP;

			PermutazioneST p(this->dimensione);

			for (int k = 0; k < p.dimensione; k++) {
				p.individuo[k] = this->dimensione - 1 - this->individuo[k];
			}

			arrayInversioni = new unsigned short[nInvApplicabili];

			randomBS(p, nInvApplicabili, arrayInversioni);
			
		}

		for (unsigned int k = 0; k < nInvApplicabili; k++) {
			unsigned short temp = this->individuo[arrayInversioni[k]];
			this->individuo[arrayInversioni[k]] = this->individuo[arrayInversioni[k] + 1];
			this->individuo[arrayInversioni[k] + 1] = temp;
		}

		if (arrayInversioni) delete[] arrayInversioni;
	}
}

void PermutazioneST::randomBS(PermutazioneST& p, unsigned int limiteTrasposizioni, unsigned short* risultato) {

	PermutazioneST pCopia(p.individuo, p.dimensione, p.seed);

	unsigned short* zeta = new unsigned short[p.dimensione - 1];
	unsigned short cursoreZeta = 0;

	unsigned int cursoreTrasposizioni = 0;
	unsigned int contatoreTrasposizioni = 0;

	for (unsigned short i = 0; i < p.dimensione - 1; i++) {
		if (p.individuo[i] > p.individuo[i + 1])
			zeta[cursoreZeta++] = i;
	}

	Random r;

	if (seed > 0) r.impostaSeed(seed + 54145914974281ULL);

	unsigned short random, randomZeta, temp;

	while (cursoreZeta > 0 && contatoreTrasposizioni < limiteTrasposizioni) {

		random = r.randIntU(0, cursoreZeta - 1);
		randomZeta = zeta[random];

		risultato[contatoreTrasposizioni++] = randomZeta;


		temp = pCopia.individuo[randomZeta];
		pCopia.individuo[randomZeta] = pCopia.individuo[randomZeta + 1];
		pCopia.individuo[randomZeta + 1] = temp;

		zeta[random] = zeta[--cursoreZeta];

		if (randomZeta > 0 && pCopia.individuo[randomZeta - 1] > pCopia.individuo[randomZeta] &&
			pCopia.individuo[randomZeta - 1] < pCopia.individuo[randomZeta + 1])
			zeta[cursoreZeta++] = randomZeta - 1;

		if (randomZeta + 2 < p.dimensione && pCopia.individuo[randomZeta + 1] > pCopia.individuo[randomZeta + 2] &&
			pCopia.individuo[randomZeta] < pCopia.individuo[randomZeta + 2])
			zeta[cursoreZeta++] = randomZeta + 1;

	}

	delete[] zeta;
}

unsigned int PermutazioneST::numeroInversioni(PermutazioneST& p) {

	unsigned int inv = 0;

	for (unsigned short i = 0; i < p.dimensione - 1; i++) {
		for (unsigned short j = i + 1; j < p.dimensione; j++) {
			if (p.individuo[i] > p.individuo[j])
				inv++;
		}
	}

	return inv;
}