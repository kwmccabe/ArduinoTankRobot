 /* HC-SR04 */
#define TRIG       A2  // Ultrasonic Trig
#define ECHO       A3  // Ultrasonic Echo

int SONIC_SCAN_DATA[5] = {0,0,0,0,0};

/* called by main tank_robot sketch in setup() */
void sonic_setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, LOW);
}

/* return pointer to sensor status array */
int * sonic_data()
{
  return SONIC_SCAN_DATA;
}

/* update sensor status array : [0] = right, [1] = left */
void sonic_scan()
{
  SONIC_SCAN_DATA[0] = sonic_get_range(0);    // 90 right
  SONIC_SCAN_DATA[1] = sonic_get_range(45);   // 45 right
  SONIC_SCAN_DATA[2] = sonic_get_range(90);   // center
  SONIC_SCAN_DATA[3] = sonic_get_range(135);  // 45 left
  SONIC_SCAN_DATA[4] = sonic_get_range(180);  // 90 left
  SONIC_SCAN_DATA[2] = sonic_get_range(90);   // center
  servo_set_position(90);

  // DEBUG info
  int i = 0;
  for (int pos = 0; pos <= 180; pos += 45) {
    Serial.println(String(pos) + "deg : " + SONIC_SCAN_DATA[i++] + "cm");
  }
}

/* set position and get range */
int sonic_get_range(int pos)
{
  servo_set_position(pos);
  return sonic_get_distance();
}

/* get distance at current position in centimeters */
int sonic_get_distance()
{
  long result;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG, LOW);
  result = pulseIn(ECHO, HIGH);
  return round(result * 0.01657);  // convert to nearest centimeters
}
