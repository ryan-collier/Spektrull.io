/*
 Fade
 
 This example shows how to fade an LED on pin 9
 using the analogWrite() function.
 
 This example code is in the public domain.
 */

// PIN ASSIGNMENT
int red1_pin = 6;
int blue1_pin = 3;
int grn1_pin = 5;
int red2_pin = 10;
int blue2_pin = 11;
int grn2_pin = 9;

int sequence_select_pin = 7;  // normal light sequence vs heartbeat mode
int sequence_speed_pin = A3; 
unsigned int sequence_select = 0;
int speed_select = 100;
unsigned int BeatUpperBound = 1100;

//********* Step count to be updated with linked step counter data over wifi
unsigned int activity = 250;

////Turns heart beat sequence on
boolean HeartBeat = false;

int BeatRunner = 0;  // What is this?

int sparkLight1        = 18; // A4
int sparkLight2        = 20; // A5  - Not working
int sparkLight3        = 15; // A0 - Kind of messed up
int sparkLight3reset   = 14; // A1  - What the fuck is this, doesn't seem to work either

// From 0 to 255, 255 being OFF and 0 being fully ON
unsigned int red1_brightness = 50;    // how bright the LED is
unsigned int blue1_brightness = 50;
unsigned int grn1_brightness = 150;
unsigned int red2_brightness = 200;
unsigned int blue2_brightness = 250;
unsigned int grn2_brightness = 70;


int red1_fade = 1;  // This is used in heartbeat sequence
int grn1_fade = 1;
int blue1_fade = 1;    // how many points to fade the LED by
int red2_fade = 1;  // This is used in heartbeat sequence
int grn2_fade = 1;
int blue2_fade = 1;


int spark1Loop = 0;
int spark2Loop = 0;
int spark3Loop = 0;

// the setup routine runs once when you press reset:
void setup()  { 
  
  pinMode(sparkLight1, OUTPUT);
  pinMode(sparkLight2, OUTPUT);
  pinMode(sparkLight3, OUTPUT);
  pinMode(sparkLight3reset, OUTPUT);
  pinMode(sequence_select_pin, INPUT);
  pinMode(sequence_speed_pin, INPUT);
 
  digitalWrite(sparkLight1, HIGH);
  digitalWrite(sparkLight2, HIGH);
  digitalWrite(sparkLight3, LOW);
  digitalWrite(sparkLight3reset, LOW);

  Serial.begin(115200);
  Serial.println("Starting Spectrull.io sequence");
  reset_pins();
 
} 

// the loop routine runs over and over again forever:
void loop()  { 
  
  //analogWrite(grn1, grn1_brightness);
  //analogWrite(red2, red2_brightness); 
  sequence_select = digitalRead(sequence_select_pin);
  speed_select = analogRead(sequence_speed_pin);
  
//  Serial.println(sequence_select);
  
  if ( sequence_select == HIGH && HeartBeat == false) {
    HeartBeat = true;
    Serial.println("Heartbeat sequence engaged");
    reset_pins();
    
    set_BeatUpperBound();
  }
  else if ( sequence_select == LOW && HeartBeat == true) {
    HeartBeat = false;
    Serial.println("Steps sequence engaged");
    reset_pins();
    activity = set_activity();
    Serial.println(activity);
  }



  if (HeartBeat == true){
    ////////////////////////////// HEARBEAT SEQUENCE  ////////////////////////////////
  
    if (BeatRunner > 0 && BeatRunner < 510) {
        analogWrite(red2_pin, red2_brightness);
        if (red2_brightness == 0) {
          red2_fade = -red2_fade; 
        }  
        red2_brightness -= red2_fade;
    } else {
      analogWrite(red2_pin, 200);
    }
    
    if (BeatRunner > 190 && BeatRunner < 705) {
        analogWrite(red1_pin, red1_brightness);
        if (red1_brightness == 0) {
          red1_fade = -red1_fade;
        }      
          red1_brightness -= red1_fade;   
    } else {
      analogWrite(red1_pin, 200);
    }
    
    
    BeatRunner++;
    
    if (BeatRunner >= BeatUpperBound) {   //This value determines time between visual beats. Higher HR would need a smaller value, like 800 or 900.
      BeatRunner = 0;
      red1_brightness = 255;
      red1_fade = 1;
      red2_brightness = 255;
      red2_fade = 1;

      set_BeatUpperBound();
    }
    delay(1); 
  // End of If (HeartBeat) sequence
  
  
  } else {
      ///////////////  STEP COUNT SEQUENCE  ////////////////////////
      
     //red1pulse();      Not using in reqgular light sequence
      grn1pulse();
      
     if (activity > 300) {
       red2pulse(); 
     }
      if ( activity > 400) {
        blue1pulse();
      }
      
      if (activity > 500) {
        grn2pulse();
      }
      
      //blue2pulse();   Not using in reqgular light sequence

      if (activity > 600) {
          if (spark1Loop == 400) {
            spark1Loop = 0;
            spark1();
          }
          spark1Loop++;
          
          if (spark2Loop == 750) {
            spark2Loop = 0;
            spark2();
          }
          spark2Loop++;
      }
 
      // wait for 30 milliseconds to see the dimming effect    
      delay(30);    
  }  

}

