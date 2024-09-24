#include <IRremote.h>
#include <SoftwareSerial.h>

#define IR_RECEIVE_PIN 4 
#define BT_RX 0  
#define BT_TX 1  

int leds[8] = {6, 7, 8, 9, 10, 11, 12, 13}; 
int sw_1 = 2;  
int sw_2 = 3;  
int relayPin = 5;  
int W = A1;  
int Y = A0;  
int d = 100;  

bool RunLights = false;
int lastIRstate;
char bluetoothData; 

SoftwareSerial BTSerial(BT_RX, BT_TX);

void setup() {
  Serial.begin(9600); 

  // Set pin modes
  pinMode(W, OUTPUT);  
  pinMode(Y, OUTPUT); 
  pinMode(relayPin, OUTPUT);  


  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 
  BTSerial.begin(9600);
  
  
  for (int i = 0; i < 6; i++) { 
    pinMode(leds[i], OUTPUT);
  }
}
void loop() {
  int sw1_val = digitalRead(sw_1);  
  int sw2_val = digitalRead(sw_2);  
  int IRstate = digitalRead(IR_RECEIVE_PIN);  

  if (sw1_val == 0 && sw2_val == 0) {
    White();  
  } 
  else if (sw1_val == 0 && sw2_val == 1) {
    Yellow();  
    
    if (IRstate != lastIRstate) {  
      if (IrReceiver.decode()) { 

        if (IrReceiver.decodedIRData.decodedRawData == 0XEF10BF00) {  // Mode 1: ON AC
          digitalWrite(relayPin, HIGH);  // Turn ON relay
          RunLights = false;
        } 
        else if (IrReceiver.decodedIRData.decodedRawData == 0xEE11BF00) {  // Mode 2: OFF AC
          digitalWrite(relayPin, LOW);  // Turn OFF relay
          RunLights = false;
        }
        else if (IrReceiver.decodedIRData.decodedRawData == 0xED12BF00) {  // Mode 3: Running Light
          digitalWrite(relayPin, LOW);  // Turn OFF relay
          RunLights = true;  // Enable running light mode
        }
        else if (IrReceiver.decodedIRData.decodedRawData == 0xEB14BF00) {  // Mode 4: No Operation
          Standby();  // Turn off all LEDs and relay
          RunLights = false;
        }

        IrReceiver.resume(); 
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      }
      lastIRstate = IRstate;  
      Running_LR();  
    }
  } 
  else if (sw1_val == 1 && sw2_val == 0) {  
    Yellow();  

    if (BTSerial.available()) {
      bluetoothData = BTSerial.read();  // Read Bluetooth data
      Serial.println(bluetoothData);

      // Bluetooth control commands
      if (bluetoothData == 'A') {  // ON AC
        digitalWrite(relayPin, HIGH);  // Turn ON relay
        RunLights = false;
      } 
      else if (bluetoothData == 'B') {  // OFF AC
        digitalWrite(relayPin, LOW);  // Turn OFF relay
        RunLights = false;
      } 
      else if (bluetoothData == 'C') {  // Running Light
        digitalWrite(relayPin, LOW);  // Turn OFF relay
        RunLights = true;  // Enable running light mode
      } 
      else if (bluetoothData == 'D') {  // No Operation
        Standby();  // Turn off all LEDs and relay
        RunLights = false;
      }

      Running_LR();  
    }
  } 
  else if (sw1_val == 1 && sw2_val == 1) {
    Yellow();  
  }
  else {
    Yellow(); 
  }
}
void White() {
  digitalWrite(W, HIGH);  
  digitalWrite(Y, LOW);
}
void Yellow() {
  digitalWrite(W, LOW);
  digitalWrite(Y, HIGH);
}
void Running_LR() {
  if (RunLights == true) {
    for (int i = 0; i < 6; i++) { 
      digitalWrite(leds[i], HIGH);
      delay(d);  
      digitalWrite(leds[i], LOW);
    }
  }
}
void Standby() {
  for (int i = 0; i < 6; i++) { 
    digitalWrite(leds[i], LOW); 
  }
  digitalWrite(relayPin, LOW); 
}