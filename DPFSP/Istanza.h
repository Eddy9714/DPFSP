#pragma once
#include <string>
using namespace std;


class Istanza {
	public:
		unsigned short macchine;
		unsigned short fabbriche;
		unsigned short lavori;

		unsigned int** p;
		unsigned int* minp;

		Istanza(string path);
		~Istanza();
};