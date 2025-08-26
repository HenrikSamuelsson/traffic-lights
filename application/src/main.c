/*
 * Traffic light on nRF5340-DK:
 * LED2 = Red, LED3 = Amber, LED4 = Green
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* ---- Timings (ms) ---- */
#define RED_MS 3000
#define RED_AMBER_MS 1000
#define GREEN_MS 4000
#define AMBER_MS 1500

/* Devicetree aliases:
 * On nRF5340-DK, printed LEDs map as:
 * LED1 -> led0, LED2 -> led1, LED3 -> led2, LED4 -> led3
 * We use LED2/3/4 for the traffic light:
 */
#define RED_NODE DT_ALIAS(led1)   /* LED2 */
#define AMBER_NODE DT_ALIAS(led2) /* LED3 */
#define GREEN_NODE DT_ALIAS(led3) /* LED4 */

#if !DT_NODE_HAS_STATUS(RED_NODE, okay) ||                                     \
    !DT_NODE_HAS_STATUS(AMBER_NODE, okay) ||                                   \
    !DT_NODE_HAS_STATUS(GREEN_NODE, okay)
#error                                                                         \
    "One or more LED aliases (led1/led2/led3) not found; check board/overlays"
#endif

static const struct gpio_dt_spec RED = GPIO_DT_SPEC_GET(RED_NODE, gpios);
static const struct gpio_dt_spec AMBER = GPIO_DT_SPEC_GET(AMBER_NODE, gpios);
static const struct gpio_dt_spec GREEN = GPIO_DT_SPEC_GET(GREEN_NODE, gpios);

static int setup_led(const struct gpio_dt_spec *led) {
  if (!gpio_is_ready_dt(led)) {
    printk("LED port not ready\n");
    return -ENODEV;
  }
  return gpio_pin_configure_dt(led, GPIO_OUTPUT_INACTIVE); /* start OFF */
}

static void set_lights(bool r, bool a, bool g) {
  /* gpio_pin_set_dt() honors active-low/active-high flags from DT */
  gpio_pin_set_dt(&RED, r);
  gpio_pin_set_dt(&AMBER, a);
  gpio_pin_set_dt(&GREEN, g);
}

int main(void) {
  if (setup_led(&RED) || setup_led(&AMBER) || setup_led(&GREEN)) {
    return 0;
  }

  printk("Traffic light starting...\n");

  while (1) {
    /* RED */
    set_lights(true, false, false);
    k_msleep(RED_MS);

    /* RED + AMBER (get ready) */
    set_lights(true, true, false);
    k_msleep(RED_AMBER_MS);

    /* GREEN */
    set_lights(false, false, true);
    k_msleep(GREEN_MS);

    /* AMBER (prepare to stop) */
    set_lights(false, true, false);
    k_msleep(AMBER_MS);
  }
}
