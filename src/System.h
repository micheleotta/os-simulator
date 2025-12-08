#pragma once
#include "Mutex.h"
#include "TCB.h"

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;

//algoritmos de escalonamento do sistema
enum class SchedulerType {FIFO, SRTF, PRIOP, PRIOPEnv};

class System{
	 
	 private:
	 	const int quantum;
	 	const int alpha;
		vector<TCB*> ready;		//tarefas prontas para execução
		vector<TCB*> waiting;	//tarefas em suspensão
		bool call_scheduler;
		bool call_aging;
		SchedulerType scheduler_type;
		TCB* current_task;
		int current_quantum;
		map<string,Mutex> mutexes;	//travas do sistema
		//define o algoritmo de escalonamento utilizado com base na config
		void define_scheduler_type (string st);
		
	public:
		System(string st = "FCFS", int q = 2, int a = 1); //valores default
		~System();
		// chamada do escalonador
		void scheduler_next(TCB* prev = nullptr); 
		TCB* better_choice(TCB* a, TCB* b, TCB* prev);
		//atualiza o sistema a cada tick da simulacao
		void update(); 
		bool finished();
		int getQuantum();
		TCB* getCurTask();
		void task_ready(TCB* t);
		void task_sleep(TCB* t);
		void plot_tasks();
		SchedulerType get_scheduler_type();
		string get_scheduler_name();
		void set_call_scheduler(bool c);
		void set_call_aging(bool a);
		void aging(TCB* current);
};
