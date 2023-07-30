/***************************************************************
   Motor driver definitions
   
   Add a "#elif defined" block to this file to include support
   for a particular motor driver.  Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   *************************************************************/

#ifdef USE_BASE
   
#ifdef POLOLU_VNH5019
  /* Include the Pololu library */
  #include "DualVNH5019MotorShield.h"

  /* Create the motor driver object */
  DualVNH5019MotorShield drive;
  
  /* Wrap the motor driver initialization */
  void initMotorController() {
    drive.init();
  }

  /* Wrap the drive motor set speed function */
  void setMotorSpeed(int i, int spd) {
    if (i == LEFT) drive.setM1Speed(spd);
    else drive.setM2Speed(spd);
  }

  // A convenience function for setting both motor speeds
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#elif defined POLOLU_MC33926
  /* Include the Pololu library */
  #include "DualMC33926MotorShield.h"

  /* Create the motor driver object */
  DualMC33926MotorShield drive;
  
  /* Wrap the motor driver initialization */
  void initMotorController() {
    drive.init();
  }

  /* Wrap the drive motor set speed function */
  void setMotorSpeed(int i, int spd) {
    if (i == LEFT) drive.setM1Speed(spd);
    else drive.setM2Speed(spd);
  }

  // A convenience function for setting both motor speeds
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#elif defined L298_MOTOR_DRIVER
  void initMotorController() {
    setMotor(0, RIGHT_MOTOR_ENABLE, 0, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_BACKWARD);
    setMotor(0, LEFT_MOTOR_ENABLE,  0, LEFT_MOTOR_FORWARD,  LEFT_MOTOR_BACKWARD);

    // digitalWrite(RIGHT_MOTOR_ENABLE, HIGH);
    // digitalWrite(LEFT_MOTOR_ENABLE, HIGH);
  }

  void setMotor(int dir, int pwmVal, int pwm, int in1, int in2) {
    analogWrite(pwm, pwmVal);
    if (dir == 1) {           // Clockwise
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    } else if (dir == -1) {   // Anti-clockwise
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    } else {                  // Stop
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
  }
  
  // void setMotorSpeed(int i, int spd) {
  //   unsigned char reverse = 0;
  
  //   if (spd < 0)
  //   {
  //     spd = -spd;
  //     reverse = 1;
  //   }
  //   if (spd > 255)
  //     spd = 255;
    
  //   if (i == LEFT) { 
  //     if      (reverse == 0) { analogWrite(LEFT_MOTOR_FORWARD, spd); analogWrite(LEFT_MOTOR_BACKWARD, 0); }
  //     else if (reverse == 1) { analogWrite(LEFT_MOTOR_BACKWARD, spd); analogWrite(LEFT_MOTOR_FORWARD, 0); }
  //   }
  //   else /*if (i == RIGHT) //no need for condition*/ {
  //     if      (reverse == 0) { analogWrite(RIGHT_MOTOR_FORWARD, spd); analogWrite(RIGHT_MOTOR_BACKWARD, 0); }
  //     else if (reverse == 1) { analogWrite(RIGHT_MOTOR_BACKWARD, spd); analogWrite(RIGHT_MOTOR_FORWARD, 0); }
  //   }
  // }

  void setMotorSpeed(int motorId, int speed) {
    int dir = 1;
    if (speed < 0) {
      dir = -1;
      speed *= -1;
    }

    if (speed > 255) {
      speed = 255;
    }

    int pwm = RIGHT_MOTOR_ENABLE;
    int in1 = RIGHT_MOTOR_FORWARD;
    int in2 = RIGHT_MOTOR_BACKWARD;
    if (motorId == LEFT) {
      pwm = LEFT_MOTOR_ENABLE;
      in1 = LEFT_MOTOR_FORWARD;
      in2 = LEFT_MOTOR_BACKWARD;
    }

    setMotor(dir, speed, pwm, in1, in2);
  }
  
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#else
  #error A motor driver must be selected!
#endif

#endif
