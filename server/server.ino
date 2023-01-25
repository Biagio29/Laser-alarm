//librerie
#include <Arduino.h>
#include <binary.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0xD4, 0xB1};
byte ip[] = {192, 168, 1, 75};

EthernetServer server(1032);

unsigned long delaytime = 500; //tempo di delay

int ledDiStato = LED_BUILTIN; //se il led è acceso l'allarme sarà attivo se il led è spento l'allarme sarà disattivato
int buzzer = 2; //pin su cui è collegato il buzzer
int laser = 6; //pin su cui è collegato il laser
int soglia = 960; //va cambiato in base al fotoresistore
int stato = 1; //1 acceso 0 spento

//funzione che attiva l'allarme per due volte ogni mezzo secondo
void allarme(){
  for(int i = 0; i<10; i++){
    digitalWrite(buzzer, HIGH);
    delay(delaytime);
    digitalWrite(buzzer, LOW);
    delay(delaytime);
  }
}

void setup() {
  Ethernet.begin(mac, ip);
  //setup porta seriale per le stampe a schermo
  Serial.begin(9600);
  Serial.println("Server Avviato!");
  //Setup Pin
  pinMode(buzzer, OUTPUT);
  pinMode(laser, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //accensione del led di stato
  digitalWrite(LED_BUILTIN, HIGH);
  //accensione del laser
  digitalWrite(laser, HIGH);
}

void loop() {
  //creazione variabile client
  EthernetClient client = server.available();

  //legge i valori provenienti dal fotoresitore
  int lettura = analogRead(A0);
  //se viene riscontrato un client
  if(client){
    //se ce comunicazione con il client
    if (client.available() > 0){
      Serial.println("Client connesso!");
      char ch = client.read();
      String str = String(ch);
      Serial.print("Il client ha inviato");
      Serial.println(str); //debug
      if (ch == 'a'){
        //attivo il laser
        digitalWrite(laser, HIGH);
        digitalWrite(ledDiStato, HIGH);
        stato = 1;
      }
      else if (ch == 'd'){
        //disattivo il laser
        stato = 0;
        digitalWrite(laser, LOW);
        // digitalWrite(ledDiStato, LOW);
      }
      else
      {
        Serial.println("Nessun input inviato corrisponde ad un'azione");
      }
    }
    else{
      Serial.println("Il client non risponde");
    }
  }
  else{
      Serial.println("Nessun client");
  }
  Serial.println(lettura); //debug
  //se il laser non punta il fotoresistore e l'allarme è attivo
  if(lettura < soglia && stato == 1){
    allarme();
  }

}
