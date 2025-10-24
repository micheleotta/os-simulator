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
	string scheduler_type;
	int quantum;
	// Le a primeira linha com o algoritmo de escalonamento e o valor do quantum
	if(getline(config_file, s)){
		stringstream ss(s);
        getline(ss, scheduler_type, ';');
        ss >> quantum;
	}
	system = new System(scheduler_type, quantum);
	
	TCB* new_task = NULL;
	string id;
	int ingress_time, duration, priority;
    string events;
    // Le as linhas das tarefas
    while (getline(config_file, s)) {
        stringstream ss(s);
        getline(ss, id, ';');
        // ss >> cor;
        // ss.ignore(); // passa ';'
        ss >> ingress_time;
        ss.ignore(); // passa ';'
        ss >> duration;
        ss.ignore();
        ss >> priority;
        
        new_task = new TCB(id, ingress_time, duration, priority);

		// Le os eventos
        while (getline(ss, events, ';')) {
            if (!events.empty()){
				// adicionar no queue task_events
				new_task->addEvent(events);
			}
        }
        
        sys_tasks.push_back(new_task);
        remaining_tasks.push_back(new_task);
    }

    config_file.close();
}

void SystemSimulator::run(){
	//fazer
	while(!system->finished()){
		
		// checa se ainda ha tarefas a serem adicionadas no sistema
		if(!remaining_tasks.empty()){
		
		// if (tempo global >= inicio tarefa){
			// system->task_ready(tarefa);
			// retira tarefa das remaining_tasks
			// auto it = find(remaining_tasks.begin(), remaining_tasks.end(), tarefa);
			// if (it != remaining_tasks.end()) {
				// remaining_tasks.erase(it);
			// }
		// }
		
		}
		
		break;
	}
}
