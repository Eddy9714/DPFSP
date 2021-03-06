#include "Permutazione.h"
#include <cstring>
#include "Random.h"
#include "Globali.h"

class PermutazioneT : public Permutazione {

public:
	PermutazioneT(unsigned short);
	PermutazioneT(unsigned short*, unsigned short);
	PermutazioneT(const PermutazioneT& p);

	void prodotto(double);

private:

	void randomSS(PermutazioneT*, unsigned short&, double, Coppia*);
	void randomMergeSS(PermutazioneT*, unsigned short&, double, Coppia*);
};