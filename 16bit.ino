#define clockPin 12
#define dataPin 11
#define latchPin 8

void setup() {
  Serial.begin(9600);

  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void decToBinary(long n, int binaryNum[]) {
  // array to store binary number
  int binaryArray[16];

  // counter for binary array
  int i = 0;
  while (n > 0) {
    // storing remainder in binary array
    binaryArray[i] = n % 2;
    n = n / 2;
    i++;
  }

  // Copy values in reverse order to the provided array
  for (int k = 0; k < i; k++) {
    binaryNum[k] = binaryArray[i - k - 1];
  }
}

void toTwoArrays(int whole[], int binaryNum1[], int binaryNum2[]){
  for(int i=0; i<8; i++){  // Fix array indices
    binaryNum1[i] = whole[i];
  }

  for(int i=8; i<16; i++){  // Fix array indices
    binaryNum2[i-8] = whole[i];
  }
}

void updateShiftRegister(int* byte1, int* byte2) {
  digitalWrite(latchPin, LOW);

  byte combinedByte1 = 0;
  byte combinedByte2 = 0;

  // Combine individual bits into a byte
  for (int i = 0; i < 8; i++) {
    combinedByte1 |= (byte1[i] << i);
    combinedByte2 |= (byte2[i] << i);
  }

  // Reverse the order of bits in the combined bytes
  combinedByte1 = reverseBits(combinedByte1);
  combinedByte2 = reverseBits(combinedByte2);

  // Shift out the reversed combined bytes
  shiftOut(dataPin, clockPin, MSBFIRST, combinedByte1);
  shiftOut(dataPin, clockPin, MSBFIRST, combinedByte2);

  digitalWrite(latchPin, HIGH);
}

byte reverseBits(byte b) {
  byte result = 0;
  for (int i = 0; i < 8; i++) {
    result |= ((b >> i) & 1) << (7 - i);
  }
  return result;
}


void loop() {
  int binPart1[8], binPart2[8];
  long t1 = 0, t2 = 1, nextTerm = 0, n;

  n = 65535;

  nextTerm = t1 + t2;

  while (nextTerm < n) {
    int nextTermToBinary[16] = {0};  // Initialize the array with zeros
    decToBinary(nextTerm, nextTermToBinary);

    toTwoArrays(nextTermToBinary, binPart1, binPart2);
    updateShiftRegister(binPart1, binPart2);

    Serial.print(nextTerm);
    Serial.print(" ");
    for (int i=7; i>=0; i--){
      Serial.print(binPart1[i]);
    }
    Serial.print(" ");
    for (int i=7; i>=0; i--){
      Serial.print(binPart2[i]);
    }
    Serial.println();

    delay(2000);
    t1 = t2;
    t2 = nextTerm;
    nextTerm = t1 + t2;
  }

  int maxBinary[16] = {0};
  decToBinary(65535, maxBinary);
  toTwoArrays(maxBinary, binPart1, binPart2);
  updateShiftRegister(binPart1, binPart2);
  delay(2000);
  Serial.print(65535);
  Serial.print(" ");
  for (int i=7; i>=0; i--){
    Serial.print(binPart1[i]);
  }
  Serial.print(" ");
  for (int i=7; i>=0; i--){
    Serial.print(binPart2[i]);
  }
  Serial.println();
}
