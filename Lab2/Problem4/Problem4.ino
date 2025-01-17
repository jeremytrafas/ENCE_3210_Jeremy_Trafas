// Define button
#define BUTTON_1 2

// init array filled with 10 ints
int x[10] = {1,2,3,4,5,6,7,8,9,10};
// init y array as empty
int y[10];

// flag for when button pressed 
volatile int button1flag; 

void setup() {
  // Begin Communication
  Serial.begin(9600);

  // Init I/Os
  pinMode(BUTTON_1, INPUT_PULLUP);

  // create ISR vector table 
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), isr_button1, CHANGE);
}

// main loop, just prints out y[n]
void loop() {
  if (button1flag == 1){
    // print y[n] 
    for (int n = 0; n < 10; n++){
      Serial.println(y[n]);
    }
    // reset flag 
    button1flag = 0;
  }
}

// ISR wil calculate the global integer array y defined as 
// y[n] = 2 * x[n] - x[n-1] where n is index of array 
void isr_button1(){
  if (digitalRead(BUTTON_1)){
    for (int n = 0; n < 10; n++){
      y[n] = 2 * x[n] - x[n-1];
    }
    // set flag 
    button1flag = 1;
  }
}