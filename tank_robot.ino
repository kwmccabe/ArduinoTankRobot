/*
 * Parts:
 * UNO R3 board [http://kookye.com/?p=5909]
 * ESP8266 WiFi board [http://kookye.com/?p=5902]
 * Bluetooth module [http://kookye.com/?p=5912]
 * Motor Driver board [http://kookye.com/?p=5916]
 * IR Tracking sensor [http://kookye.com/?p=7278]
 * SG90 Servo Motor [http://kookye.com/?p=5926]
 * HC-SR04 Ultrasonic Sensor module [http://kookye.com/?p=5945]
 * Buzzer module [http://kookye.com/?p=5994]
 * IR Receiver [http://kookye.com/?p=5953]
 * IR Remote [http://kookye.com/?p=10258]
 * Voltage Meter [http://kookye.com/?p=5959]
*/

String inputSerial = "";  // Store serial data

enum RobotCommand
{
  BUZZER_FLASH, BUZZER_ON, BUZZER_OFF,
  LED_FLASH, LED_ON, LED_OFF,
  GO_STOP, GO_FWD, GO_REV, GO_LFT, GO_RGT,
  GO_AVOIDANCE, GO_LINEFOLLOW, GO_PATHFOLLOW, 
  NONE
} CMD = NONE;
unsigned long CMD_ARG1 = 0;

void set_command(RobotCommand cmd, unsigned long arg1)
{
  CMD = cmd;
  CMD_ARG1 = arg1;
}

/* */
void setup() {
  Serial.begin(9600);

  buzzer_setup(); // Buzzer
  ir_setup();     // IR Remote
  led_setup();    // Headlights
  lf_setup();     // IR Track Sensors x2  [http://kookye.com/?p=7278 ]
  motor_setup();  // L298N
  servo_setup();  // Head for Ultrasonic
  sonic_setup();  // Ultrasonic HC-SR04
  //wifi_setup();   // Wifi/Bluetooth

  Serial.println("TANK CAR serial commands:");
  Serial.println("buzzer [b]: test buzzer");
  Serial.println("buzzer flash: alternate buzzer on/off");
  Serial.println("buzzer off: turn off buzzer");
  
  Serial.println("led [l]: test LEDs ");
  Serial.println("led flash : alternate LED headlights ");
  Serial.println("led on : turn on LED headlights ");
  Serial.println("led off : turn off LED headlights ");
  
  Serial.println("follow [f] : test line-follow sensors");
  Serial.println("range [r]: test ultrasonic range finder");
  Serial.println("servo [v]: test servo");
  Serial.println("sonic [s]: test servo and range finder");
  Serial.println("motor [m]: test motors");
  Serial.println("wifi [w]: test wifi/bluetooth");

  Serial.println("MOTOR CONTROL");
  Serial.println("fwd : motors FORWARD");
  Serial.println("rev : motors REVERSE");
  Serial.println("lft : motors LEFT");
  Serial.println("rgt : motors RIGHT");
  Serial.println("stop : motors OFF");
  Serial.println("--------------------------------------------------->>");
}

/* main control loop : dispatch to controller loops */
void loop() 
{
  serial_loop();  // read cmds from serial
  ir_loop();      // read cmds from ir reciever
  wifi_loop();    // read cmds from wifi/bluetooth reciever
  
  buzzer_loop();  // switch buzzer
  led_loop();     // switch leds
  motor_loop();   // run motors
}

