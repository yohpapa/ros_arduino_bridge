/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifdef L298_MOTOR_DRIVER
  // https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
  // PWM PINS are 3, 5, 6, 9, 10, 11 in Arduino Nano
  #define RIGHT_MOTOR_PWM      6    // ENA
  #define LEFT_MOTOR_PWM       5    // ENB
  #define RIGHT_MOTOR_FORWARD  8    // IN2
  #define RIGHT_MOTOR_BACKWARD 7    // IN1
  #define LEFT_MOTOR_FORWARD   11   // IN4
  #define LEFT_MOTOR_BACKWARD  10   // IN3
#endif

void initMotorController();
void setMotor(int dir, int pwmVal, int pwmPin, int forwardPin, int backwardPin);
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
