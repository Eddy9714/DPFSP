#pragma once

class Permutazione {

	private:
		void randomBS(Permutazione&, unsigned int, unsigned short*);
		unsigned int numeroInversioni(Permutazione&);

	public:
		Permutazione(unsigned short);
		Permutazione(unsigned short, unsigned long long);
		Permutazione(unsigned short*, unsigned short, unsigned long long);
		Permutazione(unsigned short*, unsigned short);
		Permutazione(const Permutazione& p);
		~Permutazione();

		void somma(Permutazione*);
		void differenza(Permutazione*);
		void prodotto(double);
		void inverti();
		void identita();

		Permutazione& operator=(Permutazione&);

		unsigned short* individuo;
		const unsigned short dimensione;
		unsigned int score = 0;
		unsigned long long seed = 0;
};
