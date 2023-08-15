/* Functions and type-defs for PID control.

   Taken mostly from Mike Ferguson's ArbotiX code which lives at:
   
   http://vanadium-ros-pkg.googlecode.com/svn/trunk/arbotix/
*/

/* PID setpoint info For a Motor */
typedef struct {
  double TargetTicksPerFrame;    // target speed in ticks per frame
  long Encoder;                  // encoder count
  long PrevEnc;                  // last encoder count

  /*
  * Using previous input (PrevInput) instead of PrevError to avoid derivative kick,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
  */
  int PrevInput;                // last input
  //int PrevErr;                   // last error

  /*
  * Using integrated term (ITerm) instead of integrated error (Ierror),
  * to allow tuning changes,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
  */
  //int Ierror;
  int ITerm;                    //integrated term

  long output;                    // last motor setting

  unsigned int counter;         // Count up while the output does not make any change with encoder.
}
SetPointInfo;

SetPointInfo leftPID, rightPID;

/* PID Parameters */
int Kp = 20;
int Kd = 12;
int Ki = 0;
int Ko = 50;

unsigned char moving = 0; // is the base in motion?

/*
* Initialize PID variables to zero to prevent startup spikes
* when turning PID on to start moving
* In particular, assign both Encoder and PrevEnc the current encoder value
* See http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-initialization/
* Note that the assumption here is that PID is only turned on
* when going from stop to moving, that's why we can init everything on zero.
*/
void resetPID(){
   leftPID.TargetTicksPerFrame = 0.0;
   leftPID.Encoder = readEncoder(LEFT);
   leftPID.PrevEnc = leftPID.Encoder;
   leftPID.output = 0;
   leftPID.PrevInput = 0;
   leftPID.ITerm = 0;
   leftPID.counter = 0;

   rightPID.TargetTicksPerFrame = 0.0;
   rightPID.Encoder = readEncoder(RIGHT);
   rightPID.PrevEnc = rightPID.Encoder;
   rightPID.output = 0;
   rightPID.PrevInput = 0;
   rightPID.ITerm = 0;
   rightPID.counter = 0;
}

#define THRESHOLD_LOW_PWM_VALUE 50
#define COUNT_ACCEL_PWM_VALUE 1

/* PID routine to compute the next motor commands */
void doPID(SetPointInfo * p) {
  long Perror;
  long output;
  int input;

  //Perror = p->TargetTicksPerFrame - (p->Encoder - p->PrevEnc);
  input = p->Encoder - p->PrevEnc;
  Perror = p->TargetTicksPerFrame - input;

  // Tackling the issue where the DC motor does not work at low voltage in 
  // https://forum.arduino.cc/t/how-to-make-dc-motor-start-at-low-voltages/856181/23
  //
  // My solution is, if there's no change with encoder but the PID is trying to change the speed,
  // we will flag it and start counting up.
  if (p->Encoder == p->PrevEnc && p->TargetTicksPerFrame != 0) {
    p->counter ++;
  } else {
    p->counter = 0;
  }

  /*
  * Avoid derivative kick and allow tuning changes,
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
  * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
  */
  //output = (Kp * Perror + Kd * (Perror - p->PrevErr) + Ki * p->Ierror) / Ko;
  // p->PrevErr = Perror;
  output = (Kp * Perror - Kd * (input - p->PrevInput) + p->ITerm) / Ko;
  p->PrevEnc = p->Encoder;

  output += p->output;
  // Accumulate Integral error *or* Limit output.

  // We force PID to output minimum voltage when it does not make any change with low voltage.
  if (p->counter >= COUNT_ACCEL_PWM_VALUE) {
    // Serial.print(" <Accelerated!> ");
    // long orginal = output;

    if (abs(output) < THRESHOLD_LOW_PWM_VALUE) {
      if (Perror > 0) {
        if (output > 0) {
          output = THRESHOLD_LOW_PWM_VALUE;
        } else {
          output = 0;
        }
      } else if (Perror < 0) {
        if (output < 0) {
          output = -1 * THRESHOLD_LOW_PWM_VALUE;
        } else {
          output = 0;
        }
      }

      // Serial.print(orginal);
      // Serial.print(" -> ");
      // Serial.print(output);
      // Serial.println("");
    }

    p->counter = 0;
  }

  // Stop accumulating when output saturates
  if (output >= MAX_PWM)
    output = MAX_PWM;
  else if (output <= -MAX_PWM)
    output = -MAX_PWM;
  else
  /*
  * allow turning changes, see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
  */
  p->ITerm += Ki * Perror;

  p->output = output;
  p->PrevInput = input;
}

/* Read the encoder values and call the PID routine */
void updatePID() {
  /* Read the encoders */
  leftPID.Encoder = readEncoder(LEFT);
  rightPID.Encoder = readEncoder(RIGHT);

  // if (moving) {
  //   Serial.print("Left = (target: ");
  //   Serial.print(leftPID.TargetTicksPerFrame);
  //   Serial.print(", Left encoder: ");
  //   Serial.print(leftPID.Encoder);
  //   Serial.print(", Prev encoder: ");
  //   Serial.print(leftPID.PrevEnc);
  //   Serial.print(", Error: ");
  //   Serial.print(leftPID.TargetTicksPerFrame - leftPID.Encoder + leftPID.PrevEnc);
  //   Serial.print("), ");

  //   Serial.print("Right = (target: ");
  //   Serial.print(rightPID.TargetTicksPerFrame);
  //   Serial.print(", Right encoder: ");
  //   Serial.print(rightPID.Encoder);
  //   Serial.print(", Prev encoder: ");
  //   Serial.print(rightPID.PrevEnc);
  //   Serial.print(", Error: ");
  //   Serial.print(rightPID.TargetTicksPerFrame - rightPID.Encoder + rightPID.PrevEnc);
  //   Serial.println(")");
  // }
  
  /* If we're not moving there is nothing more to do */
  if (!moving){
    /*
    * Reset PIDs once, to prevent startup spikes,
    * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-initialization/
    * PrevInput is considered a good proxy to detect
    * whether reset has already happened
    */
    if (leftPID.PrevInput != 0 || rightPID.PrevInput != 0) resetPID();
    return;
  }

  /* Compute PID update for each motor */
  doPID(&rightPID);
  doPID(&leftPID);

  // Serial.print("Output = (left: ");
  // Serial.print(leftPID.output);
  // Serial.print(", right: ");
  // Serial.print(rightPID.output);
  // Serial.println(")");

  /* Set the motor speeds accordingly */
  setMotorSpeeds(leftPID.output, rightPID.output);
}

