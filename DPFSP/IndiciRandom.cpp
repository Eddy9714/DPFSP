#pragma once
#include "IndiciRandom.h"

IndiciRandom::IndiciRandom(Random* r, unsigned short d) {
	dimensione = d;
	vettoreIndici = new unsigned short[d];
	random = r;

	for (int k = 0; k < d; k++) {
		vettoreIndici[k] = k;
	}
}

IndiciRandom::~IndiciRandom() {
	delete[] vettoreIndici;
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