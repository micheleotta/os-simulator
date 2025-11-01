#pragma once
#include <chrono>
#include <iostream>
using namespace std;

class SymClock{
	private:
		chrono::time_point<chrono::steady_clock> start_time;
		chrono::time_point<chrono::steady_clock> current_time;
		chrono::nanoseconds elapsed_time;
		int sim_time; //nº ticks da simulação ao todo
		const int period;
			
	public:
		SymClock();
		~SymClock();
		bool ticked();
		const int get_clock_period();
		int get_simulation_time();
};

