#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
#include <Ethernet.h> //INCLUSÃO DE BIBLIOTECA

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //ATRIBUIÇÃO DE ENDEREÇO MAC AO ETHERNET SHIELD W5100
IPAddress ip(10, 0, 0, 177);; //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110  **** ISSO VARIA, NO MEU CASO É: 192.168.0.175

EthernetServer server(80); //PORTA EM QUE A CONEXÃO SERÁ FEITA

const int ledPin = 9; //PINO DIGITAL UTILIZADO PELO LED

String readString = String(30); //VARIÁVEL PARA BUSCAR DADOS NO ENDEREÇO (URL)
int status = 0; //DECLARAÇÃO DE VARIÁVEL DO TIPO INTEIRA(SERÁ RESPONSÁVEL POR VERIFICAR O STATUS ATUAL DO LED)

void setup(){
  Serial.begin(9600);
  Serial.println("Ethernet WebServer Example");
  Ethernet.begin(mac, ip); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE
 
  
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA 80
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  pinMode(ledPin, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(ledPin, LOW); //LED INICIA DESLIGADO
  }
void loop(){


EthernetClient client = server.available(); //CRIA UMA CONEXÃO COM O CLIENTE
  if (client) { // SE EXISTE CLIENTE, FAZ
    Serial.println("new client");
    while (client.connected()) {//ENQUANTO EXISTIR CLIENTE CONECTADO, FAZ
      if (client.available()) { //SE O CLIENTE ESTÁ HABILITADO, FAZ
        char c = client.read(); //LÊ CARACTERE A CARACTERE DA REQUISIÇÃO HTTP
        
        if (readString.length() < 100) //SE O ARRAY FOR MENOR QUE 100, FAZ
        {
          readString += c; // "readstring" VAI RECEBER OS CARACTERES LIDO
        }  
        if (c == '\n') { //SE ENCONTRAR "\n" É O FINAL DO CABEÇALHO DA REQUISIÇÃO HTTP
          if (readString.indexOf("?") <0){ //SE ENCONTRAR O CARACTER "?", FAZ
          }
          else //SENÃO,FAZ
          if(readString.indexOf("ledParam=1") >0){
            digitalWrite(ledPin, HIGH); //LIGA O LED
            status = 1; 
           }else{ 
             digitalWrite(ledPin, LOW); //DESLIGA O LED
             status = 0; 
           }
          client.println("HTTP/1.1 200 OK"); 
          client.println("Content-Type: text/html"); 
          client.println("");
          client.println("<!DOCTYPE HTML>"); 
          client.println("<html>");
          client.println("<head>");
          client.println("</head>"); 
          client.println("<body >");
          
          if (status == 1){ 
            client.println("<center><form method=get name=LED><input type=hidden name=ledParam value=0 /><input type=submit value=APAGAR></form></center>");
          }else{ //SENÃO, FAZ
            client.println("<center><form method=get name=LED><input type=hidden name=ledParam value=1 /><input type=submit value=ACENDER></form></center>");
          }                   
          client.println("</body>"); //FECHA A TAG "body"
          client.println("</html>"); //FECHA A TAG "html"
          readString=""; //A VARIÁVEL É REINICIALIZADA
          client.stop(); //FINALIZA A REQUISIÇÃO HTTP E DESCONECTA O CLIENTE
            }
          }
        }
      }
 }
