
#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
#include <Ethernet.h> //INCLUSÃO DE BIBLIOTECA
#include <aREST.h>
#include <avr/wdt.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //ATRIBUIÇÃO DE ENDEREÇO MAC AO ETHERNET SHIELD W5100
IPAddress ip(10, 0, 0, 177);; //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110  **** ISSO VARIA, NO MEU CASO É: 192.168.0.175

EthernetServer server(80); //PORTA EM QUE A CONEXÃO SERÁ FEITA


const int ledPin = 9; //PINO DIGITAL UTILIZADO PELO LED


bool vpisca = true;

aREST rest = aREST();
void setup() {
  Serial.begin(9600);

  Serial.println("Ethernet WebServer Example");
  Ethernet.begin(mac, ip); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE

  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA 80
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  pinMode(ledPin, OUTPUT); //DEFINE O PINO COMO SAÍDA
  //LED INICIA DESLIGADO

  Serial.println("setup finished");
}
void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}



static void pisca() {
  while (vpisca){   
    digitalWrite(ledPin, 1);
    delay(200);
    digitalWrite(ledPin, 0);
    delay(200);
  }

}

  
