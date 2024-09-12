#include <EEPROM.h>
#include <SPI.h>  //INCLUSÃO DE BIBLIOTECA
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server(80);

String readString;
const String cabecalho = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: POST, GET, PUT, OPTIONS\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";

bool vpiscaLento = true;
bool vpiscaRapido = false;
bool vaceso = false;
int vRed = 1;
int vGreen = 10;
int vBlue = 255;
int alarmPin = A0;
int val = 0;
bool status;
bool statusAnterior;


int botaoReset = 3;
int resetar = 0;

#define PINLED 9


Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PINLED, NEO_GRB + NEO_KHZ800);

void(* resetFunc) (void) = 0;

void setup() {
  resetar = 0;
  IPAddress ip(lerInt(1,2), lerInt(3,4), lerInt(5,6), lerInt(7,8)); 
  IPAddress gateway(lerInt(9,10), lerInt(11,12), lerInt(13,14), lerInt(15,16));    
  IPAddress subnet(lerInt(17,18), lerInt(19,20), lerInt(21,22), lerInt(23,24));    

  Ethernet.begin(mac, ip,gateway, subnet);
  server.begin();
  Serial.begin(9600);
  strip.updateLength(lerInt(25,26));
  strip.begin();
  strip.setBrightness(50);
  status = false;
  statusAnterior = false;

  pinMode(botaoReset, INPUT);
   
}
void loop() {

  pisca();
    
 
}


static void ledControl(String command, String red, String green, String blue) {
  // Serial.println("Comando led=" + command);
  vRed = red.toInt();
  vGreen = green.toInt();
  vBlue = blue.toInt();

  // Serial.println("Comando led=" + command);
  // Serial.println("Red" + red);
  // Serial.println("Green" + green);
  // Serial.println("Blue" + blue+"eee");

  switch (command.toInt()) {
    case 0:
      vpiscaLento = false;
      vpiscaRapido = false;
      vaceso = false;
      break;
    case 1:
      vpiscaLento = true;
      vpiscaRapido = false;
      vaceso = false;
      break;
    case 2:
      vpiscaLento = false;
      vpiscaRapido = true;
      vaceso = false;
      break;
    case 3:
      vpiscaLento = false;
      vpiscaRapido = false;
      vaceso = true;
      break;
  }
}

static void escreveIP(int a, int b, int c, int d ){
  escreveInt(1,2,a); 
  escreveInt(3,4,b);
  escreveInt(5,6,c);
  escreveInt(7,8,d);  
}

void escreveGateway(int a, int b, int c, int d ){
  escreveInt(9,10,a); 
  escreveInt(11,12,b);
  escreveInt(13,14,c);
  escreveInt(15,16,d);
};

void escreveSubNet(int a, int b, int c, int d ){
  escreveInt(17,18,a); 
  escreveInt(19,20,b);
  escreveInt(21,22,c);
  escreveInt(23,24,d);
};

void escreveNumeroLeds(int a ){
  escreveInt(25,26,a);
};


void reset(){       
  if ((digitalRead(botaoReset) == HIGH) && (resetar == 0)){    
      resetar = 1;        
      escreveIP(10, 0, 0, 177);       
      escreveGateway(10, 0, 0, 1);      
      escreveSubNet(255,255,255,255);      
      escreveNumeroLeds(20); 
      delay(1000);   
      resetFunc();
       
  };
}


void activeFromCam(){
  val = analogRead(alarmPin);
  if ( val > 100 ){
    status = true;
  } else {
    status = false;
  }
  if (status != statusAnterior) {
    //sinal camera;
    statusAnterior = status;
    if (status == true ) {
      ledControl("1","255","255","255");
     
    } else {
       ledControl("1","255","0","0");
    }
  }
}

