/*
 * Programming a robot (mBot)
 */


/* ********************************************************************
 * Constants definitions
 *
 * There is much debate about #define vs const TYPE
 */
const uint8_t LED_PIN = 13;

#define TOGGLE(PIN) digitalWrite(PIN, digitalRead(PIN) ^ 1)


/* ********************************************************************
 * Function definitions
 *
 * TODO push these to a library
 */


void intermitanttly_check () {
  /* any thing that needs to be be periodically updated will live in here
   * this is called on each loop
   *
   * Checking current time against when an expected time
   */

  const uint16_t HEARTBEAT_PERIOD = 500; /* half second beat on the LED */
  static uint32_t heartbeat_next = HEARTBEAT_PERIOD;
  uint16_t now = millis();

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
  /* write to the LED_PIN */
  pinMode(LED_PIN, OUTPUT);

  for (uint8_t i = 0; i < 6; i++) {
    /* show a reboot flash sequence */
    TOGGLE(LED_PIN);
    delay(125); 		/* using delay in start up -- not delaying other processes */
  }

}


/* ********************************************************************
 * loop
 *
 * run forever
 */
void loop () {

  /* no delay period */
  intermitanttly_check();
}
