#include "TCB.h"
#include <iostream>
 
TCB::TCB(string ID, string c, int init, int dur, int prio):
	id(ID), duration(dur)
{
	// cria tarefa com os dados correspondentes
	state = States::New;
	color = c;
	ingress_time = init;
	current_time = 0;
	priority = prio;
	dynamic_priority = priority;
	events.clear();
}

TCB::~TCB(){
    removeAllEvents();
	events.clear();
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

string TCB::getColor(){
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
	// retorna a prioridade estatica da tarefa
	return priority;
}

void TCB::setDynamicPriority(int d){
	dynamic_priority = d;
}

int TCB::getDynamicPriority(){
	return dynamic_priority;
}

void TCB::addEvent(int init_time, int t, int dur, string e_id){
	Event* ev = new Event();
	ev->i_time = init_time;
	ev->id = e_id;
	ev->type = static_cast<EventType>(t);
	ev->duration = dur;
	events.push_back(ev);
}

void TCB::removeAllEvents(){
	// remove eventos
	if(!events.empty()){
		for (const auto& event : events){
			delete event;
		}
	}
}

Event* TCB::poolEvents()
{
	for (const auto& event : events){
		if (event != NULL){
			if (event->i_time >= current_time)
				return event;
		}
	}
	return NULL;
}

