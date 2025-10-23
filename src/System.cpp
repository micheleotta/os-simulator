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
		
void System::task_ready(TCB t){
	// if t na lista waiting -> remove
	
	
	// adiciona t a lista de prontas
	ready.push(t);
}

void System::task_sleep(TCB t){
	// retira t da lista de prontas
	
	// insere t na lista de waiting
	waiting.push(t);
}

void System::run(){
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
