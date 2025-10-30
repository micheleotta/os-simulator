#include "Gantt.h"

Gantt::Gantt(vector<TCB*> ts){
	// adiciona todas as tarefas do sistema
	for (auto &t : ts){
		TaskInfo ti;
		ti.task = t;
		tasks.push_back(ti);
	}
}

Gantt::~Gantt(){}

void Gantt::insertInterval(TCB* t, int s, int e){
	// procura a tarefa passada
	auto it = find_if(tasks.begin(), tasks.end(),
                           [t](const TaskInfo& ti) {
                               return ti.task == t;
                           });

	// se nao foi encontrada
    if (it == tasks.end()) return;
    else { // se encontrou
		Interval i;
		i.start = s;
		i.end = e;
		// insere o intervalo da tarefa
        (it->intervals).push_back(i);
        // atualiza o tempo de termino
        it->endtime = e;
    }
}

void Gantt::plotChart(){
	// imprime o grafico atual
	
	// DEPOIS MUDAR PRO CLOCK!!!! -> current_time
	// Determina o tempo total
    int total_time = 0;
    for (auto &t : tasks){
        for (auto &i : t.intervals){
            total_time = max(total_time, i.end);
		}
	}
	cout << "======================" << endl;
    cout << "Ticks: " << total_time << endl;

    // Imprime cada tarefa
    for (int idx = (int)tasks.size() - 1; idx >= 0; --idx) {
        const auto &task = tasks[idx];
        string color = COLORS[idx % COLORS.size()]; // mudar pra cor passada na task dps!!!
        cout << (task.task)->getId() << ": ";

        for (int t = 0; t < total_time; ++t) {
            // antes de entrar no sistema = representa espaço
            int arrival = (task.task)->getIngressTime();
            if (t < arrival) {
                cout << " ";
                continue;
            }
            
            // se tarefa ja foi terminada
            if(t >= task.endtime) break;

            // após entrar no sistema
            bool running = false;
            for (auto &interval : task.intervals) {
                if (t >= interval.start && t < interval.end) {
                    running = true;
                    break;
                }
            }

            if (running){
				// se executando, mostra a cor
                cout << color << "█" << RESET; 
            }
            else {
				// se aguardando para ser executada, quadrado opaco
				cout << "░"; 
			}
        }

        cout << "\n";
    }	
}

void Gantt::exportImg(){
	// nome do arquivo a ser exportado
	string nome_arquivo = "simulacao.svg";
	
	// dimensoes da imagem
    const int barHeight = 20;
    const int barSpacing = 10;
    const int margin = 40;
    const int textOffset = 15;

    // ALTERAR PELO CLOCK DPS
    int total_time = 0;
    for (auto &t : tasks)
        for (auto &i : t.intervals)
            total_time = max(total_time, i.end);

	// tamanho da imagem
    int height = (int)tasks.size() * (barHeight + barSpacing) + margin * 2;
    int width = total_time * 20 + margin * 2;

	// conversao das cores
    vector<string> svgColors = {
        "#e74c3c", "#27ae60", "#f1c40f", "#3498db",
        "#9b59b6", "#16a085", "#bdc3c7"
    };

	// abrir o arquivo
    ofstream svg(nome_arquivo);
    if (!svg.is_open()) {
        cerr << "Erro ao criar arquivo gantt.svg\n";
        return;
    }

    svg << "<svg xmlns='http://www.w3.org/2000/svg' width='" << width
        << "' height='" << height << "'>\n";

    svg << "<style>"
        << "text{font-family:monospace;font-size:12px;fill:#333;}"
        << "</style>\n";

    svg << "<rect width='100%' height='100%' fill='white'/>\n";

    // desenha as tarefas
    for (int idx = (int)tasks.size() - 1, drawIdx = 0; idx >= 0; --idx, ++drawIdx) {
        const auto &task = tasks[idx];
        string color = svgColors[idx % svgColors.size()]; // ALTERAR DEFINICAO DA COR
        int y = margin + drawIdx * (barHeight + barSpacing);
        int arrival = (task.task)->getIngressTime();

        // id da tarefa
        svg << "<text x='5' y='" << y + textOffset << "'>"
            << task.task->getId() << "</text>\n";

        // define intervalo total no sistema (chegada até última execução)
        int wait_x = margin + arrival * 20;
        int wait_w = (task.endtime - arrival) * 20;

        // tarefa esta no sistema aguardando -> cor cinza claro
        svg << "<rect x='" << wait_x << "' y='" << y << "' width='" << wait_w
            << "' height='" << barHeight << "' fill='#eeeeee' stroke='#aaa' stroke-width='0.5'/>\n";

        // tarefa em execução -> cor forte
        for (auto &interval : task.intervals) {
            int x = margin + interval.start * 20;
            int w = (interval.end - interval.start) * 20;
            svg << "<rect x='" << x << "' y='" << y
                << "' width='" << w << "' height='" << barHeight
                << "' fill='" << color << "' stroke='black' stroke-width='0.5'/>\n";
        }
    }

    // exibir os ticks
    svg << "<g font-size='10' fill='#555'>\n";
    for (int t = 0; t <= total_time; ++t) {
        int x = margin + t * 20;
        svg << "<text x='" << x << "' y='" << height - 10 << "'>" << t << "</text>\n";
    }
    svg << "</g>\n";

    svg << "</svg>\n";
    svg.close();

    cout << "Arquivo " << nome_arquivo << " gerado\n";
}
