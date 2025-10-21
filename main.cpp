#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define CONF_FILE "conf.txt"


int main(){
	
	// Leitura do arquivo de configuracao 
	ifstream config_file(CONF_FILE);
    if (!config_file.is_open()) {
        cerr << "Error " << CONF_FILE << endl;
        return 1;
    }
    
	string s;
	string alg_escalonamento;
	int quantum;
	// Le a primeira linha com o algoritmo de escalonamento e o valor do quantum
	if(getline(config_file, s)){
		stringstream ss(s);
        getline(ss, alg_escalonamento, ';');
        ss >> quantum;
	}
	string id;
	int ingress_time, duration, priority;
    string events;
    // Le as linhas das tarefas
    while (getline(config_file, s)) {
        stringstream ss(s);
        getline(ss, id, ';');
        ss >> ingress_time;
        ss.ignore(); // passa ';'
        ss >> duration;
        ss.ignore();
        ss >> priority;

		// Le os eventos
        while (getline(ss, events, ';')) {
            if (!events.empty())
				// adicionar no queue task_events
				break;
        }
    }

    config_file.close();
	
	return 0;
}
