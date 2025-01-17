// define pins 
#define BUTTON_1 2
#define BUTTON_2 3 
#define RED_LED 5
#define GREEN_LED 4

#define TIME 500

// init button counts 
int button1count = 0;
int button2count = 0;

// init button flags, volatile since its in main loop and ISR 
volatile int button1flag = 0;
volatile int button2flag = 0;

void setup() {
  // Begin Communication
  Serial.begin(9600);

  // Init I/Os
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);


  // Create ISR vector table
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), isr_button1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), isr_button2, CHANGE);
  
}

void loop() {
  // main loop, changes LED based on what button is pressed more 
  if (button1flag == 1){
    Serial.print("Button 1: ");
    Serial.println(button1count);
    // reset flag
    button1flag = 0;
  }
  if (button2flag == 1){
    Serial.print("Button 2: ");
    Serial.println(button2count);
    // reset flag
    button2flag = 0;
  }  
  if (button1count > button2count) { // if button 1 count higher, red on
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  } else if (button1count == button2count) { // if equal print blue led on
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    delay(TIME); 
    Serial.println("Counts equal, blue LED on");   
  } else { // else, button 2 count is higher
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH); 
  }
}

// isr for button 1, counts how many times button is pressed
void isr_button1() {
  if (digitalRead(BUTTON_1)) {
    button1count++;
    // print out count
    button1flag = 1; 
  } 
}

// isr for button 2, counts how many times button is pressed
void isr_button2() {
  if (digitalRead(BUTTON_2)) {
    button2count++;
    // print out count
    button2flag = 1;
  } 
}
