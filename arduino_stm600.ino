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
#define LED_PIN4    PB0
#define LED_PIN5    PB1
#define LED_PIN6    PB10
#define LED_PIN7    PB11

#define BOARD_BUTTON_PIN0   PA1
#define BOARD_BUTTON_PIN1   PA2
#define BOARD_BUTTON_PIN2   PB5

#define BOARD_BUTTON_PIN3   PB6
#define BOARD_BUTTON_PIN4   PB7

#define OUTPUT_LED_PIN   PB8
#define OUTPUT_LED_PIN1   PC13

#define LED_TYPE    WS2811

#define NUM_LEDS_STRIP0 64
#define NUM_LEDS_STRIP1 56
#define NUM_LEDS_STRIP2 56
#define NUM_LEDS_STRIP3 64
#define NUM_LEDS_STRIP4 85
#define NUM_LEDS_STRIP5 130
#define NUM_LEDS_STRIP6 130
#define NUM_LEDS_STRIP7 49
#define NUM_LEDS NUM_LEDS_STRIP0+NUM_LEDS_STRIP1+NUM_LEDS_STRIP2+NUM_LEDS_STRIP3+NUM_LEDS_STRIP4+NUM_LEDS_STRIP5+NUM_LEDS_STRIP6+NUM_LEDS_STRIP7

CRGB leds[NUM_LEDS];
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
  int startIndex = 0;
  FastLED.addLeds<LED_TYPE, LED_PIN0>(leds, startIndex, NUM_LEDS_STRIP0);
  startIndex = startIndex + NUM_LEDS_STRIP0;
  FastLED.addLeds<LED_TYPE, LED_PIN1>(leds, startIndex, NUM_LEDS_STRIP1);
  startIndex = startIndex + NUM_LEDS_STRIP1;
  FastLED.addLeds<LED_TYPE, LED_PIN2>(leds, startIndex, NUM_LEDS_STRIP2);
  startIndex = startIndex + NUM_LEDS_STRIP2;
  FastLED.addLeds<LED_TYPE, LED_PIN3>(leds, startIndex, NUM_LEDS_STRIP3);
  startIndex = startIndex + NUM_LEDS_STRIP3;
  FastLED.addLeds<LED_TYPE, LED_PIN4>(leds, startIndex, NUM_LEDS_STRIP4);
  startIndex = startIndex + NUM_LEDS_STRIP4;
  FastLED.addLeds<LED_TYPE, LED_PIN5>(leds, startIndex, NUM_LEDS_STRIP5);
  startIndex = startIndex + NUM_LEDS_STRIP5;
  FastLED.addLeds<LED_TYPE, LED_PIN6>(leds, startIndex, NUM_LEDS_STRIP6);
  startIndex = startIndex + NUM_LEDS_STRIP6;
  FastLED.addLeds<LED_TYPE, LED_PIN7>(leds, startIndex, NUM_LEDS_STRIP7);

  SPI.setModule(2);
  SPI.setClockDivider(SPI_CLOCK_DIV16);      // Slow speed (72 / 16 = 4.5 MHz SPI_1 speed)
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
    button5State = digitalRead(BOARD_BUTTON_PIN4);
    if (button5State != lastButton5State) {
      if (button5State == HIGH) {
        delay(1000);
        button5State = digitalRead(BOARD_BUTTON_PIN4);
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

    int counter = 0;
    do{
      int buf_len;
      if( (counter + 160) <= NUM_LEDS){
        buf_len = 480;
      } else {
        buf_len = (NUM_LEDS - counter) * 3;
      }
      char buf[buf_len];
      videofile.readBytes(buf, buf_len);
      int i;
      for(i = 0; i < buf_len / 3; i++){
        leds[counter + i].r = buf[i * 3 + 0];
        leds[counter + i].g = buf[i * 3 + 1];
        leds[counter + i].b = buf[i * 3 + 2];
      }
      counter += buf_len / 3;
    } while(counter < NUM_LEDS);

    FastLED.show();
  }

  if(playing2 && videofile2.available()){

    int counter = 0;
    do{
      int buf_len;
      if( (counter + 160) <= NUM_LEDS){
        buf_len = 480;
      } else {
        buf_len = (NUM_LEDS - counter) * 3;
      }
      char buf[buf_len];
      videofile2.readBytes(buf, buf_len);
      int i;
      for(i = 0; i < buf_len / 3; i++){
        leds[counter + i].r = buf[i * 3 + 0];
        leds[counter + i].g = buf[i * 3 + 1];
        leds[counter + i].b = buf[i * 3 + 2];
      }
      counter += buf_len / 3;
    } while(counter < NUM_LEDS);

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
