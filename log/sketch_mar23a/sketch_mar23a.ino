#define M1 33  // Motor 1 direction
#define M2 26  // Motor 2 direction
#define E1 32  // Motor 1 speed (PWM)
#define E2 25  // Motor 2 speed (PWM)

void setup() {
  Serial.begin(115200);

  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);

  Serial.println("Rover forward/backward speed test starting...");
}

void loop() {
  // --- Forward high speed ---
  Serial.println("Forward - high speed");
  digitalWrite(M1, HIGH);  // forward
  digitalWrite(M2, HIGH);  // forward
  analogWrite(E1, 255);    // full speed
  analogWrite(E2, 255);
  delay(2000);

  // --- Forward low speed ---
  Serial.println("Forward - low speed");
  analogWrite(E1, 128);    // half speed
  analogWrite(E2, 128);
  delay(2000);

  // --- Stop ---
  Serial.println("Stop");
  analogWrite(E1, 0);
  analogWrite(E2, 0);
  delay(1000);

  // --- Backward high speed ---
  Serial.println("Backward - high speed");
  digitalWrite(M1, LOW);   // backward
  digitalWrite(M2, LOW);   // backward
  analogWrite(E1, 255);
  analogWrite(E2, 255);
  delay(2000);

  // --- Backward low speed ---
  Serial.println("Backward - low speed");
  analogWrite(E1, 128);
  analogWrite(E2, 128);
  delay(2000);

  // --- Stop ---
  Serial.println("Stop");
  analogWrite(E1, 0);
  analogWrite(E2, 0);
  delay(1000);
}