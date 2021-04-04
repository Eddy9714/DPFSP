#pragma once
#include "DPFSP.h"

using namespace std;

int main(int argc, char* argv[])
{
	unsigned int seed = 0;
	unsigned short scalaElaborazione = 100;
	unsigned int nIndividui = 50;
	double theta = 0.05;
	double Fmax = 1.2;
	bool normalizzazione = false;
	string percorsoFile;

	size_t pos;

	switch (argc) {
		default:
		case 8:
			seed = stoi(argv[7], &pos);
		case 7:
			normalizzazione = stoi(argv[6], &pos);
		case 6:
			Fmax = strtod(argv[5], NULL);
		case 5:
			theta = strtod(argv[4], NULL);
		case 4:
			scalaElaborazione = stoi(argv[3], &pos);
		case 3:
			nIndividui = stoi(argv[2], &pos);
		case 2:
			percorsoFile = argv[1];
			break;
		case 1:
			cerr << "Devi inserire il percorso del file!" << endl << endl;
			//exit(-1);
	}

	/*
	cout << endl << endl;
	cout << "Il programma sara' eseguito con queste impostazioni:" << endl << endl;
	cout << "Percorso file: " << percorsoFile << endl;
	cout << "Numero individui: " << nIndividui << endl;
	cout << "Scala elaborazione: " << scalaElaborazione << endl;
	cout << "Theta: " << theta << endl;
	cout << "F massimo: " << Fmax << endl;
	cout << "Normalizzazione attiva: " << (normalizzazione ? "Si" : "No") << endl;

	if(seed > 0) cout << "Seed: " << seed << endl;
	else cout << "Seed: " << "Random" << endl << endl;
	*/

	using orologio = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;

	const auto tempoIniziale = orologio::now();


	percorsoFile = "C:/users/edu4r/Desktop/DPFSP_Large/5/Ta016_5.txt";

	ADE_DEP_DPFSP adeDep(percorsoFile);
	Permutazione migliorIndividuo = adeDep.esegui(nIndividui, scalaElaborazione, theta, 0.1, Fmax, 
		normalizzazione, seed);

	const sec durata = orologio::now() - tempoIniziale;

	//Stampo a video le informazioni

	
	cout << "Risultati: " << endl << endl;
	cout << "Tempo speso: " << durata.count() << " secondi" << endl;
	cout << "Individuo: ";
	migliorIndividuo.stampa();
	cout << "Punteggio: " << migliorIndividuo.score << endl << endl;
	
	exit(0);
	

	//Salvo le informazioni su un CSV

	int posizione = percorsoFile.find_last_of(".");
	string percorso = percorsoFile.substr(0, posizione);

	string nomeReport = percorso + "_report-" + to_string(orologio::now().time_since_epoch().count()) + ".csv";

	ofstream file(nomeReport);
	
	if (file.is_open()) {
		file << "Fabbriche; Lavori; Macchine; Numero individui; Scala elaborazione; Theta; Fmax; Normalizzazione; Seed (0 se random); Durata (secondi); Miglior punteggio; Individuo;" << endl;

		file << adeDep.istanza.fabbriche << ";";
		file << adeDep.istanza.lavori << ";";
		file << adeDep.istanza.macchine << ";";
		file << nIndividui << ";";
		file << scalaElaborazione << ";";
		file << theta << ";";
		file << Fmax << ";";
		file << (normalizzazione ? "Si" : "No") << ";";
		file << seed << ";";
		file << durata.count() << ";";
		file << migliorIndividuo.score << ";";

		for (unsigned short i = 0; i < migliorIndividuo.dimensione; i++) {
			file << migliorIndividuo.individuo[i];
			if (i != migliorIndividuo.dimensione - 1) file << ",";
		}

		file << ";" << endl;

		file.close();
	}

	//cout << "Risultati salvati in: " << nomeReport << endl << endl;

	return 0;
}
