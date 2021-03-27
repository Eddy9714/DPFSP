#pragma once
#include <string>
#include <fstream>
#include <iostream>

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