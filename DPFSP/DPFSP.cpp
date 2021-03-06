// DPFSP.cpp: definisce il punto di ingresso dell'applicazione.
//

#include "DPFSP.h"
#include "Istanza.h"
#include "ADE_DEP.h"
#include "Permutazione.h"


using namespace std;

int main()
{
	Istanza i("C:/Users/edu4r/Desktop/DPFSP/DPFSP_Small/2/I_2_4_5_2.txt");
	//ADE_DEP ade(&i);

	unsigned short a[5] = { 1,0,4,2,3 };
	unsigned short b[5] = { 1,3,2,4,0 };

	Permutazione p1(a, 5);
	Permutazione p2(b, 5);

	Permutazione inv = p1 + (p1 - p2) * 0.3;

	for (int k = 0; k < 5; k++) {
		cout << inv.individuo[k] << "\t";
	}

	//ade.esegui(10, 50);

	return 0;
}
