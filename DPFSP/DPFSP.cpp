// DPFSP.cpp: definisce il punto di ingresso dell'applicazione.
//

#include "DPFSP.h"
#include "ADE_DEP_DPFSP.h"


using namespace std;

int main()
{
	ADE_DEP_DPFSP adeDep("C:/Users/edu4r/Desktop/DPFSP/DPFSP_Small/2/I_2_4_5_2.txt");
	adeDep.esegui(50, 50, 0.01);

	return 0;
}
