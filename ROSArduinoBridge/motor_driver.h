/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifdef L298_MOTOR_DRIVER
  // #define RIGHT_MOTOR_BACKWARD 5
  // #define LEFT_MOTOR_BACKWARD  6
  // #define RIGHT_MOTOR_FORWARD  9
  // #define LEFT_MOTOR_FORWARD   10
  // #define RIGHT_MOTOR_ENABLE 12
  // #define LEFT_MOTOR_ENABLE 13

  // https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
  // PWM PINS are 3, 5, 6, 9, 10, 11 in Arduino Nano
  #define RIGHT_MOTOR_ENABLE 10   // ENA (PWM)
  #define LEFT_MOTOR_ENABLE  11   // ENB (PWM)

  #define RIGHT_MOTOR_IN1     5   // IN1
  #define RIGHT_MOTOR_IN2     6   // IN2
  #define LEFT_MOTOR_IN1      7   // IN3
  #define LEFT_MOTOR_IN2      8   // IN4
#endif

void initMotorController();
void setMotor(int dir, int pwmVal, int pwm, int in1, int in2);
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
