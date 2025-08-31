/** 
 * \file traffic_light.c
 * \brief Traffic light state control implementation.
 * \details Doxygen documentation restored after refactor to make the API discoverable.
 */

#include "traffic_light.h"

static void set_only(LED *on_led, LED *off1, LED *off2) {
  if (on_led)
    led_on(on_led);
  if (off1)
    led_off(off1);
  if (off2)
    led_off(off2);
}

/**
 * \brief Initialize a TrafficLight with three LEDs (red, amber, green).
 * Configures the three internal LED handles and ensures all lamps are off at the end.
 * \param tl TrafficLight handle to initialize.
 * \param red_port Port pointer for the red LED (or Zephyr device pointer).
 * \param red_pin Pin index for red LED.
 * \param amber_port Port pointer for amber LED.
 * \param amber_pin Pin index for amber LED.
 * \param green_port Port pointer for green LED.
 * \param green_pin Pin index for green LED.
 * \param active_high True if a logic '1' turns LEDs on (false for active-low).
 * \return None
 */
void traffic_light_init(TrafficLight *tl, void *port, uint32_t red_pin,
                        uint32_t amber_pin, uint32_t green_pin,
                        bool active_high) {
  if (!tl)
    return;
  led_init(&tl->red, port, red_pin, active_high);
  led_init(&tl->amber, port, amber_pin, active_high);
  led_init(&tl->green, port, green_pin, active_high);
  traffic_light_all_off(tl);
}

/**
 * \brief Turn all traffic-light LEDs off.
 * \param tl TrafficLight handle.
 * \return None
 */
void traffic_light_all_off(TrafficLight *tl) {
  if (!tl)
    return;
  led_off(&tl->red);
  led_off(&tl->amber);
  led_off(&tl->green);
}

/**
 * \brief Set the traffic light to a specific color/state.
 * Ensures exclusivity by turning other lamps off before enabling the requested one.
 * \param tl TrafficLight handle.
 * \param state Desired state: red, amber, or green.
 * \return None
 */
void traffic_light_set(TrafficLight *tl, TL_State state) {
  if (!tl)
    return;
  switch (state) {
  case TL_STATE_RED:
    set_only(&tl->red, &tl->amber, &tl->green);
    break;
  case TL_STATE_RED_AMBER:
    led_on(&tl->red);
    led_on(&tl->amber);
    led_off(&tl->green);
    break;
  case TL_STATE_GREEN:
    set_only(&tl->green, &tl->red, &tl->amber);
    break;
  case TL_STATE_AMBER:
  default:
    set_only(&tl->amber, &tl->red, &tl->green);
    break;
  }
}

#ifdef __ZEPHYR__
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
/**
 * \brief Zephyr helper initializer using gpio_dt_spec descriptors.
 * Convenience wrapper when using Zephyr devicetree bindings to define pins.
 * \param tl TrafficLight handle to initialize.
 * \param red Devicetree GPIO spec for the red LED.
 * \param amber Devicetree GPIO spec for the amber LED.
 * \param green Devicetree GPIO spec for the green LED.
 * \param active_high True if LED logic is active-high; false for active-low.
 * \return None
 */
void traffic_light_init_specs(TrafficLight *tl, const struct gpio_dt_spec *red,
                              const struct gpio_dt_spec *amber,
                              const struct gpio_dt_spec *green,
                              bool active_high) {
  if (!tl)
    return;
  led_init(&tl->red, (void *)red, 0, active_high);
  led_init(&tl->amber, (void *)amber, 0, active_high);
  led_init(&tl->green, (void *)green, 0, active_high);
  traffic_light_all_off(tl);
}
#endif
