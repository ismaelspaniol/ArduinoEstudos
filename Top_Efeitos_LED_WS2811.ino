/*
      Programa de Efeito de Luzes para Fita de Led WS2811
      Pode ser expandido para mais fitas ao mesmo tempo, basta ir duplicando comandos

      Componentes:
        - Arduino (Qualquer placa);
        - Fita de Leds WS2811 (qualquer quantidade de Leds);
        - Resistor de 200-500 Ohms para pino Din, caso fio seja maior que 1 metro;
        - Fonte de energia com potência compatível ao número de Leds;

      Versão 1.0 - Versão inicial com 9 Efeitos para 1 Fita - 28/Nov/2020

 *    * Criado por Cleber Borges - FunBots - @cleber.funbots  *     *

      Instagram: https://www.instagram.com/cleber.funbots/
      Facebook: https://www.facebook.com/cleber.funbots
      YouTube: https://www.youtube.com/channel/UCKs2l5weIqgJQxiLj0A6Atw
      Telegram: https://t.me/cleberfunbots

*/

// Inclusão das bibliotecas
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <stdint.h>
#include <math.h>

// Definições das fitas
#define strip1_Pin 22             //  Pino de sinal
const int strip1_NumLeds = 300;   //  Número de Leds
const int MAX_NUM_LEDS = 300;     //  Máximo Número de Leds (Soma dos Leds de todas as Fitas)

/*
    Configuração de velocidade e tipo da Fita via Adafruit_NeoPixel, mantive os comentários originais
*/
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(strip1_NumLeds, strip1_Pin, NEO_GRB + NEO_KHZ800);

/*
    Comentário original da biblioteca NeoPixel, alertando à necessidade de
    utilizar um capacitor e um resistor no pino sinal, caso fios tenham longa distância
*/
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//  Variável de brilho geral
float valBrightnessAll = 200;

// Variáveis de Contagem de tempo
unsigned long start, finished, elapsed;
unsigned long turnONTime, time1, time2, elapsedTime;
unsigned long timeTransition = 10000;
int beatCounter = 0;

// Variáveis de Cores
uint32_t white = strip1.Color(255, 255, 255);
uint32_t blue = strip1.Color(0, 0, 255);
uint32_t red = strip1.Color(255, 0, 0);
uint32_t green = strip1.Color(0, 255, 0);
uint32_t orange = strip1.Color(255, 153, 0);
uint32_t purple = strip1.Color(255, 0, 255);
uint32_t pink = strip1.Color(255, 153, 255);
uint32_t yellow = strip1.Color(255, 255, 0);
uint32_t gold = strip1.Color(153, 153, 0);
uint32_t cyan = strip1.Color(0, 255, 255);
uint32_t black = strip1.Color(0, 0, 0);
uint32_t selColor, currentColor;
uint8_t r, g, b;
uint32_t AliceBlue = 0xF0F8FF;
uint32_t Chocolate = 0xD2691E;

