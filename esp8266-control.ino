#include <SoftwareSerial.h>
SoftwareSerial ESP8266(10, 11);
//String NomduReseauWifi = "Entrez le nom de votre Box ou point d'accès Wifi"; // Garder les guillements
//String MotDePasse      = "";
String pagecontent = "<b>device is on</b>\r\n<FORM METHOD=\"LINK\" ACTION=\"/switch\">\r\n<INPUT TYPE=\"submit\" VALUE=\"Clickable Button\">\r\n</FORM>";
String pagecontent2 = "<b>device is off</b>\r\n<FORM METHOD=\"LINK\" ACTION=\"/switch\">\r\n<INPUT TYPE=\"submit\" VALUE=\"Clickable Button\">\r\n</FORM>";
boolean state = false;
char connectionid = '0';
String url = "GET /apps/thinghttp/send_request?api_key=/*your key here !*/ HTTP/1.1\\r\\nHost: api.thingspeak.com\\r\\n\\r\\n\\";
  void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ESP8266.begin(115200);  
  initESP8266();
  pinMode(5,OUTPUT);
  pinMode(4,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(4) == HIGH) {
    envoieAuESP8266("AT+CIPMODE=0");   
    recoitDuESP8266(2000,0);
    Serial.println("**********************************************************");    
   envoieAuESP8266("AT+CIPMUX=1");   
  recoitDuESP8266(2000,0);
  Serial.println("**********************************************************");  
   envoieAuESP8266("AT+CIPSERVER=1,80");
  recoitDuESP8266(2000,0);
  Serial.println("**********************************************************");
  Serial.println("***************** INITIALISATION TERMINEE ****************");
  Serial.println("**********************************************************");
  Serial.println("");
  while (true) {
    if (ESP8266.available())  {
    //if (ESP8266.find("+IDP,")) int connectionid = ESP8266.read()-48;
    if (ESP8266.find("GET / HTTP/1.1")) {
    String recieve = recoitDuESP8266(2000,0);
      envoieAuESP8266(String("AT+CIPSEND=0,")+pagecontent.length());
      delay(500);
      envoieAuESP8266(pagecontent);
      delay(1000);
      envoieAuESP8266("AT+CIPCLOSE=0");
      while(true) {
        if (ESP8266.available()) {
          //if (ESP8266.find("+IDP,")) connectionid = ESP8266.read()-48;
          if (ESP8266.find("GET /switch HTTP/1.1")) {
            String recieve = recoitDuESP8266(2000,0);
            //if (recieve.charAt(recieve.indexOf(",")+1) >='0' && recieve.charAt(recieve.indexOf(",")+1) <= '4') connectionid = recieve.charAt(recieve.indexOf(",")+1);
            state = !state;
            String val;
            if (state) {
              digitalWrite(5,HIGH);
              val = pagecontent;
            }
            else {
              digitalWrite(5,LOW);
              val = pagecontent2;
            }
              delay(1000);
              envoieAuESP8266(String("AT+CIPSEND=")+connectionid+String(",")+val.length());
              delay(500);
              envoieAuESP8266(val);
              delay(1000);
              envoieAuESP8266(String("AT+CIPCLOSE=")+connectionid);
             envoieAuESP8266("AT+CIPMODE=0");   
    recoitDuESP8266(2000,0);
    envoieAuESP8266("AT+RST");
    recoitDuESP8266(2000,0);
    Serial.println("**********************************************************");    
   envoieAuESP8266("AT+CIPMUX=1");   
  recoitDuESP8266(2000,0);
  Serial.println("**********************************************************");  
   envoieAuESP8266("AT+CIPSERVER=1,80");
  recoitDuESP8266(2000,0);
  Serial.println("**********************************************************");
  Serial.println("***************** INITIALISATION TERMINEE ****************");
  Serial.println("**********************************************************");
  Serial.println("");          
      }
    }
    }
 }
  }
}
}

  else {
    envoieAuESP8266("AT+CIPMODE=0");   
    recoitDuESP8266(2000,0);
    Serial.println("**********************************************************"); 
    envoieAuESP8266("AT+CIPMUX=1");
    recoitDuESP8266(2000,0);
    envoieAuESP8266("AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80");
    recoitDuESP8266(2000,0);
    envoieAuESP8266(String("AT+CIPSEND=0,")+String(91,DEC));
    recoitDuESP8266(500,0);
    envoieAuESP8266(url);
    //String recieve = recoitDuESP8266(2000,1);
    while(true) {
    if (ESP8266.available()) {
      if (ESP8266.find("o")) {
        Serial.print("hit");
        String recieve = recoitDuESP8266(2000,1);
        if (recieve.charAt(recieve.indexOf("o")+1) == 'n') state = false;
        else if (recieve.charAt(recieve.indexOf("o")+1) == 'f') state = true;
            String val;
            if (state) {
              digitalWrite(5,HIGH);
              val = pagecontent;
            }
            else {
              digitalWrite(5,LOW);
              val = pagecontent2;
            }          
        }
      }
      
      
    }
    
  }
}
    /*{    
     Serial.println(ESP8266.readString());
   }  */ 
void initESP8266()
{  
  Serial.println("**********************************************************");  
  Serial.println("**************** DEBUT DE L'INITIALISATION ***************");
  Serial.println("**********************************************************");  
  envoieAuESP8266("AT+RST");
  recoitDuESP8266(2000,0);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CWMODE=1");
  recoitDuESP8266(5000,0);
  Serial.println("**********************************************************");
  /*envoieAuESP8266("AT+CWJAP=\""+ NomduReseauWifi + "\",\"" + MotDePasse +"\"");
  recoitDuESP8266(10000);
  Serial.println("**********************************************************");*/
  envoieAuESP8266("AT+CIFSR");
  recoitDuESP8266(2000,0);
  Serial.println("**********************************************************");
}
void envoieAuESP8266(String commande)
{  
  ESP8266.println(commande);
}
/****************************************************************/
/*Fonction qui lit et affiche les messages envoyés par l'ESP8266*/
/****************************************************************/
String recoitDuESP8266(const int timeout,boolean type)
{
  String reponse = "";
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(ESP8266.available())
    {
      char c = ESP8266.read();
      reponse+=c;
    }
  }
  Serial.print(reponse);
  if (type) {
    return(reponse);   
  }
  else {
    return("");
  }
}
