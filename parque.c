/*
* A gestão do Parque é efetuada por 2 níveis de threads: num nível, dá-se o atendimento das chegadas
* pelos 4 threads “controladores”, cada um associado a um dos pontos de acesso (entrada/saída); a outro
* nível, dá-se o atendimento “personalizado” de cada viatura, mediante a criação “na hora” de um thread
* ”arrumador” que a acompanha durante a sua permanência no Parque: ou a rejeita logo de início ou a deixa
* estacionar durante o tempo solicitado. Cada evento relevante, e.g. pedido de acesso de uma viatura ou
* saída de outra, serão anotados num ficheiro de registos, e sempre associados ao instante de ocorrência
* (em clock ticks).
*/


/*
A comunicação entre os processos Parque e Gerador é efetuada por intermédio de pipes com nome
(FIFOs). Basicamente, cada thread “viatura” coloca no acesso estipulado na geração da viatura um pedido
de estacionamento, que será recebido pelo thread “controlador” desse acesso e respondido (positiva ou
negativamente) pelo respetivo thread “arrumador” do Parque. 
 Sempre que possível, todas as atividades ligadas à simulação deverão decorrer sem “esperas
ativas” (busy waiting).
*/


/*
usage: parque <N_LUGARES> <T_ABERTURA>

N_LUGARES é a lotação do Parque;
T_ABERTURA é o período de tempo, em segundos, em que o Parque está aberto; quando vence,
não são aceites mais viaturas; quando todas as viaturas tiverem saído, o Parque encerra.

*/


/*
- thread principal:
	 inicializa as variáveis globais necessárias, incluindo o temporizador geral que controla o tempo 
		de abertura do Parque (e o numero de lugares);
	 cria os 4 threads “controlador”, um para cada acesso, e aguarda que eles terminem;
	 finalmente, efetua e publica estatísticas globais. 

- 4 threads controlador, criados desde inicio para controlo dos 4 pontos de acesso ate encerramento
	do parque:
	 criar o seu FIFO próprio (identificado por “fifo?”, onde '?' será ou N ou E, ou S ou O);
	 receber pedidos de acesso através do seu FIFO; cada pedido inclui os seguintes dados:
		◦ porta de entrada;
		◦ tempo de estacionamento (em clock ticks);
		◦ número identificador único da viatura (inteiro);
		◦ nome do FIFO privado para a comunicação com o thread “viatura” do programa Gerador.
	 criar um thread “arrumador” para cada pedido de entrada recebido e passar-lhe a informação
		correspondente a esse pedido (struct?);
	 estar atento a uma condição de terminação (correspondendo à passagem do T_ABERTURA do
		Parque) e, nessa altura, ler todos os pedidos pendentes no seu FIFO e fechá-lo para que potenciais
		novos clientes de estacionamento sejam notificados do encerramento do Parque; encaminhar os
		últimos pedidos a correspondentes thread “arrumador” --> variaveis de condição. 

- threads arrumador, criadas pelos controladores por cada viatura chegada, acompanhando-a desde que
	chega até que sai(tendo ficado estacionada ou nao). Estas threads organizam-se na gestão do numero
	de lugares disponiveis por meio de primitivas de sincronização apresentadas nas aulas 
	(mutexes, variáveis de condição, semáforos). Tarefas:
	 recolher a informação sobre a viatura de que está encarregue;
	 verificar se há lugar para o estacionamento da viatura ou não (nota: esta operação deverá ser
		executada de forma sincronizada com todos os outros threads do mesmo tipo, não havendo
		competição por um lugar concreto do Parque!)
		◦ se não houver vaga (Parque cheio), nega à viatura o acesso ao Parque, mediante a colocação
			de uma mensagem disso indicativa (“cheio!”) no FIFO privado da viatura, que será lida pelo
			respetivo thread “viatura” do programa Gerador;
		◦ se houver vaga no Parque, reserva-a (evitando condições de competição – race conditions –
			com todos os outros threads do mesmo tipo) e envia, pelo FIFO privado da viatura, uma
			mensagem esclarecedora do estacionamento (“entrada”) ao respetivo thread “viatura”; neste
			caso a viatura ‘desaparece’.
	 na sequência da admissão de uma viatura ao Parque, ligar um temporizador local, para controlar o
		tempo de estacionamento da viatura, e quando o prazo terminar, “acompanhar a viatura à saída do
		Parque”, colocando uma mensagem indicativa ("saida") no FIFO privado da viatura, que será lida
		pelo respetivo thread “viatura”;
	 na sequência da saída de uma viatura do Parque, dar baixa do lugar ocupado, atualizando
		(de forma sincronizada com todos os outros threads do mesmo tipo) a contagem do número
		de lugares vagos no Parque.	Todos os eventos deverão ser registados num ficheiro
		único, “parque.log”, com o formato ilustrado ao lado. Trata-se de um ficheiro CSV para melhor ser importado
		por uma folha de cálculo e, assim, facilitar a criação de gráficos de visualização do andamento da simulação
/*