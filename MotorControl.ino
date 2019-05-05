/******L298N******/
#define IN1         8  // K1,K2 motor direction
#define IN2         9  // K1,K2 motor direction
#define IN3        10  // K3,K4 motor direction
#define IN4        12  // K3,K4 motor direction
#define ENA         5  // Needs to be a PWM pin to be able to control motor speed ENA
#define ENB         6  // Needs to be a PWM pin to be able to control motor speed ENB

int motors_lspeed = 255, motors_rspeed = 255;
unsigned long motors_stop_time = 0;
unsigned long motors_loop_time = 0;

enum MotorsMode
{
  MOTORS_MANUAL, MOTORS_AVOIDANCE, MOTORS_LINEFOLLOW, MOTORS_PATHFOLLOW, 
} MOTORS_CMD = MOTORS_MANUAL;

/* called by main tank_robot sketch in setup() */
/* configure pins */
void motor_setup() {  
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  pinMode(ENA, OUTPUT);  
  pinMode(ENB, OUTPUT);
}

/* called by main tank_robot sketch in loop() */
void motor_loop()
{
  motor_cmd();

  if (millis() - motors_loop_time >= 100)
  {
    motors_loop_time = millis();
    if (MOTORS_CMD == MOTORS_AVOIDANCE)  { go_avoidance(); }
    if (MOTORS_CMD == MOTORS_LINEFOLLOW) { go_linefollow(); }
    if (MOTORS_CMD == MOTORS_PATHFOLLOW) { go_pathfollow(); }  
  }

  if (motors_stop_time > 0 && millis() >= motors_stop_time) {
    go_stop();
  }
}

/* check global CMD and CMD_ARG1 for tasks */
void motor_cmd()
{
  if (CMD == NONE) { return; }
  switch (CMD) 
  {
    case GO_STOP:
      MOTORS_CMD = MOTORS_MANUAL;
      go_stop(); CMD = NONE; break;
    case GO_FWD:
      go_fwd(CMD_ARG1); CMD = NONE; break;
    case GO_REV:
      go_rev(CMD_ARG1); CMD = NONE; break;
    case GO_LFT:
      go_lft(CMD_ARG1); CMD = NONE; break;
    case GO_RGT:
      go_rgt(CMD_ARG1); CMD = NONE; break;
    case GO_AVOIDANCE:
      MOTORS_CMD = MOTORS_AVOIDANCE; break;
    case GO_LINEFOLLOW:
      MOTORS_CMD = MOTORS_LINEFOLLOW; break;
    case GO_PATHFOLLOW:
      MOTORS_CMD = MOTORS_PATHFOLLOW; break;
    default:
      break;
  }
}

/* use SonicControl to avoid obstacles */
void go_avoidance()
{
  //Serial.println("go_avoidance");
  int front    = sonic_get_range(90);
  int minFront = 30;      
  int minSide  = 25;
  
  if (front > minFront) {
    //go_fwd(300); 
    set_command(GO_FWD,200);
  }
  else
  {
    go_stop();
    //buzzer_on(100);
    sonic_scan();

    int * v = sonic_data();
    if (v[3] > minFront && v[4] > minSide) {
      motors_rev(); delay(300);
      motors_lft(); delay(500);
    }
    else if (v[1] > minFront && v[0] > minSide) {
      motors_rev(); delay(300);
      motors_rgt(); delay(500);        
    }
    else {
      motors_rev(); delay(300);
      motors_lft(); delay(1000);
    }
  }
}

/* use IrControl to keep center line between sensors */
void go_linefollow()
{
  //Serial.println("go_linefollow");
  if (lf_read_lft() == false && lf_read_rgt() == false) 
  {
    set_command(GO_FWD,120);      
  }
  else
  {
    go_stop();
    //buzzer_on(100);

    if (lf_read_lft() == false && lf_read_rgt() == true) {
      motors_rgt(); delay(300);        
    }
    else if (lf_read_lft() == true && lf_read_rgt() == false) {
      motors_lft(); delay(300);        
    }
  }
}

/* use IrControl to keep sensors inside left/right border lines */
void go_pathfollow()
{
  //Serial.println("go_pathfollow");
  if (lf_read_lft() == false && lf_read_rgt() == false) 
  {
    set_command(GO_FWD,120);      
  }
  else
  {
    go_stop();
    //buzzer_on(100);

    if (lf_read_lft() == false && lf_read_rgt() == true) {
      motors_rev(); delay(300);
      motors_lft(); delay(300);        
    }
    else if (lf_read_lft() == true && lf_read_rgt() == false) {
      motors_rev(); delay(300);
      motors_rgt(); delay(300);        
    }
  }
}
/* set motor speeds left/right : 0-255 */
void go_speed(int lspeed_in, int rspeed_in)
{
  if (0 <= lspeed_in && lspeed_in <= 255) { motors_lspeed = lspeed_in; }
  if (0 <= rspeed_in && rspeed_in <= 255) { motors_rspeed = rspeed_in; }
  Serial.println("Set SPEED (+" + String(motors_lspeed) + ",+" + String(motors_rspeed) + ")");
  motors_speed(motors_lspeed,motors_rspeed);
}

/* stop motors */
void go_stop()
{
  Serial.println("STOP");
  motors_speed(0,0);
  motors_stop_time = 0;
}
/* go forward X milliseconds */
void go_fwd(int msec)
{
  Serial.println("Go FWD " + String(msec) + " (+ " + String(motors_lspeed) + ",+" + String(motors_rspeed) + ")");
  motors_fwd();
  motors_stop_time = (msec == 0) ? 0 : millis() + msec;
}
/* go reverse X milliseconds */
void go_rev(int msec)
{ 
  Serial.println("Go REV " + String(msec) + " (-" + String(motors_lspeed) + ",-" + String(motors_rspeed) + ")");
  motors_rev(); 
  motors_stop_time = (msec == 0) ? 0 : millis() + msec;
}
/* go left X milliseconds */
void go_lft(int msec)
{
  Serial.println("Go LFT " + String(msec) + " (-" + String(motors_lspeed) + ",+" + String(motors_rspeed) + ")");
  motors_lft();
  motors_stop_time = (msec == 0) ? 0 : millis() + msec;
}
/* go right X milliseconds */
void go_rgt(int msec)
{
  Serial.println("Go RGT " + String(msec) + " (+" + String(motors_lspeed) + ",-" + String(motors_rspeed) + ")");
  motors_rgt(); 
  motors_stop_time = (msec == 0) ? 0 : millis() + msec;
}

/* BASE FUNCTIONS */

/* set motor speeds lft/rgt : 0-255 */
void motors_speed(int lspeed, int rspeed)
{
  analogWrite(ENA,lspeed);
  analogWrite(ENB,rspeed);
}

/* set motor direction lfwd, rfwd */
void motors_fwd()
{
  motors_speed(motors_lspeed,motors_rspeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); 
}
/* set motor direction lrev, rrev */
void motors_rev()
{
  motors_speed(motors_lspeed,motors_rspeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
}
/* set motor direction lrev, rfwd */
void motors_lft()
{
  motors_speed(motors_lspeed,motors_rspeed);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); 
}
/* set motor direction lfwd, rrev */
void motors_rgt()
{
  motors_speed(motors_lspeed,motors_rspeed);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
}
