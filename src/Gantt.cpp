#include "Gantt.h"

Gantt::Gantt(vector<TCB*> ts){
	// adiciona todas as tarefas do sistema
	for (auto &t : ts){
		TaskInfo ti;
		ti.task = t;
		tasks.push_back(ti);
	}
}

Gantt::~Gantt(){
	tasks.clear();
}

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

    // imprime cada tarefa
    for (int idx = (int)tasks.size() - 1; idx >= 0; --idx) {
        const auto &task = tasks[idx];
        string color = COLORS[(task.task)->getColor() % COLORS.size()]; // seleciona a cor
        cout << (task.task)->getId() << ": "; // mostra id da tarefa

        for (int t = 0; t < total_time; ++t) {
            // antes de entrar no sistema = representada por espaço em branco
            int arrival = (task.task)->getIngressTime();
            if (t < arrival) {
                cout << " ";
                continue;
            }
            
            // se tarefa ja foi terminada, nao imprimir mais
            if(t >= task.endtime && (int)(task.intervals).size() == (task.task)->getDuration()) break;

            // após entrar no sistema
            bool running = false;
            for (auto &interval : task.intervals) {
                if (t >= interval.start && t < interval.end) {
                    // bool de em execucao
                    running = true;
                    break;
                }
            }

            if (running){
				// se executando = mostra a cor
                cout << color << "█" << RESET; 
            }
            else {
				// se ja entrou no sistema, mas esta aguardando 
				// para ser executada = quadrado opaco
				cout << "░"; 
			}
        }

        cout << "\n";
    }	
}

void Gantt::exportImg(string file_name){
	// gerar imagem do grafico 
	
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
    ofstream svg(file_name);
    if (!svg.is_open()) {
        cerr << "Erro ao criar arquivo gantt.svg\n";
        return;
    }

	// definicoes
    svg << "<svg xmlns='http://www.w3.org/2000/svg' width='" << width
        << "' height='" << height << "'>\n";

    svg << "<style>"
        << "text{font-family:monospace;font-size:12px;fill:#333;}"
        << "</style>\n";

    svg << "<rect width='100%' height='100%' fill='white'/>\n";

    // desenha as tarefas
    for (int idx = (int)tasks.size() - 1, drawIdx = 0; idx >= 0; --idx, ++drawIdx) {
        const auto &task = tasks[idx];
        string color = svgColors[(task.task)->getColor() % svgColors.size()]; // selecionar a cor
        int y = margin + drawIdx * (barHeight + barSpacing);
        int arrival = (task.task)->getIngressTime(); // inicio

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

    // exibir os ticks -> ALTERAR PRO TEMPO CLOCK DPS
    svg << "<g font-size='8' fill='#555'>\n";
    for (int t = 0; t <= total_time; ++t) {
        int x = margin + t * 20;
        
        // linha vertical tracejada para melhor visualizacao intervalos
        svg << "<line x1='" << x << "' y1='" << margin - 10
            << "' x2='" << x << "' y2='" << height - margin / 2
            << "' stroke='#ccc' stroke-width='0.5' stroke-dasharray='2,2'/>\n";
        
        // numero do tick    
        svg << "<text x='" << x - 3 << "' y='" << height - 10 << "'>" << t << "</text>\n";
    }
    svg << "</g>\n";

    svg << "</svg>\n";
    svg.close(); // fecha e salva o arquivo

    cout << "Arquivo " << file_name << " gerado\n";
}
