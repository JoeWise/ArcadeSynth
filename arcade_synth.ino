#include "pitches.h"

// Set pin numbers using constants
const int LED_1    = 2;
const int BUTTON_1 = 3;
const int LED_2    = 4;
const int BUTTON_2 = 5;
const int LED_3    = 6;
const int BUTTON_3 = 7;
const int LED_4    = 8;
const int BUTTON_4 = 9;
const int LED_5    = 26;
const int BUTTON_5 = 27;
const int LED_6    = 28;
const int BUTTON_6 = 29;
const int LED_7    = 30;
const int BUTTON_7 = 31;
const int LED_8    = 32;
const int BUTTON_8 = 33;

const int OCTAVE_DOWN = 10;
const int OCTAVE_UP   = 11;
const int KEY_UP      = 12;
const int KEY_DOWN    = 13;

const int SPEAKER_PIN = 14;

int buttons[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8};
int leds[] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8};
int joystick[] = {OCTAVE_DOWN, OCTAVE_UP, KEY_UP, KEY_DOWN};

int curr_octave = 3;
char curr_key = 0;
bool arpeggio = true;

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

  // Set the speaker pin as an output
  pinMode(SPEAKER_PIN, OUTPUT);

  // Set the joystick pins as inputs
  pinMode(OCTAVE_DOWN, INPUT);
  pinMode(OCTAVE_UP, INPUT);
  pinMode(KEY_DOWN, INPUT);
  pinMode(OCTAVE_UP, INPUT);
  
  Serial.begin(9600);  // Sets the baud rate for serial transmission 
  // Send these messages to PC only once 
  Serial.println("Serial interface between PC and chipKIT");
//  Serial.println("1: LED ON, 0: LED OFF");
//  int k;
//  for(k = 0; k < 8; k = k + 1){
//    Serial.println(key_c4[k]);
//  }

  Serial.println(c_maj_keys[3][5]);
}
 
void loop(){
  if (arpeggio) {
    bool curr_buttons[] = {false, false, false, false, false, false, false, false};
    int i;
    int num_false = 0;
    int last_true = 0;
    for(i = 0; i < 8; i = i + 1){
      if (digitalRead(buttons[i]) == HIGH) {
        curr_buttons[i] = true;
        last_true = i;
//        digitalWrite(leds[i], LOW);
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
      if (digitalRead(buttons[last_true]) != HIGH){
        noTone(SPEAKER_PIN);
        digitalWrite(leds[last_true], HIGH);
        break;
      }

      digitalWrite(leds[last_true], LOW);
      
      // Check if any other buttons are being pressed, if so break.
      int j;
      for(j = 0; j < 8; j = j + 1){
        if (j != last_true && digitalRead(buttons[j]) == HIGH) {
          curr_buttons[j] = true;
          last_true = j;
          goto multiple_notes_2;
        }
      }

      // Check if any joystick buttons are being pressed
      int k;
      for(k = 0; k < 4; k = k + 1){
        if (digitalRead(joystick[k]) == LOW){
          joystick_input(k);
        }
      }
      
      delay(10);
    }
    multiple_notes_2:
    
    for(i = 0; i < 8; i = i + 1){
      if (curr_buttons[i]) {
        // Serial.print("multiple buttons pressed: "); Serial.println(i);
        tone(SPEAKER_PIN, determine_note(i));
        digitalWrite(leds[i], LOW);
        delay(32);
        digitalWrite(leds[i], HIGH);
      }
    }
  }
  else {
    int i;
    for(i = 0; i < 8; i = i + 1){
      if (digitalRead(buttons[i]) == HIGH) {
        Serial.print("i: "); Serial.println(i);
        
        tone(SPEAKER_PIN, determine_note(i));
        digitalWrite(leds[i], LOW);
        
        while (digitalRead(buttons[i]) == HIGH) {
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
        
          // Check if any joystick buttons are being pressed
          int k;
          for(k = 0; k < 4; k = k + 1){
            if (digitalRead(joystick[k]) == LOW){
              joystick_input(k);
            }
          }
        }

        //Once the button isn't low, stop the tone and turn the LED on
        noTone(SPEAKER_PIN);
        digitalWrite(leds[i], HIGH);
      }   
    }    
  }
 
  // Check if any joystick buttons are being pressed
  int k;
  for(k = 0; k < 4; k = k + 1){
    if (digitalRead(joystick[k]) == LOW){
      joystick_input(k);
    }
  }
}

void joystick_input(int type){
  Serial.print("octave was: "); Serial.println(curr_octave);
  Serial.print("key was: "); Serial.println(curr_key);
  
  // octave down
  if(type == 1){
    if(curr_octave == 0){
      curr_octave = 7;
    }
    else{
      curr_octave = curr_octave - 1;
    }
  }
  
  // octave up
  else if(type == 2){
    if(curr_octave == 7){
      curr_octave = 0;
    }
    else{
      curr_octave = curr_octave + 1;
    }    
  }
  
  // key up
  else if(type == 3){
    if(curr_key == 11){
      curr_key = 0;
    }
    else{
      curr_key = curr_key + 1;
    }    
  }
  
  // key down
  else if(type == 4){
    if(curr_key == 0){
      curr_key = 11;
    }
    else{
      curr_key = curr_key - 1;
    }    
  }

  Serial.print("octave is now: "); Serial.println(curr_octave);
  Serial.print("key is now: "); Serial.println(curr_key);
}

int determine_note(int button_num){
  switch (curr_key) {
    case 0:
      return c_maj_keys[curr_octave][button_num];
      break;
    case 1:
      return cs_maj_keys[curr_octave][button_num];
      break;
    case 2:
      return d_maj_keys[curr_octave][button_num];
      break;
    case 3:
      return ds_maj_keys[curr_octave][button_num];
      break;
    case 4:
      return e_maj_keys[curr_octave][button_num];
      break;
    case 5:
      return f_maj_keys[curr_octave][button_num];
      break;
    case 6:
      return fs_maj_keys[curr_octave][button_num];
      break;
    case 7:
      return g_maj_keys[curr_octave][button_num];
      break;
    case 8:
      return gs_maj_keys[curr_octave][button_num];
      break;
    case 9:
      return a_maj_keys[curr_octave][button_num];
      break;
    case 10:
      return as_maj_keys[curr_octave][button_num];
      break;
    case 11:
      return b_maj_keys[curr_octave][button_num];
      break;
  }
}
