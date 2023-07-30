/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifdef L298_MOTOR_DRIVER
  #define RIGHT_MOTOR_BACKWARD  7   // IN1A
  #define LEFT_MOTOR_BACKWARD  6
  #define RIGHT_MOTOR_FORWARD   8   // IN2A
  #define LEFT_MOTOR_FORWARD   10
  #define RIGHT_MOTOR_ENABLE    9   // PWMA
  #define LEFT_MOTOR_ENABLE    13
#endif

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
