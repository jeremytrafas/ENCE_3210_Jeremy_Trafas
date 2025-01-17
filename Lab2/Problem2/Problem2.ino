// Redo 6 from last lab but with interrupt

// Define button
#define BUTTON_1 2

#define TIME 500

// init button flags, volatile since in main loop and ISR
volatile int button1flag = 0;

void setup() {
  // Begin Communication
  Serial.begin(9600);

  // Init I/Os
  pinMode(BUTTON_1, INPUT_PULLUP);

  // create ISR vector table 
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), isr_button1, CHANGE);
  
}

void loop() {
  // if button pressed, begin loop
  if (button1flag == 1) {
      Serial.println("R");
      delay(TIME);

      Serial.println("GB");
      delay(TIME);

      Serial.println("RGB");
      delay(TIME);

      Serial.println("GB");
      delay(TIME);

      Serial.println("B");
      delay(TIME);

      Serial.println("RB");
      delay(TIME);
  }
  // if button 1 is pressed again, reset flag which will stop loop
  if (button1flag == 2){
    button1flag = 0;
  }
}

// ISR for button 1 press 
// count of flag can be 0 for no loop, 1 for loop, or 2 to reset to 0 
void isr_button1(){
  if (digitalRead(BUTTON_1)){
    button1flag++;
  }
}

