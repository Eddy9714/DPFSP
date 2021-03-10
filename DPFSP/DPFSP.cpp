// DPFSP.cpp: definisce il punto di ingresso dell'applicazione.
//

#include "DPFSP.h"
#include "ADE_DEP_DPFSP.h"


using namespace std;

int main()
{
	ADE_DEP_DPFSP adeDep("C:/Users/edu4r/Desktop/DPFSP/DPFSP_Large/2/Ta001_2.txt");
	adeDep.esegui(100, 1, 0.01, 0.1, 1.3);

	return 0;
}
