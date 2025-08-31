/**
 * \file traffic_light.h
 * \brief Public API and data structures for a simple traffic light controller.
 * \details Doxygen documentation restored after refactor to make the API
 * discoverable.
 */

#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "led.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { TL_RED = 0, TL_AMBER, TL_GREEN } TL_Color;

typedef enum {
  TL_STATE_RED = 0,
  TL_STATE_RED_AMBER,
  TL_STATE_GREEN,
  TL_STATE_AMBER
} TL_State;

typedef struct TrafficLight {
  LED red;
  LED amber;
  LED green;
} TrafficLight;

/* Generic init (non-Zephyr path uses port/pin). Kept for portability. */
/**
 * \brief Initialize a TrafficLight with three LEDs (red, amber, green).
 * Configures the three internal LED handles and ensures all lamps are off at
 * the end.
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
                        bool active_high);

/* Turn all LEDs off */
/**
 * \brief Turn all traffic-light LEDs off.
 * \param tl TrafficLight handle.
 * \return None
 */
void traffic_light_all_off(TrafficLight *tl);

/* Set composite state */
/**
 * \brief Set the traffic light to a specific color/state.
 * Ensures exclusivity by turning other lamps off before enabling the requested
 * one.
 * \param tl TrafficLight handle.
 * \param state Desired state: red, amber, or green.
 * \return None
 */
void traffic_light_set(TrafficLight *tl, TL_State state);

/* Zephyr-friendly initializer: pass three gpio_dt_spec handles */
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
                              bool active_high);
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRAFFIC_LIGHT_H */
