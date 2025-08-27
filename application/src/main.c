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
 *
 * Timings are configurable via preprocessor macros.
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/** \name Traffic light timings
 *  Durations in milliseconds for each light phase.
 */
/**\{*/
#define RED_MS 3000       /**< Duration of the RED phase. */
#define RED_AMBER_MS 1000 /**< Duration of the RED+AMBER phase. */
#define GREEN_MS 4000     /**< Duration of the GREEN phase. */
#define AMBER_MS 1500     /**< Duration of the AMBER phase. */
/**\}*/

/**
 * \name Devicetree LED aliases
 * On the nRF5340-DK, the four user LEDs map as follows:
 * - LED1 → led0
 * - LED2 → led1
 * - LED3 → led2
 * - LED4 → led3
 *
 * This application uses LED2, LED3, and LED4 for the traffic light.
 */
/**\{*/
#define RED_NODE DT_ALIAS(led1)   /**< LED2 → Red light. */
#define AMBER_NODE DT_ALIAS(led2) /**< LED3 → Amber light. */
#define GREEN_NODE DT_ALIAS(led3) /**< LED4 → Green light. */
/**\}*/

#if !DT_NODE_HAS_STATUS(RED_NODE, okay) ||                                     \
    !DT_NODE_HAS_STATUS(AMBER_NODE, okay) ||                                   \
    !DT_NODE_HAS_STATUS(GREEN_NODE, okay)
#error                                                                         \
    "One or more LED aliases (led1/led2/led3) not found; check board/overlays"
#endif

/** GPIO descriptor for the RED LED. */
static const struct gpio_dt_spec RED = GPIO_DT_SPEC_GET(RED_NODE, gpios);
/** GPIO descriptor for the AMBER LED. */
static const struct gpio_dt_spec AMBER = GPIO_DT_SPEC_GET(AMBER_NODE, gpios);
/** GPIO descriptor for the GREEN LED. */
static const struct gpio_dt_spec GREEN = GPIO_DT_SPEC_GET(GREEN_NODE, gpios);

/**
 * \brief Configure an LED pin for output.
 *
 * \param led Pointer to a GPIO descriptor (from Devicetree).
 * \retval 0 on success.
 * \retval -ENODEV if the LED port is not ready.
 */
static int setup_led(const struct gpio_dt_spec *led) {
  if (!gpio_is_ready_dt(led)) {
    printk("LED port not ready\n");
    return -ENODEV;
  }
  return gpio_pin_configure_dt(led, GPIO_OUTPUT_INACTIVE); /**< Start OFF. */
}

/**
 * \brief Set the traffic light outputs.
 *
 * This helper sets each LED according to boolean flags.
 * The Zephyr GPIO API automatically respects active-low/active-high
 * configuration from the Devicetree.
 *
 * \param r true to turn RED on.
 * \param a true to turn AMBER on.
 * \param g true to turn GREEN on.
 */
static void set_lights(bool r, bool a, bool g) {
  gpio_pin_set_dt(&RED, r);
  gpio_pin_set_dt(&AMBER, a);
  gpio_pin_set_dt(&GREEN, g);
}

/**
 * \brief Application entry point.
 *
 * Initializes the three LEDs and runs an infinite loop cycling through
 * the traffic light sequence with predefined timings.
 *
 * \return 0 (this function never returns under normal operation).
 */
int main(void) {
  if (setup_led(&RED) || setup_led(&AMBER) || setup_led(&GREEN)) {
    return 0;
  }

  printk("Traffic light starting...\n");

  while (1) {
    /** RED */
    set_lights(true, false, false);
    k_msleep(RED_MS);

    /** RED + AMBER (get ready) */
    set_lights(true, true, false);
    k_msleep(RED_AMBER_MS);

    /** GREEN */
    set_lights(false, false, true);
    k_msleep(GREEN_MS);

    /** AMBER (prepare to stop) */
    set_lights(false, true, false);
    k_msleep(AMBER_MS);
  }
}
