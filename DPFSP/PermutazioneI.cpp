#include "PermutazioneI.h"
#include "Random.h"

PermutazioneI::PermutazioneI(const PermutazioneI& p) : Permutazione(p) {}

PermutazioneI::PermutazioneI(unsigned short d, unsigned int seed) : Permutazione(d, seed) {}

PermutazioneI::PermutazioneI(unsigned short d) : Permutazione(d) {}

PermutazioneI::PermutazioneI(unsigned short* p, unsigned short d, unsigned int seed) : Permutazione(p, d, seed) {};

PermutazioneI::PermutazioneI(unsigned short* p, unsigned short d) : Permutazione(p, d) {}

void PermutazioneI::prodotto(double F) {
	seed = max(1U, (unsigned int)((1 + F) * seed));

	if (F >= 0) {

		unsigned short spostamenti;
		Coppia* coppie;

		if (F < 1) {
			coppie = new Coppia[ceil(dimensione * F)];
			PermutazioneI copia(*this);
			copia.inversa();

			randomIS(&copia, spostamenti, F, coppie, -1);
			
			this->identita();
		}
		else {
			PermutazioneI p(this->dimensione);

			for (int k = 0; k < p.dimensione; k++) {
				p.individuo[k] = this->dimensione - 1 - this->individuo[k];
			}

			unsigned short wx = insWeight(&p);
			unsigned short limite = ceil(F * wx) - wx;

			coppie = new Coppia[limite];

			randomIS(&p, spostamenti, F, coppie, limite);

			for (unsigned short i = 0; i < spostamenti; i++) {
				coppie[i].x = dimensione - 1 - coppie[i].x;
				coppie[i].y = dimensione - 1 - coppie[i].y;
			}
		}

		for (unsigned short i = 0; i < spostamenti; i++) {
			inserisci(this, coppie[i]);
		}
		
		delete[] coppie;
	}
}

void PermutazioneI::randomIS(PermutazioneI* p, unsigned short& spostamenti, double F, Coppia* risultato, int limiteSpostamenti){

	Random r;
	if (seed > 0) r.impostaSeed(seed + 124498728);

	int i, ind, a = 0, j, k = 0, y, b, m, ul, w;
	unsigned short ll;
	double tempDouble;
	unsigned short* lis = new unsigned short[p->dimensione];
	unsigned short* u = new unsigned short[p->dimensione];
	unsigned short* su = new unsigned short[p->dimensione];
	unsigned short* t = new unsigned short[p->dimensione];
	unsigned short* q = new unsigned short[p->dimensione];

	createLengthsArray(p, t, q, ll, ind);

	ul = p->dimensione - ll;
	for (i = p->dimensione - 1; i > ind; i--)
		u[--ul] = i;

	m = ll - 1;
	lis[m] = ind;
	
	a = ll;

	for (i = ind - 1; i >= 0; i--) {
		y = t[p->individuo[i]];
		if (y >= m && y < ll && p->individuo[i] < p->individuo[lis[y]]) {
			if (y == m) { 
				if (m < a)
					a = m;
				else
					u[--ul] = lis[m - 1];
				
				q[m] = 1;
				lis[--m] = i;
			}
			else { 
				q[y]++;
				
				if (r.randIntU(0, q[y] - 1) == 0) {   
					m = y - 1;         
					u[--ul] = lis[m];
					lis[m] = i;       
				}
				else 
					u[--ul] = i;
			}
		}
		else { 
			u[--ul] = i;
		}
	}
	ul = p->dimensione - ll;
	insertionSortWithValues(u, ul, p);
	if (limiteSpostamenti < 0) {
		
		spostamenti = tempDouble = F * ul;
		if (spostamenti < tempDouble) spostamenti++;
	}
	else {
		spostamenti = limiteSpostamenti;
		if (spostamenti > ul) spostamenti = ul;
	}
	j = 0;
	for (i = 0; i < ul; i++) {
		while (j < ll && p->individuo[lis[j]] < p->individuo[u[i]])
			j++;
		su[i] = j<ll&& p->individuo[lis[j]]>p->individuo[u[i]] ? j : ll;
	}

	w = 0;
	
	while (w < spostamenti) { 
		m = 0;                                     
		for (i = 0; i < ul; i++) {
			b = su[i] == ll ? p->dimensione : lis[su[i]];       
			a = su[i] == 0 ? 0 : lis[su[i] - 1];    
			y = a == b ? 1 : b - a;                  
			m += y;                                
			if (r.randIntU(0, m - 1) < y)                     
				k = i;                          
		}
		i = u[k]; 
		b = su[k] == ll ? p->dimensione : lis[su[k]];           
		a = su[k] == 0 ? 0 : lis[su[k] - 1];         
		j = a == b ? a : (a + (i < a ? 0 : 1) + r.randIntU(0, b - a - 1));
		risultato[w].x = i;	
		risultato[w].y = j; 
		w++;
		a = su[k];  
		if (a < ll)   
			memmove(lis + a + 1, lis + a, sizeof(unsigned short) * (ll - a));
		ll++;
		lis[a] = j;
		
		ul--;
		if (k < ul) {
			memmove(u + k, u + k + 1, sizeof(unsigned short) * (ul - k));
			memmove(su + k, su + k + 1, sizeof(unsigned short) * (ul - k));
		}
		
		y = p->individuo[i];

		if (i < j) {

			for (; --a >= 0 && lis[a] > i;)  
				lis[a]--;
			for (a = 0; a < ul; a++)
				if (u[a] > i && u[a] <= j)
					u[a]--;
			memmove(p->individuo + i, p->individuo + i + 1, sizeof(unsigned short) * (j - i));
		}
		else {
			for (; ++a < ll && lis[a] < i;) 
				lis[a]++;
			for (a = 0; a < ul; a++)
				if (u[a] >= j && u[a] < i)
					u[a]++;

			memmove(p->individuo + j + 1, p->individuo + j, sizeof(unsigned short) * (i - j));
		}
		for (a = k; a < ul; a++)
			su[a]++;
		p->individuo[j] = y;
	} 

	delete[] lis;
	delete[] u;
	delete[] su;
	delete[] t;
	delete[] q;
}

