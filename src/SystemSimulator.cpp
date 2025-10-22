#include "SystemSimulator.h"

SystemSimulator* SystemSimulator::instancia_SS(NULL);
		
SystemSimulator::SystemSimulator(){
	config_path = CONF_FILE;
}

// Singleton
SystemSimulator* SystemSimulator::getSystemSimulator(){
	if(instancia_SS == NULL)
		instancia_SS = new SystemSimulator();
	return instancia_SS;
}

SystemSimulator::~SystemSimulator(){

}

void SystemSimulator::Create(){
	create_system();
	run();
	
}
		
void SystemSimulator::create_system(){
	// Leitura do arquivo de configuracao 
	ifstream config_file(config_path);
    if (!config_file.is_open()) {
        cerr << "Error " << config_path << endl;
        return;
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
            if (!events.empty()){
				// adicionar no queue task_events
				break;
			}
        }
    }

    config_file.close();
}

void SystemSimulator::run(){
	//fazer
	while(!finished()){
		break;
	}
}

bool SystemSimulator::finished(){
	return true; // mudar
}
