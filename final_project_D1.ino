#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s1;                //xy=186,246
AudioAmplifier amp1;               //xy=369,307
AudioFilterStateVariable filter1;  //xy=540,306
AudioOutputI2S i2s2;               //xy=727,245
AudioConnection patchCord1(i2s1, 0, amp1, 0);
AudioConnection patchCord2(i2s1, 1, amp1, 0);
AudioConnection patchCord3(amp1, 0, filter1, 0);
AudioConnection patchCord4(filter1, 0, i2s2, 0);
AudioConnection patchCord5(filter1, 0, i2s2, 1);
AudioControlSGTL5000 sgtl5000_1;  //xy=461,458
// GUItool: end automatically generated code

int filterPod = A13;
int volumePod = A12;

void setup() {
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(55);
  amp1.gain(0.15);
  Serial.begin(9600);
}

void loop() {
  lowPassFilter();
  //volumeManager();
}

void lowPassFilter() {
  int filterControl = analogRead(filterPod);
  int cutoff = map(filterControl, 0, 1023, 150, 3000);
  filter1.frequency(cutoff);
  delay(5); 
}

//void volumeManager() {
  //int volume = analogRead(volumePod);
  //int level = map(volume, 0, 1023, 0, 800) / 1000.0;
 //sgtl5000_1.dacVolume(volumePod);
 //delay(5);
//}

