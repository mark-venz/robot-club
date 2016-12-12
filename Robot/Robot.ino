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
 *
 */
uint8_t button_pressed = 0;


/* ********************************************************************
 * Drive
 *
 *
 */
typedef enum {
  STOP = 0,
  FORWARD,
  SPIN_L,
  SPIN_R,
  REVERSE
} Drive_Direction;
Drive_Direction drive_state = STOP;
Drive_Direction last_drive_state = STOP;

const uint8_t M1_DIR_PIN = 7;
const uint8_t M1_PWM_PIN = 6;
const uint8_t M2_DIR_PIN = 4;
const uint8_t M2_PWM_PIN = 5;

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
  case SPIN_L:
    /* TODO  */
    speed = 0;			/* stopping for the moment */
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

  static uint32_t time_to_drive = 0;

  /* heartbeat checking  */
  if (now > heartbeat_next) {
    TOGGLE(LED_PIN);
    heartbeat_next = now + HEARTBEAT_PERIOD; /* set the next time this should be changed */
  }

  /* After 0.5 sec move for 0.5 sec */
  if (button_pressed) {
    time_to_drive = now + HEARTBEAT_PERIOD; /* use some thing else if more that 1/2sec is required */
    drive_state = FORWARD;
  }
  if (now > time_to_drive) {
    drive_state = STOP;
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

  /* no delay periodic actions  */
  intermitanttly_check();

  /* Update the drive */
  if (last_drive_state != drive_state) {
    change_drive(drive_state, 100);
  }

  /* look for the button checked */
  button_pressed = !(analogRead(SW_PIN) > 10); /* pin should be normally high */

}