/*
   Aqui deixo outras cores que achei na internet, mas achei muito e não utilizei
  uint32_t AliceBlue = 0xF0F8FF
  uint32_t Amethyst = 0x9966CC
  uint32_t AntiqueWhite = 0xFAEBD7
  uint32_t  Aqua = 0x00FFFF
  uint32_t Aquamarine = 0x7FFFD4
  uint32_t Azure = 0xF0FFFF
  uint32_t Beige = 0xF5F5DC
  uint32_t Bisque = 0xFFE4C4
  uint32_t BlanchedAlmond = 0xFFEBCD
  uint32_t Blue = 0x0000FF
  uint32_t BlueViolet = 0x8A2BE2
  uint32_t Brown = 0xA52A2A
  uint32_t BurlyWood = 0xDEB887
  uint32_t CadetBlue = 0x5F9EA0
  uint32_t Chartreuse = 0x7FFF00
  uint32_t Chocolate = 0xD2691E;
  uint32_t Coral = 0xFF7F50
  uint32_t CornflowerBlue = 0x6495ED
  uint32_t Cornsilk = 0xFFF8DC
  uint32_t Crimson = 0xDC143C
  uint32_t Cyan = 0x00FFFF
  uint32_t DarkBlue = 0x00008B
  uint32_t DarkCyan = 0x008B8B
  uint32_t DarkGoldenrod = 0xB8860B
  uint32_t DarkGray = 0xA9A9A9
  uint32_t DarkGreen = 0x006400
  uint32_t DarkKhaki = 0xBDB76B
  uint32_t DarkMagenta = 0x8B008B
  uint32_t DarkOliveGreen = 0x556B2F
  uint32_t DarkOrange = 0xFF8C00
  uint32_t DarkOrchid = 0x9932CC
  uint32_t DarkRed = 0x8B0000
  uint32_t DarkSalmon = 0xE9967A
  uint32_t DarkSeaGreen = 0x8FBC8F
  uint32_t DarkSlateBlue = 0x483D8B
  uint32_t DarkSlateGray = 0x2F4F4F
  uint32_t DarkTurquoise = 0x00CED1
  uint32_t DarkViolet = 0x9400D3
  uint32_t DeepPink = 0xFF1493
  uint32_t DeepSkyBlue = 0x00BFFF
  uint32_t DimGray = 0x696969
  uint32_t DodgerBlue = 0x1E90FF
  uint32_t FireBrick = 0xB22222
  uint32_t FloralWhite = 0xFFFAF0
  uint32_t ForestGreen = 0x228B22
  uint32_t Fuchsia = 0xFF00FF
  uint32_t Gainsboro = 0xDCDCDC
  uint32_t GhostWhite = 0xF8F8FF
  uint32_t Gold = 0xFFD700
  uint32_t Goldenrod = 0xDAA520
  uint32_t Gray = 0x808080
  uint32_t Green = 0x008000
  uint32_t GreenYellow = 0xADFF2F
  uint32_t Honeydew = 0xF0FFF0
  uint32_t HotPink = 0xFF69B4
  uint32_t IndianRed = 0xCD5C5C
  uint32_t Indigo = 0x4B0082
  uint32_t Ivory = 0xFFFFF0
  uint32_t Khaki = 0xF0E68C
  uint32_t Lavender = 0xE6E6FA
  uint32_t LavenderBlush = 0xFFF0F5
  uint32_t LawnGreen = 0x7CFC00
  uint32_t LemonChiffon = 0xFFFACD
  uint32_t LightBlue = 0xADD8E6
  uint32_t LightCoral = 0xF08080
  uint32_t LightCyan = 0xE0FFFF
  uint32_t LightGoldenrodYellow = 0xFAFAD2
  uint32_t LightGreen = 0x90EE90
  uint32_t LightGrey = 0xD3D3D3
  uint32_t LightPink = 0xFFB6C1
  uint32_t LightSalmon = 0xFFA07A
  uint32_t LightSeaGreen = 0x20B2AA
  uint32_t LightSkyBlue = 0x87CEFA
  uint32_t LightSlateGray = 0x778899
  uint32_t LightSteelBlue = 0xB0C4DE
  uint32_t LightYellow = 0xFFFFE0
  uint32_t Lime = 0x00FF00
  uint32_t LimeGreen = 0x32CD32
  uint32_t Linen = 0xFAF0E6
  uint32_t Magenta = 0xFF00FF
  uint32_t Maroon = 0x800000
  uint32_t MediumAquamarine = 0x66CDAA
  uint32_t MediumBlue = 0x0000CD
  uint32_t MediumOrchid = 0xBA55D3
  uint32_t MediumPurple = 0x9370DB
  uint32_t MediumSeaGreen = 0x3CB371
  uint32_t MediumSlateBlue = 0x7B68EE
  uint32_t MediumSpringGreen = 0x00FA9A
  uint32_t MediumTurquoise = 0x48D1CC
  uint32_t MediumVioletRed = 0xC71585
  uint32_t MidnightBlue = 0x191970
  uint32_t MintCream = 0xF5FFFA
  uint32_t MistyRose = 0xFFE4E1
  uint32_t Moccasin = 0xFFE4B5
  uint32_t NavajoWhite = 0xFFDEAD
  uint32_t Navy = 0x000080
  uint32_t OldLace = 0xFDF5E6
  uint32_t Olive = 0x808000
  uint32_t OliveDrab = 0x6B8E23
  uint32_t Orange = 0xFFA500
  uint32_t OrangeRed = 0xFF4500
  uint32_t Orchid = 0xDA70D6
  uint32_t PaleGoldenrod = 0xEEE8AA
  uint32_t PaleGreen = 0x98FB98
  uint32_t PaleTurquoise = 0xAFEEEE
  uint32_t PaleVioletRed = 0xDB7093
  uint32_t PapayaWhip = 0xFFEFD5
  uint32_t PeachPuff = 0xFFDAB9
  uint32_t Peru = 0xCD853F
  uint32_t Pink = 0xFFC0CB
  uint32_t Plaid = 0xCC5533
  uint32_t Plum = 0xDDA0DD
  uint32_t PowderBlue = 0xB0E0E6
  uint32_t Purple = 0x800080
  uint32_t Red = 0xFF0000
  uint32_t RosyBrown = 0xBC8F8F
  uint32_t RoyalBlue = 0x4169E1
  uint32_t SaddleBrown = 0x8B4513
  uint32_t Salmon = 0xFA8072
  uint32_t SandyBrown = 0xF4A460
  uint32_t SeaGreen = 0x2E8B57
  uint32_t Seashell = 0xFFF5EE
  uint32_t Sienna = 0xA0522D
  uint32_t Silver = 0xC0C0C0
  uint32_t SkyBlue = 0x87CEEB
  uint32_t SlateBlue = 0x6A5ACD
  uint32_t SlateGray = 0x708090
  uint32_t Snow = 0xFFFAFA
  uint32_t SpringGreen = 0x00FF7F
  uint32_t SteelBlue = 0x4682B4
  uint32_t Tan = 0xD2B48C
  uint32_t Teal = 0x008080
  uint32_t Thistle = 0xD8BFD8
  uint32_t Tomato = 0xFF6347
  uint32_t Turquoise = 0x40E0D0
  uint32_t Violet = 0xEE82EE
  uint32_t Wheat = 0xF5DEB3
  uint32_t White = 0xFFFFFF
  uint32_t WhiteSmoke = 0xF5F5F5
  uint32_t Yellow = 0xFFFF00
  uint32_t YellowGreen = 0x9ACD32
*/

