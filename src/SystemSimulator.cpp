#include "SystemSimulator.h"

SystemSimulator* SystemSimulator::instancia_SS(NULL);
		
SystemSimulator::SystemSimulator(){
	// define o caminho do arquivo .txt de configuracao
	config_path = CONF_FILE;
	// por padrao, deixa o tipo de simulacao passo a passo
	sim_type = SimulationType::PassoaPasso;
}

// Singleton
SystemSimulator* SystemSimulator::getSystemSimulator(){
	if(instancia_SS == NULL)
		instancia_SS = new SystemSimulator();
	return instancia_SS;
}

SystemSimulator::~SystemSimulator(){
	// destrutora
	system = nullptr;
	gantt = nullptr;
	
	for (int i = 0; i < (int)sys_tasks.size(); i++) {
		delete(sys_tasks[i]);
	}
	sys_tasks.clear();
	remaining_tasks.clear();
}

void SystemSimulator::Create(){
	create_system();
	run();
}
		
void SystemSimulator::create_system(){
	
	// leitura do arquivo de configuracao 
	ifstream config_file(config_path);
	// no caso de erro
    if (!config_file.is_open()) {
        cerr << "Error " << config_path << endl;
        return;
    }
    
	string s;
	string scheduler_type;
	int quantum;
	// le a primeira linha com o algoritmo de escalonamento e o valor do quantum
	if(getline(config_file, s)){
		stringstream ss(s);
        getline(ss, scheduler_type, ';');
        ss >> quantum;
	}
	// cria sistema com o tipo de escalonamento e valor do quantum
	system = new System(scheduler_type, quantum);
	
	TCB* new_task = NULL;
	string id;
	int color, ingress_time, duration, priority;
    string events;
    // le as linhas das tarefas
    while (getline(config_file, s)) {
        stringstream ss(s);
        getline(ss, id, ';');
        ss >> color;
        ss.ignore(); // passa ';'
        ss >> ingress_time;
        ss.ignore();
        ss >> duration;
        ss.ignore();
        ss >> priority;
        
        new_task = new TCB(id, color, ingress_time, duration, priority);

		// le os eventos
        while (getline(ss, events, ';')) {
            if (!events.empty()){
				// adicionar no queue task_events da tarefa
				new_task->addEvent(events);
			}
        }
        
        // adiciona as tarefas na lista de tarefas do SystemSimulator
        // e tambem nas tarefas ainda nao adicionadas ao sistema
        sys_tasks.push_back(new_task);
        remaining_tasks.push_back(new_task);
    }

	// apos leitura, fecha o arquivo
    config_file.close();
}

void SystemSimulator::run(){
	
	// passa todas as tarefas para gerar o grafico de gantt
	gantt = new Gantt(sys_tasks);
	
	// apagar essa bomba depois -> CLOCK
	int tempo_temporario = 0;
		
	// executar enquanto ainda ha tarefas para serem executadas
	while(!system->finished() or !remaining_tasks.empty()){
		
		// checa se ainda ha tarefas a serem adicionadas no sistema
		if(!remaining_tasks.empty()){
			
			// para comparar ao final se tarefas foram adicionadas ao sistema no tick atual
			long unsigned int qtd_remaining = remaining_tasks.size();
			
			for (auto it = remaining_tasks.begin(); it != remaining_tasks.end();) {
				TCB* task = *it;
				
				// se o tempo corresponde ao tempo de inicio da tarefa,
				// adiciona-a no sistema e na fila de tarefas prontas
				if (tempo_temporario >= task->getIngressTime()){
					system->task_ready(task);
					// retira tarefa das remaining_tasks			
					it = remaining_tasks.erase(it); // retorna o proximo iterador
				}
				else{ 
					++it;
				}
			}
			
			if(qtd_remaining != remaining_tasks.size()){
				// se entrada de nova tarefa -> syscall
				// chama o escalonador para eleger a tarefa a executar
				system->scheduler_next();
			}
		}
		
		// chama o sistema para rodar a tarefa
		system->run();
		
		// arrumar os tempos dessa bomba tb -> CLOCK
		TCB* cur = system->getCurTask();
		// se tarefa executou, adiciona o intervalo de execucao para o grafico
		if (cur) {
			// cout << cur->getId() << "{ " << tempo_temporario << ", " << tempo_temporario + 1 << " }" << endl;
			gantt->insertInterval(cur, tempo_temporario, tempo_temporario + 1);
			
			// se tipo de simulacao passo a passo, mostra o grafico atual
			if(sim_type == SimulationType::PassoaPasso){
				gantt->plotChart();
			}
		}
		
		tempo_temporario++; // apagar arrumar com o CLOCK
	}
	
	// mostra o resultado final
	gantt->plotChart();
	// gerar o grafico final em imagem
	gantt->exportImg();
	
}

void SystemSimulator::setSimType(int st){
	// define o tipo de simulacao
	if (st == 1){
		sim_type = SimulationType::PassoaPasso;
	}
	else{
		sim_type = SimulationType::Completa;
	}
}
