#pragma once
#include <cstring>
#include "Globali.h"
#include <iostream>

class Permutazione {

	public:
		Permutazione(unsigned short);
		Permutazione(unsigned short*, unsigned short);
		Permutazione(const Permutazione& p);
		virtual ~Permutazione();
		virtual Permutazione& operator=(Permutazione&);

		void somma(Permutazione*);
		void differenza(Permutazione*);
		virtual void prodotto(double) {};
		void random();
		void inversa();
		void identita();
		void stampa();
		void scambia(Permutazione*);

		unsigned short* individuo;
		const unsigned short dimensione;
		unsigned int score = 0;
};
