#include <Servo.h>
Servo servomotor;

#define pinSt A0             //Sensor de temperatura TMP36
#define pinHum A1            //Sensor de humedad simulado (potenciometro)
#define pinBombilla 3        //Rele de bombilla
#define pinExtractor 4       //Rele de extractor
#define pinHumidificador 5   //Rele de humidificador
#define pinServo 6           //Servo
#define pinLed 7             //LED rojo intermitente

int estado = 0, fila, estadoTempVolteo = 0, estadoTempTotal = 0;
unsigned long tiempoInicialVolteo, tiempoInicialTotal;

void iniciaTemporizadorVolteo(){
  estadoTempVolteo = 1;
  tiempoInicialVolteo = millis();
}

byte temporizadorVolteo(){
  byte condicion = (millis() - tiempoInicialVolteo >= 1000) && estadoTempVolteo;
  if (condicion){
    estadoTempVolteo = 0;
    return 1;
  }else return 0;
}

void iniciaTemporizadorTotal(){
  tiempoInicialTotal = millis();
}

byte temporizadorTotal(){
  return (millis() - tiempoInicialTotal >= 10000);
}

//Tabla de transicion de estados - fila = entrada y columna = estado
int tablaTran[18][2] =
{{0, 1},//0
 {1, 0},//1
 {0, 1},//2
 {1, 0},//3
 {0, 1},//4
 {1, 0},//5
 {0, 1},//6
 {1, 0},//7
 {0, 1},//8
 {1, 0},//9
 {0, 1},//10
 {1, 0},//11
 {0, 1},//12
 {1, 0},//13
 {0, 1},//14
 {1, 0},//15
 {0, 1},//16
 {1, 0}};//17

//Tabla de salida - fila = entrada y columna = estados
int tablaSalida[18][2] =
{{0,0},//0
 {1,2},//1
 {3,3},//2
 {4,5},//3
 {6,6},//4
 {7,8},//5
 {9,9},//6
 {10,11},//7
 {12,12},//8
 {13,14},//9
 {15,15},//10
 {16,17},//11
 {18,18},//12
 {19,20},//13
 {15,15},//14
 {16,17},//15
 {15,15},//16
 {16,17}};//17

float leerTemperatura(){
  return (analogRead(pinSt)*5/1024.0-0.5)*100;
}

float leerHumedad(){
  return analogRead(pinHum)*100.0/1023.0;//Convertir a %
}

void avisoFinIncubacion() {
  digitalWrite(pinLed, HIGH); delay(100); digitalWrite(pinLed, LOW); delay(100);
}

int lectura_entradas(){
  float tem = leerTemperatura();
  float hum = leerHumedad();
  int st = (tem >= 37 && tem <= 38) + (tem > 38) * 2;
  int sh = (hum >= 55 && hum <= 60) + (hum > 60) * 2;
  int tv = temporizadorVolteo();
  Serial.print("st: "); Serial.print(st);
  Serial.print(" sh: "); Serial.print(sh);
  Serial.print(" tv: "); Serial.println(tv);
  return st * 6 + sh * 2 + tv;
}

void escritura_salidas(int fila, int estado){

  switch (tablaSalida[fila][estado]){
    case 0: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	break;
    case 1: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	servomotor.write(180);//MD
    	iniciaTemporizadorVolteo(); 
    	break;
    case 2: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	servomotor.write(90);//MI
    	iniciaTemporizadorVolteo(); 
    	break;
    case 3: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, LOW);//AH
    	break;
    case 4: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, LOW);//AH
    	servomotor.write(180);//MD
    	iniciaTemporizadorVolteo(); 
    	break;
    case 5: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, LOW);//AH
    	servomotor.write(90);//MI
    	iniciaTemporizadorVolteo(); 
    	break;
    case 6: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, LOW);//AH
    	break;
    case 7: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, LOW);//AH
    	servomotor.write(180);//MD
    	iniciaTemporizadorVolteo(); 
    	break;
    case 8: 
    	digitalWrite(pinBombilla, HIGH);//EB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, LOW);//AH
    	servomotor.write(90);//MI
    	iniciaTemporizadorVolteo(); 
    	break;
    case 9: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	break;
    case 10: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	servomotor.write(180);//MD
    	iniciaTemporizadorVolteo(); 
    	break;
    case 11: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, LOW);//AE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	servomotor.write(90);//MI
    	iniciaTemporizadorVolteo(); 
    	break;
    case 12: 
    	break;//NOP
    case 13: 
    	servomotor.write(180);//MD
    	iniciaTemporizadorVolteo(); 
    	break;
    case 14: 
    	servomotor.write(90);//MI
    	iniciaTemporizadorVolteo(); 
    	break;
    case 15: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, LOW);//AH
    	break;
    case 16: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, LOW);//AH
    	servomotor.write(180);//MD
    	iniciaTemporizadorVolteo(); 
    	break;
    case 17: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, LOW);//AH
    	servomotor.write(90);//MI
    	iniciaTemporizadorVolteo(); 
    	break;
    case 18: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	break;
    case 19: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	servomotor.write(180);//MD
    	iniciaTemporizadorVolteo(); 
    	break;
    case 20: 
    	digitalWrite(pinBombilla, LOW);//AB
    	digitalWrite(pinExtractor, HIGH);//EE
    	digitalWrite(pinHumidificador, HIGH);//EH
    	servomotor.write(90);//MI
    	iniciaTemporizadorVolteo(); 
    	break;
  }
}

void setup(){
  pinMode(pinBombilla, OUTPUT);
  pinMode(pinExtractor, OUTPUT);
  pinMode(pinHumidificador, OUTPUT);
  pinMode(pinServo, OUTPUT);
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);
  servomotor.attach(pinServo);
  servomotor.write(90);//MI
  iniciaTemporizadorTotal();
  iniciaTemporizadorVolteo();
}

void loop(){
  while (temporizadorTotal()) avisoFinIncubacion();

  fila = lectura_entradas(); escritura_salidas(fila, estado);
  Serial.print("fila: "); Serial.print(fila);
  Serial.print(" estado: "); Serial.println(estado);
  estado = tablaTran[fila][estado];
}
