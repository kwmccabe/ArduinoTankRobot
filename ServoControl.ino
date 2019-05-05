#include <Servo.h>
#define SERVO  11  // servo motor

Servo HEAD;        // Servo object to control a servo

/* called by main tank_robot sketch in setup() */
void servo_setup() {
  HEAD.attach(SERVO);
  HEAD.write(90);
}

/* set servo position 0-180, right-left */
void servo_set_position(int pos)
{
  if (0 <= pos && pos <= 180) {
    int oldpos = servo_get_position();
    int timeout = 50 + round(abs(pos - oldpos) * 2.5);  // 500msec/180deg
    Serial.println("servo_set_position : timeout=" + String(timeout));
 
    HEAD.write(pos); 
    delay(timeout); 
  }
}

/* get servo position */
int servo_get_position()
{
  if (!HEAD.attached()) { 
    Serial.println("ERROR: Servo NOT Attached");
    return -1; 
  } 
  int result = HEAD.read();
  return result;
}
