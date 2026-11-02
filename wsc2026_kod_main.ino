#include <AFMotor.h> 
#include <HCSR04.h>
#include <HUSKYLENS.h>
#include <Servo.h>
#include <Wire.h>
unsigned long lastPrintTime = 0;
const unsigned long repeatInterval = 1000;
// Konstanta brzine na 150
#define SPEED 130

// Pinovi za boje
#define BCRVENA 30 // Pin za crvenu boju
#define BPLAVA  32 // Pin za plavu boju                                                                                                                                                                                                                           
#define BZELENA 28 // Pin za zelenu boju
int lastPrintedID = 0;
#define SERVO   10 // Pin za servo motor
int s[1][3]; // Niz za pohranu boja

int i = 0; // Brojač za petlju

// Definiranje funkcija
void lineFollower();
void birajBoju();
void pCrvena();
void pPlava();
void pZelena();
int  prepoznajBoje();
bool ultraZvucni();

// Senzori za praćenje linije
int OUTs = 38;  // Senzor za sredinu
int OUTl = 36;  // Senzor za lijevo
int OUTr = 40;  // Senzor za desno
int pomOUT = 38; // Senzor koji će biti korišten u funkciji IRSensor
int p=0, c=0, sv=0;
Servo servo; // Definiraj servo motor
int j=0;
// Definiranje gdje su spojeni motori
AF_DCMotor ldM3(1); 
AF_DCMotor ddM4(2); 
AF_DCMotor dgM1(3); 
AF_DCMotor lgM2(4); 

HCSR04 zvucni(26, 28); // Ultrazvučni senzor za udaljenost

HUSKYLENS huskylens; // Kamera za prepoznavanje boja

String daleko = String(zvucni.dist());
 
void setup() {
  // Postavljanje pinova senzora
  pinMode(OUTs, INPUT); 
  pinMode(OUTl, INPUT); 
  pinMode(OUTr, INPUT); 

  // Debug serijski izlaz
  Serial.begin(9600); 
 // Serial2.begin(9600);
  Wire.begin();
  // Postavljanje motora
  dgM1.setSpeed(SPEED);
  lgM2.setSpeed(SPEED);
  ldM3.setSpeed(SPEED);
  ddM4.setSpeed(SPEED); 

  dgM1.run(RELEASE);
  lgM2.run(RELEASE);
  ldM3.run(RELEASE);
  ddM4.run(RELEASE); 

  // Postavi tipke za boje
  pinMode(BCRVENA, INPUT_PULLUP); 
  pinMode(BPLAVA, INPUT_PULLUP); 
  pinMode(BZELENA, INPUT_PULLUP); 

  // Inicijalizacija huskylens kamere
  Wire.begin();
  huskylens.begin(Wire);
  
}

void loop() {
  // Funkcionalnosti koje se mogu aktivirati (zakomentirane dok ne budu potrebne)
  
      dgM1.run(FORWARD);
      lgM2.run(FORWARD);
      ldM3.run(FORWARD);
      ddM4.run(FORWARD); 
      delay(1000);
      dgM1.run(RELEASE);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(RELEASE);
      delay(3000);

}

void udaljenostIspis(){
  daleko = String(zvucni.dist());
  while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        delay(100);
    }
    while (!huskylens.customText(daleko,230,35))  // bool customText(String text,uint16_t x,uint8_t y)
    {
      Serial.println(F("custom text failed!")); 
      delay(100);
    }
}

// Funkcija za senzore da nam ispisuje na Serial Monitoru ako vidi Crno ili Bijelo
void IRSensor() {
  int psensorValue = digitalRead(pomOUT); // Čitanje senzora
  if (psensorValue == 1) {
    Serial.println("Crno"); // Ako je crno
  }
  if (psensorValue == 0) {
    Serial.println("Bijelo"); // Ako je bijelo
  }
}

// Funkcija za zaustavljanje robota kada detektira preblizu ili daleko objekt
void stani() {
  if(zvucni.dist() > 20 || zvucni.dist() < 3 ){  // Ako je udaljenost manja od 3 ili veća od 20 cm
      dgM1.run(FORWARD);
      lgM2.run(FORWARD);
      ldM3.run(FORWARD);
      ddM4.run(FORWARD); 
    }
    else if(zvucni.dist() < 20 && zvucni.dist() > 3 ){ // Ako je udaljenost između 3 i 20 cm
      dgM1.run(RELEASE);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(RELEASE); 
    }
}

