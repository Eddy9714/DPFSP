#include "Permutazione.h";
#include <memory>
#include "Random.h"
#include <iostream>

Permutazione::Permutazione(const Permutazione& p) : dimensione(p.dimensione) {
	individuo = new unsigned short[p.dimensione];
	std::memcpy(individuo, p.individuo, dimensione * sizeof(unsigned short));
	score = p.score;
	seed = p.seed;
}

Permutazione::Permutazione(unsigned short d, unsigned long long seed) : dimensione(d), seed(seed) {
	individuo = new unsigned short[d];
}

Permutazione::Permutazione(unsigned short d) : dimensione(d) {
	individuo = new unsigned short[d];
}

void Permutazione::identita() {
	for (int k = 0; k < dimensione; k++) {
		individuo[k] = k;
	}
}

Permutazione::Permutazione(unsigned short* p, unsigned short d, unsigned long long seed) : dimensione(d), seed(seed) {
	individuo = new unsigned short[dimensione];
	std::memcpy(individuo, p, dimensione * sizeof(unsigned short));
}

Permutazione::Permutazione(unsigned short* p, unsigned short d) : dimensione(d) {
	individuo = new unsigned short[dimensione];
	std::memcpy(individuo, p, dimensione * sizeof(unsigned short));
}

Permutazione::~Permutazione() {
	delete[] individuo;
}

void Permutazione::somma(Permutazione* p) {
	unsigned short* individuo = new unsigned short[dimensione];

	for (int k = 0; k < dimensione; k++) {
		individuo[k] = this->individuo[p->individuo[k]];
	}

	delete[] this->individuo;
	this->individuo = individuo;
}

void Permutazione::inverti(){

	unsigned short* individuo = new unsigned short[dimensione];

	for (int k = 0; k < dimensione; k++) {
		individuo[this->individuo[k]] = k;
	}

	delete[] this->individuo;
	this->individuo = individuo;
}

void Permutazione::differenza(Permutazione* p) {
	Permutazione copia(*p);
	copia.inverti();
	this->somma(&copia);
}

void Permutazione::prodotto(double F) {

	if (F > 0) {

		Permutazione nuovaPermutazione(dimensione);
		if (seed > 0) nuovaPermutazione.seed = seed + 980849103648ULL;

		unsigned int numeroInversioniMassime = dimensione * (dimensione - 1) / 2;
		unsigned short* arrayInversioni = new unsigned short[numeroInversioniMassime];

		unsigned int numeroInversioniP = numeroInversioni(*this);

		unsigned int nInvApplicabili = (unsigned int)ceil(F * numeroInversioniP);

		if (F < 1.) {
			Permutazione copia(*this);
			copia.inverti();
			randomBS(copia, nInvApplicabili, arrayInversioni);
			nuovaPermutazione.identita();
		}
		else {
			nInvApplicabili = min(numeroInversioniMassime, nInvApplicabili);
			nInvApplicabili -= numeroInversioniP;

			Permutazione p(this->dimensione);

			if (seed > 0) p.seed = seed + 19581005720294ULL;

			for (int k = 0; k < p.dimensione; k++) {
				p.individuo[k] = this->dimensione - 1 - this->individuo[k];
			}

			randomBS(p, nInvApplicabili, arrayInversioni);
			nuovaPermutazione = *this;
		}


		for (unsigned int k = 0; k < nInvApplicabili; k++) {
			unsigned short temp = nuovaPermutazione.individuo[arrayInversioni[k]];
			nuovaPermutazione.individuo[arrayInversioni[k]] = nuovaPermutazione.individuo[arrayInversioni[k] + 1];
			nuovaPermutazione.individuo[arrayInversioni[k] + 1] = temp;
		}

		delete[] arrayInversioni;
		*this = nuovaPermutazione;
	}
}

Permutazione& Permutazione::operator=(Permutazione& p) {

	memcpy(this->individuo, p.individuo, sizeof(unsigned short) * p.dimensione);
	score = p.score;
	seed = p.seed;

	return *this;
}

void Permutazione::randomBS(Permutazione& p, unsigned int limiteTrasposizioni, unsigned short* risultato) {

	Permutazione pCopia(p.individuo, p.dimensione, p.seed);
	
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

		if(randomZeta + 2 < p.dimensione && pCopia.individuo[randomZeta + 1] > pCopia.individuo[randomZeta + 2] &&
			pCopia.individuo[randomZeta] < pCopia.individuo[randomZeta + 2])
				zeta[cursoreZeta++] = randomZeta + 1;

	}

	delete[] zeta;
}

unsigned int Permutazione::numeroInversioni(Permutazione& p) {

	unsigned int inv = 0;

	for (unsigned short i = 0; i < p.dimensione - 1; i++) {
		for (unsigned short j = i + 1; j < p.dimensione; j++) {
			if (p.individuo[i] > p.individuo[j])
				inv++;
		}
	}

	return inv;
}

