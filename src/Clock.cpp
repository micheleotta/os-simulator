#include "Clock.h"

// um clock de processador tem em média 100 ns de periodo
Clock::Clock():
	start_time(chrono::system_clock::now()), period(100)
{
	current_time = start_time;
	raising_edge = false;
	is_running = true;
}
		
Clock::~Clock(){}

bool Clock::get_raising_edge(){
	return raising_edge;
}

void Clock::run(){
	while(is_running){
		tick();
	}
}

void Clock::tick(){
	auto current_time = chrono::system_clock::now();
	elapsed_time = current_time - start_time;
	int n_ticks = (int) elapsed_time.count();
	if (n_ticks > period){
		raising_edge = true;
		start_time = current_time;
	}
	else{
		raising_edge = false;
	}
}
