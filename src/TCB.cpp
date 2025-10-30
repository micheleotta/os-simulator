#include "TCB.h"
 
TCB::TCB(string ID, int c, int init, int dur, int prio):
	id(ID), duration(dur)
{
	// cria tarefa com os dados correspondentes
	state = States::New;
	color = c;
	ingress_time = init;
	current_time = 0;
	priority = prio;
}

TCB::~TCB(){
  while (!events.empty()) {
    removeEvent();
  }
}

const string TCB::getId(){
	// retorna o id da tarefa
	return id;
}

int TCB::getStateInt(){
	// retorna o estado da tarefa como inteiro
	return static_cast<int>(state);
}

States TCB::getState(){
	// retorna o estado da tarefa como enum
	return state;
}

void TCB::setState(States s){
	// define o estado da tarefa
	state = s;
}

int TCB::getColor(){
	// retorna a cor da tarefa
	return color;
}

int TCB::getIngressTime(){
	// retorna o tempo de ingresso da tarefa
	return ingress_time;
}

int TCB::getCurrentTime(){
	// retorna quanto tempo a tarefa ja executou 
	return current_time;
}

void TCB::setCurrentTime(int t){
	// atualiza quanto tempo a tarefa executou
	current_time = t;
}

const int TCB::getDuration(){
	// retorna a duracao da tarefa
	return duration;
}

int TCB::getPriority(){
	// retorna a prioridade da tarefa
	return priority;
}

void TCB::addEvent(string ev){
	// adiciona evento da tarefa
	events.push(ev);
}

void TCB::removeEvent(){
	// remove evento
	if(!events.empty())
		events.pop();
}

queue<string> TCB::getEventQueue(){
	// retorna eventos da tarefa
	return events;
}
