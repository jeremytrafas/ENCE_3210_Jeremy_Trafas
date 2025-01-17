// Repeat prob 7 with interrupts 

// Define buttons and delay time
#define BUTTON_1 2
#define BUTTON_2 3
#define TIME 100

// First two, 10 ints, third empty
int array1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int array2[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int array3[10];
// init counter
int counter = 0;

// init button flags, volatile since its in main loop and ISR 
volatile int button1flag = 0;
volatile int button2flag = 0;


void setup() {
  // init comms
  Serial.begin(9600);

  // init buttons
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  // Create ISR vector table
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), isr_button1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), isr_button2, CHANGE);  
}

void loop() {
  // Detect button 1 press from flag 
  if (button1flag == 1) {
    // wait to avoid double press then increment counter
    delay(TIME);
    counter++; 

    // Turn on LEDs based on counter value
    if (counter == 1) {
      Serial.print("Counter: ");
      Serial.println(counter);
      Serial.println("Red LED on");
    } else if (counter == 2) {
      Serial.print("Counter: ");
      Serial.println(counter);      
      Serial.println("Green LED on");
    } else if (counter == 3) {
      Serial.print("Counter: ");
      Serial.println(counter);      
      Serial.println("Blue LED on");
    } else {
      counter = 0; // Reset counter if it goes beyond 3
      Serial.print("Counter: ");
      Serial.println(counter);      
      Serial.println("All LED off");
    }
    // reset flag
    button1flag = 0;
  }

  // Detect button 2 press from flag 
  if (button2flag == 1) {
    Serial.println("Calculating...");
    // delay to look cool
    delay(TIME);

    // Turn off all LEDs while calc
    Serial.println("All LED off");

    // Find array 3 using selected operation
    for (int i = 0; i < 10; i++) {
      if (counter == 1) { // Addition
        array3[i] = array1[i] + array2[i];
      } else if (counter == 2) { // Subtraction
        array3[i] = array1[i] - array2[i];
      } else if (counter == 3) { // Multiplication
        array3[i] = array1[i] * array2[i];
      }      
    }
    // Turn on all LEDs once array 3 is saved
    Serial.println("All LED on");

    // Print results
    for (int i = 0; i < 10; i++) {
      Serial.print("array3[");
      Serial.print(i);
      Serial.print("]: ");
      Serial.println(array3[i]);
    }
    // reset flag 
    button2flag = 0;
  }
}

// isr for button 1
void isr_button1() {
  if (digitalRead(BUTTON_1)) {
    button1flag = 1; 
  } 
}

// isr for button 2
void isr_button2() {
  if (digitalRead(BUTTON_2)) {
    button2flag = 1;
  } 
}