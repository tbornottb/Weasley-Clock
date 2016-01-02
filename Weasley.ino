// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define PIXEL_COUNT 50


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, D3, WS2812B);
int brite = 3;
int buttonPin = D0;

struct entry {
    String str;
    int n;
};

struct entry places[] = {
    "peril", 1,
    "holiday", 2,
    "work", 3,
    "home", 4,
	"way", 5,
	"forest", 6,
    0,0
};

struct entry people[] = {
    "T", 1,
    "D", 2,
    "R", 3,
    "K", 4,
	"B", 5,
	"A", 6,
    0,0
};

struct entry colors[] = {
    "R", 0x66ccff,
    "D", 0x00736A4,
    "T", 0x00cc00,
    "A", 0xe5ccff,
	"B", 0xcc00cc,
	"K", 0xff6600,
    0,0
};


int updateClock(String command) {
  int commaIndex = command.indexOf(' ');
  int secondCommaIndex = command.indexOf(' ', commaIndex+1);
  String name = command.substring(0, commaIndex);
  String loc = command.substring(commaIndex+1, secondCommaIndex);
  int person = number_for_key(name, people);
  if(person != 0){
	  flashB(number_for_key(name, colors));
	  int place = number_for_key(loc, places);
	  if(place != 0){
		  allMineOff(person);
		  strip.setPixelColor(7+person+6*place, strip.Color(255, 255, 255));
		  strip.show();
	  }
  }
  return 1;
}

void setup() {
    Particle.function("updateClock", updateClock);
    pinMode(buttonPin, INPUT);
	strip.begin();
    strip.show();
	startupSwirl();
	strip.setBrightness(80);
	allOff();
	for(int j=1; j<=6; j++) { // set everyone to home
	    strip.setPixelColor(7+j+6*4, strip.Color(255, 255, 255));
	}
}

void loop() {
    strip.show();
    
    int buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
        if(brite==3){
            brite--;
            strip.setBrightness(50);
        }
        else if(brite==2){
            brite--;
            strip.setBrightness(20);
        }
        else{
            brite=3;
            strip.setBrightness(80);
        }
        delay(500);
    }

}

int number_for_key(String key, struct entry dict[]){
    for(int j=0; j<6; j++) {
        String name = dict[j].str;
        if (strcmp(name, key) == 0)
            return dict[j].n;
    }
    return 0;
}

void startupSwirl() { // outer hands counterclockwise, inner hands clockwise
	int swirldelay = 50;
	for(int j=0; j<2; j++) {
    	for(int i=14; i<50; i+=3) {
    		allOff();
    		strip.setPixelColor(i, strip.Color(255, 255, 255));
    		strip.setPixelColor(62-i, strip.Color(255, 255, 255));
    		strip.show();
    		delay(swirldelay);
    		
    		allOff();
    		strip.setPixelColor(i+2, strip.Color(255, 255, 255));
    		strip.setPixelColor(62-i, strip.Color(255, 255, 255));
    		strip.show();
    		delay(swirldelay);
    	}
	}
}

void flashB(int hexValue){
	int red = floor(hexValue / (256*256));
    int green = (int)floor(hexValue / 256) % 256;
    int blue = hexValue % 256;
	for(int j=0; j<5; j++) {
		for(int i=0; i<14; i++) {
			strip.setPixelColor(i, strip.Color(green, red, blue));
		}
		strip.show();
		delay(300);
		for(int i=0; i<14; i++) {
			strip.setPixelColor(i, strip.Color(0,0,0));
		}
		strip.show();
		delay(300);
	}
}

void allMineOff(int n) {
	for(int i=7+n; i<PIXEL_COUNT; i+=6) {
		strip.setPixelColor(i, strip.Color(0, 0, 0));
	}
	strip.show();
}

void allOff() {
	for(int i=0; i<PIXEL_COUNT; i++) {
		strip.setPixelColor(i, strip.Color(0, 0, 0));
	}
	strip.show();
}

void debug(String message, int value) {
    char msg [50];
    sprintf(msg, message.c_str(), value);
    Spark.publish("DEBUG", msg);
}
