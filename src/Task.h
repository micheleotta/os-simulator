#pragma once

#include <queue>
#include <string>
using namespace std;

enum States { New = 1, Ready = 2, Running = 3, Waiting = 4, Terminated = 5 };

class Task{
	private:
		string id;
		States state;
		// color
		int ingress_time;
		int duration;
		int priority;
		queue<string> events;
		// TCB pointer
		
	public:
		Task(string ID = "None", States s = 1, int init = -1, int dur = -1, int prio = -1, queue<string> events = NULL);
		~Task();
		string getId();
		int getState();
		States getState();
		void setState(States s);
		int getIngressTime();
		int getPriority();
		void addEvent(string ev);
		void removeEvent(string ev);
		queue<string> getEventQueue();
};