// Variáveis para Efeito Coração
long heartBeatArray[] = {50, 100, 15, 1200};
int hbeatIndex = 1;   // this initialization is important or it starts on the "wrong foot"
long prevMillis;

// Variáveis para Modo de operação
int modo = 1;
int modoAnterior = 2;

// Variável para Efeito Fogo
static int n = strip1.numPixels();


/****************************************************************************************/
/************************************* Função SETUP *************************************/
/****************************************************************************************/

void setup() {

  // Inicia Fita
  strip1.begin();
  strip1.show();

  // Inicia Brilho
  strip1.setBrightness(valBrightnessAll);

  // Contagem de Tempo
  turnONTime = millis();
  time1 = millis();
  time2 = millis();

  // Seleciona cor
  selColor = corAleatoria();

  // Apaga Fita
  apagaLEDS();

  //Debug
  /*
    Serial.begin(115200);
    Serial.println("Serial 1 Inicializado\n");

    Serial.print("Cor: ");
    Serial.print(selColor);
    Serial.println();
  */

}

/****************************************************************************************/
/************************************** Função LOOP *************************************/
/****************************************************************************************/

void loop() {

  // Randomiza Modo de operação, sem repetir modo duas vezes seguidas
  modo = random (1, 9);

  while (modo == modoAnterior) {
    modo = random (1, 9);
  }

  // Faz o efeito conforme numero sorteado
  switch (modo) {
    case 1:             // Repiração
      time1 = millis();
      time2 = millis();
      apagaLEDS();
      while (time2 - time1 < timeTransition) {
        breathing(selColor);
        time2 = millis();
      }
      selColor = corAleatoria();
      brilhoTotal(valBrightnessAll);
      break;

    case 2:           // Cinema
      time1 = millis();
      time2 = millis();
      while (time2 - time1 < timeTransition) {
        theaterChase(selColor, 20);
        time2 = millis();
      }
      apagaLEDS();
      selColor = corAleatoria();
      break;

    case 3:         // Cinema Arco-Iris
      theaterChaseRainbow(20);
      apagaLEDS();
      selColor = corAleatoria();
      break;

    case 4:         // Cintilante
      time1 = millis();
      time2 = millis();
      apagaLEDS();
      while (time2 - time1 < timeTransition) {
        Sparkle(selColor, 10);
        time2 = millis();
      }
      selColor = corAleatoria();
      apagaLEDS();
      break;

    case 5:         // Coração
      time1 = millis();
      time2 = millis();
      apagaLEDS();
      while (time2 - time1 < timeTransition) {
        heartBeat(1.0);
        time2 = millis();
      }
      selColor = corAleatoria();
      brilhoTotal(valBrightnessAll);
      break;

    case 6:         // Fogo
      apagaLEDS();
      time1 = millis();
      time2 = millis();
      while (time2 - time1 < timeTransition) {
        Fire(40, 20, 2); //Fire(40, 120, 1);
        time2 = millis();
      }
      brilhoTotal(valBrightnessAll);
      break;

    case 7:         // Strobo
      Strobe (selColor, 8, 2, 100);
      delay (3000);
      selColor = corAleatoria();
      brilhoTotal(valBrightnessAll);
      apagaLEDS();

      break;

    case 8:         // Arco-Iris
      time1 = millis();
      time2 = millis();
      while (time2 - time1 < timeTransition) {
        rainbow(8);
        time2 = millis();
      }
      apagaLEDS();
      break;

    case 9:         // Rebate
      selColor = corAleatoria();
      CylonBounce(selColor, 4, 1, 1);
      break;

    default:
      break;

  }
  modoAnterior = modo;
  valBrightnessAll = 200;

}


