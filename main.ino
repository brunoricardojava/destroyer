// Código do robô sumô
// Usamos uma biblioteca de RTOS para gerenciamento de tasks, 
// em especifico é um gerencimento de tarefas ciclicas definido por intervalos de tempo.
// Padronização:
// Threads: Todas maiúsculas com underline separando as palavras
// Definições: Todas minúsculas com underline separando as palavras
// Funções: Cada palavra começa com maiúscula 
// Variáveis: 


#include "Thread.h" //Biblioteca de threads
#include "ThreadController.h" //Biblioteca para controle das threads

//Definição dos tempos de chamada para cada thread
#define tempo_sensor_linha  200 //Tempo de leitura dos sensores de linha em mili segundos
#define tempo_sensor_ir     200 //Tempo de leitura dos sensores de infra-vermelho
#define tempo_operacao_robo 50  //Tempo de ciclos de operação do robô.
#define tempo_debug_serial 	500 //Tempo de ciclos de debug da serial

//Ativar ou desativar o modo de debug
#define modo_debug_serial 1 //"1" para ativar e "0" para desativar

//Instanciando um controlador de threads
ThreadController ROBO; 

//Definindo as threads a serem usadas
//----------------------------------------------------//
Thread LEITURA_SENSOR_DE_LINHA;
Thread LEITURA_SENSOR_IR;
Thread CONTROLLER_ROBO;
Thread DEBUG_SERIAL;
//----------------------------------------------------//

//Definindo as Variáveis
//----------------------------------------------------//

//----------------------------------------------------//



//Função para fazer leitura dos sensores de linha
void SensorDeLinha(){

}

//Função para fazer leitura dos sensores IR
void SensorIR(){

}

//Função para controle dos motores
void ControleDosMotores(){

}

//Função para ajudar a debugar o código
void DebugSerial(){
	//Print das variáveis

}


void setup(){

	if(modo_debug_serial){
		Serial.begin(9600);
		Serial.print("Modo serial iniciado\n");
	}

	LEITURA_SENSOR_DE_LINHA.setInterval(tempo_sensor_linha);
	LEITURA_SENSOR_DE_LINHA.onRun(SensorDeLinha);

	LEITURA_SENSOR_IR.setInterval(tempo_sensor_ir);
	LEITURA_SENSOR_IR.onRun(SensorIR);

	CONTROLLER_ROBO.setInterval(tempo_operacao_robo);
	CONTROLLER_ROBO.onRun(ControleDosMotores);

	DEBUG_SERIAL.setInterval(tempo_debug_serial);
	DEBUG_SERIAL.onRun(DebugSerial);

	ROBO.add(&LEITURA_SENSOR_DE_LINHA);
	ROBO.add(&LEITURA_SENSOR_IR);
	ROBO.add(&CONTROLLER_ROBO);
	ROBO.add(&DEBUG_SERIAL);

}

void loop(){
	ROBO.run();

}