#include "System.h"

System::System(string st, int q, int a) : quantum(q), alpha(a), current_task(NULL), current_quantum(0)
{	
	define_scheduler_type (st);
	call_scheduler = false;
	call_aging = false;
}
	
System::~System(){
	// destrutora
	ready.clear();
	waiting.clear();
	current_task = nullptr;
}

void System::define_scheduler_type(string st)
{
	if (st == "PRIOP") scheduler_type = SchedulerType::PRIOP;
	else if (st == "SRTF") scheduler_type = SchedulerType::SRTF;
	else if (st == "PRIOPEnv") scheduler_type = SchedulerType::PRIOPEnv;
	else scheduler_type = SchedulerType::FIFO;
}

void System::scheduler_next(TCB* prev){
	// para o primeiro trabalho, as tarefas em espera
	// so esperam o processador. portanto, aqui elas ja
	// sao colocadas como prontas novamente
	for (const auto& waiting_task : waiting) {
        task_ready(waiting_task);
    }
    waiting.clear();
    
    // se não houver tarefas prontas, não escolhe nada
    if (ready.empty()){
        current_task = nullptr;
        return;
    }
    
    // escolhe a prox tarefa conforme algoritmo especificado
    TCB* next_task = nullptr;
	switch(scheduler_type){
		case SchedulerType::FIFO: {
			// atender a ordem das tarefas prontas
			// com preempcao = Round Robin
			next_task = ready.front();
			break;
		}
		case SchedulerType::SRTF: {
			// proximo a executar -> menor tempo restante
			if (current_task) next_task = current_task;
			else next_task = ready.front();
			int min_time = next_task->getDuration() - next_task->getCurrentTime();
			for (const auto& task : ready) {
				int remaining_time = task->getDuration() - task->getCurrentTime();
				// atualiza se o tempo restante < que tempo atual
				if(remaining_time < min_time){
					min_time = remaining_time;
					next_task = task;
				}
				else if(remaining_time == min_time){
					next_task = better_choice(task, next_task, prev);
					min_time = next_task->getDuration() - next_task->getCurrentTime();
				}
			}			
			break;
		}
		case SchedulerType::PRIOP: {
			// proximo a executar -> maior prioridade
			if (current_task) next_task = current_task;
			else next_task = ready.front();
			for (const auto& task : ready) {
				// atualiza se prioridade > prioridade atual
				if(task->getPriority() > next_task->getPriority()){
					next_task = task;
				}
				else if(task->getPriority() == next_task->getPriority()){
					next_task = better_choice(task, next_task, prev);
				}
			}
			break;
		}
		case SchedulerType::PRIOPEnv: {
			// proximo a executar -> maior prioridade dinamica
			if (current_task) next_task = current_task;
			else next_task = ready.front();
			for (const auto& task : ready) {
				// atualiza se prioridade dinamica > prioridade atual
				if(task->getDynamicPriority() > next_task->getDynamicPriority()){
					next_task = task;
				}
				else if(task->getDynamicPriority() == next_task->getDynamicPriority()){
					// em caso de empate:
					// 1) tarefa com maior prioridade estatica prevalece
					if(task->getPriority() != next_task->getPriority()){
						next_task = (task->getPriority() > next_task->getPriority()) ? task : next_task;
					}
					else next_task = better_choice(task, next_task, prev);
				}
			}
			break;
		}
		default: {
			// em default, retorna a primeira na fila de tarefas prontas (FIFO)
			next_task = ready.front();
			break;
		}
	}
	
	if (current_task != next_task){
	    // reseta o quantum
		current_quantum = 0;
		// se tarefa antiga estava rodando, insere-a na lista de prontas
		if(current_task && current_task->getState() == States::Running){
			auto itr = find(ready.begin(), ready.end(), current_task);
			if (itr == ready.end()) {
				ready.push_back(current_task);
			}
			// atualiza estado
			current_task->setState(States::Ready);
		}
		
		// if(current_task != nullptr && next_task != nullptr) cout << "current = " << current_task->getId() << "  nova = " << next_task->getId() << endl; 
		// atualiza tarefa
		current_task = next_task;
		
		// atualização da prioridade dinâmica toda vez que o escalonador 
		// escolhe uma nova tarefa
		if (get_scheduler_type() == SchedulerType::PRIOPEnv && prev != nullptr && prev != current_task) set_call_aging(true);
		
	}
}

TCB* System::better_choice(TCB* task1, TCB* task2, TCB* prev){
	// para casos de empate
	
	// evitar erros
	if(!task1) return task2;
	if(!task2) return task1;
	
	// 1) tarefa selecionada é a que está executando imediatamente antes
    if (task1 == prev) return task1;
    if (task2 == prev) return task2;

    // 2) menor instante de ingresso
    if (task1->getIngressTime() != task2->getIngressTime())
        return (task1->getIngressTime() < task2->getIngressTime()) ? task1 : task2;

    // 3) menor duração da tarefa
    if (task1->getDuration() != task2->getDuration())
        return (task1->getDuration() < task2->getDuration()) ? task1 : task2;

    // 4) sorteio
    return (rand() % 2 == 0) ? task1 : task2;
}
		
void System::task_ready(TCB* t){
	// tarefa fica pronta para executar

	// evitar erro
	if (!t) return;
	
	// não re-adicionar tarefas já terminadas
    if (t->getState() == States::Terminated or t->getState() == States::Running) return;
	
	// retira t da lista waiting	
	auto it = find(waiting.begin(), waiting.end(), t);
    if (it != waiting.end()) {
        waiting.erase(it);
    }
	
	// adiciona t a lista de prontas caso não esteja na lista
	auto itr = find(ready.begin(), ready.end(), t);
    if (itr == ready.end()) {
        ready.push_back(t);
    }
    // atualiza estado
    t->setState(States::Ready);
}