/****************************************************************************************/
/*********************************** EFEITO ARCO-ÍRIS ***********************************/
/****************************************************************************************/

void rainbow(uint8_t wait) {
  uint16_t i, j, n;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip1.show();
    delay(wait);
  }
}

/****************************************************************************************/
/************************************ EFEITO CINEMA *************************************/
/****************************************************************************************/

void theaterChase(uint32_t c, uint8_t wait) {

  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip1.numPixels(); i = i + 3) {
        strip1.setPixelColor(i + q, c);  //turn every third pixel on
      }
      
      strip1.show();
      delay(wait);

      for (uint16_t i = 0; i < strip1.numPixels(); i = i + 3) {
        strip1.setPixelColor(i + q, 0);      //turn every third pixel off
      }

    }
  }
}

/****************************************************************************************/
/***************************** EFEITO CINEMA COM ARCO-IRIS ******************************/
/****************************************************************************************/

void theaterChaseRainbow(uint8_t wait) {
  uint16_t n = 0;

  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip1.numPixels(); i = i + 3) {
        strip1.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      
      strip1.show();
      delay(wait);

      for (uint16_t i = 0; i < strip1.numPixels(); i = i + 3) {
        strip1.setPixelColor(i + q, 0);      //turn every third pixel off
      }
      
    }
  }
}

/****************************************************************************************/
/*********************************** EFEITO RESPIRAÇÃO **********************************/
/****************************************************************************************/

void breathing(uint32_t selColor) {

  valBrightnessAll = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
  strip1.setBrightness(valBrightnessAll);
  acendeTudo(selColor);

}

/****************************************************************************************/
/*********************************** EFEITO CINTILANTE **********************************/
/****************************************************************************************/

void Sparkle(uint32_t selColor, int SpeedDelay) {

  int Pixel1 = random(strip1.numPixels());
  strip1.setPixelColor(Pixel1, selColor);
  int Pixel14 = random(strip1.numPixels());
  strip1.setPixelColor(Pixel14, selColor);
  strip1.show();
  delay(SpeedDelay);
  strip1.setPixelColor(Pixel1, 0);
  strip1.setPixelColor(Pixel14, 0);

}

/****************************************************************************************/
/************************************ EFEITO CORAÇÃO ************************************/
/****************************************************************************************/

void heartBeat(float tempo) {
  if ((millis() - prevMillis) > (long)(heartBeatArray[hbeatIndex] * tempo)) {
    hbeatIndex++;
    if (hbeatIndex > 3) hbeatIndex = 0;

    if ((hbeatIndex % 2) == 0) {
      //digitalWrite(LEDpin, HIGH);
      acendeTudo(selColor);

      delay((int)heartBeatArray[hbeatIndex]) ;
      apagaLEDS();

      //digitalWrite(LEDpin, LOW);
    }
    hbeatIndex++;
    // Serial.println(hbeatIndex);
    prevMillis = millis();

  }
}

/****************************************************************************************/
/************************************* EFEITO STROBO ************************************/
/****************************************************************************************/