// Funkcija za vožnju robota i detekciju prepreka
void obstacles() {
    if(zvucni.dist() > 20 || zvucni.dist() < 3 ){ // Ako je objekt preblizu ili daleko
    // Izjednačavanje brzina
      dgM1.run(FORWARD);
      lgM2.run(FORWARD);
      ldM3.run(FORWARD);
      ddM4.run(FORWARD);
    }
    else if(zvucni.dist() < 20 && zvucni.dist() > 3 ){ // Ako je udaljenost između 3 i 20 cm
      dgM1.run(BACKWARD);
      lgM2.run(BACKWARD);
      ldM3.run(BACKWARD);
      ddM4.run(BACKWARD);
      delay(1000);
      dgM1.run(RELEASE);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(RELEASE); // Zaustavi robota
      delay(500);
      dgM1.run(BACKWARD);
      lgM2.run(FORWARD);
      ldM3.run(FORWARD);
      ddM4.run(BACKWARD); // Kretanje unazad
      delay(1900);
      dgM1.run(RELEASE);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(RELEASE); // Zaustavi robota
      delay(1000);
    }
    else if(zvucni.dist() < 20 && zvucni.dist() > 3){
      dgM1.run(BACKWARD);
      lgM2.run(BACKWARD);
      ldM3.run(BACKWARD);
      ddM4.run(BACKWARD);
      delay(500);
      dgM1.run(RELEASE);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(RELEASE); // Zaustavi robota
      delay(500);
      dgM1.run(FORWARD);
      lgM2.run(BACKWARD);
      ldM3.run(BACKWARD);
      ddM4.run(FORWARD); // Kretanje unazad
      delay(3800);
      dgM1.run(RELEASE);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(RELEASE); // Zaustavi robota
      delay(1000);
    }
}

// Funkcija za praćenje linije pomoću 3 senzora
void lineFollower() {
  int ssensorValue = digitalRead(OUTs); // Središnji senzor
  int lsensorValue = digitalRead(OUTl); // Lijevi senzor
  int rsensorValue = digitalRead(OUTr); // Desni senzor

  if (ssensorValue == 1) {
    if (lsensorValue == 0 && rsensorValue == 0) {
      // Vozi ravno ako su svi senzori na bijeloj liniji

      dgM1.run(FORWARD);
      lgM2.run(FORWARD);
      ldM3.run(FORWARD);
      ddM4.run(FORWARD);
    }
 
    if (lsensorValue == 1 && rsensorValue == 0) {
      // Skreće lijevo ako je lijevi senzor na crnom
      dgM1.run(FORWARD);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(FORWARD);
    }

    if (lsensorValue == 0 && rsensorValue == 1) {
      // Skreće desno ako je desni senzor na crnom
      dgM1.run(RELEASE);
      lgM2.run(FORWARD);
      ldM3.run(FORWARD);
      ddM4.run(RELEASE);
    }

    if (lsensorValue == 1 && rsensorValue == 1) {
      // Ako su oba senzora na crnom, stani
      dgM1.run(RELEASE);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(RELEASE);
      sv++;
    }
  }
  else {
    // Funkcija za vožnju kada su senzori na bijeloj liniji
    if (lsensorValue == 1 && rsensorValue == 0) {

      dgM1.run(FORWARD);
      lgM2.run(RELEASE);
      ldM3.run(RELEASE);
      ddM4.run(FORWARD);
    }

    if (lsensorValue == 1 && rsensorValue == 1) {
      
      dgM1.run(FORWARD);
      lgM2.run(FORWARD);
      ldM3.run(FORWARD);
      ddM4.run(FORWARD);
    }

    if (lsensorValue == 0 && rsensorValue == 0) {
      
      dgM1.run(BACKWARD);
      lgM2.run(BACKWARD);
      ldM3.run(BACKWARD);
      ddM4.run(BACKWARD); 
    }

    if (lsensorValue == 0 && rsensorValue == 1) {
     
      dgM1.run(RELEASE);
      lgM2.run(FORWARD);
      ldM3.run(FORWARD);
      ddM4.run(RELEASE);
    }
  }
}

