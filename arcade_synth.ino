#include "pitches.h"

// Set pin numbers using constants
const int LED_1 = 8;
const int BUTTON_1 = 9;
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
bool arpeggio = true;
bool slide = false;


void setup() {
// Set all Button pins as inputs
  int i;
  for(i = 0; i < 8; i = i + 1){
    pinMode(buttons[i], INPUT);
  }

// Set all LED pins as outputs and turn the LEDs on
  int j;
  for(j = 0; j < 8; j = j + 1){
    pinMode(leds[j], OUTPUT);
    digitalWrite(leds[j], HIGH);
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
  if (arpeggio) {
    bool curr_buttons[] = {false, false, false, false, false, false, false, false};
    int i;
    int num_false = 0;
    int last_true = 0;
    for(i = 0; i < 8; i = i + 1){
      if (digitalRead(buttons[i]) == LOW) {
        curr_buttons[i] = true;
        last_true = i;
      }
      else{
        num_false = num_false + 1;
      }
    }

    if (num_false == 8){
//      Serial.println("no buttons being pressed");
      noTone(SPEAKER_PIN);
    }

    if (num_false == 7){
      tone(SPEAKER_PIN, determine_note(last_true));
    }

//    Serial.print("last_true: "); Serial.println(last_true);

    // When only one button is being pressed, dont arpeggiate
    while (num_false == 7){
      // If that one button isn't being pressed anymore, break
      if (digitalRead(buttons[last_true]) != LOW){
        noTone(SPEAKER_PIN);
        break;
      }

      // Check if any other buttons are being press, if so break.
      int j;
      for(j = 0; j < 8; j = j + 1){
        if (j != last_true && digitalRead(buttons[j]) == LOW) {
          curr_buttons[j] = true;
          last_true = j;
          goto multiple_notes_2;
        }
      }
      delay(10);
    }
    multiple_notes_2:
    
    for(i = 0; i < 8; i = i + 1){
      if (curr_buttons[i]) {
//        Serial.print("multiple buttons pressed: "); Serial.println(i);
        tone(SPEAKER_PIN, determine_note(i));
        delay(32);
      }
    }  
  }

  else if(slide){
    bool curr_buttons[] = {false, false, false, false, false, false, false, false};
    int i;
    int num_false = 0;
    int last_true = 0;
    // Determine which buttons are being pressed
    for(i = 0; i < 8; i = i + 1){
      if (digitalRead(buttons[i]) == LOW) {
        curr_buttons[i] = true;
        last_true = i;
      }
      else{
        num_false = num_false + 1;
      }
    }

    // If no buttons are being pressed, turn off the sound
    if (num_false == 8){
//      Serial.println("no buttons being pressed");
      noTone(SPEAKER_PIN);
    }

    // If only one button is being pressed, play the corresponding note
    if (num_false == 7){
      tone(SPEAKER_PIN, determine_note(last_true));
    }

    // When only one button is being pressed, dont arpeggiate
    while (num_false == 7){
      if (digitalRead(buttons[last_true]) != LOW){
        noTone(SPEAKER_PIN);
        break;
      }
      
//    Serial.print("only one button being pressed: "); Serial.println(last_true);
      int j;
      for(j = 0; j < 8; j = j + 1){
        if (j != last_true && digitalRead(buttons[j]) == LOW) {
//          Serial.print("second button pressed: "); Serial.println(j);
          curr_buttons[j] = true;
          last_true = j;
          goto multiple_notes;
        }
      }
      delay(10);
    }
    multiple_notes:
    
    for(i = 0; i < 8; i = i + 1){
      if (curr_buttons[i]) {
//        Serial.print("multiple buttons pressed: "); Serial.println(i);
        tone(SPEAKER_PIN, determine_note(i));
        delay(32);
      }
    }  

     
  }
  else {
    int i;
    for(i = 0; i < 8; i = i + 1){
      if (digitalRead(buttons[i]) == LOW) {
        Serial.print("i: "); Serial.println(i);
        tone(SPEAKER_PIN, determine_note(i));
        digitalWrite(leds[i], LOW);

        tone(SPEAKER_PIN, determine_note(i));
        while (digitalRead(buttons[i]) == LOW) {
//        hold note
//          bool new_note = false;
//          int j;
//          for(j = 0; i < 8; j = j + 1){
//            Serial.print("j: "); Serial.println(j);
//            if (j != i && digitalRead(buttons[j]) == LOW) {
//              new_note = true;
//              break;
//            }
//          }
//          if(new_note){
//            break;
//          }
        }

        //Once the button isn't low, stop the tone and turn the LED on
        noTone(SPEAKER_PIN);
        digitalWrite(leds[i], HIGH);
      }   
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

