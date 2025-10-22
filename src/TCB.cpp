#include "TCB.h"
 
TCB::TCB(string ID, int init, int dur, int prio):
	id(ID), duration(dur)
{
	state = States::New;
	// color
	ingress_time = init;
	current_time = 0;
	priority = prio;
}

TCB::~TCB(){
	
}

const string TCB::getId(){
	return id;
}

int TCB::getStateInt(){
	return static_cast<int>(state);
}

States TCB::getState(){
	return state;
}

void TCB::setState(States s){
	state = s;
}

int TCB::getIngressTime(){
	return ingress_time;
}

int TCB::getCurrentTime(){
	return current_time;
}

void TCB::setCurrentTime(int t){
	current_time = t;
}

const int TCB::getDuration(){
	return duration;
}

int TCB::getPriority(){
	return priority;
}

void TCB::addEvent(string ev){
	events.push(ev);
}

void TCB::removeEvent(){
	if(!events.empty())
		events.pop();
}

queue<string> TCB::getEventQueue(){
	return events;
}