// Funkcija za prepoznavanje boja pomoću HuskyLens
void birajBoju() {
  while (1) {
    if (!digitalRead(BCRVENA)) {
      // Crvena boja
      Serial.println("Crveni");
      pCrvena();
    }
    if (!digitalRead(BPLAVA)) {
      // Plava boja
      Serial.println("Plavi");
      pPlava();
    }
    if (!digitalRead(BZELENA)) {
      // Zelena boja
      Serial.println("Zeleni");
      pZelena();
    }
  }
}

void pCrvena() { return; }
void pPlava() { return; }
void pZelena() { return; }

// Funkcija za prepoznavanje boje pomoću HuskyLens
int prepoznajBoje() {
  huskylens.request(); // Pošaljite zahtjev za očitavanjem podataka
  if (huskylens.available()) { // Ako je dostupna nova slika
    HUSKYLENSResult result = huskylens.read(); // Očitaj rezultat
    if (result.ID == 1) {
      Serial.println("Prepoznata crvena");
      return 1;
    }
    else if (result.ID == 2) {
      Serial.println("Prepoznata plava");
      return 2;
    }
    else if (result.ID == 3) {
      Serial.println("Prepoznata zelena");
      return 3;
    }
    else {
      Serial.println("Prepoznata boja nije definirana");
      return -1;
    }
  }
}

void boca() {
  if (!huskylens.request()) {
    Serial.println("HUSKYLENS request failed");
    delay(50);
    return;
  }

  int currentID = 0;

  // Find first valid learned ID
  while (huskylens.available()) {
    HUSKYLENSResult r = huskylens.read();
    if (r.ID > 0) {
      currentID = r.ID;
      break;
    }
  }

  unsigned long now = millis();

  // Print if new ID OR same ID after interval
  if (currentID > 0 &&
      (currentID != lastPrintedID || (now - lastPrintTime >= repeatInterval))) {
    if (currentID == 1) {
      Serial.println("ID1 bottle");
    } else if (currentID == 2) {
      Serial.println("ID2 can");
    } else if (currentID == 3) {
      Serial.println("ID3 sponge");
    } else {
      Serial.print("unknown ID ");
      Serial.println(currentID);
    }

    lastPrintedID = currentID;
    lastPrintTime = now;
  }

  // Reset when nothing recognized
  if (currentID == 0) {
    lastPrintedID = 0;
  }

  delay(30); // small pacing helps stability
}
// Funkcija za provjeru udaljenosti pomoću ultrazvučnog senzora
bool ultraZvucni() {
  if (zvucni.dist() > 10 || !zvucni.dist()) {
    Serial.println("Paket nedostupan.");
    return true;
  }
  else {
    Serial.println("Paket na vidiku");
    return false;
  }
  delay(400);
}

void testlinija(){
int ssensorValue = digitalRead(OUTs); // Središnji senzor
  int lsensorValue = digitalRead(OUTl); // Lijevi senzor
  int rsensorValue = digitalRead(OUTr); // Desni senzor

  if (ssensorValue == 1) {
    if (lsensorValue == 0 && rsensorValue == 0) {
      // Vozi ravno ako su svi senzori na bijeloj liniji

     Serial.print("ravno");
    }
 
    if (lsensorValue == 1 && rsensorValue == 0) {
      // Skreće lijevo ako je lijevi senzor na crnom
           Serial.print("desno");

    }

    if (lsensorValue == 0 && rsensorValue == 1) {
      // Skreće desno ako je desni senzor na crnom
           Serial.print("livo");

    }

    if (lsensorValue == 1 && rsensorValue == 1) {
      // Ako su oba senzora na crnom, stani
           Serial.print("sta je");

      sv++;
    }
  }
  else {
    // Funkcija za vožnju kada su senzori na bijeloj liniji
    if (lsensorValue == 1 && rsensorValue == 0) {

          Serial.print("desno");

    }

    if (lsensorValue == 1 && rsensorValue == 1) {
      
           Serial.print("ravno");

    }

    if (lsensorValue == 0 && rsensorValue == 0) {
      
           Serial.print("nazad");

    }

    if (lsensorValue == 0 && rsensorValue == 1) {
     
          Serial.print("livo");

    }
  }
}




