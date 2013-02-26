

#include <Wtv020sd16p.h>

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/COS2048_int8.h> // table for Oscils to play
//#include <utils.h>
#include <fixedMath.h>
#include <EventDelay.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>

int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;  // The pin number of the data pin.
int busyPin = 5;  // The pin number of the busy pin.

/*
Create an instance of the Wtv020sd16p class.
 1st parameter: Reset pin number.
 2nd parameter: Clock pin number.
 3rd parameter: Data pin number.
 4th parameter: Busy pin number.
 */
Wtv020sd16p wtv020sd16p(resetPin,clockPin,dataPin,busyPin);

int file = 0; //which file to play.

//MOZZI
#define CONTROL_RATE 64 // powers of 2 please
// audio oscils
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModDepth(COS2048_DATA);

// for scheduling note changes in updateControl()
EventDelay <CONTROL_RATE>  kNoteChangeDelay;

// synthesis parameters in fixed point formats
Q8n8 ratio; // unsigned int with 8 integer bits and 8 fractional bits
Q24n8 carrier_freq; // unsigned long with 24 integer bits and 8 fractional bits
Q24n8 mod_freq; // unsigned long with 24 integer bits and 8 fractional bits

// for random notes
Q8n0 octave_start_note = 42;

EventDelay <CONTROL_RATE>  sampleDelay;

void setup() {
  //Initializes the module.
  wtv020sd16p.reset();
  //delay(500);
  //Serial.begin(115200);
  ratio = float_to_Q8n8(3.0f);   // define modulation ratio in float and convert to fixed-point
  kNoteChangeDelay.set(200); // note duration ms, within resolution of CONTROL_RATE
  sampleDelay.set(10000);
  aModDepth.setFreq(13.f);     // vary mod depth to highlight am effects
  randSeed(); // reseed the random generator for different results each time the sketch runs
  startMozzi(CONTROL_RATE);
  
}

void loop(){
  

  audioHook();
}
void updateControl(){
  static Q16n16 last_note = octave_start_note;
if(sampleDelay.ready()){
  //wtv020sd16p.asyncPlayVoice(file);
  file++;
  if(file > 10) file = 0;
}

  if(kNoteChangeDelay.ready()){

    // change octave now and then
    if(rand((unsigned char)5)==0){
      last_note = 36+(rand((unsigned char)6)*12);
    }

    // change step up or down a semitone occasionally
    if(rand((unsigned char)13)==0){
      last_note += 1-rand((unsigned char)3);
    }

    // change modulation ratio now and then
    if(rand((unsigned char)5)==0){
      ratio = ((Q8n8) 1+ rand((unsigned char)5)) <<8;
    }

    // sometimes add a fractionto the ratio
    if(rand((unsigned char)5)==0){
      ratio += rand((unsigned char)255);
    }

    // step up or down 3 semitones (or 0)
    last_note += 3 * (1-rand((unsigned char)3));

    // convert midi to frequency
    Q16n16 midi_note = Q8n0_to_Q16n16(last_note); 
    carrier_freq = Q16n16_to_Q24n8(Q16n16_mtof(midi_note));

    // calculate modulation frequency to stay in ratio with carrier
    mod_freq = (carrier_freq * ratio)>>8; // (Q24n8 * Q8n8) >> 8 = Q24n8

      // set frequencies of the oscillators
    aCarrier.setFreq_Q24n8(carrier_freq);
    aModulator.setFreq_Q24n8(mod_freq);

    // reset the note scheduler
    kNoteChangeDelay.start();
  }


}

int updateAudio(){
  long mod = (128u+ aModulator.next()) * ((unsigned char)128+ aModDepth.next());
  int out = (mod * aCarrier.next())>>16;
  return out;
}

