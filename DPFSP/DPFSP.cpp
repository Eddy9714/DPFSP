// DPFSP.cpp: definisce il punto di ingresso dell'applicazione.
//

#include "DPFSP.h"
#include "ADE_DEP_DPFSP.h"


using namespace std;

int main(int argc, char* argv[])
{
	unsigned long long seed = 2450;
	unsigned short nGenerazioni = 50;
	unsigned int nIndividui = 100;
	double theta = 0.01;
	double Fmax = 2;


	ADE_DEP_DPFSP adeDep("C:/Users/edu4r/Desktop/DPFSP/DPFSP_Large/2/Ta001_2.txt");
	Permutazione migliorIndividuo = adeDep.esegui(nIndividui, nGenerazioni, theta, 0.1, Fmax, seed);

	cout << migliorIndividuo.score << endl;

	return 0;
}
