# schedule-process
Simulador de escalonador de processos implementado na matéria de Sistemas Operacionais

Como entrada do simulador especificamos a quantidade de cpu's e memória junto ao arquivo de entrada indicando a quantidade de processos
e sendo separado por vírgula cada parâmetro do trabalho:

- chegada: representa o tempo em que o processo é recebido pelo SO
- duração: número de slices necessários para executar o processo
- memória: quantidade de memória (em GB) necessário à execução do processo (use apenas múltiplos de 64 GB)
- prioridade: um valor entre 0 (zero) e 4, sendo 0 a mais alta prioridade
