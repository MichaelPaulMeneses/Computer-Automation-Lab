int ledPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; 
int switchPin1 = A0;                    
int switchPin2 = A1;                    
int switchPin3 = A2; 
int switchPin4 = A3; 

void setup() {
  for (int i = 0; i < 8; i++) { 
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(switchPin1, INPUT);
  pinMode(switchPin2, INPUT);
  pinMode(switchPin3, INPUT);
  pinMode(switchPin4, INPUT);
}

void loop() {
  int mode = readDIPSwitch(); 

  switch (mode) {
    case 0b0101: // 0101
      LeftToRightAndBack();
      break;
      
    case 0b1010: // 1010
      LeftToRight();
      break;

    default:
      break;
  }
}

int readDIPSwitch() {
  int switch1 = digitalRead(switchPin1);
  int switch2 = digitalRead(switchPin2);
  int switch3 = digitalRead(switchPin3);
  int switch4 = digitalRead(switchPin4);
  return (switch1 << 3) | (switch2 << 2) | (switch3 << 1) | switch4;
}

void LeftToRight() {
  for (int i = 0; i < 8; i++) { 
    digitalWrite(ledPins[i], HIGH);
    delay(500);
    digitalWrite(ledPins[i], LOW);
  }
}

void LeftToRightAndBack() {
  
  for (int i = 0; i < 8; i++) { 
    digitalWrite(ledPins[i], HIGH);
    delay(500);
    digitalWrite(ledPins[i], LOW);
  }
  
  for (int i = 7; i >= 0; i--) { 
    digitalWrite(ledPins[i], HIGH);
    delay(500);
    digitalWrite(ledPins[i], LOW);
  }
}
