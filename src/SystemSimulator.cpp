#include "SystemSimulator.h"

SystemSimulator* SystemSimulator::instancia_SS(NULL);
		
SystemSimulator::SystemSimulator(){
	m_clock = new SymClock();
	// define o caminho do arquivo .txt de configuracao
	config_path = "conf.txt";
	// por padrao, deixa o tipo de simulacao completo
	sim_type = SimulationType::Complete;
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
	m_clock = nullptr;
	
	//desaloca todas as tarefas do sistema
	for (int i = 0; i < (int)sys_tasks.size(); i++) {
		delete(sys_tasks[i]);
	}
	sys_tasks.clear();
	remaining_tasks.clear();
}

void SystemSimulator::Create(){

	open_user_menu();

	if (create_system())
		run();
	else
		cerr << "Unable to create the system.";
}
		
bool SystemSimulator::create_system()
{
	
	// cria o sistema com base na leitura do arquivo de configuracao 
	ifstream config_file(config_path);
	// no caso de erro
    if (!config_file.is_open()) {
        cerr << "Error opening:" << config_path << endl;
        return false;
    }
    
	string s;
	string scheduler_type, qs, as;
	int quantum = 0, alpha = 0;
	// le a primeira linha com o algoritmo de escalonamento e o valor do quantum
	if(getline(config_file, s)){
		stringstream ss(s);
        if (getline(ss, scheduler_type, ';') && 
        	getline(ss, qs, ';') &&
        	getline(ss, as, ';')){
				quantum = stoi(qs); // string -> int
				alpha = stoi(as);
        }
		// se nao forem informados os valores, retorna mensagem de erro
		else {
			cerr << "\nError: Invalid input on config. archive. \n Please inform scheduler type and system quantum to run the simulation." << endl;
			return false;
		}
	}

	// se o quantum não for informado ou for inválido (zero ou negativo)
	if (quantum <= 0){
		cerr <<  "\nError: Please inform a valid system quantum.\n";
		return false;
	}

	// se detectou um algoritmo invalido no arquivo retorna erro
	if (!valid_st(scheduler_type)){
		cerr << "Error: Unable to define scheduler algorithm informed at " << config_path
		<< " \n System suports: FIFO, SRTF and PRIOP.\n";
		return false;
	}

	// cria sistema com o tipo de escalonamento e valor do quantum
	system = new System(scheduler_type, quantum, alpha);
	
	TCB* new_task = NULL;
	string id, color;
	int ingress_time, duration, priority;
    string events;
    // le as linhas das tarefas
    while (getline(config_file, s))
	{
        stringstream ss(s);
        getline(ss, id, ';');
        getline(ss, color, ';');
		getline(ss, s, ';'); 
		ingress_time = stoi(s);
		getline(ss, s, ';'); 
		duration = stoi(s);
		getline(ss, s, ';');
		priority = stoi(s);

        new_task = new TCB(id, color, ingress_time, duration, priority);

		// le os eventos
        while (getline(ss, events))
		{
			stringstream e(events);
			string e_info;
            if (!events.empty())
			{
				//para cada evento (delimitado com ;)
				while (getline(e, e_info, ';')){

					string init, dur, id;
					int type;
					std::cout << s;

					//string option = e_info.substr(0,2);
					stringstream st (e_info);
					string option;
					option = e_info.substr(0,2);
					
					if (option == "IO")
					{
						getline (st, option,':');
						id = "io";
						type = 3;
						getline(st, init, '-');
						getline(st, dur, ';');
					}
					else if (option == "ML")
					{
						getline (st, option,':');
						dur = "-1";
						type = 1;
						id = e_info.substr(2,2);
						getline(st, init, ';');
					}
					else if (option == "MU")
					{
						getline (st, option,':');
						dur = "-1";
						type = 2;
						id = e_info.substr(2,2);
						getline(st, init, ';');
					}
					else 
					{
						cerr << "Unable to create event specified as ";
						cerr << option;
						return false;
					}
				
				//std::cout << "\n" << "init:" << init << " dur:" << dur << "id: " <<id << "\n";
				new_task->addEvent(stoi(init), type, stoi(dur),id);
				}
			}
		}
        
        // adiciona as tarefas na lista de tarefas do SystemSimulator
		sys_tasks.push_back(new_task);

        // e tambem nas tarefas ainda nao adicionadas ao sistema
		// quando a simulação adicionar a tarefa, ela sairá dessa lista
        remaining_tasks.push_back(new_task);
    }

	// apos leitura, fecha o arquivo
    config_file.close();

	// sistema criado
	return true;
}

