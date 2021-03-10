// DPFSP.cpp: definisce il punto di ingresso dell'applicazione.
//

#include "DPFSP.h"
#include "ADE_DEP_DPFSP.h"


using namespace std;

int main()
{
	ADE_DEP_DPFSP adeDep("C:/Users/edu4r/Desktop/DPFSP/DPFSP_Large/2/Ta001_2.txt");
	unsigned int migliorPunteggio = adeDep.esegui(10000, 50, 0.01, 0.1, 2);

	cout << migliorPunteggio << endl;

	return 0;
}
