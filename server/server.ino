//Librerie
#include <Arduino.h>
#include <binary.h>
#include <SPI.h>
#include <Ethernet.h>

//indirizzo MAC scheda di rete arduino
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0xD4, 0xB1};
//indirizzo ip arduino
byte ip[] = {192, 168, 1, 75};

//Oggetti
EthernetServer server(1032);

//Variabili
unsigned long delaytime = 1000;
int buzzer = 2;
int laser = 6;
int soglia = 960; //va cambiato in base al fotoresistore
int stato = 1 //1-acceso 0-spento                                                                                                                                      ; //1 acceso 0 spento

//fa suonare l'allarme(buzzer) - int A indica lo stato dell'allarme
void allarme(int A){
  int n = 1; //numero di volte che l'arduino deve suonare
  if(A == 1){
    for(int i = 0; i<n; i++){
      digitalWrite(buzzer, HIGH);
      delay(delaytime);
      digitalWrite(buzzer, LOW);
      delay(delaytime);
    }
  }
}

void setup() {
  //setup ethernet
  Ethernet.begin(mac, ip);
  //setup porta seriale per debug
  Serial.begin(9600);
  //setup server 
  server.begin();

  Serial.println("Server Avviato!"); //debug
  //setup pin
  pinMode(buzzer, OUTPUT); //setup pin buzzer
  pinMode(laser, OUTPUT); //setup pin laser
  digitalWrite(laser, HIGH); //accenzione laser
  ///suono di accensione
  digitalWrite(buzzer, HIGH); 
  delay(1000);
  digitalWrite(buzzer, LOW);
}

void loop() {
  EthernetClient client = server.available();
  int lettura = analogRead(A0);
  if(stato== 1){ //se allarme acceso
    digitalWrite(laser, HIGH); 
  }
  else if(stato == 0){//se allarme spento
    digitalWrite(laser, LOW);
  }

  if(client){ //se il client esiste
    if (client.available() > 0){ //se client connesso
      Serial.println("Client connesso!"); //debug
      char ch = client.read(); //lettura valore inviato dal client
      String str = String(ch); //conversione valore inviato in stringa
      Serial.println("il valore inviato è: " + ch); //debug
      if (str == "1"){
        //attivo il laser e cassa
        stato = 1;
        Serial.println("Attivato"); //debug
      }
      else if (str == "2"){
        //disattivo il laser e la cassa
        stato = 0;
        Serial.println("Disattivato");
      }
      else
      {
        Serial.println("Nessun input inviato corrisponde a nessun'azione");
      }
    }
  }
  Serial.println(lettura); //debug
  Serial.println(String(stato)); //debug
  if(lettura < soglia){ //se il laser non punta la fotoresistenza
    allarme(stato); //fai scattare l'allarme
  }  
}
