#include <AFMotor.h> 
#include <Servo.h>

#define SPEED 200 //brzina motora
#define SERVO   10 // Pin za servo motor

#define echoPin 52
#define trigPin 53 //senzor kamera pinovi

long duration;
int distance;

Servo servo; // Definiraj servo motor
AF_DCMotor ldM3(1); //prednje desno
AF_DCMotor ddM4(2); //prednje ljevo
AF_DCMotor dgM1(3); //zadnje ljevo
AF_DCMotor lgM2(4); //zadnje desno

void spin()
 {
 dgM1.run(FORWARD);
 lgM2.run(FORWARD);
 ldM3.run(FORWARD);
 ddM4.run(FORWARD);  
}


void naprijed()
{
  dgM1.run(BACKWARD);
  lgM2.run(FORWARD);
  ldM3.run(BACKWARD);
  ddM4.run(FORWARD); 
}

void stani()
{
  dgM1.run(RELEASE);
  lgM2.run(RELEASE);
  ldM3.run(RELEASE);
  ddM4.run(RELEASE);

}



void setup() {
  pinMode(trigPin, OUTPUT); // kamera
  pinMode(echoPin, INPUT); 
/*
  //provjera senzora
  Serial.begin(9600);
  Serial.println("Izmjerena udaljenost do objekta:");
  delay(500);
  */

  // Postavljanje motora
  dgM1.setSpeed(SPEED);
  lgM2.setSpeed(SPEED);
  ldM3.setSpeed(SPEED);
  ddM4.setSpeed(SPEED); 

  dgM1.run(RELEASE);
  lgM2.run(RELEASE);
  ldM3.run(RELEASE);
  ddM4.run(RELEASE); 

}

void loop() {
  
 stani();
 delay(3000);
 spin();
 delay(1000);

 /* digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Čekaj 2 mikrosekunde da se izbjegne kolizija

  digitalWrite(trigPin, HIGH); // Uključuje trigPin da generira i pošalje impuls
  delayMicroseconds(10); // trigPin je upaljen 10 mikrosekundi kako bi slao signal 10 mikro sekundi
  digitalWrite(trigPin, LOW); // Pin se ugasi kako bi prestao slati signale

  duration = pulseIn(echoPin, HIGH); // Funkcija koja mjeri vrijeme koje je potrebno da signal ode i vrati se nazad
  distance = duration * 0.0344 / 2; // Izraz za mjerenje udaljenosti

  Serial.print("Udaljenost: ");
  Serial.print(distance); // Ispis udaljenosti na serijski monitor
  Serial.println(" cm");

  delay(100); /
  
  
  */
}

