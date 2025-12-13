int pins[] = {7, 8, 9};
bool state[] = {0, 0, 0};  // led initial state 

int index = 0;
unsigned long prevTime = 0;
int interval = 100;

void setup() {
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  if (millis() - prevTime >= interval) {
    prevTime = millis();

    // toggle krenge
    state[index] = !state[index];
    digitalWrite(pins[index], state[index]);

    index++;
    if (index > 2) index = 0; // agar index >2 means all led complet till 3 again repeat the cycle by doing index=0;
  }
}
