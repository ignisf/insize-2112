/*
  INSIZE 2112 series dial indicator reading decoder

  Decodes the current reading of the dial indicator and prints it to serial.

  The circuit:
  - TODO

  created 02 Aug 2018
  by Petko Bordjukov
*/

const int CLOCK_IN = 2;
const int DATA_IN  = 3;

const unsigned long MIN_VALID_INITIAL_PULSE_LENGTH = 100000;
const unsigned long MIN_VALID_DATA_PULSE_LENGTH = 15;
const unsigned long MAX_VALID_DATA_PULSE_LENGTH = 512;

bool bits[28];
unsigned long lastClock;

long measurement = 0;

void setup() {
  pinMode(CLOCK_IN, INPUT);
  pinMode(DATA_IN, INPUT);

  Serial.begin(115200);
}

void loop() {
  if (pulseIn(DATA_IN, LOW) > MIN_VALID_INITIAL_PULSE_LENGTH) {
    for (int i = 0; i < 27; i++) {
      lastClock = pulseIn(CLOCK_IN, HIGH, MAX_VALID_DATA_PULSE_LENGTH);
      bits[i] = digitalRead(DATA_IN);
      
      if (lastClock < MIN_VALID_DATA_PULSE_LENGTH) {
        return;
      }
    }
    
    measurement = 0;
    measurement += bits[4] + (bits[5] << 1) + (bits[6] << 2) + (bits[7] << 3);
    measurement += 10 * (bits[8] + (bits[9] << 1) + (bits[10] << 2) + (bits[11] << 3));
    measurement += 100 * (bits[12] + (bits[13] << 1) + (bits[14] << 2) + (bits[15] << 3));
    measurement += 1000 * (bits[16] + (bits[17] << 1) + (bits[18] << 2) + (bits[19] << 3));
    measurement += 10000 * (bits[20] + (bits[21] << 1) + (bits[22] << 2) + (bits[23] << 3));
    
    if (bits[25]) {
      measurement *= -1;
    }

    if (!bits[24]) {
      Serial.print(measurement / 100.0, 2);
      Serial.println(" mm");
    } else {
      Serial.print(measurement / 10000.0, 4);
      Serial.println(" in");
    }
  }
}
