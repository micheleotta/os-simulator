#include "System.h"

System::System(string st, int q):
	scheduler_type(st), quantum(q), current_task(NULL)
{
}
	
System::~System(){}

TCB* System::scheduler_next(){
	// implementar algoritmos de escalonamento -> switch case ponto de interrogacao
	
	// por hora retornar current p n dar erro
	return current_task;
}

// tempo System::sys_clock(){}

void System::interrupt(){
	
}
		
void System::task_ready(TCB* t){
	if (!t) return;
	
	// retira t da lista waiting	
	auto it = find(waiting.begin(), waiting.end(), t);
    if (it != waiting.end()) {
        waiting.erase(it);
    }
	
	// adiciona t a lista de prontas
	ready.push_back(t);
}

void System::task_sleep(TCB* t){
	if (!t) return;
	
	// retira t da lista de prontas
	auto it = find(ready.begin(), ready.end(), t);
    if (it != ready.end()) {
        ready.erase(it);
    }
    
	// insere t na lista de waiting
	waiting.push_back(t);
}

void System::run(){
	int current_quantum = 0; // criar na classe maybe
	
	// incrementa no current_time ++
	current_task->setCurrentTime(current_task->getCurrentTime() + 1);
	
	// se tarefa ja executou tudo
	if(current_task->getCurrentTime() == current_task->getDuration()){
		current_task->setState(States::Terminated); // estado de terminada
		current_task = scheduler_next(); // seleciona a proxima tarefa a executar
	}
	// se quantum encerrou
	else if(current_quantum >= getQuantum()){
		// desativa a tarefa atual
		task_sleep(current_task);
		current_task = scheduler_next(); // seleciona a proxima tarefa a executar
	}
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
