#include <LiquidCrystal.h>              //LCD-Display Bibliothek

#include <DHT.h>                        //Temperatur-/Feuchtigkeitssensor Bibliothek
#include <DHT_U.h>                      

LiquidCrystal lcd(12,11,5,4,3,8);       //LCD-Display an folgenden Pins angeschlossen

#define interruptPin 2                  //Interrupt Pin ist PIN 2 
#define DHTPIN 2                        //Sensor am PIN 2 angeschlossen
#define DHTTYPE DHT11                   //Sensortyp DHT11
DHT dht(DHTPIN, DHTTYPE);              

int roterPin = 6;                       //an folgenenden Pins ist die RGB-LED angeschlossen
int gruenerPin = 9;
int blauerPin = 10;

int minTemperatur =20;                  //mindest Temperatur hier eingeben
int maxTemperatur =27;                  //maximal Temperatur hier eingeben

void setup() {
 lcd.begin (16, 2);                     //LCD-Display mit 16 Zeilen/2 Spalten
 dht.begin();

 pinMode(interruptPin, INPUT);

 attachInterrupt(digitalPinToInterrupt(interruptPin), fehler, CHANGE);
 
 pinMode(roterPin, OUTPUT);
 pinMode(gruenerPin, OUTPUT);
 pinMode(blauerPin, OUTPUT);
}

void loop() {

float feuchtigkeit = dht.readHumidity();               //Feuchtigkeitssensordaten in einer Variablen speichern
float temperatur = dht.readTemperature();              //Temperatursensordaten in einer Variablen speichern

//-----------------------------Überprüfung, ob ein Fehler aufgetreten ist----------------
  if (isnan(feuchtigkeit) || isnan(temperatur)) {
    lcd.print("Sensorfehler");
     asm (                                              //Port 13 auf HIGH -> LED leuchtet
      "sbi %0, %1 \n\t"
     : : "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB5)
);
    delay(2000);
    lcd.clear();
  }  
//-----------------------------Ausgabe der Daten auf Display mit Einheit-----------------
  
  else {
  lcd.setCursor(0, 0);
  lcd.print("Temp.:   ");
  lcd.print(temperatur);
  lcd.print("\337");
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Feucht.: ");
  lcd.print((int)feuchtigkeit);
  lcd.print(" %");
  asm (
  "cbi %0, %1 \n\t"                                    //Port 13 Standardmäßig auf LOW -> LED leuchtet nicht
  : : "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB5)
);
  }

 //------------------------Unterschiedliche Fälle, zur Veränderung der RGB Farbe-----------------

 
     if(temperatur <= minTemperatur){    //zu niedrige Temperatur
        digitalWrite(blauerPin, HIGH);
        digitalWrite(roterPin, LOW);
        digitalWrite(gruenerPin, LOW );
         
     }
    else if(temperatur >= maxTemperatur){ //zu hohe Temperatur
      digitalWrite(blauerPin, LOW);
      digitalWrite(roterPin, HIGH);
      digitalWrite(gruenerPin, LOW);
}
    else if(temperatur > minTemperatur && temperatur < maxTemperatur){  //optimale Temperatur
      digitalWrite(blauerPin, LOW);
      digitalWrite(roterPin, LOW);
      digitalWrite(gruenerPin, HIGH);
    }

}

void fehler(){
  digitalWrite(blauerPin, HIGH);
  digitalWrite(roterPin, HIGH);
  digitalWrite(gruenerPin, HIGH);
}
