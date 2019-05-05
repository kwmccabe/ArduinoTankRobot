#define LFSensor_1 A0  // left line follow sensor
#define LFSensor_2 A1  // right line follow sensor

/* called by main tank_robot sketch in setup() */
void lf_setup() {
  pinMode(LFSensor_1,INPUT);
  pinMode(LFSensor_2,INPUT); 
}

/* read lft sensor */
bool lf_read_lft()
{
  char v = digitalRead(LFSensor_1);
  return (v == HIGH);
}

/* read rgt sensor */
bool lf_read_rgt()
{
  char v = digitalRead(LFSensor_2);
  return (v == HIGH);
}
