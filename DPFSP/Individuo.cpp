#include "Individuo.h"
#include <memory>

using namespace std;

Individuo::Individuo(unsigned short dimensione, Istanza* i) {
	individuo = new unsigned short[dimensione];
	istanza = i;
}

Individuo::~Individuo() {
	delete[] individuo;
}

unsigned int Individuo::makeSpan() {
	auto Cm = make_unique<unsigned int[]>(istanza->macchine);

	unsigned int Cmax = 0;

	for (unsigned short k = 0; k < istanza->lavori + istanza->fabbriche - 1; k++) {

		if (individuo[k] >= istanza->lavori) {
			Cmax = max(Cmax, Cm[istanza->macchine - 1]);
			for (unsigned short k = 0; k < istanza->macchine; k++) {
				Cm[k] = 0;
			}
		}
		else {
			for (unsigned short j = 0; j < istanza->macchine; j++) {

				if (k == 0 && j == 0) {
					Cm[j] = istanza->p[individuo[k]][j];
				}
				else if (k != 0 && j != 0) {
					Cm[j] = istanza->p[individuo[k]][j] + max(Cm[j - 1], Cm[j]);
				}
				else if (k == 0 && j != 0) {
					Cm[j] = istanza->p[individuo[k]][j] + Cm[j - 1];
				}
				else {
					Cm[j] = istanza->p[individuo[k]][j] + Cm[j];
				}
			}
		}
	}
	
	return max (Cmax, Cm[istanza->macchine - 1]);
}