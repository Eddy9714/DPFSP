#pragma once
#include "Istanza.h";
#include "Individuo.h";

class ADE {

	protected:
		virtual Individuo** creaPopolazione(unsigned short) = 0;
		virtual void eliminaPopolazione(Individuo**, unsigned short) = 0;
		virtual void inizializzaPopolazione(Individuo**, unsigned short) = 0;
		virtual unsigned int valutaIndividuo(Individuo*) = 0;
		unsigned int esegui(unsigned short, unsigned short);

};