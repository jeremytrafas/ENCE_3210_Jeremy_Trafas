#define SIZE 512 
int results[SIZE]; 
// Function to calculate twice the square of a given integer
int twiceSquare(int num) {
  return 2 * num * num;
}

void setup() {
  Serial.begin(9600);
  // Calculate twice the square of numbers between 0 and 511
  for (int i = 0; i < SIZE; i++) {
    results[i] = twiceSquare(i);
  }
  // Print the results
  for (int i = 0; i < SIZE; i++) {
    Serial.print("results[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(results[i]);
  }
}
void loop() {
}
