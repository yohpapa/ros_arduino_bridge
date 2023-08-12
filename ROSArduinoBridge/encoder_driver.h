/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
   
   
#ifdef ARDUINO_ENC_COUNTER
  //below can be changed, but should be PORTD pins; 
  //otherwise additional changes in the code are required
  #define LEFT_ENC_PIN_A PD2  //pin 2
  #define LEFT_ENC_PIN_B PD3  //pin 3
  // #define LEFT_ENC_PIN_A 2  //pin 2
  // #define LEFT_ENC_PIN_B 3  //pin 3
  
  //below can be changed, but should be PORTC pins
  #define RIGHT_ENC_PIN_A PC4  //pin A4
  #define RIGHT_ENC_PIN_B PC5   //pin A5
  // #define RIGHT_ENC_PIN_A A4  //pin A4
  // #define RIGHT_ENC_PIN_B A5   //pin A5
#endif

void readLeftEncoder_Int();
void readRightEncoder_Int();
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();

