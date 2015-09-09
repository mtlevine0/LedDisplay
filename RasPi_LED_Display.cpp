//
// g++ -o led RasPi_LED_Display.cpp -lwiringPi
//
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <wiringPi.h>
#include <signal.h>
#include <string>
#include <cstring>

//#include "font.h"
//#include "franklinGothicMed24Bold.h"
//#include "wingdings22.h"
//#include "impact24.h"
//#include "algerian.h"
#include "french_script.h"

#define uint8_t char

using namespace std;

// Constants
int outputEn = 1;
int latchPin = 9;
int clockPin = 7;
int dataPinA = 0;
int dataPinB = 2;
int dataPinC = 3;
int dataPinD = 4;

int brightness = 960; // range is 0-1024

uint8_t buffer[128];
std::string message = "Librul Faggots   ";
int length = message.length();

// Functions
void writeDisplay(void);
void shiftOut(uint8_t dataPinA, uint8_t dataPinB, uint8_t dataPinC, uint8_t dataPinD, uint8_t clockPin, uint8_t valA, uint8_t valB, uint8_t valC, uint8_t valD);
void pinSetup(void);
void sigint(int a);
void clearBuffer(void);

// BEGIN Logic
// Main
int main (void)
{
  std::cout << length << std::endl;
  signal(SIGINT,sigint);
  wiringPiSetup () ;
  pinSetup();
  for (;;)
  {
    const char* strdata = message.c_str();

    for(uint8_t letter = 0; letter < message.length(); letter++){// Scroll through entire font
      for(uint8_t i = 0; i < fontInfo[strdata[letter]-32][0]*4 + 1; i+=4){
        if(fontInfo[strdata[letter]-32][0]*4 == i){// Place one pixel spaces between each character

          for(uint8_t j = 0; j < 124; j+=4){
            buffer[j] = buffer[j+4];
            buffer[j+1] = buffer[j+5];
			buffer[j+2] = buffer[j+6];
			buffer[j+3] = buffer[j+7];
          }

          buffer[127]=0x00;
          buffer[126]=0x00;
		
        } else {
          ::cout << strdata[letter] << std::endl;
          for(uint8_t j = 0; j < 124; j+=4){// Scroll buffer one pixel at a time
            buffer[j] = buffer[j+4];
            buffer[j+1] = buffer[j+5];
			buffer[j+2] = buffer[j+6];
			buffer[j+3] = buffer[j+7];
          }

          buffer[127] = font[fontInfo[strdata[letter]-32][1] + i + 3];
          buffer[126] = font[fontInfo[strdata[letter]-32][1] + i + 2];
		  buffer[125] = font[fontInfo[strdata[letter]-32][1] + i + 1];
		  buffer[124] = font[fontInfo[strdata[letter]-32][1] + i];
        }
        writeDisplay();// Write buffer to display
        delay(40);
      }
    }
  }
  return 0 ;
}

void writeDisplay(void){
  digitalWrite(latchPin, LOW);
  //PORTB = PORTB | (1<<2);
  for(uint8_t i = 0; i < 128; i+=4){
    shiftOut(dataPinA, dataPinB, dataPinC, dataPinD, clockPin, buffer[i], buffer[i+1], buffer[i+2], buffer[i+3]);
  }
  digitalWrite(latchPin, HIGH);
}

void shiftOut(uint8_t dataPinA, uint8_t dataPinB, uint8_t dataPinC, uint8_t dataPinD, uint8_t clockPin, uint8_t valA, uint8_t valB, uint8_t valC, uint8_t valD){
  uint8_t i;
  for (i = 0; i < 8; i++)  {
    /*if(bitOrder == LSBFIRST){
      digitalWrite(dataPinA, !!(valA & (1 << i)));
      digitalWrite(dataPinB, !!(valB & (1 << i)));
    }else{*/
      digitalWrite(dataPinA, !!(valA & (1 << (7 - i))));
      digitalWrite(dataPinB, !!(valB & (1 << (7 - i))));
	  digitalWrite(dataPinC, !!(valC & (1 << (7 - i))));
      digitalWrite(dataPinD, !!(valD & (1 << (7 - i))));
    //}
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

// capture ctrl-c sigint
void sigint(int a) {
  clearBuffer();
  printf("Shutting down...\n");
  exit(a);
}

// writes a blank buffer to display
void clearBuffer(void) {
  memset(buffer, 0, sizeof(buffer));
  writeDisplay();
}

// Pin setup
void pinSetup(void){
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(outputEn, PWM_OUTPUT);
  pinMode(dataPinA, OUTPUT);
  pinMode(dataPinB, OUTPUT);
  pinMode(dataPinC, OUTPUT);
  pinMode(dataPinD, OUTPUT);
  pwmWrite(outputEn, brightness);
}
