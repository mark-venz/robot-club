/* -*- mode: c; -*- */
/*
 * Programming a robot (mBot)
 */


/* ********************************************************************
 * Constants definitions
 *
 * There is much debate about #define vs const TYPE
 */
const uint8_t LED_PIN = 13;
const uint8_t SW_PIN  = A7;

#define TOGGLE(PIN) digitalWrite(PIN, digitalRead(PIN) ^ 1)

/* ********************************************************************
 * Global definitions
 *
 * if any
 */



/* ********************************************************************
 * Line sensor
 *
 *
 */
typedef enum {

  BOTH_BLACK   = 0,   /* sensor1 and sensor2 are both inside of black line */
  LEFT_BLACK   = 1,   /* sensor1 is inside of black line and sensor2 is outside of black line */
  RIGHT_BLACK  = 2,   /* sensor1 is outside of black line and sensor2 is inside of black line */
  BOTH_WHITE   = 3    /* sensor1 and sensor2 are both outside of black line */

} Line_Sensor_State;

const uint8_t LINE_SENSOR_1_PIN =  9;
const uint8_t LINE_SENSOR_2_PIN = 10;



/* ********************************************************************
 * Drive
 *
 *
 */
typedef enum {

  STOP    = 0,
  FORWARD,
  SPIN_L,
  SPIN_R,
  REVERSE

} Drive_Direction;
Drive_Direction drive_state      = STOP;
Drive_Direction last_drive_state = STOP;

const uint8_t M1_DIR_PIN = 7;
const uint8_t M1_PWM_PIN = 6;
const uint8_t M2_DIR_PIN = 4;
const uint8_t M2_PWM_PIN = 5;


/* ********************************************************************
 * functions
 */

Line_Sensor_State read_line_follower (void) {

  uint8_t state = (1 & digitalRead(LINE_SENSOR_1_PIN)) << 1 | digitalRead(LINE_SENSOR_2_PIN);

  return (Line_Sensor_State) state;
}


void change_drive (Drive_Direction dir, uint8_t speed) {

  /* stop so directions can change with out messing things up */
  analogWrite(M1_PWM_PIN, 0);
  analogWrite(M2_PWM_PIN, 0);

  switch (dir) {
  case REVERSE:
    digitalWrite(M1_DIR_PIN, 1);
    digitalWrite(M2_DIR_PIN, 0);
    break;

  case SPIN_R:
    digitalWrite(M1_DIR_PIN, 0);
    digitalWrite(M2_DIR_PIN, 0);
    break;

  case SPIN_L:
    digitalWrite(M1_DIR_PIN, 1);
    digitalWrite(M2_DIR_PIN, 1);
    break;

  case FORWARD:
    digitalWrite(M1_DIR_PIN, 0);
    digitalWrite(M2_DIR_PIN, 1);
    break;

  case STOP:			/* fall-through */
  default:
    speed = 0;
    break;
  }

  analogWrite(M1_PWM_PIN, speed);
  analogWrite(M2_PWM_PIN, speed);

  last_drive_state = drive_state;
}



/* ********************************************************************
 * Function definitions
 *
 * TODO push these to a library?
 */

void intermitanttly_check () {
  /* any thing that needs to be be periodically updated will live in here
   * this is called on each loop
   *
   * Checking current time against when an expected time
   */
  uint16_t now = millis();

  const uint16_t HEARTBEAT_PERIOD = 500; /* half second beat on the LED */
  static uint32_t heartbeat_next = HEARTBEAT_PERIOD;


  /* heartbeat checking  */
  if (now > heartbeat_next) {
    TOGGLE(LED_PIN);
    heartbeat_next = now + HEARTBEAT_PERIOD; /* set the next time this should be changed */
  }

}



/* ********************************************************************
 * start up
 *
 * anything run once at start-up / reboot should be added here
 */
void setup () {

  pinMode(LED_PIN, OUTPUT);	/* write to pin 13 - on board LED connected to  */
  pinMode(SW_PIN, INPUT);

  for (uint8_t i = 0; i < 6; i++) {
    /* show a reboot flash sequence */
    TOGGLE(LED_PIN);
    delay(125);			/* using delay in start up -- not delaying other processes */
  }

  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M2_DIR_PIN, OUTPUT);

  change_drive(STOP, 0);

}


/* ********************************************************************
 * loop
 *
 * run forever
 */
void loop () {

  static uint8_t motion_enabled = 0; /* Controlled by the button */

  static uint8_t last_line_reading;
  uint8_t now_line_reading;

  /* no delay periodic actions  */
  intermitanttly_check();

  /* look for the button checked */
  if (!(analogRead(SW_PIN) > 10)) {

    motion_enabled = 1 ^ motion_enabled; /* toggle the motion_enabled via button */

  }

  /* get the line sensor state and do something with it */
  if (motion_enabled) {
    now_line_reading = read_line_follower();

    switch (now_line_reading) {

    case BOTH_BLACK:

      drive_state = (last_line_reading == LEFT_BLACK)
	? SPIN_R
	: (last_line_reading == RIGHT_BLACK) ? SPIN_L :  drive_state;

      break;

    case LEFT_BLACK:
      /* Fall-through */
    case RIGHT_BLACK:
      drive_state = FORWARD;
      break;

    case BOTH_WHITE:

      drive_state = (last_line_reading == LEFT_BLACK)
	? SPIN_L
	: (last_line_reading == RIGHT_BLACK) ? SPIN_R :  drive_state;

      break;

    }

    last_line_reading = now_line_reading;

  } else {

    drive_state = STOP;

  }


  /* Update the drive */
  if (last_drive_state != drive_state) {
    change_drive(drive_state, 100);
  }

}
