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
    setMotor(0, 0, RIGHT_MOTOR_PWM, RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_BACKWARD);
    setMotor(0, 0, LEFT_MOTOR_PWM,  LEFT_MOTOR_FORWARD,  LEFT_MOTOR_BACKWARD);
  }

  void setMotor(int dir, int pwmVal, int pwmPin, int forwardPin, int backwardPin) {
    analogWrite(pwmPin, pwmVal);
    if (dir == 1) {           // Clockwise
      digitalWrite(forwardPin, HIGH);
      digitalWrite(backwardPin, LOW);
    } else if (dir == -1) {   // Counter-clockwise
      digitalWrite(forwardPin, LOW);
      digitalWrite(backwardPin, HIGH);
    } else {                  // Stop
      digitalWrite(forwardPin, LOW);
      digitalWrite(backwardPin, LOW);
    }
  }

  void setMotorSpeed(int motorId, int speed) {
    int dir = 1;
    if (speed < 0) {
      dir = -1;
      speed *= -1;
    }

    if (speed > 255) {
      speed = 255;
    }

    int pwmPin = RIGHT_MOTOR_PWM;
    int forwardPin = RIGHT_MOTOR_FORWARD;
    int backwardPin = RIGHT_MOTOR_BACKWARD;
    if (motorId == LEFT) {
      pwmPin = LEFT_MOTOR_PWM;
      forwardPin = LEFT_MOTOR_FORWARD;
      backwardPin = LEFT_MOTOR_BACKWARD;
    }

    setMotor(dir, speed, pwmPin, forwardPin, backwardPin);
  }
  
  void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
  }
#else
  #error A motor driver must be selected!
#endif

#endif
