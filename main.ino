// Código do robô sumô
// Usamos uma biblioteca de RTOS para gerenciamento de tasks,
// em especifico é um gerencimento de tarefas ciclicas definido por intervalos de tempo.
// Padronização:
// Threads: Todas maiúsculas com underline separando as palavras
// Definições: Todas minúsculas com underline separando as palavras
// Funções: Cada palavra começa com maiúscula
// Variáveis: Mesma forma que as definições, minúscula com underline separando as palavras


#include "Thread.h" //Biblioteca de threads
#include "ThreadController.h" //Biblioteca para controle das threads

//--------------------------------------------------------------------------------------------//
//------------Definição dos tempos de chamada para cada thread--------------------------------//
//--------------------------------------------------------------------------------------------//
#define tempo_logica_robo    50   //Tempo de ciclos de operação do robô.
#define tempo_motor			     100  //Tempo de controle dos motores.
#define tempo_sensor_linha   200  //Tempo de leitura dos sensores de linha em mili segundos.
#define tempo_sensor_ir      200  //Tempo de leitura dos sensores de infra-vermelho.
#define tempo_sensor_sonoro  300  //Tempo de leitura dos sensores sonoros.
#define tempo_debug_serial   1000 //Tempo de ciclos de debug da serial.
#define tempo_batery_status  1000 //Tempo de leitura da tensão da bateria
////////////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------//
//------------Definição dos pinos usados no projeto-------------------------------------------//
//--------------------------------------------------------------------------------------------//

//Pino do botão para mudança de stado em software
#define pin_bouton_state 3

//Pinos dos sensores de linha
#define pin_line_sensor1 A0
#define pin_line_sensor2 A1
#define pin_line_sensor3 A2

//Pino para leitura da tensão da bateria
#define pin_batery_status A5

//Pinos do sensor sonoro
#define echo 8 //Pino do sensor ultra sônico correspndente ao echo
#define trig 9 //Pino do sensor ultra sônico correspndente ao Trig

//Pinos dos sensores de IR
#define pin_ir1 10
#define pin_ir2 11

//Pinos dos motores
#define pin1_motor 4
#define pin2_motor 5
#define pin3_motor 6
#define pin4_motor 7

//////////////////////////////////////////////////////////////////////////////////////////////////

// Valor limiar de detecção da linha-> Este valor significa uma especie de vizinhança do valor lido pela
// entrada analogica dos sensores de linha, dependendo da cor da linha esse limiar será alterado...
// Vamos criar uma tabela aqui na documentação contendo os limiares de várias cores padrão:
// preto, branco, cinza, azul, amarelo... Caso precisemos mudar para outra cor de detecção já vamos ter o limiar.
// OBS: Seria interesante criarmos um modo de calibração, para fazermos uma leitura da fita
// e armazenar em uma variável
#define threshold_line_sensors 300 //Não sei o valor em especifico, tem que colocar o valor correto.

//Define a velocidade da serial
#define velocidade_serial 115200 //9600

//Ativar ou desativar o modo de debug
#define modo_debug_serial 1 //"1" para ativar e "0" para desativar o modo de debug

//--------------------------------------------------------------------------------------------//
//-----------Instancias da biblioteca "Threads"-----------------------------------------------//
//--------------------------------------------------------------------------------------------//
//Instanciando um controlador de threads
ThreadController ROBO;

//Definindo as threads a serem usadas
//----------------------------------------------------//
Thread LEITURA_SENSOR_DE_LINHA;
Thread LEITURA_SENSOR_IR;
Thread LEITURA_SENSOR_SONORO;
Thread LOGICA_ROBO;
Thread CONTROLE_MOTOR;
Thread DEBUG_SERIAL;
Thread BATERY_STATUS;
//----------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------------------------//
//-------Definindo as Variáveis-----------------------------------------------------------------//
//---------------------------------------------------------------------------------------------//

// Variável "state" define o estado do robô.
// Vamos usar essa variável como parametro de uma maquina de estado.
// Alguns estados são:
// Parado-'P': Robô parado;
// Busca--'B': Robô entra em modo de busca;
// Alvo---'A': Robô encontrou seu alvo
// Outros estado podem ser configurados aqui...
char state = 'P';

// Variável "config_motor" define a configuração dos motores
// As configurações possiveis do motor são:
// Parado---'P': Robô parado;
// Frente---'F': Robô vai pra frente;
// Direita--'D': Robô vira pra direita;
// Esquerda-'E': Robô vira pra esquerda;
// Ré-------'R': Robô vai pra trás;
char config_motor = 'P';

//Distância do sensor ultra sônico
float distancia_sonora = 0;

//Tensão da bateria
float tensao_bateria = 0;

//Armazena o estado dos sensores de linha "true" para linha detectada e "false" para linha não detectada
bool state_line_sensor1 = false;
bool state_line_sensor2 = false;
bool state_line_sensor3 = false;

//Armazena o estado dos sensores de IR "true" para obstáculo detectado e "false" para obstáculo não detectado
bool state_ir_sensor1 = false;
bool state_ir_sensor2 = false;

////////////////////////////////////////////////////////////////////////////////////////////////



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

	state_ir_sensor1 = digitalRead(pin_ir1);
	state_ir_sensor2 = digitalRead(pin_ir2);

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

//Função para fazer leitura da tensão da bateria
void LeituraBateryStatus(){
  tensao_bateria = (analogRead(pin_batery_status) * 4.98 / 1023.0) * 3;
}


