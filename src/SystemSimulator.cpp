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
	sim_type = SimulationType::PassoaPasso;
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
        
        // adiciona as tarefas na lista de tarefas do sistema
        // e tambem nas tarefas ainda nao adicionadas ao sistema
        sys_tasks.push_back(new_task);
        remaining_tasks.push_back(new_task);
    }

    config_file.close();
}

void SystemSimulator::run(){
	
	// apagar essa bomba depois
	int tempo_temporario = 0;
		
	while(!system->finished() or !remaining_tasks.empty()){
		
		// checa se ainda ha tarefas a serem adicionadas no sistema
		if(!remaining_tasks.empty()){
			
			// para comparar ao final se tarefas foram adicionadas ao sistema
			long unsigned int qtd_remaining = remaining_tasks.size();
			
			for (auto it = remaining_tasks.begin(); it != remaining_tasks.end();) {
				TCB* task = *it;
				
				// se o tempo corresponde ao tempo de inicio da tarefa,
				// adiciona-a no sistema e na fila de tarefas prontas
				if (tempo_temporario >= task->getIngressTime()){
					system->task_ready(task);
					// retira tarefa das remaining_tasks			
					remaining_tasks.erase(it);
				}
				else{ 
					++it;
				}
			}
			
			if(qtd_remaining != remaining_tasks.size()){
				// entrada de nova tarefa -> syscall
				// chama o escalonador para eleger a tarefa a executar
				system->scheduler_next();
			}

		}
			
		// APAGAR
		cout << "tick: " << tempo_temporario;
		
		system->run();
		
		// FAZER		
		// if(sim_type == SimulationType::PassoaPasso){
			// grafico do tick atual
		// }
		
		
		tempo_temporario++;
	}
	
	// gerar o grafico final
}

void SystemSimulator::setSimType(int st){
	if (st == 1){
		sim_type = SimulationType::PassoaPasso;
	}
	else{
		sim_type = SimulationType::Completa;
	}
}
