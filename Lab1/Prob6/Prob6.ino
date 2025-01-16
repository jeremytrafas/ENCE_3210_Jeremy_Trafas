/*
We would like to turn on and off the red, green, and blue LEDs in a sequence. 
Therefore, write a C program with the following specifications:
a) When the button is pressed, turn on the red, green, 
and blue color LEDs with the following
order: R – RG – RGB – GB – B – RB
b) This procedure is repeated until the button is pressed again.
*/

// Define button
#define BUTTON_1 2
#define TIME 500

void setup() {
  // Begin Communication
  Serial.begin(9600);

  // Init I/Os
  pinMode(BUTTON_1, INPUT_PULLUP);
}

void loop() {
  // if button pressed, begin loop
  if (digitalRead(BUTTON_1)) {
    // while button is not pressed, continue loop
    while (!digitalRead(BUTTON_1)){
      // Sequence: R -> RG -> RGB -> GB -> B -> RB
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
  }
}