static void requisicao() {

  reset();
  activeFromCam();

  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == '\n') {
          Serial.println(readString);
          client.println(cabecalho);
          Serial.println("inicia");

          //http://10.0.0.177?led=0&R=255&G=0&B=0
          if (readString.indexOf("led=") > 0) {
            ledControl(buscastring(readString.indexOf("led=") + 4, readString.indexOf("&") - 1, readString),
                       buscastring(readString.indexOf("R=") + 2, readString.indexOf("&G") - 1, readString),
                       buscastring(readString.indexOf("G=") + 2, readString.indexOf("&B") - 1, readString),
                       buscastring(readString.indexOf("B=") + 2, readString.indexOf("HTTP") - 1, readString));
            client.print(jsonStatus());
            client.stop();
            readString = "";
          }  //http://10.0.0.177?IPA=10&B=0&C=0&D=177
          else 
          if (readString.indexOf("IPA=") > 0) {
            int a,b,c,d;                       
            a = buscastring(readString.indexOf("IPA=") + 4, readString.indexOf("&") - 1, readString).toInt();
            b = buscastring(readString.indexOf("B=") + 2, readString.indexOf("&C") - 1, readString).toInt();
            c = buscastring(readString.indexOf("C=") + 2, readString.indexOf("&D") - 1, readString).toInt();
            d = buscastring(readString.indexOf("D=") + 2, readString.indexOf("HTTP") - 1, readString).toInt();            
            escreveIP(a,b,c,d);                       
            client.print(jsonStatus());
            client.stop();
            readString = "";           
            } 
            else 
            http://10.0.0.177?GTWA=10&B=0&C=0&D=1
            if (readString.indexOf("GTWA=") > 0) {
              int a,b,c,d;                       
              a = buscastring(readString.indexOf("GTWA=") + 5, readString.indexOf("&") - 1, readString).toInt();
              b = buscastring(readString.indexOf("B=") + 2, readString.indexOf("&C") - 1, readString).toInt();
              c = buscastring(readString.indexOf("C=") + 2, readString.indexOf("&D") - 1, readString).toInt();
              d = buscastring(readString.indexOf("D=") + 2, readString.indexOf("HTTP") - 1, readString).toInt();            
                     
              escreveGateway(a,b,c,d);                       
              client.print(jsonStatus());
              client.stop();
              readString = "";           
            }
            else 
            //http://10.0.0.177?SUBA=255&B=255&C=255&D=255
            if (readString.indexOf("SUBA=") > 0) {
              int a,b,c,d;                       
              a = buscastring(readString.indexOf("SUBA=") + 5, readString.indexOf("&") - 1, readString).toInt();
              b = buscastring(readString.indexOf("B=") + 2, readString.indexOf("&C") - 1, readString).toInt();
              c = buscastring(readString.indexOf("C=") + 2, readString.indexOf("&D") - 1, readString).toInt();
              d = buscastring(readString.indexOf("D=") + 2, readString.indexOf("HTTP") - 1, readString).toInt();            
                           
              escreveSubNet(a,b,c,d);                       
              client.print(jsonStatus());
              client.stop();
              readString = "";           
            }
            else 
            //http://10.0.0.177?LEDS=10
            if (readString.indexOf("LEDS=") > 0) {
              int a;                       
              a = buscastring(readString.indexOf("LEDS=") + 5, readString.indexOf("HTTP") - 1, readString).toInt();                   
              Serial.println(a);                        
              escreveNumeroLeds(a);                       
              client.print(jsonStatus());
              client.stop();
              readString = "";           
            }                                                              
            //http://10.0.0.177?reiniciar=1 
            else if (readString.indexOf("reiniciar=") > 0) {
              client.print(jsonStatus());              
              client.stop();
              resetFunc();
              readString = "";
              
            }
            
            else
            {
              client.print(jsonNaoEncontrado());
              client.stop();
              readString = "";                         
          }
        }
      }
    }
  }
}

void meteorRain(int red, int green, int blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
  setAll(0, 0, 0);


  for (int i = 0; i < strip.numPixels() + strip.numPixels(); i++) {
    requisicao();
    for (int j = 0; j < strip.numPixels(); j++) {
      if ((!meteorRandomDecay) || (random(10) > 5)) {
        fadeToBlack(j, meteorTrailDecay);
      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ((i - j < strip.numPixels()) && (i - j >= 0)) {
        setPixel(i - j, red, green, blue);
      }
    }

    showStrip();
    delay(SpeedDelay);
  }
}


static void pisca() {
  
  requisicao();
  if (vpiscaLento == false and vpiscaRapido == false and vaceso == false) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  //apagado
      strip.show();
    }
  }

  while (vpiscaLento) {
    requisicao();
    meteorRain(vRed, vGreen, vBlue, 10, 19, false, 20);
    // red, green, blue,tamanho do meteoro,rapidez com que a cauda do meteoro decaia,
    //Como os meteoros não são perfeitos, adicionei o 6º parâmetro para imitar alguns tipos de diferença nos detritos, tornando a decadência um pouco aleatória. Se este valor for definido como “verdadeiro”, alguma aleatoriedade será adicionada ao trilho.
    //Finalmente, há o último parâmetro, que basicamente indica quanto a velocidade de desenho deve ser atrasada. Um valor de zero (0) significa velocidade máxima.
  }



  while (vpiscaRapido) {
    requisicao();
    meteorRain(vRed, vGreen, vBlue,10, 19, false, 10);
  }

  while (vaceso) {
    requisicao();
    for (uint16_t i = 0; i < strip.numPixels(); i++) {

      strip.setPixelColor(i, strip.Color(vRed, vGreen, vBlue));  // Draw new pixel
      strip.show();
    }
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int)r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int)g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int)b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, r, g, b);
}


void showStrip() {
  strip.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < strip.numPixels(); i++) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}




