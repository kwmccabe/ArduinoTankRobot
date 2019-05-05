#define LED1        2  // left led
#define LED2        3  // right led

unsigned long led_stop_time = 0;
unsigned long led_loop_time = 0;
int led_flash_cnt = 0;

/* called by main tank_robot sketch in setup() */
void led_setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
}

/* called by main tank_robot sketch in loop() */
void led_loop()
{
  led_cmd();
  
  if (led_flash_cnt > 0 && millis() - led_loop_time >= 500)
  {
    led_loop_time = millis();
    if (led_flash_cnt % 2 == 0) {
      led_lft_on(); led_rgt_off();
    } else {
      led_lft_off(); led_rgt_on();
    }
    //Serial.println(String(led_flash_cnt) + " : " + String(led_loop_time));
    led_flash_cnt++;
  }

  if (led_stop_time > 0 && millis() >= led_stop_time) {
    led_off();
  }
}

/* check global CMD and CMD_ARG1  for tasks */
void led_cmd()
{
  if (CMD == NONE) { return; }
  switch (CMD) 
  {
    case LED_FLASH:
      led_flash(CMD_ARG1); CMD = NONE; break;
    case LED_ON:
      led_on(CMD_ARG1); CMD = NONE; break;
    case LED_OFF:
      led_off(); CMD = NONE; break;
    default:
      break;
  }
}

/* flash headlights for X milliseconds */
void led_flash(int msec)
{
  Serial.println("LEDs FLASH (+" + String(msec) + ")");
  if (led_flash_cnt == 0) {
    led_flash_cnt = 1;  
  } 
  led_stop_time = (msec == 0) ? 0 : millis() + msec;
}

/* lft and rgt headlights ON for X milliseconds */
void led_on(int msec)
{
  Serial.println("LEDs ON (+" + String(msec) + ")");
  led_lft_on();
  led_rgt_on();
  led_flash_cnt = 0;
  led_stop_time = (msec == 0) ? 0 : millis() + msec;
}
/* lft and rgt headlights OFF */
void led_off()
{
  Serial.println("LEDs OFF");
  led_lft_off();
  led_rgt_off();
  led_flash_cnt = 0;
  led_stop_time = 0;
}

/* lft headlight */
void led_lft_on()
{
  digitalWrite(LED1, LOW);
}
void led_lft_off()
{
  digitalWrite(LED1, HIGH);
}

/* rgt headlight */
void led_rgt_on()
{
  digitalWrite(LED2, LOW);
}
void led_rgt_off()
{
  digitalWrite(LED2, HIGH);
}
