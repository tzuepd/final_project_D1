#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

int filterPod = A12;
int onOffSwitch = 25;
bool wasOn = false;

void setup() {
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(55);
  amp1.gain(0.2);

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  pinMode(onOffSwitch, INPUT);
}

void loop() {
  int switchState = digitalRead(onOffSwitch);

  if (switchState == LOW) {
    // SWITCH IS ON
    if (!wasOn) {
      startupTransition();  // run ONCE when switching ON
      wasOn = true;
    }
    lowPassFilter();

  } else {
    // SWITCH IS OFF
    wasOn = false; // reset
    displayOffScreen();
    filter1.frequency(0);
    amp1.gain(0);
  }
}

void lowPassFilter() {
  int filterControl = analogRead(filterPod);
  int cutoff = map(filterControl, 0, 1023, 100, 4500);
  filter1.frequency(cutoff);
  amp1.gain(0.2);  // restore gain when ON

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(10, 20);
  display.setTextColor(SSD1306_WHITE);
  display.print(cutoff);
  display.print(F("Hz"));
  display.display();

  delay(5);
}

void displayOffScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 25);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println("SkullFltr");
  display.display();
}

void startupTransition() {

  // --- Line animation first ---
  for (int i = 0; i < max(display.width(), display.height()); i += 4) {
    display.clearDisplay();

    // top-left corner
    display.drawLine(0, 0, i, display.height() - 1, SSD1306_WHITE);
    display.drawLine(0, 0, display.width() - 1, i, SSD1306_WHITE);

    // bottom-left
    display.drawLine(0, display.height() - 1, i, 0, SSD1306_WHITE);
    display.drawLine(0, display.height() - 1, display.width() - 1, display.height() - 1 - i, SSD1306_WHITE);

    // top-right
    display.drawLine(display.width() - 1, 0, display.width() - 1 - i, display.height() - 1, SSD1306_WHITE);
    display.drawLine(display.width() - 1, 0, 0, i, SSD1306_WHITE);

    // bottom-right
    display.drawLine(display.width() - 1, display.height() - 1, display.width() - 1 - i, 0, SSD1306_WHITE);
    display.drawLine(display.width() - 1, display.height() - 1, 0, display.height() - 1 - i, SSD1306_WHITE);

    display.display();
    delay(35);
  }

  //blank screen AFTER animation
  display.clearDisplay();
  display.display();
  delay(500);
}

