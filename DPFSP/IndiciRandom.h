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
		~IndiciRandom();
		void generaIndici(unique_ptr<unsigned short[]>&, unsigned short);
};
