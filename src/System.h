#pragma once
#include "TCB.h"
#include "Clock.h"

#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

enum class SchedulerType {FIFO, SRTF, PRIOP};

class System{
	 
	 private:
		vector<TCB*> ready;
		vector<TCB*> waiting;
		SchedulerType scheduler_type;
		const int quantum;
		TCB* current_task;
		int current_quantum;
		Clock* global_clock;
		
	public:
		System(string st = "FCFS", int q = 2);
		~System();
		void scheduler_next();
		// tempo sys_clock()
		void interrupt();
		void task_ready(TCB* t);
		void task_sleep(TCB* t);
		void run();
		bool finished();
		int getQuantum();
		TCB* getCurTask();
};
