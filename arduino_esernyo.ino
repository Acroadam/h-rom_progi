#include <FastLED.h>
#include <SPI.h>
#include <SD.h>

#define FILENAME     "VIDEO.BIN"
#define FILENAME2     "BACK.BIN"
#define SD_CS         PB12

#define LED_PIN0    PA8
#define LED_PIN1    PA9
#define LED_PIN2    PA10
#define LED_PIN3    PA3

#define BOARD_BUTTON_PIN0   PA1
#define BOARD_BUTTON_PIN1   PA2
#define BOARD_BUTTON_PIN2   PB5

#define BOARD_BUTTON_PIN3   PB6
#define BOARD_BUTTON_PIN4   PB7

#define OUTPUT_LED_PIN   PB8
#define OUTPUT_LED_PIN1   PC13

#define LED_TYPE    WS2811

#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 140
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
bool playing = false;
bool playing2 = false;
File videofile;
File videofile2;
int buttonState = 0;

int button1State = 0;
int lastButtonState = 0;

int button2State = 0;
int lastButton2State = 0;

int button4State = 0;
int lastButton4State = 0;

int button5State = 0;
int lastButton5State = 0;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN0>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE, LED_PIN1>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE, LED_PIN2>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<LED_TYPE, LED_PIN3>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

  SPI.setModule(2);
  if (!SD.begin(SD_CS)) {
     Serial.println("initialization failed!");
    return;
  }

  Serial.println("initialization done.");
  videofile = SD.open(FILENAME, FILE_READ);
  if (!videofile) stopWithErrorMessage("Could not read " FILENAME);
  videofile2 = SD.open(FILENAME2, FILE_READ);
  if (!videofile2) stopWithErrorMessage("Could not read " FILENAME2);
  Serial.println("File opened");
  playing = false;
  playing2 = false;
  SPI.setModule(1);

  pinMode(BOARD_BUTTON_PIN3, INPUT);
  pinMode(BOARD_BUTTON_PIN4, INPUT);
  delay(1000);
  for(int i = 0; i < NUM_LEDS; i++){
      leds[i].r = 0;
      leds[i].g = 50;
      leds[i].b = 0;
  }
  FastLED.show();
  delay(1000);
  for(int i = 0; i < NUM_LEDS; i++){
      leds[i].r = 0;
      leds[i].g = 0;
      leds[i].b = 0;
  }
  FastLED.show();
}

void loop() {

  if(!playing && !playing2) {
    //Button4
    button4State = digitalRead(BOARD_BUTTON_PIN3);
    if (button4State != lastButton4State) {
      if (button4State == HIGH) {
        delay(1000);
        button4State = digitalRead(BOARD_BUTTON_PIN3);
        if (button4State == HIGH) {
            playing = true;
        }
      } else {
        Serial.println("off");
      }
    }
    lastButton4State = button4State;

    //Button5
    button5State = digitalRead(BOARD_BUTTON_PIN0);
    if (button5State != lastButton5State) {
      if (button5State == HIGH) {
        delay(1000);
        button5State = digitalRead(BOARD_BUTTON_PIN0);
        if (button5State == HIGH) {
            playing2 = true;
        }
      } else {
        Serial.println("off");
      }
    }
    lastButton5State = button5State;
  }
  if(playing && videofile.available()){
    SPI.setModule(2);
    videofile.readBytes((char*)leds, NUM_LEDS * 3);
    SPI.setModule(1);
    FastLED.show();}
  if(playing2 && videofile2.available()){
    SPI.setModule(2);
    videofile2.readBytes((char*)leds, NUM_LEDS * 3);
    SPI.setModule(1);
    FastLED.show();
  }


}



void stopWithErrorMessage(const char *str)
{
  while (1) {
    //Serial.println(str);
    delay(1000);
  }
}
