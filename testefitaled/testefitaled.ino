#include <Adafruit_NeoPixel.h>




#define PINLED      9
#define N_LEDS 19

const int buttonPin = 2;
int buttonState = 0;
bool status = true;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PINLED, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  pinMode(buttonPin, INPUT); 
}

void loop() {
chase(strip.Color(255, 255, 255));
    
}

static void chase(uint32_t c) {
  
    for(uint16_t i=0; i<strip.numPixels()+3; i++) {
        strip.setPixelColor(i  , c); // Draw new pixel
        strip.setPixelColor(i-3, 0); // Erase pixel a few steps back
        strip.show();
     
        delay(50);      
    }
  
  
    
  
}
