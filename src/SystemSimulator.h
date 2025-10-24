#pragma once
#include "System.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define CONF_FILE "conf.txt"


 class SystemSimulator{
	 
	 private:
		System* system;
		vector<int> myVector;
		vector<TCB> sys_tasks;
		vector<TCB> remaining_tasks;
		string config_path;
		// sim type enum ()
		// tempo  global_clock
		
		// Singleton
		SystemSimulator();
		static SystemSimulator* instancia_SS;
		void create_system();
		void run();
		bool finished();
		
	public:
		static SystemSimulator* getSystemSimulator();
		~SystemSimulator();
		void Create();
		// getGlobalClock()
};
