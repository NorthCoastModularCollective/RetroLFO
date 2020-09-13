#define SAMPLE fixed_point::fixed<int8_t,1,6>
#define FREQUENCY fixed_point::fixed<unsigned short int,14,2>
#define COEFFICIENT fixed_point::fixed<unsigned short int,2,14>

#include <tinySPI.h>
#include "fixed.h"
#include "dsp.h"


SinOsc makeSignalChain(float freq, float gain, int sampleRate, int modIndex){
  
//  return GainNGainN<FmOsc<SinOsc>> { gain, 
//            FmOsc<SinOsc> { freq, sampleRate, modIndex,
//              SinOsc{freq, sampleRate}
//            }
//         };

  
  return SinOsc::make(freq, sampleRate);
}

template <int bufsize>
struct SampleBuffer
{
  private:
    int buf[bufsize];
    int writeIndex = 1;
    int readIndex = 0;
  public:
    static SampleBuffer make(int b[bufsize], int w, int r){
      SampleBuffer ret;
      ret.buf[bufsize] = b;
      ret.writeIndex = w;
      ret.readIndex = r;
      return ret;
    };
    SampleBuffer readNext(){
      int nextReadIndex = (readIndex+1);
      return SampleBuffer::make(buf, writeIndex, nextReadIndex);
    };
    SampleBuffer writeNext(int samp){
      int nextWriteIndex = (writeIndex+1);
      int nextBuf[bufsize];
      for(int i =0; i<bufsize;i++){
        nextBuf[i] = buf[i];
      }
      nextBuf[nextWriteIndex%bufsize] = samp;
      return SampleBuffer::make(nextBuf, nextWriteIndex, readIndex);
    };
    int value(){
      return buf[readIndex%bufsize];
    }
    int getReadIndex(){return readIndex;}
    int getWriteIndex(){return writeIndex;}

};

// pin definitions
const int
    LATCH_PIN(0),               // storage register clock (slave select)
    DATA_PIN(1),                // data in
    CLOCK_PIN(2);               // shift register clock

const int SAMPLERATE = 31250;
const int BLOCKSIZE = 64;
const int CONTROL_UPDATE_PERIOD = 25;

SampleBuffer<BLOCKSIZE> sampleBuffer;
SinOsc signalChain = makeSignalChain(1,0.5,SAMPLERATE,1);
unsigned long timeOfLastControlUpdate;
int sampleNumber;

SinOsc updateControl(){
  //read knobs
  //
  int gain = 1;
  float freq = 1;
  int modIndex = 1;
  //create signalchain with values
  return makeSignalChain(freq,gain,SAMPLERATE,modIndex);
  
}

void setup() {
  // pullups on for unused pins to minimize power consumption
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
   
  SPI.begin();  
  pinMode(LATCH_PIN, OUTPUT);    // latch pin needs to be set up for hardware or software
  digitalWrite(LATCH_PIN, HIGH);
  // put your setup code here, to run once:
  initTimer1();
  sei();
}

void loop() {

  unsigned long currentTime = millis();
  //every CONTROL_UPDATE_PERIOD, check for changes in control values and update synth params 
  if((currentTime-timeOfLastControlUpdate)>CONTROL_UPDATE_PERIOD){
    signalChain=updateControl();
    timeOfLastControlUpdate = currentTime;
  }

  //while able, fill the sample buffer up to the block size
  if((sampleBuffer.getWriteIndex()-sampleBuffer.getReadIndex())<BLOCKSIZE){
    sampleBuffer = sampleBuffer.writeNext(signalChain(sampleNumber++).get());
  }

}

void updateAudio(){
  
  // write next value out of the buffer to the DAC
//  sampleBuffer = sampleBuffer.readNext();
//  int sampleOut = sampleBuffer.value();
//  digitalWrite(LATCH_PIN, LOW);
//  SPI.transfer(sampleOut);
//  digitalWrite(LATCH_PIN, HIGH);

//   PORTB &= 0b11111011; //faster digitalWrite(10,LOW);
//   SPI.transfer(dac_out>>8);
//   SPI.transfer(dac_out & 255);
//   PORTB |= 0b00000100; // faster digitalWrite(10,HIGH);
  
}


static inline void initTimer1(void)
{
  TCCR1 |= (1 << CTC1);  // clear timer on compare match
  
  TCCR1 |= (0 << CS13) | (1 << CS12) | (1 << CS11) | (0 << CS10); //timer hits at 1/32 of cpu speed
  //at 1mhz clock this is sample rate of 31250hz
  OCR1C = 0; // compare match value ... 0 means it matches every time it counts
  TIMSK |= (1 << OCIE1A); // enable compare match interrupt
}

ISR(TIMER1_COMPA_vect)
{
  updateAudio();
}