// adiciona ao sistema tarefas que ingressam no instante de tempo 'time'
void SystemSimulator::check_remaining_tasks(int time)
{	
	for (auto it = remaining_tasks.begin(); it != remaining_tasks.end();) {
		TCB* task = *it;	
		// se o tempo corresponde ao tempo de inicio da tarefa,
		// adiciona-a no sistema e na fila de tarefas prontas
		if (time >= task->getIngressTime()){
			system->task_ready(task);
			// retira tarefa das remaining_tasks			
			it = remaining_tasks.erase(it); // retorna o proximo iterador
			}
			else 
				++it;
	}
}

void SystemSimulator::run(){
	
	// passa todas as tarefas para gerar o grafico de gantt ao final da simulação
	gantt = new Gantt(sys_tasks);
	int time = m_clock -> get_simulation_time();

	//se nao foi especificada nenhuma task no arquivo retorna um erro
	if (remaining_tasks.empty()){
		cout << "There are no tasks in the system. Please check the config. file.\n";
		return;
	}
	
	// executar enquanto ainda ha tarefas para serem executadas
	while(!system->finished() or !remaining_tasks.empty()){

		if (m_clock -> ticked()){
			//verifica primeiro se existem tarefas que precisam ser adicionadas ao sistema
			if(!remaining_tasks.empty()){
				long unsigned int qtd_remaining = remaining_tasks.size();
				check_remaining_tasks(time);
				if(qtd_remaining != remaining_tasks.size() && system->get_scheduler_type() != SchedulerType::FIFO){
					// se entrada de nova tarefa -> syscall
					// chama o escalonador para eleger a tarefa a executar
					system->set_call_scheduler(true);
					if (system->get_scheduler_type() == SchedulerType::PRIOPEnv) system->set_call_aging(true);
				}
			}
			
			// chama o sistema para rodar a tarefa atual
			system->update();
			TCB* cur = system->getCurTask();

			// se tarefa executou, adiciona o intervalo de execucao para o grafico
			if (cur) {
				gantt->insertInterval(cur, time, time + 1);
				// se tipo de simulacao passo a passo, mostra o grafico atual
				if(sim_type == SimulationType::DebugMode){
					inform_debug_data(time);
				}
			}

			// atualiza o tempo atual da simulação
			time = m_clock -> get_simulation_time();
		}
	}
	
	// mostra os resultados finais
	// o tempo é informado -1 pois demora um tick para a verificação
	if (sim_type == SimulationType::DebugMode)
		inform_debug_data(time - 1);
	else 
		gantt->plotChart(time - 1, system->get_scheduler_name());

	// gerar o grafico final em imagem
	gantt->exportImg(time - 1, system->get_scheduler_name());
	
}


// User Interface:

void SystemSimulator::open_user_menu(){

	cout << " ============================================"
	<< '\n' << "| Simulador de Sistema Operacional           |" << "\n"
	<< " ============================================ \n";

	ifstream config_file(config_path);
	char answer;
	
    if (!config_file.is_open()){
		build_config_file();
	}

    else {
    	cout << " Já existe um arquivo de configuração para a simulação em: " << config_path
		<< ". \n Deseja sobrescrever o arquivo? [Y/N] \n";

		cin >> answer;

		if (answer == 'Y') {
			build_config_file();
		}
	}

	//define o modo de simulação, por padrão é completa
	cout << "\n =========== \n Qual modo de simulacao deve ser executado [C/D]?"
	<< "\n  - C | Completo (default): mostra o resultado da simulação do sistema."
	<< "\n  - D | Debugger: apresenta o passo a passo da simulação a cada tick. \n";

	cin >> answer;
	
	if (answer == 'D'){
		cout << "\n Carregando simulação em modo Debug . . .\n";
		setSimType(1);
		cout << "Pressione 'enter' para avançar para o próximo tick";
	}
}

