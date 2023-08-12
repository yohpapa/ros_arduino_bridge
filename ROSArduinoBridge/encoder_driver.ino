/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(ARDUINO_ENC_COUNTER)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
  static const int8_t ENC_STATES [] = {
        //  prev A, prev B, curr A, curr B
     0, //       0,      0,      0,      0 (No change)
     1, //       0,      0,      0,      1 (Detect clockwise start)
    -1, //       0,      0,      1,      0 (Detect anti-clockwise start)
     0, //       0,      0,      1,      1 (NA)
    -1, //       0,      1,      0,      0 (Detect anti-clockwise end)
     0, //       0,      1,      0,      1 (During a rising edge - clockwise)
     0, //       0,      1,      1,      0 (NA)
     1, //       0,      1,      1,      1
     1, //       1,      0,      0,      0 (Detect clockwise end)
     0, //       1,      0,      0,      1 (NA)
     0, //       1,      0,      1,      0 (During a risig edge - anti-clockwise)
    -1, //       1,      0,      1,      1
     0, //       1,      1,      0,      0 (NA)
    -1, //       1,      1,      0,      1
     1, //       1,      1,      1,      0
     0, //       1,      1,      1,      1 (No change)
  };  //encoder lookup table
    
  /* Interrupt routine for LEFT encoder, taking care of actual counting */
  ISR (PCINT2_vect){
  // void readLeftEncoder_Int() {
  	static uint8_t enc_last=0;

    enc_last <<=2; //shift previous state two places

    enc_last |= (PIND & (3 << 2)) >> 2; //read the current state into lowest 2 bits
  
    // int encA = digitalRead(LEFT_ENC_PIN_A);
    // int encB = digitalRead(LEFT_ENC_PIN_B);

    // int enc_curr = encB << 1 | encA;
    // enc_last |= enc_curr;

  	left_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  ISR (PCINT1_vect){
  // void readRightEncoder_Int() {
    static uint8_t enc_last=0;

    enc_last <<=2; //shift previous state two places
    enc_last |= (PINC & (3 << 4)) >> 4; //read the current state into lowest 2 bits

    // int encA = digitalRead(RIGHT_ENC_PIN_A);
    // int encB = digitalRead(RIGHT_ENC_PIN_B);

    // int enc_curr = encB << 1 | encA;
    // enc_last |= enc_curr;
  
  	right_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
  }
#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif

