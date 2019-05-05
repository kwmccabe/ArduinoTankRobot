#define BUZZERPIN  7  // buzzer connect to D7

unsigned long buzz_off_time = 0;
unsigned long buzz_loop_time = 0;
int buzz_flash_cnt = 0;

/* called by main tank_robot sketch in setup() */
void buzzer_setup() {
  pinMode(BUZZERPIN, OUTPUT);
  buzzer_off();
}

/* called by main tank_robot sketch in loop() */
void buzzer_loop()
{
  buzzer_cmd();

  if (buzz_flash_cnt > 0 && millis() - buzz_loop_time >= 400)
  {
    buzz_loop_time = millis();
    if (buzz_flash_cnt % 2 == 0) {
      buzz_off();
    } else {
      buzz_on();
    }
    //Serial.println(String(buzz_flash_cnt) + " : " + String(buzz_loop_time));
    buzz_flash_cnt++;
  }

  if (buzz_off_time > 0 && millis() >= buzz_off_time) {
    buzz_off();
  }
}

/* check global CMD and CMD_ARG1  for tasks */
void buzzer_cmd()
{
  if (CMD == NONE) { return; }
  switch (CMD) 
  {
    case BUZZER_FLASH:
      buzzer_flash(CMD_ARG1); CMD = NONE; break;
    case BUZZER_ON:
      buzzer_on(CMD_ARG1); CMD = NONE; break;
    case BUZZER_OFF:
      buzzer_off(); CMD = NONE; break;
    default:
      break;
  }
}

/* buzzer ON for X milliseconds */
void buzzer_flash(int msec)
{
  Serial.println("BUZZER FLASH (+" + String(msec) + ")");
  if (buzz_flash_cnt == 0) { buzz_flash_cnt = 1; }
  buzz_off_time = (msec == 0) ? 0 : millis() + msec;
  buzz_loop_time = millis() + 1000; // start immediately
}

/* buzzer ON for X milliseconds */
void buzzer_on(int msec)
{
  Serial.println("BUZZER ON (+" + String(msec) + ")");
  buzz_on();  
  buzz_flash_cnt = 0;
  buzz_off_time = (msec == 0) ? 0 : millis() + msec;
}

/* buzzer OFF */
void buzzer_off()
{
  Serial.println("BUZZER OFF");
  buzz_off();
  buzz_flash_cnt = 0;
  buzz_off_time = 0;
}

/* buzzer ON */
void buzz_on()
{
  digitalWrite(BUZZERPIN, LOW);  
}
/* buzzer OFF */
void buzz_off()
{
  digitalWrite(BUZZERPIN, HIGH);
}
