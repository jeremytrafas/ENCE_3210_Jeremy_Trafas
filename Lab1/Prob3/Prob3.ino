int array[50] = {0};
int* memoryLocation = array+50;

int secondMaximum(int arr[], int size) {
    int max = 0, secondMax = 1;

    for (int i = 0; i < size; i++) {
        if (arr[i] > max) {
            secondMax = max;
            max = arr[i];
        } else if (arr[i] > secondMax && arr[i] < max) {
            secondMax = arr[i];
        }
    }

    return secondMax;
}

void setup() {
  Serial.begin(9600);
    for (int i = 1; i < 51; i++) {
    array[i] = i;
  }
  *memoryLocation = secondMaximum(array,50);
  Serial.println(*memoryLocation);
}
// *memoryLocation = value;
void loop() {
  // put your main code here, to run repeatedly:

}
