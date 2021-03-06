#include <string>
#include <fstream>
#include <iostream>

#include "Istanza.h"


using namespace std;

Istanza::Istanza(string percorso) {

	ifstream file(percorso);

	if (file.is_open()) {
		file >> macchine >> lavori >> fabbriche;

		p = new unsigned int* [lavori];

		for (int i = 0; i < lavori; i++) {
			p[i] = new unsigned int[macchine];
		}

		for (int m = 0; m < macchine; m++) {
			for (int i = 0; i < lavori; i++) {
				file >> p[i][m] >> p[i][m];
			}
		}
	}
	else {
		cerr << "File non trovato!";
	}
}

Istanza::~Istanza(){

	for (int m = 0; m < macchine; m++) {
		delete[] p[m];
	}

	delete[] p;
}