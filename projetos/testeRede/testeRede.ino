
#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
#include <Ethernet.h> //INCLUSÃO DE BIBLIOTECA
#include <aREST.h>
#include <avr/wdt.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //ATRIBUIÇÃO DE ENDEREÇO MAC AO ETHERNET SHIELD W5100
IPAddress ip(10, 0, 0, 177);; //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110  **** ISSO VARIA, NO MEU CASO É: 192.168.0.175

EthernetServer server(80); //PORTA EM QUE A CONEXÃO SERÁ FEITA
int temperature;
int humidity;

const int ledPin = 9; //PINO DIGITAL UTILIZADO PELO LED

int ledControl(String command);

bool vpisca = true;

aREST rest = aREST();
void setup() {
  Serial.begin(9600);
  temperature = 24;
  humidity = 40;
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);

  rest.function("led", ledControl);
  rest.set_id("008");
  rest.set_name("retorno");

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
  pisca();
}

int ledControl(String command) {
  Serial.println("teste");
  // Get state from command
  int state = command.toInt();
  vpisca = state;
  
  digitalWrite(ledPin, state);
  return state;

}

static void pisca() {
  EthernetClient client = server.available();
  rest.handle(client);
  wdt_reset();
  while (vpisca){
    EthernetClient client = server.available();
    rest.handle(client);
    wdt_reset();    
    digitalWrite(ledPin, 1);
    delay(200);
    digitalWrite(ledPin, 0);
    delay(200);
  }

}

  
