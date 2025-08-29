/**
 * \file main.c
 * \brief Traffic light application for the nRF5340-DK using Zephyr RTOS.
 *
 * This program controls three on-board LEDs to simulate a simple
 * traffic light sequence: RED → RED+AMBER → GREEN → AMBER.
 *
 * - LED2 = Red
 * - LED3 = Amber
 * - LED4 = Green
 */

#include "traffic_light.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/** \name Traffic light timings (milliseconds) */
#define RED_MS 3000
#define RED_AMBER_MS 1000
#define GREEN_MS 4000
#define AMBER_MS 1500

int main(void) {
  /* Bind to board LED aliases (nRF5340-DK has active-low LEDs) */
#if DT_NODE_HAS_STATUS(DT_ALIAS(led1), okay) &&                                \
    DT_NODE_HAS_STATUS(DT_ALIAS(led2), okay) &&                                \
    DT_NODE_HAS_STATUS(DT_ALIAS(led3), okay)

  const struct gpio_dt_spec red =
      GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios); /* LED2 */
  const struct gpio_dt_spec amber =
      GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios); /* LED3 */
  const struct gpio_dt_spec green =
      GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios); /* LED4 */

  TrafficLight tl;
  /* nRF5340-DK LEDs are active-low -> active_high=false */
  traffic_light_init_specs(&tl, &red, &amber, &green, false);

  printk("Traffic light starting...\n");

  while (1) {
    traffic_light_set(&tl, TL_STATE_RED);
    k_msleep(RED_MS);

    traffic_light_set(&tl, TL_STATE_RED_AMBER);
    k_msleep(RED_AMBER_MS);

    traffic_light_set(&tl, TL_STATE_GREEN);
    k_msleep(GREEN_MS);

    traffic_light_set(&tl, TL_STATE_AMBER);
    k_msleep(AMBER_MS);
  }

#else
#warning "LED aliases led1..led3 not defined; check board/overlays"
#endif

  return 0;
}
