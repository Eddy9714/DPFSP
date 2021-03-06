#pragma once
#include <string>
using namespace std;


class Istanza {
	public:
		unsigned short macchine;
		unsigned short fabbriche;
		unsigned short lavori;

		unsigned int** p;

		Istanza(string path);
		~Istanza();
};