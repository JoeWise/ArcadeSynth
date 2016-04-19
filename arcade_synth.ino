#include "pitches.h"

// Set pin numbers using constants
const int LED_1 = 8;
const int BUTTON_1 = 1;
const int LED_2 = 2;
const int BUTTON_2 = 3;
const int LED_3 = 4;
const int BUTTON_3 = 5;
const int LED_4 = 6;
const int BUTTON_4 = 7;
const int LED_5 = 26;
const int BUTTON_5 = 27;
const int LED_6 = 28;
const int BUTTON_6 = 29;
const int LED_7 = 30;
const int BUTTON_7 = 31;
const int LED_8 = 32;
const int BUTTON_8 = 33;
const int SPEAKER_PIN = 14;

int buttons[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8};
int leds[] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8};

int const key_c4[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};

int curr_octave = 4;
char curr_key = 'c';


void setup() {
// Set all Button pins as inputs
  int i;
  for(i = 0; i < 8; i = i + 1){
    pinMode(buttons[i], INPUT);
  }

// Set all LED pins as outputs
  int j;
  for(j = 0; j < 8; j = j + 1){
    pinMode(leds[j], OUTPUT);
  }

  // the speaker pin as an output
  pinMode(SPEAKER_PIN, OUTPUT);

  Serial.begin(9600);  // Sets the baud rate for serial transmission 
  // Send these messages to PC only once 
  Serial.println("Serial interface between PC and chipKIT");
//  Serial.println("1: LED ON, 0: LED OFF");
//  int k;
//  for(k = 0; k < 8; k = k + 1){
//    Serial.println(key_c4[k]);
//  }

}
 
void loop(){
  int i;
  for(i = 0; i < 8; i = i + 1){
    if (digitalRead(buttons[i]) == LOW) {
      Serial.println(i);
      int pitch = determine_note(i);
      tone(SPEAKER_PIN, pitch);
      digitalWrite(leds[i], LOW);
      while (digitalRead(buttons[i]) == LOW) {
        delay(10);
      }
    }
    else {
      noTone(SPEAKER_PIN);
      digitalWrite(leds[i], HIGH);    
    }
  }
}

int determine_note(int button_num){
  switch (curr_key) {
    case 'c':
      return get_note_c(button_num);
      break;
    case 'd':
      //do something when var equals 2
      break;
    case 'e':
      //do something when var equals 2
      break;
    case 'f':
      //do something when var equals 2
      break;
    case 'g':
      //do something when var equals 2
      break;
    case 'a':
      //do something when var equals 2
      break;
    case 'b':
      //do something when var equals 2
      break;
  }
}

int get_note_c(int button_num) {
  switch (curr_octave) {
    case 1:
      //do something when curr_octave equals 1
      break;
    case 2:
      //do something when var equals 2
      break;
    case 3:
      //do something when var equals 2
      break;
    case 4:
      return key_c4[button_num];
      break;
    case 5:
      //do something when var equals 2
      break;
    case 6:
      //do something when var equals 2
      break;
    case 7:
      //do something when var equals 2
      break;
    case 8:
      //do something when var equals 2
      break;      
  }
}

