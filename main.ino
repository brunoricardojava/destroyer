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

//--------------------------------------------------------------------------------------------//
//------------Definição dos tempos de chamada para cada thread--------------------------------//
//--------------------------------------------------------------------------------------------//
#define tempo_operacao_robo  50   //Tempo de ciclos de operação do robô.
#define tempo_sensor_linha   200  //Tempo de leitura dos sensores de linha em mili segundos
#define tempo_sensor_ir      200  //Tempo de leitura dos sensores de infra-vermelho
#define tempo_sensor_sonoro  300  //Tempo de leitura dos sensores sonoros
#define tempo_debug_serial   1000 //Tempo de ciclos de debug da serial
//--------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------//
//------------Definição dos pinos usados no projeto--------------------------------//
//--------------------------------------------------------------------------------------------//

//Pinos do sensor sonoro
#define echo 5 //Pino do sensor ultra sônico correspndente ao echo
#define trig 4 //Pino do sensor ultra sônico correspndente ao Trig

//Pinos dos sensores de linha
#define pin_line_sensor1 A0
#define pin_line_sensor2 A1
#define pin_line_sensor3 A2

//--------------------------------------------------------------------------------------------//

//Valor limiar de detecção da linha
#define threshold_line_sensors 300 //Não sei o valor em especifico, tem que colocar o valor correto.

#define velocidade_serial 19200 //Velocidade da serial

//Ativar ou desativar o modo de debug
#define modo_debug_serial 1 //"1" para ativar e "0" para desativar o modo de debug

//Instanciando um controlador de threads
ThreadController ROBO; 

//Definindo as threads a serem usadas
//----------------------------------------------------//
Thread LEITURA_SENSOR_DE_LINHA;
Thread LEITURA_SENSOR_IR;
Thread LEITURA_SENSOR_SONORO;
Thread CONTROLLER_ROBO;
Thread DEBUG_SERIAL;
//----------------------------------------------------//


//----------------------------------------------------//
//-------Definindo as Variáveis-----------------------//
//----------------------------------------------------//

float distancia_sonora; //Distância do sensor ultra sônico

//Armazena o estado dos sensores de linha "true" para linha detectada e "false" para linha não detectada
bool state_line_sensor1 = false;
bool state_line_sensor2 = false;
bool state_line_sensor3 = false;

//----------------------------------------------------//



//Função para fazer leitura dos sensores de linha
void LeituraSensorDeLinha(){
	int sensor1, sensor2, sensor3;
	sensor1 = analogRead(pin_line_sensor1);
	sensor2 = analogRead(pin_line_sensor2);
	sensor3 = analogRead(pin_line_sensor1);

	if(sensor1>= threshold_line_sensors && sensor1 < threshold_line_sensors + 100) state_line_sensor1 = true;
	else state_line_sensor1 = false;

	if(sensor2>= threshold_line_sensors && sensor2 < threshold_line_sensors + 100) state_line_sensor2 = true;
	else state_line_sensor2 = false;

	if(sensor3>= threshold_line_sensors && sensor2 < threshold_line_sensors + 100) state_line_sensor2 = true;
	else state_line_sensor2 = false;

}

//Função para fazer leitura dos sensores IR
void LeituraSensorIR(){

}

//Função para fazer leitura dos sensores sonoros
void LeituraSensorSonoro(){

  float tempo;

  //Mandando o sinal do trigger
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  //Calculando o tempo de ida
  tempo = pulseIn(echo, HIGH);

  //Calculando a distância
  distancia_sonora = tempo / 29.4 / 2;

}

//Função para controle dos motores
void ControleDosMotores(){

}

//Função para ajudar a debugar o código
void DebugSerial(){
  //Print das variáveis
  Serial.print("Distancia do sensor sonoro: ");
  Serial.print(distancia_sonora);
  Serial.print("\n");

}

//Função que irá setar a configuração dos pinos do arduino
void SetPin(){

  //Pino trig do sensor ultra sônico
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW); //Deixar trigger em nível lógico baixo
  delayMicroseconds(2); //Não sei se é necessario essa função

  //Pino echo do sensor ultra sônico
  pinMode(echo, INPUT);

}

//Função para iniciar a comunicação serial
void BeginSerial(){

	//Condição para iniciar a serial
	if(modo_debug_serial){
    	Serial.begin(velocidade_serial);
    	Serial.print("Modo debug_serial iniciado\n");
	}

}


void setup(){

  //Chamada de função para configuração dos pinos
  SetPin();

  //Chamada da função que inicia a serial
  BeginSerial();

  //----------------------------------------------------------//
  //-----------Setando as configurações das threads-----------//
  //----------------------------------------------------------//
  LEITURA_SENSOR_DE_LINHA.setInterval(tempo_sensor_linha);
  LEITURA_SENSOR_DE_LINHA.onRun(LeituraSensorDeLinha);

  LEITURA_SENSOR_IR.setInterval(tempo_sensor_ir);
  LEITURA_SENSOR_IR.onRun(LeituraSensorIR);
  
  LEITURA_SENSOR_SONORO.setInterval(tempo_sensor_sonoro);
  LEITURA_SENSOR_SONORO.onRun(LeituraSensorSonoro);

  CONTROLLER_ROBO.setInterval(tempo_operacao_robo);
  CONTROLLER_ROBO.onRun(ControleDosMotores);

  DEBUG_SERIAL.setInterval(tempo_debug_serial);
  DEBUG_SERIAL.onRun(DebugSerial);

  ROBO.add(&LEITURA_SENSOR_DE_LINHA);
  ROBO.add(&LEITURA_SENSOR_IR);
  ROBO.add(&LEITURA_SENSOR_SONORO);
  ROBO.add(&CONTROLLER_ROBO);
  ROBO.add(&DEBUG_SERIAL);
  //----------------------------------------------------------//

}

void loop(){
	
	//Chama o thread controller
	ROBO.run();
  	
  	//Habilita ou não a thread de debug
  	if(modo_debug_serial)	DEBUG_SERIAL.enabled = true;
  	else DEBUG_SERIAL.enabled = false;

}