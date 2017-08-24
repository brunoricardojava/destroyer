/*
 * =====================================================================================
 *
 *       Filename:  algoritmos.c
 *
 *    Description:  Funções principais a serem usadas pelo robô
 *
 *        Version:  1.0
 *        Created:  07/25/2017 08:15:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  Cheira Solda Corporation
 *
 * =====================================================================================
 */

//Funções de movimentação básicas
function goAhead(){
	//Aqui ficam os comandos que ativam os motores e fazem com que os dois girem pra frente
	Serial.println("Acelerando");
}

function goBack(){
	//Aqui o comando para fazer com que os motores girem no sentido contrário e o motor dê ré
	Serial.println("Ré");
}

function search(int rotation){
	//rotation = 0 => rotação no sentido horário
	//rotation = 1 => rotação no sentido anti-horário
	switch(rotation){
		case 0:
			//comando que faça a esteira esquerda do robô girar para frente e a direita para traz, fazendo o robô girar no sentido horário
			Serial.println("Girando sentido horário");
		break;

		case 1:
			//Comando que faça a esteira esquerda do robô girar para trás e a direita para a frente,  fazendo o robô girar no sentido anti-horário
			Serial.println("Girando sentido anti-horário");
		break;

		default:
			//pode ser o mesmo que o case 1,  fazendo que o default seja girar no sentido horário
			Serial.println("Girando sentido horário");
		break;
	}	

}
//Fim funções de movimentação basicas




