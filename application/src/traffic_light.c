#include "traffic_light.h"

static void set_only(LED *on_led, LED *off1, LED *off2) {
  if (on_led)
    led_on(on_led);
  if (off1)
    led_off(off1);
  if (off2)
    led_off(off2);
}

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

void traffic_light_all_off(TrafficLight *tl) {
  if (!tl)
    return;
  led_off(&tl->red);
  led_off(&tl->amber);
  led_off(&tl->green);
}

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
