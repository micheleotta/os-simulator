#pragma once

#include <queue>
#include <string>
using namespace std;

//estados possiveis de uma tarefa
enum States { New = 1, Ready = 2, Running = 3, Waiting = 4, Terminated = 5 };

class TCB{
	private:
		// dados da tarefa
		const string id;
		States state;
		string color;
		int ingress_time;
		int current_time;
		const int duration;
		int priority;
		int dynamic_priority;
		queue<string> events;
		
	public:
		TCB(string ID = "None", string c = "F255E0D0", int init = -1, int dur = -1, int prio = -1);
		~TCB();
		// sets e gets
		const string getId();
		int getStateInt();
		States getState();
		void setState(States s);
		string getColor();
		int getIngressTime();
		int getCurrentTime();
		void setCurrentTime(int t);
		const int getDuration();
		int getPriority();
		void setDynamicPriority(int d);
		int getDynamicPriority();
		queue<string> getEventQueue();
		// eventos
		void addEvent(string ev);
		void removeEvent();
};
