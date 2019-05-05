#include <IRremote.h>

#define IRPIN      13  // IR receiver

#define IR_DUPE    0xFFFFFFFF    // button codes from IR controller
#define IR_OK      0x00FF38C7
#define IR_UP      0x00FF18E7
#define IR_DOWN    0x00FF4AB5
#define IR_LEFT    0x00FF10EF
#define IR_RIGHT   0x00FF5AA5
#define IR_NUM     0x00FFB04F
#define IR_STAR    0x00FF6897
#define IR_0       0x00FF9867
#define IR_1       0x00FFA25D
#define IR_2       0x00FF629D
#define IR_3       0x00FFE21D
#define IR_4       0x00FF22DD
#define IR_5       0x00FF02FD
#define IR_6       0x00FFC23D
#define IR_7       0x00FFE01F
#define IR_8       0x00FFA857
#define IR_9       0x00FF906F

IRrecv IR(IRPIN);  // IRrecv object to get code from IR remote
decode_results IRresults;   
unsigned long ir_dupe_value = 0;

/* called by main tank_robot sketch in setup() */
void ir_setup() {
  pinMode(IRPIN, INPUT); 
  digitalWrite(IRPIN, HIGH);  
  IR.enableIRIn();
}

/* called by main tank_robot sketch in loop() */
void ir_loop() {
  if (IR.decode(&IRresults)) // listen
  {
    //Serial.println(IRresults.value, HEX);
    if (IRresults.value == IR_DUPE)       
      { IRresults.value = ir_dupe_value; }
    switch (IRresults.value) 
    {
      //case IR_NUM:
      //case IR_STAR:
      case IR_0:     // same as IR_OK
      case IR_OK:    set_command(GO_STOP,0); break;
      case IR_UP:    set_command(GO_FWD,200); break;
      case IR_DOWN:  set_command(GO_REV,300); break;
      case IR_LEFT:  set_command(GO_LFT,200); break;
      case IR_RIGHT: set_command(GO_RGT,200); break;
      case IR_1:     set_command(LED_ON,0); break;
      case IR_2:     set_command(LED_FLASH,0); break;
      case IR_3:     set_command(LED_OFF,0); break;
      case IR_4:     set_command(BUZZER_ON,300); break;
      case IR_5:     set_command(BUZZER_FLASH,0); break;
      case IR_6:     set_command(BUZZER_OFF,0); break;
      case IR_7:     set_command(GO_AVOIDANCE,0); break;
      case IR_8:     set_command(GO_LINEFOLLOW,0); break;
      case IR_9:     set_command(GO_PATHFOLLOW,0); break;
      default:       break;
    }
    ir_dupe_value = IRresults.value;
    IR.resume(); // Receive the next value
  }
}
