#include "globals.h"

void handleRover(JsonDocument &doc) {
  int temp = doc["dir"] | 0;  // default 0 if missing
  int spd = doc["spd"] | 0;

  Serial.println("Dir: " + String(temp) + " Spd: " + String(spd));
  controlRover(temp, spd);
}

// Rover control function
void controlRover(int temp, int spd) {
  int sector = (temp / 45) + 1;
  float angle = temp % 45;

  if (spd <= 0) {  // deadzone
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    analogWrite(E1, 0);
    analogWrite(E2, 0);
    Serial.println("Stopping motors");
    return;
  }

  int e1Val = 0, e2Val = 0;


  switch (sector) {
    case 1:
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      e1Val = spd;
      e2Val = (int)((45 - angle) * spd / 45);
      break;
    case 2:
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      e1Val = spd;
      e2Val = (int)(angle * spd / 45);
      break;
    case 3:
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      e1Val = (int)((45 - angle) * spd / 45);
      e2Val = spd;
      break;
    case 4:
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      e1Val = (int)(angle * spd / 45);
      e2Val = spd;
      break;
    case 5:
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      e1Val = (int)((45 - angle) * spd / 45);
      e2Val = spd;
      break;
    case 6:
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      e1Val = spd;
      e2Val = (int)(angle * spd / 45);
      break;
    case 7:
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      e1Val = spd;
      e2Val = (int)((45 - angle) * spd / 45);
      break;
    case 8:
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      e1Val = spd;
      e2Val = (int)(angle * spd / 45);
      break;
    default:
      Serial.printf("Error: invalid sector %d\n", sector);
      return;
  }

  analogWrite(E1, e1Val);
  analogWrite(E2, e2Val);

  Serial.printf("Sector: %d | Angle: %.2f | M1:%d M2:%d | E1:%d E2:%d\n",
                sector, angle,
                digitalRead(M1), digitalRead(M2),
                e1Val, e2Val);
}
