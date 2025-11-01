#include "SymClock.h"


// o clock está definido para tickar a cada 0.01 segundos
// 1 segundo -> 1000 milisegundos

//obs: usando steady_clock, clocks diferentes não são compatíveis entre si!
//steady_clock é o relógio recomendado para operações de tempo

SymClock::SymClock():
	start_time(chrono::steady_clock::now()), period(10)
{
	current_time = start_time;
	sim_time = 0; // quantos ticks já ocorreram desde o ínicio da simulação

}
		
SymClock::~SymClock(){}

bool SymClock::ticked(){
	auto current_time = chrono::steady_clock::now();
	elapsed_time = current_time - start_time;

	//converte para milisegundos
	int n_ticks = (int) elapsed_time.count();
	n_ticks /= 1000000;

	if (n_ticks >= period){
		sim_time = sim_time + 1;
		start_time = current_time;
		return true;
	}

	return false;
}

const int SymClock::get_clock_period()
{
    return period;
}

int SymClock::get_simulation_time()
{
    return sim_time;
}
