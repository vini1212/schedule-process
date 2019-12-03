#include <list>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <list>
#include <fstream>
#include <vector>
#include <queue>
#include <iostream>
#include <string.h>


typedef struct{
  int chegada;
  int duracao;
  int lancamento = -1;
  int slices;
  int ac_slices;
  int memoria;
  int prioridade;
  int prioridade_estatica;
  int observado;
  int mark_ciclo;
}PROCESSO;

using namespace std;

void StringSplit(string str, string delim, vector<string>& results); //Função para ler e separar o arquivo de entrada. Separa parametros pela virgula
PROCESSO CPU(PROCESSO process, int inc, int lanc); //Função que simula a cpu


int main(int argc, char *argv[]){
  int cpus=0, mem_total=0, cont_slices=0, go_cpu=0, inc = 0, memoria, slice_lanc=0, flag_processo =0;
  char *file;
  std::list <PROCESSO> prioridade0;
  std::list <PROCESSO> prioridade1;
  std::list <PROCESSO> prioridade2;
  std::list <PROCESSO> prioridade3;
  std::list <PROCESSO> prioridade4;
  std::list <PROCESSO> disco;
  std::vector <string> parametros;
  std::list <PROCESSO> dados_finais;
  PROCESSO processos;
  cpus = atoi(argv[1]);
  memoria = atoi(argv[2]);
  file = argv[3];
  ifstream pf(file);
  string line;
  int total;

  // Lê o arquivo de entrada e coloca os processos em uma lista de processos chamada disco
  while(getline(pf,line)){
  	StringSplit(line,",",parametros);
  	processos.chegada = atoi(parametros[0].c_str());
  	processos.slices = atoi(parametros[1].c_str());
  	processos.memoria = atoi(parametros[2].c_str());
  	processos.prioridade = atoi(parametros[3].c_str());
  	processos.prioridade_estatica = atoi(parametros[3].c_str());
  	processos.observado = 1;
  	processos.ac_slices = 0;
  	processos.mark_ciclo = 0;
  	processos.duracao = processos.slices;
  	disco.push_back(processos);
  	parametros.pop_back();
  	parametros.pop_back();
  	parametros.pop_back();
  	parametros.pop_back();
  	
  	}
//while de condição de parada. O programa só termina quando não existem mais processos no disco e nem nas listas de prioridades
while(disco.size() != 0 or prioridade1.size()!=0 or prioridade0.size()!=0 or prioridade2.size()!=0 or prioridade3.size()!=0 or prioridade4.size()!=0){
  	cont_slices++; //Contador de slices processadas
  	go_cpu = 0; 
  	//Sequencia de for que serve para incrementar a duração observada dos processos
  	for(int i=0;i<prioridade0.size();i++){
  		prioridade0.front().observado++;
  		prioridade0.push_back(prioridade0.front());
  		prioridade0.pop_front();
  	}
  	for(int i=0;i<prioridade1.size();i++){
  		prioridade1.front().observado++;
  		prioridade1.push_back(prioridade1.front());
  		prioridade1.pop_front();
  	}
  	for(int i=0;i<prioridade2.size();i++){
  		prioridade2.front().observado++;
  		prioridade2.push_back(prioridade2.front());
  		prioridade2.pop_front();
  	}
  	for(int i=0;i<prioridade3.size();i++){
  		prioridade3.front().observado++;
  		prioridade3.push_back(prioridade3.front());
  		prioridade3.pop_front();
  	}
  	for(int i=0;i<prioridade4.size();i++){
  		prioridade4.front().observado++;
  		prioridade4.push_back(prioridade4.front());
  		prioridade4.pop_front();
  	}
  	/*For que tira os processos do disco e cola na prioridade correta (somente os processos que tem a chegada correspondente ao momento)
  	 e tira esses mesmos processos do disco, ou seja, somente processos que estão prontos para serem executados saem do disco*/
  	for(int i =0;i<disco.size();i++){ 
  		if(disco.front().chegada <= slice_lanc){
  			if(disco.front().prioridade == 0){
  				prioridade0.push_back(disco.front());
  			}
  			else if(disco.front().prioridade == 1){
  				prioridade1.push_back(disco.front());
  			}
  			else if(disco.front().prioridade == 2){
  				prioridade2.push_back(disco.front());
  			}
  			else if(disco.front().prioridade == 3){
  				prioridade3.push_back(disco.front());
  			}
  			else if(disco.front().prioridade == 4){
  				prioridade4.push_back(disco.front());
  			}
  			disco.pop_front();

  		}
  		else{
  			disco.push_back(disco.front());
  			disco.pop_front();
  		}
  	}
  	//while que controla o numero de cpus e os processos que irão para as mesmas
  	while(cpus != go_cpu){
  		/*Testa se existem processos na fila de prioridade 0. Flags servem para que o programa não entre em loop e busque apenas processos de prioridade 0*/
  		if((prioridade0.size()!= 0) and flag_processo == 0){ 
  			//Testa a memoria. Caso a memoria do processo seja maior que a memoria disponível o processo é descartado, pois nunca conseguirá ser executado.
  			//Processos que estão prontos mas extrapolam a memoria são colocados no final da fila e o proximo é testado
  			for(int i=0;i<prioridade0.size();i++){
  				if(prioridade0.front().memoria > memoria){
  					prioridade0.pop_front();
  				}
  				if(prioridade0.front().mark_ciclo == 1){
  					break;
  				}
  				mem_total = mem_total + prioridade0.front().memoria;
  				
  				if(mem_total<=memoria){
  					flag_processo = 1;
  					break;
  				}
  				else{
  					flag_processo = 0;
  					mem_total = mem_total - prioridade0.front().memoria;
  					prioridade0.push_back(prioridade0.front());
  					prioridade0.pop_front();
  				}
  			}
  			if(flag_processo == 1 and prioridade0.front().mark_ciclo == 0){
  			//Chamo a funcao da simula uma CPU, logo depois coloco o processo no final da fila
  			prioridade0.push_back(CPU(prioridade0.front(),inc,slice_lanc));
  			prioridade0.pop_front();
  			//Testa de o processo nao tem mais slices restantes, guarda o valores finais em uma lista e retira o processo da fila
  			if(prioridade0.back().slices <= 0){
  				dados_finais.push_back(prioridade0.back());
  				prioridade0.pop_back();
  			}
  			}
  			
  		}
  		if((prioridade1.size()!= 0) and flag_processo == 0){
  			//Mesma coisa que na prioridade 0
  			for(int i=0;i<prioridade1.size();i++){
  				if(prioridade1.front().memoria > memoria){
  					prioridade1.pop_front();
  				}
  				if(prioridade1.front().mark_ciclo == 1){
  					break;
  				}
  				mem_total = mem_total + prioridade1.front().memoria;
  				
  				if(mem_total<=memoria){
  					flag_processo = 1;
  					break;
  				}
  				else{
  					flag_processo = 0;
  					mem_total = mem_total - prioridade1.front().memoria;
  					prioridade1.push_back(prioridade1.front());
  					prioridade1.pop_front();
  				}
  			}
  			if(flag_processo == 1 and prioridade1.front().mark_ciclo == 0){
  			prioridade1.push_back(CPU(prioridade1.front(),inc,slice_lanc));
  			prioridade1.pop_front();
  			flag_processo = 1;
  			if(prioridade1.back().slices <= 0){
  				dados_finais.push_back(prioridade1.back());
  				prioridade1.pop_back();
  			}
  			//Faz a troca de prioridade
  			else if(prioridade1.back().prioridade != 1){
  				prioridade2.push_back(prioridade1.back());
  				prioridade1.pop_back();
  			}
  			}	
  			
  			inc = 1;
  		}
  		if((prioridade2.size() != 0) and flag_processo == 0){
  			//Mesma coisa que na prioridade 0
  			for(int i=0;i<prioridade2.size();i++){
  				if(prioridade2.front().memoria > memoria){
  					prioridade2.pop_front();
  				}
  				if(prioridade2.front().mark_ciclo == 1){
  					break;
  				}
  				mem_total = mem_total + prioridade2.front().memoria;
  				
  				if(mem_total<=memoria){
  					flag_processo = 1;
  					break;
  				}
  				else{
  					flag_processo = 0;
  					mem_total = mem_total - prioridade2.front().memoria;
  					prioridade2.push_back(prioridade2.front());
  					prioridade2.pop_front();
  				}
  			}
  			if(flag_processo == 1 and prioridade2.front().mark_ciclo == 0){
  			prioridade2.push_back(CPU(prioridade2.front(),inc,slice_lanc));
  			prioridade2.pop_front();
  			flag_processo = 1;
  			if(prioridade2.back().slices <= 0){
  				dados_finais.push_back(prioridade2.back());
  				prioridade2.pop_back();
  			} //Faz a troca de prioridade
  			else if (prioridade2.back().prioridade != 2){
  				if(prioridade2.back().prioridade == 1){
  					prioridade1.push_back(prioridade2.back());
  					prioridade2.pop_back();
  				}
  				else if(prioridade2.back().prioridade == 3){
  					prioridade3.push_back(prioridade2.back());
  					prioridade2.pop_back();
  				}
  			}
  		}
  			
  			inc = 1;
  		}
  		if((prioridade3.size() != 0) and flag_processo == 0){
  			//Mesma coisa que na prioridade 0
  			for(int i=0;i<prioridade3.size();i++){
  				if(prioridade3.front().memoria > memoria){
  					prioridade3.pop_front();
  				}
  				if(prioridade3.front().mark_ciclo == 1){
  					break;
  				}
  				mem_total = mem_total + prioridade3.front().memoria;
  				
  				if(mem_total<=memoria){
  					flag_processo = 1;
  					break;
  				}
  				else{
  					flag_processo = 0;
  					mem_total = mem_total - prioridade3.front().memoria;
  					prioridade3.push_back(prioridade3.front());
  					prioridade3.pop_front();
  				}
  			}
  			if(flag_processo == 1 and prioridade3.front().mark_ciclo == 0){
  			prioridade3.push_back(CPU(prioridade3.front(),inc,slice_lanc));
  			prioridade3.pop_front();
  			flag_processo = 1;
  			if(prioridade3.back().slices <= 0){
  				dados_finais.push_back(prioridade3.back());
  				prioridade3.pop_back();
  			}
  			else if (prioridade3.back().prioridade != 3){
  				if(prioridade3.back().prioridade == 2){
  					prioridade2.push_back(prioridade3.back());
  					prioridade3.pop_back();
  				}
  				else if(prioridade3.back().prioridade == 4){
  					prioridade4.push_back(prioridade3.back());
  					prioridade3.pop_back();
  				}
  			}
  		}
  			
  			
  			inc = 1;
  		}
  		if((prioridade4.size() != 0) and flag_processo == 0){
  			//Mesma coisa que na prioridade 0
  			for(int i=0;i<prioridade4.size();i++){
  				if(prioridade4.front().memoria > memoria){
  					prioridade4.pop_front();
  				}
  				if(prioridade4.front().mark_ciclo == 1){
  					break;
  				}
  				mem_total = mem_total + prioridade4.front().memoria;
  				
  				if(mem_total<=memoria){
  					flag_processo = 1;
  					break;
  				}
  				else{
  					flag_processo = 0;
  					mem_total = mem_total - prioridade4.front().memoria;
  					prioridade4.push_back(prioridade4.front());
  					prioridade4.pop_front();
  				}
  			}
  			if(flag_processo == 1 and prioridade4.front().mark_ciclo == 0){
  			prioridade4.push_back(CPU(prioridade4.front(),inc,slice_lanc));
  			prioridade4.pop_front();
  			flag_processo = 1;
  			if(prioridade4.back().slices <= 0){
  				dados_finais.push_back(prioridade4.back());
  				prioridade4.pop_back();
  			}
  			//prioridade já vem trocada da cpu, ou seja, processos que tem inicialmente prioridade 4 não mudam
  			else if(prioridade4.back().prioridade != 4){
  				prioridade3.push_back(prioridade4.back());
  				prioridade4.pop_back();
  			}
  		}
  			
  			inc = 1;
  		}
  		flag_processo = 0; //zerando a flag indico que posso jogar outro processo para o outro processador disponivel
  		go_cpu++;
  	}
  	//Sequencia de for para zerar os marcadores de ciclo, responsaveis por indicar os processos que ja foram para o processador no ciclo atual
  	for(int i=0;i<prioridade0.size();i++){
  		prioridade0.front().mark_ciclo = 0;
  		prioridade0.push_back(prioridade0.front());
  		prioridade0.pop_front();
  	}
  	for(int i=0;i<prioridade1.size();i++){
  		prioridade1.front().mark_ciclo = 0;
  		prioridade1.push_back(prioridade1.front());
  		prioridade1.pop_front();
  	}
  	for(int i=0;i<prioridade2.size();i++){
  		prioridade2.front().mark_ciclo = 0;
  		prioridade2.push_back(prioridade2.front());
  		prioridade2.pop_front();
  	}
  	for(int i=0;i<prioridade3.size();i++){
  		prioridade3.front().mark_ciclo = 0;
  		prioridade3.push_back(prioridade3.front());
  		prioridade3.pop_front();
  	}
  	for(int i=0;i<prioridade4.size();i++){
  		prioridade4.front().mark_ciclo = 0;
  		prioridade4.push_back(prioridade4.front());
  		prioridade4.pop_front();
  	}
  	mem_total = 0;
  	inc = 0;
  	slice_lanc++;
}
//Printa os dados finais
printf("\nDados Finais para: \n Numero de CPUS: %d\n Quantidade de memória: %d\n", cpus, memoria);
for(int i=0;i<dados_finais.size();i++){
	printf("chegada: %d, lancamento: %d, duracao projetada: %d, duracao observada: %d\n", dados_finais.front().chegada, dados_finais.front().lancamento, dados_finais.front().duracao, dados_finais.front().observado);
  	dados_finais.push_back(dados_finais.front());
  	dados_finais.pop_front();	
}
}

PROCESSO CPU(PROCESSO process, int inc, int lanc){
	process.slices--; //decrementa uma slice a cada "ciclo"
	process.ac_slices++; //Acumulador de slices para fazer a troca de prioridade
	if(process.lancamento == -1){
		process.lancamento = lanc;
	}
	//Faz o teste se o processo ja teve 10 slices executadas e faz a troca de prioridade
	if(process.ac_slices == 10){
	if((process.prioridade_estatica != 0) and (process.prioridade_estatica !=4)){ //processos com prioridade inicial 0 e 4 não sofrem alteração
		if((process.prioridade != 4) or (process.prioridade_estatica == process.prioridade)){
			process.prioridade++;
		}
		else{
			process.prioridade--;
		}
	}
	process.ac_slices = 0;
	}
	process.mark_ciclo = 1;
	
	return process;
}
void StringSplit(string str, string delim, vector<string>& results)
{
  int cutAt;
  while((cutAt = str.find_first_of(delim)) != str.npos)
  {
    if(cutAt > 0) {
      results.push_back(str.substr(0,cutAt));
    }
    str = str.substr(cutAt+1);
  }
  if(str.length() > 0) {
    results.push_back(str);
  }

}