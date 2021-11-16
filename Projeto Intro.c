#include <Servo.h>
#include <IRremote.h>
#include <avr/wdt.h>

#define INFRA 9  //Porta Digital Infravermelho
#define SERVO 6 // Porta Digital Servo Motor
#define PIR   5 // Porta Digital Sensor Presença
#define ULTRA 7 // Porta Digital Sensor Ultrassonico
#define LED_R 10
#define LED_G 11
#define LED_B 12
IRrecv receptor(INFRA);
decode_results resultado;
Servo s; // Variável Servo
int pos; // Posição Servo

void setup ()
{
  
  Serial.begin(9600);
  pinMode( PIR, INPUT);
  pinMode( INFRA,INPUT);
  pinMode ( LED_R,OUTPUT);
  pinMode ( LED_G,OUTPUT);
  pinMode ( LED_B,OUTPUT);
  s.attach(SERVO); // Servo
  receptor.enableIRIn();
  s.write(0); // Inicia motor posição zero
  wdt_enable(WDTO_4S); // Inicia watchdog de 4s
}

void loop()
{
  
 long duration, cm;
 int leitura_presenca = (digitalRead(PIR));
 
 if (leitura_presenca == HIGH){
     	 
         pinMode(ULTRA, OUTPUT);
            digitalWrite(ULTRA, LOW);
            delayMicroseconds(2);
            digitalWrite(ULTRA, HIGH);
            delayMicroseconds(5);
            digitalWrite(ULTRA, LOW);
            pinMode(ULTRA, INPUT);
            duration = pulseIn(ULTRA, HIGH);
            cm = microsecondsToCentimeters(duration);
            Serial.print(cm);
            Serial.println("cm");
            delay(500);
   		while (cm >27){	digitalWrite(LED_G,HIGH);digitalWrite(LED_B,HIGH);}
   		while (cm < 10) {digitalWrite(LED_R,HIGH);}	
   		while ( 11 < cm < 26){digitalWrite(LED_R,HIGH);digitalWrite(LED_G,HIGH);}
 } 
 if (receptor.decode(&resultado)) {  
		if (resultado.value == 0xFD00FF) // código em hexadecimal do botão de liga e desliga do controle
  		 motor(); // inverte o valor da variável booleana 
				receptor.resume(); // prepara o receptor para uma próxima leitura
		}
  wdt_reset();
}


void motor (){
	for(pos = 0; pos < 90; pos++)
  {
    s.write(pos);
  delay(15);
  }
delay(1000);
  for(pos = 90; pos >= 0; pos--)
  {
    s.write(pos);
    delay(30);
  }
  wdt_reset();
}
long microsecondsToCentimeters(long microseconds)
{
  // propagação do som na agua 	1480 m/s --> 1.000.000/148.000 = 6.75
  // This 340 m/s or 29 microseconds per centimeter 1.000.000/34.000
  // 13 cm de diametro da 6cm de leitura no ar
  // 13 cm de diametro da 28 cm de leitura na agua
  wdt_reset();
  return microseconds / 6.75 / 2;
}
