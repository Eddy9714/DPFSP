#pragma once

class Permutazione {

	public:
		Permutazione(unsigned short);
		Permutazione(unsigned short*, unsigned short);
		~Permutazione();

		Permutazione operator+(Permutazione&);
		Permutazione operator-(Permutazione&);
		Permutazione operator*(const double);
		Permutazione operator!();
		unsigned short* individuo;
		const unsigned short dimensione;
};
