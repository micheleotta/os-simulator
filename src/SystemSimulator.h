#pragma once
#include "System.h"
#include "Gantt.h"

#include <fstream>
#include <sstream>
#include <string>

#define CONF_FILE "conf.txt"

enum class SimulationType {PassoaPasso, Completa};

 class SystemSimulator{
	 
	 private:
		System* system;
		Gantt* gantt;
		vector<TCB*> sys_tasks;
		vector<TCB*> remaining_tasks;
		string config_path;
		SimulationType sim_type;
		
		// Singleton
		SystemSimulator();
		static SystemSimulator* instancia_SS;
		void create_system();
		void run();
		
	public:
		static SystemSimulator* getSystemSimulator();
		~SystemSimulator();
		void Create();
		void setSimType(int st = 1);
};
