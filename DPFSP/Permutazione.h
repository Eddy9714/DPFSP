#pragma once

class Permutazione {

	private:
		void randomBS(Permutazione&, unsigned int, unsigned short*);
		unsigned int numeroInversioni(Permutazione&);

	public:
		Permutazione(unsigned short);
		Permutazione(unsigned short*, unsigned short);
		~Permutazione();

		Permutazione operator+(Permutazione&);
		Permutazione operator-(Permutazione&);
		Permutazione operator*(const double);
		Permutazione operator=(Permutazione&);
		Permutazione operator!();

		unsigned short* individuo;
		const unsigned short dimensione;
		double score = 0;
};
