#pragma once
#include "TCB.h"

#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

//algoritmos de escalonamento do sistema
enum class SchedulerType {FIFO, SRTF, PRIOP};

class System{
	 
	 private:
	 	const int quantum;
		vector<TCB*> ready;		//tarefas prontas para execução
		vector<TCB*> waiting;	//tarefas em suspensão
		SchedulerType scheduler_type;
		TCB* current_task;
		int current_quantum;

		void define_scheduler_type (string st);
		
	public:
		System(string st = "FCFS", int q = 2); //valores default
		~System();
		void scheduler_next(); // chamada do escalonador
		void task_ready(TCB* t);
		void task_sleep(TCB* t);
		void update();
		bool finished();
		int getQuantum();
		TCB* getCurTask();
};