unsigned int set_activity() {
  return analogRead(sequence_speed_pin);
}

void set_BeatUpperBound() {
    speed_select = analogRead(sequence_speed_pin);
    BeatUpperBound = 2000 - speed_select*2;
    Serial.print("speed_select"); Serial.println(speed_select);
    Serial.println(BeatUpperBound);
}

void reset_pins() {
  red1_brightness = 50;    // how dim the LED is
  blue1_brightness = 50;
  grn1_brightness = 150;
  red2_brightness = 200;
  blue2_brightness = 250;
  grn2_brightness = 70;

  red1_fade = 1;  // This is used in heartbeat sequence
  grn1_fade = 1;
  blue1_fade = 1;    // how many points to fade the LED by
  red2_fade = 1;  // This is used in heartbeat sequence
  grn2_fade = 1;
  blue2_fade = 1;

  analogWrite(red1_pin, 255);
  analogWrite(blue1_pin, 255);
  analogWrite(grn1_pin, 255);
  analogWrite(red2_pin, 255);
  analogWrite(blue2_pin, 255);
  analogWrite(grn2_pin, 255);
}

//void reset_pins_step_sequence() {
//  reset_pins_heartbeat_sequence();
//  blue1_brightness = 50;
//  grn1_brightness = 150;
//
//  blue2_brightness = 250;
//  grn2_brightness = 70;
//    
//}

void red1pulse() {
  analogWrite(red1_pin, red1_brightness);
  if (red1_brightness == 0 || red1_brightness == 255) {
    red1_fade = -red1_fade; 
  }  
  red1_brightness -= red1_fade;
}

void grn1pulse() {
  analogWrite(grn1_pin, grn1_brightness);
  if (grn1_brightness == 0 || grn1_brightness == 255) {
    grn1_fade = -grn1_fade; 
  }  
  grn1_brightness -= grn1_fade;
}

void blue1pulse() {
  analogWrite(blue1_pin, blue1_brightness);
  
  // reverse the direction of the fading at the ends of the fade: 
  if (blue1_brightness == 0 || blue1_brightness == 255) {
    blue1_fade = -blue1_fade; 
  }     
  blue1_brightness -= blue1_fade;
}

void red2pulse() {
  analogWrite(red2_pin, red2_brightness);
  if (red2_brightness == 0 || red2_brightness == 255) {
    red2_fade = -red2_fade; 
  }  
  red2_brightness -= red2_fade;
}

void grn2pulse() {
  analogWrite(grn2_pin, grn2_brightness); 
  if (grn2_brightness == 0 || grn2_brightness == 255) {
    grn2_fade = -grn2_fade; 
  }   
  grn2_brightness -= grn2_fade;
}

void blue2pulse() {
  analogWrite(blue2_pin, blue2_brightness);
  if (blue2_brightness == 0 || blue2_brightness == 255) {
    blue2_fade = -blue2_fade; 
  }  
  blue2_brightness -= blue2_fade;
}



void spark1() {
    digitalWrite(sparkLight1, LOW);
    delay(100);
    digitalWrite(sparkLight2, LOW);
    delay(100);
    digitalWrite(sparkLight1, HIGH);
    delay(100);
    digitalWrite(sparkLight2, HIGH);
    delay(100);
}

void spark2() {
    digitalWrite(sparkLight1, LOW);
    delay(100);
    digitalWrite(sparkLight2, LOW);
    delay(100);
    digitalWrite(sparkLight1, HIGH);
    delay(100);
    digitalWrite(sparkLight2, HIGH);
    delay(100);
    digitalWrite(sparkLight1, LOW);
    delay(100);
    digitalWrite(sparkLight2, LOW);
    delay(100);
    digitalWrite(sparkLight1, HIGH);
    delay(100);
    digitalWrite(sparkLight2, HIGH);
}
 


    

/* This LED array kind of sux.
void spark3() {
    digitalWrite(sparkLight3, HIGH);
    delay(7500);
    digitalWrite(sparkLight3, LOW);
    digitalWrite(sparkLight3reset, HIGH);
    delay(10);
    digitalWrite(sparkLight3reset, LOW);
}
  */  
    
