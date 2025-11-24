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

const string RESET = "\033[0m";

class Gantt{ 
	private:
		vector<TaskInfo> tasks;
		
	public:
		Gantt(vector<TCB*> ts);
		~Gantt();
		void insertInterval(TCB* t, int s, int e);
		void plotChart(int total_time, string type);
		void exportImg(int total_time, string type = "", string file_name = "simulacao_");
		string rgbToSvg(const string& rgb);
};
