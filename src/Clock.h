#pragma once

#include <chrono>
using namespace std;

class Clock{
	private:
		bool ticked;
		chrono::time_point<chrono::system_clock> start_time;
		chrono::time_point<chrono::system_clock> current_time;
		const double period;
		chrono::nanoseconds elapsed_time;
		bool raising_edge;
		bool is_running;
		
		void run();
		void tick();
			
	public:
		Clock();
		~Clock();
		bool get_raising_edge();	
};
