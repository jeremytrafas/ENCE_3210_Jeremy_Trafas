char array[5] = {'A','B','C','R','B'};
int Rcount = 0;
int Gcount = 0;
int Bcount = 0; 

void setup() {
  Serial.begin(9600);
    for (int i = 0; i < 5; i++) {
      if (array[i] == 'R') {
          Serial.println("R found");
          Rcount++;
      } else if (array[i] == 'G') {
          Serial.println("G found");
          Gcount++;
      } else if (array[i] == 'B') {
          Serial.println("B found");
          Bcount++;
      }
  }
}

void loop() {

}
