/*
We would like to develop a simple calculator with the following specifications:
a) There will be three arrays each with 10 elements. The first two arrays will have integer numbers.
The third array will be empty. We will apply arithmetic operations on the first two arrays.
b) When the button is pressed once, a counter will be incremented once. This will correspond to
the addition operation. The red color LED will turn on.
c) When the button is pressed twice, the counter will be incremented twice. This will correspond
to the subtraction operation. The green color LED will turn on.
d) When the button is pressed three times, the counter will be incremented thrice. This will
correspond to the multiplication operation. The blue color LED will turn on.
e) When a second button is pressed, the selected operation will be applied on the two elements of
the first and second arrays. The result will be saved in the third array and all LEDs will turn off.
The operation will be applied to all remaining array elements. When all the results are saved, all
LEDs will turn on.
*/

// Define buttons and delay time
#define BUTTON_1 2
#define BUTTON_2 3
#define TIME 500

// First two, 10 ints, third empty
int array1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int array2[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int array3[10];
// init counter
int counter = 0;

void setup() {
  // init comms
  Serial.begin(9600);

  // init buttons
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
}

void loop() {
  // Detect button 1 press - operation select
  if (digitalRead(BUTTON_1)) {
    // wait then increment counter
    delay(TIME);
    counter++; 
    delay(TIME);

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
  }

  // Detect button 2 press
  if (digitalRead(BUTTON_2)) {
      Serial.println("Calculating...");

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
  }
}
