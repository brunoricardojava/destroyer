/*  A função será usada para guardar 
 *  valores de refletância importantes, 
 *  como a do tatame e das linhas. 
 *  Faremos a leitura dos sensores de forma individual.
*/
//Pinos dos sensores de linha
#define pin_line_sensor1 A0
#define pin_line_sensor2 A1
#define pin_line_sensor3 A2

void setup() {
  Serial.begin(9600);
}

void loop() {
  calib_line_s(1);
  
}
/*A função de calibragem lê o OUTPUT do sensor
   e depois de 2 segundo faz uma segunda leitura,
   caso o valor seja igual ao anterior, ele armazena
   esse valor na variável VALUE. Caso seja diferente,
   a função é chamada novamente para o mesmo sensor
   até que o valor seja de fato fixo.
*/
int calib_line_s(char sensor) {
  int value1, value2, value3 = 0;
  int ref = 0;
  if (sensor == 1) {
    ref = analogRead(pin_line_sensor1);
    delay(2000);
    if (analogRead(pin_line_sensor1) == ref ) {
      value1 = ref;
      Serial.print("Valor atualizado para: ");
      Serial.println(value1);
      return (value1);
    } else {
      Serial.println("Tentando novamente em 2 segundos.");
      delay(2000);
      calib_line_s(sensor);
    }
  }
  else if (sensor == 2) {
    ref = analogRead(pin_line_sensor2);
    delay(2000);
    if (analogRead(pin_line_sensor2) == ref ) {
      value2 = ref;
      Serial.print("Valor atualizado para: ");
      Serial.println(value2);
      return (value2);
    } else {
      Serial.println("Tentando novamente em 2 segundos.");
      delay(2000);
      calib_line_s(sensor);
    }
  }
  else if (sensor == 3) {
    ref = analogRead(pin_line_sensor3);
    delay(2000);
    if (analogRead(pin_line_sensor3) == ref ) {
      value3 = ref;
      Serial.print("Valor atualizado para: ");
      Serial.println(value3);
      return (value3);
    } else {
      Serial.println("Tentando novamente em 2 segundos.");
      delay(2000);
      calib_line_s(sensor);
    }
  } else {
    Serial.println("erro");
  }
}