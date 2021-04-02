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
		IndiciRandom(Random*, unsigned short);
		~IndiciRandom();
		void generaIndici(unsigned short[], unsigned short);
};