/* receive serial data */
void serial_loop()
{
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();

    // check for newline
    if ((int)inChar != 10) {
      inputSerial += inChar;
    } else {
      Serial.println("'" + inputSerial + "'");

      if (inputSerial == "buzzer" || inputSerial == "b") { test_buzzer(); }
      else if (inputSerial == "buzzer flash") { set_command(BUZZER_FLASH,10000); }
      else if (inputSerial == "buzzer off") { set_command(BUZZER_OFF,0); }
      
      else if (inputSerial == "led" || inputSerial == "l") { test_led(); }
      else if (inputSerial == "led flash") { set_command(LED_FLASH,10000); }
      else if (inputSerial == "led on") { set_command(LED_ON,0); }
      else if (inputSerial == "led off") { set_command(LED_OFF,0); }

      else if (inputSerial == "follow" || inputSerial == "f") { test_follow(); }
      else if (inputSerial == "range" || inputSerial == "r") { test_distance(); }
      else if (inputSerial == "servo" || inputSerial == "v") { test_servo(); }
      else if (inputSerial == "sonic" || inputSerial == "s") { test_sonic(); }
      else if (inputSerial == "motor" || inputSerial == "m") { test_motors(); }
      else if (inputSerial == "wifi" || inputSerial == "w") { test_wifi(); }
      
      else if (inputSerial == "fwd") { set_command(GO_FWD,5000); }
      else if (inputSerial == "rev") { set_command(GO_REV,5000); }
      else if (inputSerial == "lft") { set_command(GO_LFT,5000); }
      else if (inputSerial == "rgt") { set_command(GO_RGT,5000); }
      else if (inputSerial == "stop") { set_command(GO_STOP,0); }

      else if (inputSerial == "avoid") { set_command(GO_AVOIDANCE,0 ); }

      inputSerial = "";
    }
  }
}

/* test buzzer : on 1 sec, off 1 sec, on 3 sec */
void test_buzzer()
{
  Serial.println("Buzzer ON 2 seconds");
  buzzer_on(0); delay(2000);
  buzzer_off(); delay(2000);
  
  Serial.println("Buzzer ON/OFF 3 second");
  buzzer_flash(3000);  // calls buzzer_off()
}

/* test led headlights */
void test_led()
{
  Serial.println("LED Left ON");
  led_lft_on(); led_rgt_off(); delay(2000);

  Serial.println("LED Right ON");
  led_lft_off(); led_rgt_on(); delay(2000);

  Serial.println("LED ON");
  led_on(0); delay(2000);
  Serial.println("LED OFF");
  led_off(); delay(2000);
  
  Serial.println("LED FLASH");
  led_flash(4000);  // calls led_off()
}

/* read line-follow sensors */
void test_follow()
{
  Serial.println("Left sensor " + String(lf_read_lft() ? "ON" : "OFF") + " black");
  Serial.println("Right sensor " + String(lf_read_rgt() ? "ON" : "OFF") + " black");
}

/* test ultrasonic range finder */
void test_sonic()
{
  Serial.println("Manual sonic scan");
  for (int pos = 0; pos <= 180; pos += 45) {
    int range = sonic_get_range(pos);
    Serial.println(String(pos) + "deg : " + String(range) + "cm");
  }
  servo_set_position(90);

  Serial.println("Auto sonic scan");
  sonic_scan();
  int * v = sonic_data();
  for (int i = 0; i < 5; i++) {
    Serial.println("SONIC_SCAN_DATA[" + String(i) + "] : " + String(v[i]) + "cm");
  }
}

/* test ultrasonic range finder */
void test_distance()
{
  int range = sonic_get_distance();
  Serial.println("Distance: " + String(range) + "cm");
}

/* test servo */
void test_servo()
{
  int head_pos;

  for (int pos = 0; pos <= 180; pos += 1) {
    servo_set_position(pos);
    delay(15);
  }
  head_pos = servo_get_position();
  Serial.println("Servo Position: " + String(head_pos));

  for (int pos = 180; pos >= 0; pos -= 1) {
    servo_set_position(pos);
    delay(15);
  }
  head_pos = servo_get_position();
  Serial.println("Servo Position: " + String(head_pos));
  
  servo_set_position(90); // return to center
  head_pos = servo_get_position();
  Serial.println("Servo Position: " + String(head_pos));
}

/* test motors functions */
void test_motors()
{
  motors_speed(255,255);
  Serial.println("motors_fwd()"); motors_fwd(); delay(5000);
  Serial.println("motors_rev()"); motors_rev(); delay(5000);
  Serial.println("motors_lft()"); motors_lft(); delay(5000);
  Serial.println("motors_rgt()"); motors_rgt(); delay(5000);
  go_stop(); delay(5000);

  // rev motors 50-255
  motors_speed(50,50);
  motors_fwd();
  for (int i = 105; i <= 255; i += 10) {
    motors_speed(i,i);
    delay(300);
  }
  
  go_fwd(3000);  // calls go_stop() - non-blocking
  led_on(3000);  // calls led_off()
}

/* test wifi functions */
void test_wifi() 
{
  Serial.println("test_wifi");
}