// Aqui dentro será definido a logica do robõ de acordo com o estado das variáveis
// ou seja a logica de ataque e defesa...! ahahahah
void LogicaDoRobo(){
	switch(state){
		case 'P':
			config_motor = 'P';
			break;
		case 'B':
			//Lógica de busca do robô
			break;
		case 'A':
			//Lógica de ataque do robô
			break;
		//Novos estados do robô podem ser inseridos aqui...
		default:
			config_motor = 'P';
			break;
	}

}

//Função para controle dos motores
void ControleDosMotores(){
	switch(config_motor){
		case 'P':
			digitalWrite(pin1_motor, LOW);
			digitalWrite(pin2_motor, HIGH);
			digitalWrite(pin3_motor, LOW);
			digitalWrite(pin4_motor, HIGH);
			break;

		case 'F':
			digitalWrite(pin1_motor, LOW);
			digitalWrite(pin2_motor, LOW);
			digitalWrite(pin3_motor, LOW);
			digitalWrite(pin4_motor, LOW);
			break;

		case 'D':
			digitalWrite(pin1_motor, LOW);
			digitalWrite(pin2_motor, LOW);
			digitalWrite(pin3_motor, HIGH);
			digitalWrite(pin4_motor, HIGH);
			break;

		case 'E':
			digitalWrite(pin1_motor, HIGH);
			digitalWrite(pin2_motor, HIGH);
			digitalWrite(pin3_motor, LOW);
			digitalWrite(pin4_motor, LOW);
			break;

		case 'R':
			digitalWrite(pin1_motor, HIGH);
			digitalWrite(pin2_motor, HIGH);
			digitalWrite(pin3_motor, HIGH);
			digitalWrite(pin4_motor, HIGH);
			break;

		default:
			digitalWrite(pin1_motor, LOW);
			digitalWrite(pin2_motor, HIGH);
			digitalWrite(pin3_motor, LOW);
			digitalWrite(pin4_motor, HIGH);
			break;
	}

}

//Função para ajudar a debugar o código
void DebugSerial(){
  //Print das variáveis
  /*Serial.print("Distancia do sensor sonoro: ");
  Serial.print(distancia_sonora);
  Serial.print("\n");*/
  Serial.print("Status da bateria: ");
  Serial.println(tensao_bateria);

}

//------------------------------------------------------------------------//
//-----------Funções que são chamadas dentro da função setup--------------//
//------------------------------------------------------------------------//
//Função que irá setar a configuração dos pinos do arduino
void SetPin(){

  //Pino trig do sensor ultra sônico
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW); //Deixar trigger em nível lógico baixo
  delayMicroseconds(2); //Não sei se é necessario essa função

  //Pino echo do sensor ultra sônico
  pinMode(echo, INPUT);

  //Pino dos sensores IR
  pinMode(pin_ir1, INPUT);
  pinMode(pin_ir2, INPUT);

  //Pino que controlam os motores
  pinMode(pin1_motor, OUTPUT);
  pinMode(pin2_motor, OUTPUT);
  pinMode(pin3_motor, OUTPUT);
  //pinMode(pin4_motor, OUTPUT); //Caso usemos 4 pinos de controle


}

//Função para iniciar a comunicação serial
void BeginSerial(){

    	Serial.begin(velocidade_serial);
    	Serial.print("Modo debug_serial iniciado\n");

}

void ConfigThreads(){
	//----------------------------------------------------------//
	//-----------Setando as configurações das threads-----------//
	//----------------------------------------------------------//

	//Sensor de linha
	LEITURA_SENSOR_DE_LINHA.setInterval(tempo_sensor_linha);
	LEITURA_SENSOR_DE_LINHA.onRun(LeituraSensorDeLinha);

	//Sensor de IR
	LEITURA_SENSOR_IR.setInterval(tempo_sensor_ir);
	LEITURA_SENSOR_IR.onRun(LeituraSensorIR);

	//Sensor sonoro
	LEITURA_SENSOR_SONORO.setInterval(tempo_sensor_sonoro);
	LEITURA_SENSOR_SONORO.onRun(LeituraSensorSonoro);

	//Controle do robô
	LOGICA_ROBO.setInterval(tempo_logica_robo);
	LOGICA_ROBO.onRun(LogicaDoRobo);

	CONTROLE_MOTOR.setInterval(tempo_motor);
	CONTROLE_MOTOR.onRun(ControleDosMotores);

	//Debug da serial
	DEBUG_SERIAL.setInterval(tempo_debug_serial);
	DEBUG_SERIAL.onRun(DebugSerial);

  //Status da bateria
  BATERY_STATUS.setInterval(tempo_batery_status);
  BATERY_STATUS.onRun(LeituraBateryStatus);

	//Adicionando as threads no seu controlador
	ROBO.add(&LEITURA_SENSOR_DE_LINHA);
	ROBO.add(&LEITURA_SENSOR_IR);
	ROBO.add(&LEITURA_SENSOR_SONORO);
	ROBO.add(&LOGICA_ROBO);
	ROBO.add(&CONTROLE_MOTOR);
	ROBO.add(&DEBUG_SERIAL);
  ROBO.add(&BATERY_STATUS);
	//----------------------------------------------------------//

}
///////////////////////////////////////////////////////////////////////////


void setup(){

	//Chamada da função que configura as threads
	ConfigThreads();

	//Chamada de função para configuração dos pinos
	SetPin();

	//Habilita ou não a thread de debug
	if(modo_debug_serial){
		DEBUG_SERIAL.enabled = true;

		//Chamada da função que inicia a serial
		BeginSerial();
	}
	else DEBUG_SERIAL.enabled = false;

}

void loop(){

	//Chama o thread controller
	ROBO.run();

}
