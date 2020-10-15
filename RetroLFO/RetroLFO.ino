// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
// https://github.com/JChristensen/tinySPI
//
// Example sketch to demonstrate use of hardware or software SPI on an
// ATtiny25/45/85 to make an 8-LED chaser with a 74HC595 shift register.
// Tested with Arduino 1.8.5 and the ATTiny Core,
// https://github.com/SpenceKonde/ATTinyCore
//
// Set the system clock to 1MHz internal.
//
// Connections:
//   ATtiny85 pin 0 (DIP pin 5, PB0) to 74HC595 pin 12 (storage register clock)
//   ATtiny85 pin 1 (DIP pin 6, PB1) to 74HC595 pin 14 (data in)
//   ATtiny85 pin 2 (DIP pin 7, PB2) to 74HC595 pin 11 (shift register clock)
//
// Connect eight LEDs from the shift register's outputs (Q0-Q7) to ground, through
// appropriate current-limiting resistors.
//
// Connect the 74HC595 Master Reset pin (MR) to Vcc (e.g. +5V) and the Output
// Enable pin (OE) to ground.
//
// Jack Christensen 24Oct2013

#include <tinySPI.h>            // https://github.com/JChristensen/tinySPI

#define HARDWARE_SPI 1          // set to 1 to use hardware SPI, set to 0 to use shiftOut (software SPI)

// pin definitions
const int
    LATCH_PIN(0),               // storage register clock (slave select)
    DATA_PIN(1),                // data in
    CLOCK_PIN(2);               // shift register clock

void setup()
{
    // pullups on for unused pins to minimize power consumption
    //pinMode(3, INPUT_PULLUP);
    //pinMode(4, INPUT_PULLUP);
    
    #if HARDWARE_SPI == 1
    SPI.begin();                   // start hardware SPI (the library sets up the clock and data pins)
    #else
    pinMode(CLOCK_PIN, OUTPUT);    // set up the pins for software SPI
    pinMode(DATA_PIN, OUTPUT);
    #endif
    pinMode(LATCH_PIN, OUTPUT);    // latch pin needs to be set up for hardware or software
    digitalWrite(LATCH_PIN, HIGH);
}

uint8_t sig = 0;
uint16_t myByte=0;
//bit writeCMD = 0;
//bit gainCMD  = 0;
//bit shutdownCMD = 1;


long nextTime=0;
void loop()
{       
        
    //for (int b=0; b<8; b++){
        long currentTime = millis();
        if((currentTime-nextTime)>0){
          sig = (sig+1)%128;
          nextTime = currentTime + 200;
        }

        myByte = 0b0001000000000000|(((uint16_t)sig)<<4);
        
        digitalWrite(LATCH_PIN, LOW);
        
        // write the same byte 1000 times so the difference can be seen
        // between hardware and software SPI.
        //for(int n=0; n<1000; n++){
            #if HARDWARE_SPI == 1
            SPI.transfer(myByte);
            #else
            shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, myByte);
            #endif
        //}
        digitalWrite(LATCH_PIN, HIGH);
    //}
}

///*
//#define SAMPLE fixed_point::fixed<int8_t,1,6>
//#define FREQUENCY fixed_point::fixed<unsigned short int,14,2>
//#define COEFFICIENT fixed_point::fixed<unsigned short int,2,14>
//*/
//
//#include <tinySPI.h>
////#include "fixed.h"
////#include "dsp.h"
//
//
//// pin definitions
//const int
//    LATCH_PIN(5),               // chip select sck
//    DATA_PIN(6),                // data in
//    CLOCK_PIN(7);               // shift register clock
//
///*
//const int SAMPLERATE = 31250;
//const int BLOCKSIZE = 64;
//const int CONTROL_UPDATE_PERIOD = 25;
//*/
//
//
////int sampleNumber;
//long timeToSquare=0;
//long period = 100;
//bool isHigh = true;
//#define HARDWARE_SPI 1  
//
//void setup() {
//  pinMode(CLOCK_PIN, OUTPUT); 
//  pinMode(DATA_PIN, OUTPUT); 
//  pinMode(LATCH_PIN, OUTPUT);    // latch pin needs to be set up for hardware or software
//  digitalWrite(LATCH_PIN, HIGH);
//  //SPI.begin();  
//  // put your setup code here, to run once:
////  initTimer1();
////  sei();
//}
//
//void loop() {
//  long currentTime = millis();
//  if((currentTime-timeToSquare)>0){
//    digitalWrite(LATCH_PIN, LOW);
//    int output=isHigh?127:0;
//    isHigh=!isHigh;
//    timeToSquare+=period;
//    //SPI.transfer(output);
//    //analogWrite(DATA_PIN, )
//    digitalWrite(LATCH_PIN, HIGH);
//  }
//
//}
//
////void updateAudio(){
//  
//  // write next value out of the buffer to the DAC
//  //sampleBuffer = sampleBuffer.readNext();
//  //int sampleOut = sampleBuffer.value();
////  digitalWrite(LATCH_PIN, LOW);
////  SPI.transfer(signalChain(sampleNumber++).get());
////  digitalWrite(LATCH_PIN, HIGH);
//
////   PORTB &= 0b11111011; //faster digitalWrite(10,LOW);
////   SPI.transfer(dac_out>>8);
////   SPI.transfer(dac_out & 255);
////   PORTB |= 0b00000100; // faster digitalWrite(10,HIGH);
//  
////}
//
//
////static inline void initTimer1(void)
////{
////  TCCR1 |= (1 << CTC1);  // clear timer on compare match
////  
////  TCCR1 |= (0 << CS13) | (1 << CS12) | (1 << CS11) | (0 << CS10); //timer hits at 1/32 of cpu speed
////  //at 1mhz clock this is sample rate of 31250hz
////  OCR1C = 0; // compare match value ... 0 means it matches every time it counts
////  TIMSK |= (1 << OCIE1A); // enable compare match interrupt
////}
////
////ISR(TIMER1_COMPA_vect)
////{
////  updateAudio();
////}