void System::task_sleep(TCB* t){
	// quando tarefa eh suspensa
	
	// evitar erros
	if (!t) return;
	
	// retira t da lista de prontas
	auto it = find(ready.begin(), ready.end(), t);
    if (it != ready.end()) {
        ready.erase(it);
    }
    
	// insere t na lista de waiting
    auto itw = find(waiting.begin(), waiting.end(), t);
    if (itw == waiting.end()) {
        waiting.push_back(t);
    }
    t->setState(States::Waiting);
}

void System::plot_tasks()
{
	if (finished()){
		cout << "\nFIM DA EXECUCAO" << "\nNENHUMA TASK NO SISTEMA";
		return;
	}
	
	cout << "\n";
	cout << "NUMERO DE TASKS NO SISTEMA: " << (ready.size() + waiting.size() + 1);

	// imprime task atual e suas informacoes
	cout << "\nTASK ATUAL: " << current_task->getId() 
		<< " (remaining time: " << (current_task->getDuration() - current_task->getCurrentTime())
		<< " | priority: " << current_task->getPriority()  
		<< (get_scheduler_type() == SchedulerType::PRIOPEnv ? " | dynamic priority: " + to_string(current_task->getDynamicPriority()) : "")
			<< ')';

	cout << "\nPRONTAS: ";
	
	if (ready.empty())
		cout << "None";
		
	for (const auto& task : ready) {
		if (task->getState() == Ready){
		cout << '\n' 
		<< "- " << task->getId() 
		<< " (remaining time: " << (task->getDuration() - task->getCurrentTime())
		<< " | priority: " << task->getPriority()  
		<< (get_scheduler_type() == SchedulerType::PRIOPEnv ? " | dynamic priority: " + to_string(task->getDynamicPriority()) : "")
			<< ')';
		}
	}

	cout << "\nSUSPENSAS: ";

	if (waiting.empty())
		cout << "None";

	for (const auto& task : waiting) {
		cout << 'n' << task->getId() << ' ';
	}

}

void System::update(){	
	
	TCB* prev_task = nullptr;
	// rodar tarefa	se existe uma tarefa no 'processador'
	if(current_task){
		// se tarefa ja executou tudo
		if(current_task->getCurrentTime() >= current_task->getDuration()){
			current_task->setState(States::Terminated); // estado de terminada
			// retira tarefa da lista de prontas
			auto it = find(ready.begin(), ready.end(), current_task);
			if (it != ready.end()) {
				ready.erase(it);
			}
			prev_task = current_task;
			current_task = nullptr;
			// deve chamar o escalonador para escolher prox
			set_call_scheduler(true);
		}
		// se quantum encerrou, sai por preempcao
		else if(current_quantum >= getQuantum()){
			// desativa a tarefa atual
			task_sleep(current_task); //verificar
			// nesse primeiro trabalho nao precisa esperar
			// ja volta imediatamente para ready, mas ao final da lista
			task_ready(current_task);
			prev_task = current_task;
			current_task = nullptr;
			// deve chamar o escalonador para escolher prox
			set_call_scheduler(true);
		}
	}	
	
	// se nao ha tarefa atual, elege uma
	if(!current_task or call_scheduler) { 
		(prev_task != nullptr) ? scheduler_next(prev_task) : scheduler_next();
		set_call_scheduler(false);
		if(!current_task) return; // prevenir erros
	}
	
	// roda a tarefa atual!
	current_task->setState(States::Running);
	// retira-a da lista de prontas
	auto it = find(ready.begin(), ready.end(), current_task);
    if (it != ready.end()) {
        ready.erase(it);
    }
    
    if(call_aging){
		aging(current_task);
		set_call_aging(false);
	}
	
	// incrementa no current_time ++
	current_task->setCurrentTime(current_task->getCurrentTime() + 1);
	current_quantum++; // tambem incrementa considerando o quantum
}

bool System::finished(){
	// sistema encerra quando nao ha mais tarefas a serem executadas
	return waiting.empty() && ready.empty() && current_task == nullptr;
}
		
int System::getQuantum(){
	// retorna o quantum
	return quantum;
}

TCB* System::getCurTask(){
	// retorna a tarefa atual
	return current_task;
}

SchedulerType System::get_scheduler_type(){
	return scheduler_type;
}

string System::get_scheduler_name(){		
	if (get_scheduler_type() == SchedulerType::PRIOP) return "PRIOP";
	else if (get_scheduler_type() == SchedulerType::SRTF) return "SRTF";
	else if (get_scheduler_type() == SchedulerType::PRIOPEnv) return "PRIOPEnv";
	else return "FIFO";
}

void System::set_call_scheduler(bool c){
	call_scheduler = c;
}

void System::set_call_aging(bool a){
	call_aging = a;
}
			

void System::aging(TCB* current){
	// evitar erro
	if (!current) return;
	
	// a que esta rodando atualmente volta a prioridade dinamica ao valor da estatica
	current->setDynamicPriority(current->getPriority());
	// as demais tarefas envelhecem
	for (const auto& task : ready) {
		// prioridade dinamica envelhece conforme constante alpha
		task->setDynamicPriority(task->getDynamicPriority() + alpha);
	}
}
