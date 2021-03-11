#pragma once
#include <memory>
#include "Random.h"

using namespace std;

class IndiciRandom {

	private:
		unsigned short* vettoreIndici;
		unsigned short dimensione;
		Random* random;

	public:
		IndiciRandom(unsigned short);
		IndiciRandom(unsigned short, unsigned long long);
		~IndiciRandom();
		void generaIndici(unsigned short[], unsigned short);
		void impostaSeed(unsigned long long);
};

