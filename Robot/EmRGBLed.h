#ifndef EmRGBLed_H
#define EmRGBLed_H

/**
 * Reduced MeRGBLed library --for this sesesion.
 * Taking only what we need.
 * Attempting to avoid bringing in the entire library
 *
 * Original is available at
 * https://github.com/Makeblock-official/Makeblock-Libraries-for-Codebender.git
 * \par Copyright (C), 2012-2016, MakeBlock
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>


/// @brief Class for RGB Led Module
struct cRGB
{
  uint8_t g;
  uint8_t r;
  uint8_t b;
};

class EmRGBLed {
 public:
  EmRGBLed(uint8_t port, uint8_t led_num);
  ~EmRGBLed();

  bool setColorAt(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
  bool setColorAll(uint8_t red, uint8_t green, uint8_t blue);

  void setpin(uint8_t port);
  void show(void);


 private:
  uint8_t count_led;
  uint8_t *pixels;

  void rgbled_sendarray_mask(uint8_t *array, uint16_t length, uint8_t pinmask, uint8_t *port);
  void setNumber(uint8_t num_led);

  const volatile uint8_t *ws2812_port;
  volatile uint8_t *ws2812_port_reg;
  uint8_t pinMask;

};
#endif /* EmRGBLed_H */
