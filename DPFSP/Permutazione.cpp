#include "Permutazione.h";
#include <memory>

Permutazione::Permutazione(unsigned short d) : dimensione(d){
	individuo = new unsigned short[dimensione];

	for (int k = 0; k < dimensione; k++) {
		individuo[k] = k;
	}
}

Permutazione::Permutazione(unsigned short* p, unsigned short d) : dimensione(d) {
	individuo = new unsigned short[dimensione];
	std::memcpy(individuo, p, dimensione * sizeof(unsigned short));
}

Permutazione::~Permutazione() {
	//delete[] individuo;  Errore nello heap(?)
}

Permutazione Permutazione::operator+(Permutazione& p) {
	Permutazione nuovaPermutazione(dimensione);

	for (int k = 0; k < dimensione; k++) {
		nuovaPermutazione.individuo[k] = this->individuo[p.individuo[k]];
	}

	return nuovaPermutazione;
}

Permutazione Permutazione::operator!() {
	Permutazione nuovaPermutazione(dimensione);

	for (int k = 0; k < dimensione; k++) {
		nuovaPermutazione.individuo[this->individuo[k]] = k;
	}

	return nuovaPermutazione;
}

Permutazione Permutazione::operator-(Permutazione& p) {
	Permutazione nuovaPermutazione(dimensione);
	Permutazione permutazioneInversa = !p;
	return permutazioneInversa + *this;
}

Permutazione Permutazione::operator*(const double f) {
	Permutazione nuovaPermutazione(dimensione);

	if (f == 0.) {
		for (int k = 0; k < dimensione; k++) {
			nuovaPermutazione.individuo[k] = k;
		}
	}
	else if (f == 1.) {
		std::memcpy(nuovaPermutazione.individuo, this->individuo, sizeof(unsigned short) * dimensione);
	}
	else {

	}

	return nuovaPermutazione;
}


