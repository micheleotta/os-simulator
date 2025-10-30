#include "System.h"

System::System(string st, int q):
	quantum(q), current_task(NULL), current_quantum(0)
{
	global_clock = new Clock();
	
	if (st == "FCFS") scheduler_type = SchedulerType::FCFS;
	else if (st == "SRTF") scheduler_type = SchedulerType::SRTF;
	else scheduler_type = SchedulerType::PRIOP;
}
	
System::~System(){}

void System::scheduler_next(){
	// para o primeiro trabalho, as tarefas em espera
	// so esperam o processador. portanto, aqui elas ja
	// sao colocadas como prontas
	for (const auto& waiting_task : waiting) {
        task_ready(waiting_task);
    }
    waiting.clear();
    
    // reseta o quantum
    current_quantum = 0;
    
    // se não houver tarefas prontas, não escolhe nada
    if (ready.empty()){
        current_task = nullptr;
        return;
    }
    
    // escolhe conforme algoritmo
    TCB* next_task = nullptr;
	
	// implementar algoritmos de escalonamento
	switch(scheduler_type){
		case SchedulerType::FCFS: {
			// atender a ordem das tarefas prontas
			next_task = ready.front();
			break;
		}
		case SchedulerType::SRTF: {
			// proximo a executar -> menor tempo restante
			next_task = ready.front();
			int min_time = next_task->getDuration() - next_task->getCurrentTime();
			for (const auto& task : ready) {
				int remaining_time = task->getDuration() - task->getCurrentTime();
				if(remaining_time < min_time){
					min_time = remaining_time;
					next_task = task;
				}
			}			
			break;
		}
		case SchedulerType::PRIOP: {
			// proximo a executar -> maior prioridade
			next_task = ready.front();
			for (const auto& task : ready) {
				if(task->getPriority() > next_task->getPriority()){
					next_task = task;
				}
			}
			break;
		}
		default: {
			// em default, retorna a primeira na fila de tarefas prontas
			next_task = ready.front();
			break;
		}
	}
	
	current_task = next_task;
}

// tempo System::sys_clock(){}

void System::interrupt(){
	
}
		
void System::task_ready(TCB* t){
	if (!t) return;
	
	// não re-adicionar tarefas já terminadas
    if (t->getState() == States::Terminated) return;
	
	// retira t da lista waiting	
	auto it = find(waiting.begin(), waiting.end(), t);
    if (it != waiting.end()) {
        waiting.erase(it);
    }
	
	// adiciona t a lista de prontas
	t->setState(States::Ready);
	// se ja nao esta em ready, adiciona
	auto itr = find(ready.begin(), ready.end(), t);
    if (itr == ready.end()) {
        ready.push_back(t);
    }
}

void System::task_sleep(TCB* t){
	if (!t or scheduler_type == SchedulerType::FCFS) return;
	
	// retira t da lista de prontas
	auto it = find(ready.begin(), ready.end(), t);
    if (it != ready.end()) {
        ready.erase(it);
    }
    
	// insere t na lista de waiting
	t->setState(States::Waiting);
	waiting.push_back(t);
}

void System::run(){	
	
	if(current_task){
		// se tarefa ja executou tudo
		if(current_task->getCurrentTime() >= current_task->getDuration()){
			current_task->setState(States::Terminated); // estado de terminada
			// retira tarefa da lista de prontas
			auto it = find(ready.begin(), ready.end(), current_task);
			if (it != ready.end()) {
				ready.erase(it);
			}
			current_task = nullptr;
			scheduler_next(); // seleciona prox tarefa a executar
		}
		// se quantum encerrou, sai por preempcao
		// excecao de FCFS que nao eh preemptivo
		else if(current_quantum >= getQuantum() && scheduler_type != SchedulerType::FCFS){
			// desativa a tarefa atual
			task_sleep(current_task);
			current_task = NULL;
			scheduler_next(); // seleciona prox tarefa a executar
		}
	}	
	if(!current_task) { // se nao ha tarefa atual, elege uma
		scheduler_next();
		if(!current_task) return; // prevenir erros
	}
	
	// roda a tarefa atual!
	current_task->setState(States::Running);
	// incrementa no current_time ++
	current_task->setCurrentTime(current_task->getCurrentTime() + 1);
	current_quantum++; // tambem incrementa considerando o quantum
}
		
bool System::finished(){
	return waiting.empty() && ready.empty();
}
		
int System::getQuantum(){
	return quantum;
}

TCB* System::getCurTask(){
	return current_task;
}
