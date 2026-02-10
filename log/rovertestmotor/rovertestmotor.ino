// # Editor     : Lauren from DFRobot
// # Date       : 17.02.2012

// # Product name: L298N motor driver module DF-MD v1.3
// # Product SKU : DRI0002
// # Version     : 1.0

// # Description:
// # The sketch for using the motor driver L298N
// # Run with the PWM mode

// # Connection:
// #        M1 pin  -> Digital pin 4
// #        E1 pin  -> Digital pin 5
// #        M2 pin  -> Digital pin 7
// #        E2 pin  -> Digital pin 6
// #        Motor Power Supply -> Centor blue screw connector(5.08mm 3p connector)
// #        Motor A  ->  Screw terminal close to E1 driver pin
// #        Motor B  ->  Screw terminal close to E2 driver pin
// #
// # Note: You should connect the GND pin from the DF-MD v1.3 to your MCU controller. They should share the GND pins.
// #
#define M1 33
#define M2 26
#define E1 32
#define E2 25

void setup()
{
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
}

void loop()
{
  int value;
  for(value = 0 ; value <= 255; value+=5)
  {
    digitalWrite(M1,HIGH);
    digitalWrite(M2,HIGH);
    analogWrite(E1, value);   //PWM Speed Control
    analogWrite(E2, value);   //PWM Speed Control
    delay(30);
  }
}


// // Pin mapping (your wiring)
// #define M1 33
// #define M2 26
// #define E1 32
// #define E2 25

// // Choose two LEDC PWM channels
// #define CH_E1 0
// #define CH_E2 1

// void setup() {
//   pinMode(M1, OUTPUT);
//   pinMode(M2, OUTPUT);

//   // Setup hardware PWM for ESP32
//   ledcAttachPin(E1, CH_E1);
//   ledcAttachPin(E2, CH_E2);

//   // freq = 1000 Hz, resolution = 8-bit
//   ledcSetup(CH_E1, 1000, 8);
//   ledcSetup(CH_E2, 1000, 8);

//   Serial.begin(115200);
//   Serial.println("LEDC motor test starting...");
// }

// void loop() {
//   // Sweep motor speed from 0 to 255
//   for (int value = 0; value <= 255; value += 5) {

//     digitalWrite(M1, HIGH);
//     digitalWrite(M2, HIGH);

//     ledcWrite(CH_E1, value);
//     ledcWrite(CH_E2, value);

//     Serial.printf("PWM = %d\n", value);
//     delay(30);
//   }
// }