void Strobe (uint32_t selColor, int StrobeCount, int FlashDelay, int EndPause) {
  for (int j = 0; j < StrobeCount; j++) {
    acendeTudo(selColor);

    delay(FlashDelay);
    apagaLEDS();

    delay(FlashDelay);
  }
  delay(EndPause);
}


/****************************************************************************************/
/************************************** EFEITO FOGO *************************************/
/****************************************************************************************/

void Fire(int Cooling, int Sparking, int SpeedDelay) {

  static byte heat[131];
  int cooldown, i;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < strip1.numPixels(); i++) {
    cooldown = random(0, ((Cooling * 10) / strip1.numPixels()) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = strip1.numPixels() - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < strip1.numPixels(); j++) {
    setPixelHeatColor(/*strip1.numPixels() - */j, heat[j] );
  }

  strip1.show();


  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    //setPixel(Pixel, 255, 255, heatramp);
    strip1.setPixelColor(Pixel, strip1.Color(255, 255, heatramp));
  } else if ( t192 > 0x40 ) {            // middle
    //setPixel(Pixel, 255, heatramp, 0);
    strip1.setPixelColor(Pixel, strip1.Color(255, heatramp, 0));
  } else {                               // coolest
    //setPixel(Pixel, heatramp, 0, 0);
    strip1.setPixelColor(Pixel, strip1.Color(heatramp, 0, 0));
  }

}

/****************************************************************************************/
/************************************* EFEITO REBATE ************************************/
/****************************************************************************************/

void CylonBounce(uint32_t selColor, int EyeSize, int SpeedDelay, int ReturnDelay) {

  r = (uint8_t)(selColor >> 16),
  g = (uint8_t)(selColor >>  8),
  b = (uint8_t)selColor;

  for (int i = 0; i < strip1.numPixels() - EyeSize - 2; i++) {
    apagaStrip1();

    //setPixel(i, red/10, green/10, blue/10);
    strip1.setPixelColor(i, strip1.Color(r / 10, g / 10, b / 10));

    for (int j = 1; j <= EyeSize; j++) {
      //setPixel(i + j, red, green, blue);
      strip1.setPixelColor(i + j, selColor);

    }
    //setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    strip1.setPixelColor(i + EyeSize + 1, strip1.Color(r / 10, g / 10, b / 10));

    //showStrip();
    strip1.show();

    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for (int i = strip1.numPixels() - EyeSize - 2; i > 0; i--) {
    apagaStrip1();

    //setPixel(i, red / 10, green / 10, blue / 10);
    strip1.setPixelColor(i, strip1.Color(r / 10, g / 10, b / 10));

    for (int j = 1; j <= EyeSize; j++) {
      //setPixel(i + j, red, green, blue);
      strip1.setPixelColor(i + j, selColor);

    }
    //setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    strip1.setPixelColor(i + EyeSize + 1, strip1.Color(r / 10, g / 10, b / 10));


    //showStrip();
    strip1.show();

    delay(SpeedDelay);
  }

  delay(ReturnDelay);
}

/****************************************************************************************/
/************************************ FUNCOES SUPORTE ***********************************/
/****************************************************************************************/

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void brilhoTotal(int valBrightnessAll) {
  strip1.setBrightness(valBrightnessAll);
}

void acendeTudo(uint32_t selColor) {
  for (int i = 0; i < MAX_NUM_LEDS; i++) {
    if (i < strip1.numPixels()) {
      strip1.setPixelColor(i, selColor);
    }
  }
  strip1.show();
  delay (10);
}

uint32_t corAleatoria () {

  int num = random (1, 10);

  switch (num) {
    case 1:
      return white;
      break;
    case 2:
      return blue;
      break;
    case 3:
      return red;
      break;
    case 4:
      return green;
      break;
    case 5:
      return orange;
      break;
    case 6:
      return purple;
      break;
    case 7:
      return pink;
      break;
    case 8:
      return yellow;
      break;
    case 9:
      return Chocolate;
      break;
    case 10:
      return cyan;
      break;
    default:
      return black;
      break;
  }

}

void apagaLEDS() {
  int i = 0;
  for (i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, black);
  }
  strip1.show();
  delay (10);

}

void apagaStrip1() {
  int i = 0;
  for (i = 0; i < strip1.numPixels(); i++) {
    strip1.setPixelColor(i, black);
  }
  strip1.show();
  delay (2);
}

