#pragma once

#include <vector>
#include <string>
using namespace std;

//estados possiveis de uma tarefa
enum States { New = 1, Ready = 2, Running = 3, Waiting = 4, Terminated = 5 };

//tipos de eventos para uma tarefa
enum EventType{ NONE = -1, ML = 1, MU = 2, IO = 3};

struct Event {
	int i_time;
	EventType type;
	int duration; //para mutexes = -1 (indeterminado)
	string id; //para IO id = "io" por padrão
};

class TCB{
	private:
		//dados da tarefa
		const string id;
		States state;
		string color;
		int ingress_time;
		int current_time;
		const int duration;
		int priority;
		vector<Event*> events;
		
	public:
		TCB(string ID = "None", string c = "F255E0D0", int init = -1, int dur = -1, int prio = -1);
		~TCB();
		//sets e gets
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
		//eventos
		void addEvent(int init_time, int t, int dur, string e_id);
		void removeAllEvents();
		Event* poolEvents ();
};
