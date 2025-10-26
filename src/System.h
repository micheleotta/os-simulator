#pragma once
#include "TCB.h"

#include <vector>
#include <algorithm>
using namespace std;


class System{
	 
	 private:
		vector<TCB*> ready;
		vector<TCB*> waiting;
		const string scheduler_type;
		const int quantum;
		TCB* current_task;
		// tempo*  global_clock
		
	public:
		System(string st = "FCFS", int q = 2);
		~System();
		TCB* scheduler_next();
		// tempo sys_clock()
		void interrupt();
		void task_ready(TCB* t);
		void task_sleep(TCB* t);
		void run();
		bool finished();
		int getQuantum();
		TCB* getCurTask();
};
