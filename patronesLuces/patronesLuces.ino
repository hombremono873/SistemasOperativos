const int ledPins[] = {13, 12, 14, 27, 26, 25, 33, 32};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
const int numPatterns = 200;

const int patterns[][numLeds] = {
  {0b00011000, 0b00111100, 0b01111110, 0b11111111, 0b01111110, 0b00111100, 0b00011000, 0b00000000},
  {0b00010000, 0b00110000, 0b01110000, 0b11110000, 0b11111000, 0b11111100, 0b11111110, 0b11111111},
  {0b10000001, 0b11000011, 0b11100111, 0b11111111, 0b11110111, 0b11101111, 0b11011111, 0b10111111},
  {0b00000000, 0b10001000, 0b11001100, 0b11101110, 0b11111111, 0b11111110, 0b11111100,  0b11111000},
  {0b11110000, 0b11100000, 0b11000000, 0b10000000, 0b00000000, 0b11111111, 0b00000000, 0b11111111},

  {0b11111111, 0b11111111, 0b11110000, 0b11110000, 0b00001111, 0b00001111, 0b11111111, 0b11111111},
  {0b11000011, 0b11000011, 0b00111100, 0b00111100, 0b11000011, 0b11000011, 0b00111100, 0b00111100},
  {0b11111111, 0b00000000, 0b11111111, 0b00000000, 0b11111111, 0b00000000, 0b11111111, 0b00000000},
  {0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010},
  {0b11110000, 0b00001111, 0b11110000, 0b00001111, 0b11110000, 0b00001111, 0b11110000, 0b00001111},
  // ... y muchos más ...
};

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < numPatterns; i++) {
    for (int j = 0; j < numLeds; j++) {
      displayPattern(patterns[i % numPatterns][j]);
      delay(500); // Tiempo fijo entre patrones (ajusta según tu preferencia)
    }
  }
}

void displayPattern(int pattern) {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], (pattern & (1 << i)) ? HIGH : LOW);
  }
}

