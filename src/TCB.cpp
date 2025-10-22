#include "TCB.h"

TCB(string ID = "None", int init = -1, int dur = -1, int prio = -1){
	id = ID;
	state = States::New;
	// color
	ingress_time = init;
	current_time = 0;
	duration = dur;
	priority = prio;
}

~TCB(){
	
}

const string getId(){
	return id;
}

int getStateInt(){
	return (int)state;
}

States getState(){
	return state;
}

void setState(States s){
	state = s;
}

int getIngressTime(){
	return ingress_time;
}

int getCurrentTime(){
	return current_time;
}

void setCurrentTime(int t){
	current_time = t;
}

const int getDuration(){
	return duration;
}

int getPriority(){
	return priority;
}

void addEvent(string ev){
	events.append(ev);
}

void removeEvent(string ev){
	events.remove(ev);
}

queue<string> getEventQueue(){
	return events;
}
