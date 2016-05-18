/*
* O programa Gerador, de forma pseudo-aleatória, “cria” viaturas e associa um novo thread a cada uma.
* Cada thread associado a uma viatura (thread “viatura”) acompanha-la-á no seu “ciclo de vida”, desde que é
* “criada”, passando pelo acesso e eventual estacionamento no Parque, até que sai do Parque se nele tiver
* estacionado e “desaparece”. 
*/

/*
usage: gerador <T_GERACAO> <U_RELOGIO>

- T_GERACAO é o período de tempo, em segundos, em que o Gerador irá funcionar;
- U_RELOGIO é a unidade geral de tempo; o intervalo de tempo mínimo entre quaisquer dois eventos que devem ser medidos
(em tiques de relógio). Por exemplo, se for 10 ticks, a duração de um estacionamento só poderá ser 10, 20, 30... ticks.
*/


/*
- thread principal:
	 depois de preparar todas as variáveis globais relativas ao funcionamento do processo,
	 gera de forma pseudo-aleatória dados que indiquem quando irá ser criada uma nova viatura (struct?), para
		que acesso do Parque se irá dirigir (N, E, S, O) e quanto tempo irá estar estacionada; calcula
		também um número identificador da viatura (único, portanto);
		◦ nota: a probabilidade de se selecionar qualquer acesso é idêntica; os tempos de
			estacionamento devem estar compreendidos entre 1 e 10 múltiplos de U_RELOGIO com
			idêntica probabilidade; o intervalo entre geração de viaturas deve ser 0, 1 ou 2 múltiplos de
			U_RELOGIO com probabilidades de 50%, 30% e 20% respetivamente;
	 seguidamente cria um novo thread “viatura” (no instante determinado de criação) que ficará
		encarregue do “ciclo de vida” da viatura, e passa-lhe toda a informação pertinente;
	 finalmente, aguarda que termine um temporizador próprio, iniciado a T_GERACAO, e termina.

- threads “viatura” (do tipo “detached”):
	 recebe os dados relativos à sua viatura do thread principal (como argumentos da função do thread);
	 cria um FIFO privado, de nome único;
	 escreve todos os dados relativos à viatura (e que permitirão o acompanhamento do seu ciclo de
		vida) no correspondente FIFO do Parque (fifoN, fifoE, etc.).
	 Seguidamente aguardará, bloqueado no seu FIFO privado, a indicação de que a viatura já saiu do Parque,
	de que não foi aceite por o Parque estar cheio ou de que o Parque encerrou e não aceita	mais viaturas.
	 Todos os eventos deverão	ser registados num ficheiro	único, “gerador.log”, com o formato:
		* t(ticks); id_viat; destin; t_estacion; t_vida; observ
*/