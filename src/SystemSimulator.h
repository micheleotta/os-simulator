#pragma once
#include "SymClock.h"
#include "System.h"
#include "Gantt.h"

#include <fstream>
#include <sstream>
#include <string>

#define CONF_FILE "conf.txt"

enum class SimulationType {DebugMode, Complete};

class SystemSimulator{
	 
	 private:
		System* system;
		Gantt* gantt;
		SymClock* m_clock;
		vector<TCB*> sys_tasks;			//lista de tarefas indicadas
		vector<TCB*> remaining_tasks;	//tarefas ainda não colocadas no sistema
		string config_path;
		SimulationType sim_type;
		
		// Singleton
		SystemSimulator();
		static SystemSimulator* instancia_SS;
		bool create_system();
		long unsigned int check_remaining_tasks(int time);
		void run();
		
	public:
		static SystemSimulator* getSystemSimulator();
		~SystemSimulator();
		void Create();
		void setSimType(int st = 1);
};
