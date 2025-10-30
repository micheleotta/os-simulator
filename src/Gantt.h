#pragma once
#include "TCB.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

struct Interval {
    int start;
    int end;
};

struct TaskInfo {
    TCB* task;
    vector<Interval> intervals; // intervalos de execução
    int endtime;
};

// Códigos de cores ANSI (funcionam no Linux, macOS e Windows 10+)
const vector<string> COLORS = {
    "\033[31m", // vermelho
    "\033[32m", // verde
    "\033[33m", // amarelo
    "\033[34m", // azul
    "\033[35m", // magenta
    "\033[36m", // ciano
    "\033[37m"  // branco
};
const string RESET = "\033[0m";

class Gantt{ 
	private:
		vector<TaskInfo> tasks;
		
	public:
		Gantt(vector<TCB*> ts);
		~Gantt();
		void insertInterval(TCB* t, int s, int e);
		void plotChart();
		void exportImg();
};
