#include <Servo.h>
#include <IRremote.h>

#define INFRA 9  //Porta Digital Infravermelho
#define SERVO 6 // Porta Digital Servo Motor
#define PIR   5 // Porta Digital Sensor Presença
#define ULTRA 7 // Porta Digital Sensor Ultrassonico

IRrecv receptor(INFRA);
decode_results resultado;
Servo s; // Variável Servo
int pos; // Posição Servo

void setup ()
{
  
  Serial.begin(9600);
  pinMode( PIR, INPUT);
  pinMode( INFRA,INPUT);
  s.attach(SERVO); // Servo
  receptor.enableIRIn();
  s.write(0); // Inicia motor posição zero
}

void loop()
{
 long duration, cm;
 int leitura_presenca = (digitalRead(PIR));
  
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
  delay(1000);
   if (leitura_presenca == HIGH){
     	delay(500);
       }  
 if (receptor.decode(&resultado)) {  
		if (resultado.value == 0xFD00FF) // código em hexadecimal do botão de liga e desliga do controle
  		 motor(); // inverte o valor da variável booleana 
				receptor.resume(); // prepara o receptor para uma próxima leitura
		}
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
}
long microsecondsToCentimeters(long microseconds)
{
  // propagação do som na agua 	1480 m/s --> 1.000.000/148.000
  // The speed of sound is 340 m/s or 29 microseconds per centimeter 1.000.000/34.000
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 6.75 / 2;
}