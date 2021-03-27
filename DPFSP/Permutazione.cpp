#include "Permutazione.h"
#include <cstring>
#include "Random.h"
#include <iostream>

using namespace std;

Permutazione::Permutazione(const Permutazione& p) : dimensione(p.dimensione) {
	individuo = new unsigned short[p.dimensione];
	memcpy(individuo, p.individuo, dimensione * sizeof(unsigned short));
	score = p.score;
	seed = p.seed;
}

Permutazione::Permutazione(unsigned short d, unsigned int seed) : dimensione(d), seed(seed) {
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

Permutazione::Permutazione(unsigned short* p, unsigned short d, unsigned int seed) : dimensione(d), seed(seed) {
	individuo = new unsigned short[dimensione];
	memcpy(individuo, p, dimensione * sizeof(unsigned short));
}

Permutazione::Permutazione(unsigned short* p, unsigned short d) : dimensione(d) {
	individuo = new unsigned short[dimensione];
	memcpy(individuo, p, dimensione * sizeof(unsigned short));
}

Permutazione& Permutazione::operator=(Permutazione& p) {
	memcpy(this->individuo, p.individuo, sizeof(unsigned short) * p.dimensione);
	score = p.score;
	seed = p.seed;

	return *this;
}

Permutazione::~Permutazione() {
	delete[] individuo;
}

void Permutazione::somma(Permutazione* p) {

	//p + this

	seed = max(1U, seed + p->seed);

	unsigned short* individuo = new unsigned short[dimensione];

	for (unsigned short k = 0; k < dimensione; k++) {
		individuo[k] = p->individuo[this->individuo[k]];
	}

	delete[] this->individuo;
	this->individuo = individuo;
}

void Permutazione::inversa(){

	seed = max(1U, -seed);

	unsigned short* individuo = new unsigned short[dimensione];

	for (unsigned short k = 0; k < dimensione; k++) {
		individuo[this->individuo[k]] = k;
	}

	delete[] this->individuo;
	this->individuo = individuo;
}

void Permutazione::differenza(Permutazione* p) {

	seed = max(1U, this->seed - p->seed);
	
	unsigned short* pCopia = new unsigned short[dimensione];
	unsigned short* thisCopia = new unsigned short[dimensione];
	
	//copiamo l'individuo p
	memcpy(pCopia, p->individuo, sizeof(unsigned short) * dimensione);

	//p^-1 
	for (unsigned short k = 0; k < dimensione; k++) {
		pCopia[p->individuo[k]] = k;
	}

	//p^-1 + this
	for (unsigned short k = 0; k < dimensione; k++) {
		thisCopia[k] = pCopia[this->individuo[k]];
	}

	//assegnamolo a this
	delete[] this->individuo;
	delete[] pCopia;
	this->individuo = thisCopia;
}

void Permutazione::scambia(Permutazione* p) {
	unsigned short* tmpIndividuo = this->individuo;
	unsigned int tmpScore = this->score;
	unsigned int tmpSeed = this->seed;

	this->individuo = p->individuo;
	p->individuo = tmpIndividuo;

	this->score = p->score;
	p->score = tmpScore;

	this->seed = p->seed;
	p->seed = tmpSeed;
}

void Permutazione::random() {
	Random r;
	if (seed > 0)
		r.impostaSeed(seed + 814292110);

	for (unsigned short j = 0; j < dimensione; j++) {
		individuo[j] = j;
	}

	for (unsigned short k = dimensione - 1; k > 0; k--) {
		int valoreRandom = r.randIntU(0, k);

		unsigned short tmp = individuo[valoreRandom];
		individuo[valoreRandom] = individuo[k];
		individuo[k] = tmp;
	}
}

void Permutazione::stampa() {
	for (unsigned i = 0; i < dimensione; i++) {
		cout << individuo[i] << " ";
	}
	cout << endl;
}

