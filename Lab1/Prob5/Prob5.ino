/*
Write a C program to detect a specified pattern with 
three entries in an array. The pattern is saved in another array. 
When the pattern is detected, the green color LED will turn on. 
If the pattern is not detected, the red color LED will turn on 
at the end of the program. 
Your program should count the
number of occurrences of the pattern. */
#define RED_LED 5
#define GREEN_LED 4
#define PATTERN_LEN 3

// Input test array
char array[8] = {'A', 'B', 'A', 'A', 'A', 'C', 'A', 'A'};
// Pattern to detect is 3 A's 
char pattern[PATTERN_LEN] = {'A', 'A', 'A'};
int patternCount = 0;

void setup() {
  // Init comms
  Serial.begin(9600);
  // Init LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  // Pattern detection, loop through each point in array where pattern could be
  for (int i = 0; i <= sizeof(array) - PATTERN_LEN; i++) {
    int match = 1; // 1 for match, start by assuming
    // check each char of pattern 
    for (int j = 0; j < PATTERN_LEN; j++) {
      if (array[i + j] != pattern[j]) {
        match = 0; // no match 
        break;
      }
    } // if there is no 0, then there is match 
    if (match == 1) {
      patternCount++;
    }
  }

  // LED control
  if (patternCount > 0) {
    digitalWrite(GREEN_LED, HIGH);
  } else {
    digitalWrite(RED_LED, HIGH);
  }
  
  // Print pattern count
  Serial.print(F("Pattern count: "));
  Serial.println(patternCount);
}

void loop() {
}
