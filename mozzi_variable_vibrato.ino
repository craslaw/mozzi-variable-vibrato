#define MOZZI_CONTROL_RATE 128
#include <Mozzi.h> // this makes everything work
#include <Oscil.h>  // a template for an oscillator
#include <tables/sin2048_int8.h>  // a wavetable holding a sine wave

Oscil <2048, MOZZI_AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <2048, MOZZI_CONTROL_RATE> kVib(SIN2048_DATA);

//float centre_freq = 440.0;
float depth = 0.2;

const char VOLUME_PIN = 0; // analog pin 0
const char PITCH_PIN = 1;  // analog pin 1
const char DEPTH_PIN = 2;  // analog pin 2

byte volume;

// pre-calculate as much as possible in setup()
void setup() {
  //kVib.setFreq(10.5f);
  Serial.begin(115200);
  startMozzi(MOZZI_CONTROL_RATE);
}

// put slow changing values in updateControl()
void updateControl(){
  // read the potentiometer as only 8 bit volume range for efficient calculations in updateAudio
  volume = mozziAnalogRead<8>(VOLUME_PIN); // value is 0-255

  // read pitch knob
  int centre_freq = mozziAnalogRead<10>(PITCH_PIN); // We request 10 bits, here, however. Value is 0-1023

  // read depth knob
  //int depth = mozziAnalogRead<10>(DEPTH_PIN); // We request 10 bits, here, however. Value is 0-1023
  float vib_freq = mozziAnalogRead<8>(DEPTH_PIN);
  kVib.setFreq(vib_freq);


	//float vibrato = depth / 1023 * kVib.next();
	float vibrato = depth * kVib.next();
	aSin.setFreq(centre_freq+vibrato);
}

// Keep updateAudio() lean, ususally don't put anything else here
// Control values which directly modify audio synthesis can be efficiently
// interpolated with a Line object in updateAudio() if necessary.
AudioOutput updateAudio(){
	return MonoOutput::from16Bit((int)aSin.next() * volume);
}

void loop() {
  audioHook();
}
