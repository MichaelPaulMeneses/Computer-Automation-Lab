#include <IRremote.h>
#define IR_RECEIVE_PIN 4
int leds[8] = {6, 7, 8, 9, 10, 11, 12, 13};
int sw_1 = 2;
int sw_2 = 3;
int d = 150;

int R = A0;
int G = A1;
int Y = A2;

bool RunLights = false;
int lastIRstate;

void setup() {
  Serial.begin(9600); 
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(Y, OUTPUT);
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 
  for (int x = 0; x <= 8; x++) { 
    pinMode(leds[x], OUTPUT);
  }
  pinMode(5, OUTPUT);
}
void loop() {
  int sw1_val = digitalRead(sw_1);
  int sw2_val = digitalRead(sw_2);
  
  if (sw1_val == 0 && sw2_val == 0) {
    Red();
  } 
  else if (sw1_val == 0 && sw2_val == 1) {
    Green();
    IR();
  } 
  else if (sw1_val == 1 && sw2_val == 0) {
    Yellow();
    BT();
  }
  else {
    Red();
  }
}
void IR(){
  int IRstate = digitalRead(4);
  if (IRstate != lastIRstate) {
    if (IrReceiver.decode()) {
      if (IrReceiver.decodedIRData.decodedRawData == 0xF30CFF00) {//1
        digitalWrite(5, HIGH); 
        RunLights = false;
      } 
      else if (IrReceiver.decodedIRData.decodedRawData == 0xE718FF00) {//2
        digitalWrite(5, LOW); 
        RunLights = false;
      }
      else if (IrReceiver.decodedIRData.decodedRawData == 0xA15EFF00) {//3
        RunLights = true;
        Running_LR();
      }
      else if (IrReceiver.decodedIRData.decodedRawData == 0xF708FF00) {//4
        Standby();
        RunLights = false;
      }

      IrReceiver.resume(); 
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    }
    lastIRstate = IRstate;
    
  }
}
void BT() {
  if (Serial.available() > 0) {
    char blth = Serial.read(); 

    switch (blth) {
      case '1':
        digitalWrite(5, HIGH);
        break;
        
      case '2':
        digitalWrite(5, LOW);
        break;
        
      case '3':
        Running_LR(); 
        break;
        
      case '4':
        Standby(); 
        break;
    }
  }
}
void Red(){
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(Y, LOW);
}

void Green(){
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(Y, LOW);
}

void Yellow(){
  digitalWrite(R, LOW);
  digitalWrite(G, LOW);
  digitalWrite(Y, HIGH);
}
void Running_LR() {
  for (int x = 0; x <= 8; x++) { 
    digitalWrite(leds[x], HIGH);
    delay(d);
    digitalWrite(leds[x], LOW);
  }
}
void Standby(){
  for (int x = 0; x <= 8; x++) { 
    digitalWrite(leds[x], LOW);
    digitalWrite(5, LOW);
  }
}
