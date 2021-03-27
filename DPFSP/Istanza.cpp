#pragma once
#include "Istanza.h"

using namespace std;

Istanza::Istanza(string percorso) {

	ifstream file(percorso);

	if (file.is_open()) {
		file >> lavori >> macchine >> fabbriche;

		p = new unsigned int* [lavori];
		minp = new unsigned int[lavori];

		for (int i = 0; i < lavori; i++) {
			p[i] = new unsigned int[macchine];
			minp[i] = UINT32_MAX;
		}

		for (int i = 0; i < lavori; i++) {
			for (int m = 0; m < macchine; m++) {
				file >> p[i][m] >> p[i][m];
			}
		}

		for (int i = 0; i < lavori; i++) {
			for (int m = 0; m < macchine; m++) {
				minp[i] = min(minp[i], p[i][m]);
			}
		}

		file.close();
	}
	else {
		cerr << "File non trovato!" << endl << endl;
		exit(-2);
	}
}

Istanza::~Istanza(){

	for (int i = 0; i < lavori; i++) {
		delete[] p[i];
	}

	delete[] p;
}