#pragma once

#include <queue>
#include <string>
using namespace std;

enum States { New = 1, Ready = 2, Running = 3, Waiting = 4, Terminated = 5 };

class TCB{
	private:
		const string id;
		States state;
		// color
		int ingress_time;
		const int duration;
		int priority;
		queue<string> events;
		// TCB pointer ou struct 
		
	public:
		TCB(string ID = "None", int init = -1, int dur = -1, int prio = -1);
		~TCB();
		const string getId();
		int getStateInt();
		States getState();
		void setState(States s);
		int getIngressTime();
		int getCurrentTime();
		void setCurrentTime(int t);
		const int getDuration();
		int getPriority();
		void addEvent(string ev);
		void removeEvent(string ev);
		queue<string> getEventQueue();
};
