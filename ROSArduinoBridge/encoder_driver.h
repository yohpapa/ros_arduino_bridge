/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
   
   
#ifdef ARDUINO_ENC_COUNTER
  // https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  //
  // | BOARD                            | DIGITAL PINS USABLE FOR INTERRUPTS |
  // | -------------------------------- | ---------------------------------- |
  // | Uno, Nano, Mini, other 328-based | 2, 3
  //
  #define LEFT_ENC_PIN_A   2  // Used for RISING interrupt
  #define LEFT_ENC_PIN_B   4
  #define RIGHT_ENC_PIN_A  3  // Used for RISING interrupt
  #define RIGHT_ENC_PIN_B  9
#endif

void readLeftEncoder_Int();
void readRightEncoder_Int();
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
void updateEncoder(int pinB, long* pos);
