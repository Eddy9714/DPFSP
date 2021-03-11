#include "IndiciRandom.h"

IndiciRandom::IndiciRandom(unsigned short d) {
	dimensione = d;
	vettoreIndici = new unsigned short[d];
	random = new Random();

	for (int k = 0; k < d; k++) {
		vettoreIndici[k] = k;
	}
}

IndiciRandom::IndiciRandom(unsigned short d, unsigned long long seed) {
	dimensione = d;
	vettoreIndici = new unsigned short[d];
	random = new Random();

	for (int k = 0; k < d; k++) {
		vettoreIndici[k] = k;
	}

	random->impostaSeed(seed);
}


IndiciRandom::~IndiciRandom() {
	delete[] vettoreIndici;
	delete random;
}

void IndiciRandom::impostaSeed(unsigned long long seed) {
	random->impostaSeed(seed);
}

void IndiciRandom::generaIndici(unsigned short risultato[], unsigned short nIndiciGenerati) {
	if (nIndiciGenerati > dimensione) return;

	unsigned short ran, tmp, dim;

	for (int k = 0; k < nIndiciGenerati; k++) {
		dim = dimensione - 1 - k;
		ran = random->randIntU(0, dim);
		tmp = vettoreIndici[ran];
		vettoreIndici[ran] = vettoreIndici[dim];
		vettoreIndici[dim] = tmp;

		risultato[k] = vettoreIndici[dim];
	}
}