#include "globals.h"

void handleRover() {

  String direction = server.arg("direction");
  String speed = server.arg("speed");

  int temp = direction.toInt();
  int spd = speed.toInt();

  Serial.printf("dir: %d  spd: %d\n", temp , spd);

  int sector = (temp/45) + 1;
  float angle = temp % 45;

  Serial.printf("sector: %d  angle: %.2f\n", sector , angle);

  switch(sector) {
    case 1:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,HIGH);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(45 - angle)*spd/45);
      Serial.printf("case 1 | M1: HIGH, M2: HIGH | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    case 2:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,LOW);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(angle)*spd/45);
      Serial.printf("case 2 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    case 3:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,LOW);
      analogWrite(E1, (int)(45 - angle)*spd/45);
      analogWrite(E2, spd);
      Serial.printf("case 3 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", (int)((45 - angle)*spd/45), spd, angle);
      break;

    case 4:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,LOW);
      analogWrite(E1, (int)(angle)*spd/45);
      analogWrite(E2, spd);
      Serial.printf("case 4 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", (int)((45 - angle)*spd/45), spd, angle);
      break;

    case 5:
      digitalWrite(M1,LOW);
      digitalWrite(M2,LOW);
      analogWrite(E1, (int)(45 - angle)*spd/45);
      analogWrite(E2, spd);
      Serial.printf("case 5 | M1: LOW, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", (int)((45 - angle)*spd/45), spd, angle);
      break;

    case 6:
      digitalWrite(M1,LOW);
      digitalWrite(M2,HIGH);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(angle)*spd/45);
      Serial.printf("case 6 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    case 7:
      digitalWrite(M1,LOW);
      digitalWrite(M2,HIGH);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(45 - angle)*spd/45);
      Serial.printf("case 7 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    case 8:
      digitalWrite(M1,HIGH);
      digitalWrite(M2,HIGH);
      analogWrite(E1, spd);
      analogWrite(E2, (int)(angle)*spd/45);
      Serial.printf("case 8 | M1: HIGH, M2: LOW | E1: %d, E2: %d | angle: %.2f\n", spd, (int)((45 - angle)*spd/45), angle);
      break;

    default:
      Serial.printf("Error for direction | sector: %d | angle: %.2f\n", sector, angle);
  }
  server.send(200);
}