void SystemSimulator::build_config_file()
{
	//constroi um arquivo de configuração para o sistema
	ofstream config_file (config_path);
	string scheduler;
	int quantum, alpha;

	//pede ao usuário a configuração do sistema
	cout << " Indique o algoritmo de escalonamento do sistema:" << "\n"
		<< " - FIFO (default)" << '\n' << " - SRTF" << '\n' << " - PRIOP (Prioridade Preemptivo)" << '\n' << " - PRIOPEnv (Prioridade Preemptivo com Envelhecimento)" << '\n';
	cin >> scheduler;
	cout << "Indique o valor do quantum do sistema (Default = 2 ticks):" << "\n";
	cin >> quantum;
	cout << "Indique o valor do parametro alpha para envelhecimento do sistema (Default = 1):" << "\n";
	cin >> alpha;
	
	// escreve no arquivo os valores indicados
	config_file << scheduler << ";" << quantum << ";" << alpha;

	//indicação de tarefas
	string input = "";
	int id = 0;
	string color = "F0E0D0";
	int i_time = 0;
	int dur = 0;
	int prio = 0;

	cout << "\n ===== Criação de tarefas: =====" << "\n";

	while (input != "ok"){
		cout << "\n == Nova tarefa ";
		//computa as entradas do usuario sobre a tarefa
		cout << "\n - Tempo de ingresso: ";
		cin >> i_time;
		cout << " - Duracao: ";
		cin >> dur;
		cout << " - Prioridade: ";
		cin >> prio;

		//adiciona ao arquivo
		config_file << '\n' << "t0" << id << ";"
		<< color << ";" << i_time << ";" << dur << ";" << prio;

		//adiciona eventos para cada tarefa
		cout << "\n = Eventos da tarefa (conforme legenda): " 
		<< "\n   - IO: operação de I/O em algum dispositivo externo"
		<< "\n   - ML: mutex lock" << "\n   - MU: mutex unlock"
		<< "\n Digite 'none' caso nao queira adicionar um novo evento ou 'new' para continuar \n";
		
		string ans = "";
		cin >> ans;

		cout << " !! Formato Mutex -> Evento:instante do evento";
		cout << "\n Para I/0 -> Evento:inicio-duracao";
		cout << "\n Exemplos: IO:1-2, ML:3, MU:4";
		cout << "\n Atencao: os tempos sao relativos ao ingresso da tarefa no sistema!\n";

		//se o usuario deseja criar um novo evento
		while (ans != "none"){
			string event;
			cout << "== Novo evento: ";	
			cin >> ans;
			//adiciona o evento ao arquivo caso a resposta nao seja um pedido de saida
			if (ans != "none"){
				config_file << ";" << ans;
			}
		}

		//atualiza id e cor
		id++;
		
		// cor RGB em hex baseada no id
		stringstream ss;
		ss << std::uppercase << std::hex << std::setfill('0');
		ss << std::setw(2) << ((id * 70) % 256);
		ss << std::setw(2) << ((id * 150) % 256);
		ss << std::setw(2) << ((id * 200) % 256);
		color = ss.str(); 

		cout << "== Tarefa criada. \n Digite 'ok' para finalizar ou 'nova' para criar uma outra tarefa: ";
		cin >> input;
	}
	//fecha o arquivo
	config_file.close();
}

bool SystemSimulator::valid_st(string st)
{
	// strings de algoritmos validos para o escalonador
	return (st == "FIFO" || st == "SRTF" || st == "PRIOP" || st == "PRIOPEnv");
}

//mostra as informações importantes das tarefas em cada tick
void SystemSimulator::inform_debug_data(int tick)
{
	//espera usuario indicar que deseja passar o tick ou sair
	fflush(stdin);
	while (getchar() != '\n');
	//sincroniza clock
	m_clock->sync();
	//mostra o grafico atual com as tarefas e as informacoes relevantes do sistema
	cout << "\n======================" << endl;
	cout << "Estado atual: \n";
	gantt->plotChart(tick, system->get_scheduler_name());
	cout << "- Info:";
	system->plot_tasks();
}

void SystemSimulator::setSimType(int st){
	// define o tipo de simulacao
	if (st == 1){
		sim_type = SimulationType::DebugMode;
	}
	else{
		sim_type = SimulationType::Complete;
	}
}
