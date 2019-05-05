#define MAX_PACKETSIZE 32    //Serial receive buffer

char buffUART[MAX_PACKETSIZE];
unsigned int buffUARTIndex = 0;
unsigned long preUARTTick = 0;

/* called by main tank_robot sketch in setup() */
void wifi_setup() {
  return;
}

/* called by main tank_robot sketch in loop() */
void wifi_loop() {
  char Uart_Data = 0;
  if (Serial.available() > 0)
  {
    size_t len = Serial.available();
    uint8_t sbuf[len + 1];
    sbuf[len] = 0x00;
    Serial.readBytes(sbuf, len);

    memcpy(buffUART + buffUARTIndex, sbuf, len);
    buffUARTIndex += len;
    preUARTTick = millis();
    if(buffUARTIndex >= MAX_PACKETSIZE - 1) 
    {
      buffUARTIndex = MAX_PACKETSIZE - 2;
      preUARTTick = preUARTTick - 200;
    }
  }

  if(buffUARTIndex > 0 && (millis() - preUARTTick >= 100))
  {
    buffUART[buffUARTIndex] = 0x00;
    if(buffUART[0] == 'C') 
    {
      Serial.println(buffUART);
      Serial.println("You have modified the parameters!");//indicates that the obstacle avoidance distance parameter has been modified
      //sscanf(buffUART,"CMD%d,%d,%d",&distancelimit,&sidedistancelimit,&turntime);
      // Serial.println(distancelimit);
      // Serial.println(sidedistancelimit);
      // Serial.println(turntime);
    }
    else  Uart_Data = buffUART[0];
    buffUARTIndex = 0;
  }

  switch (Uart_Data)
  {
    case '2':
      set_command(GO_FWD,200); break;
    case '4':
      set_command(GO_LFT,200); break;
    case '6':
      set_command(GO_RGT,200); break;
    case '8':
      set_command(GO_REV,300); break;
    case '5':
      set_command(GO_STOP,0); break;
    case '3':
      set_command(GO_AVOIDANCE,0); break;
    case '1':
      set_command(GO_LINEFOLLOW,0); break;
    default: break;
  }

}
