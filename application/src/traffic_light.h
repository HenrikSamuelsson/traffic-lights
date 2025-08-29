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
void traffic_light_init(TrafficLight *tl, void *port, uint32_t red_pin,
                        uint32_t amber_pin, uint32_t green_pin,
                        bool active_high);

/* Turn all LEDs off */
void traffic_light_all_off(TrafficLight *tl);

/* Set composite state */
void traffic_light_set(TrafficLight *tl, TL_State state);

/* Zephyr-friendly initializer: pass three gpio_dt_spec handles */
#ifdef __ZEPHYR__
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
void traffic_light_init_specs(TrafficLight *tl, const struct gpio_dt_spec *red,
                              const struct gpio_dt_spec *amber,
                              const struct gpio_dt_spec *green,
                              bool active_high);
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRAFFIC_LIGHT_H */
