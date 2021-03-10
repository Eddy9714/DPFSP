#include <string>
#include <fstream>
#include <iostream>

#include "Istanza.h"


using namespace std;

Istanza::Istanza(string percorso) {

	ifstream file(percorso);

	if (file.is_open()) {
		file >> lavori >> macchine >> fabbriche;

		p = new unsigned int* [lavori];

		for (int i = 0; i < lavori; i++) {
			p[i] = new unsigned int[macchine];
		}

		for (int i = 0; i < lavori; i++) {
			for (int m = 0; m < macchine; m++) {
				file >> p[i][m] >> p[i][m];
			}
		}
	}
	else {
		cerr << "File non trovato!";
	}
}

Istanza::~Istanza(){

	for (int i = 0; i < lavori; i++) {
		delete[] p[i];
	}

	delete[] p;
}