void PermutazioneI::inserisci(PermutazioneI* p, Coppia& c) {

	unsigned short tmp = p->individuo[c.x];

	if (c.x < c.y) {
		memmove(&p->individuo[c.x], &p->individuo[c.x+1], sizeof(unsigned short) * (c.y - c.x));
	}
	else {
		memmove(&p->individuo[c.y+1], &p->individuo[c.y], sizeof(unsigned short) * (c.x - c.y));
	}

	p->individuo[c.y] = tmp;
}

unsigned short PermutazioneI::ricercaBinaria(unsigned short* vOrdinato, unsigned short lunghezza, 
	unsigned short valore) {

	unsigned short inizio = 0, fine = lunghezza;
	unsigned int i;

	while (inizio < fine) {
		i = (inizio + fine) / 2;
		if (valore > vOrdinato[i])
			inizio = i + 1;
		else fine = i;
	}

	return inizio;
}

void PermutazioneI::insertionSortWithValues(unsigned short* a, unsigned short l, PermutazioneI* v) {
	int i, j, t;
	for (i = 1; i < l; i++) {
		t = a[i];
		for (j = i - 1; j >= 0 && v->individuo[a[j]] > v->individuo[t]; j--)
			a[j + 1] = a[j];
		a[j + 1] = t; 
	}
}

void PermutazioneI::createLengthsArray(PermutazioneI* p, unsigned short* l, unsigned short* q, unsigned short& llis, int& imax) {
	int ql = 0, y, i, j;
	//init imax and llis
	llis = 0;
	imax = -1; //it avoids compiler complaints
	//scan the array (left-to-right) in order to: set l[x[i]], update llis and imax
	for (i = 0; i < p->dimensione; i++) {
		//y is the current value
		y = p->individuo[i];
		//find position j where to insert y in q
		j = ricercaBinaria(q, ql, y);
		//replace q[j] with y and increase ql if j==ql
		q[j] = y;
		if (j == ql) ql++;
		//set l[y]
		l[y] = j == 0 ? 1 : 1 + l[q[j - 1]];
		//update llis and imax
		if (l[y] >= llis) {
			llis = l[y];
			imax = i;
		}
	}
}

unsigned short PermutazioneI::lisLength(PermutazioneI* p) {
	unsigned short llis;
	int  imax;

	unsigned short* l = new unsigned short[p->dimensione];
	unsigned short* q = new unsigned short[p->dimensione];

	createLengthsArray(p, l, q, llis, imax);

	delete[] l;
	delete[] q;
	return llis;
}


unsigned short PermutazioneI::insWeight(PermutazioneI* p) {
	return p->dimensione - lisLength(p);
}
