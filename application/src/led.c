/** 
 * \file led.c
 * \brief LED control implementation using either Zephyr GPIO or platform hooks.
 * \details Doxygen documentation restored after refactor to make the API discoverable.
 */

#include "led.h"

#ifdef __ZEPHYR__
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(led_mod, LOG_LEVEL_INF);
#endif

/* Platform hooks (weak by default for non-Zephyr) */
__attribute__((weak)) void platform_gpio_write(void *port, uint32_t pin, int value) {
    (void)port; (void)pin; (void)value;
}

__attribute__((weak)) int platform_gpio_read(void *port, uint32_t pin) {
    (void)port; (void)pin;
    return 0;
}

/**
 * \brief Initialize an LED handle for the target platform.
 * Binds the LED to the provided port/pin and records its active level.
 * For Zephyr, prefer traffic_light_init_specs() to pass devicetree pin specs directly.
 * \param led Handle to initialize.
 * \param port Platform-specific GPIO port/base pointer (Zephyr: pointer to gpio_dt_spec when 'is_dt_spec' is set).
 * \param pin GPIO pin number or unused when passing a gpio_dt_spec.
 * \param active_high True if logic '1' turns the LED on; false for active-low wiring.
 * \return None
 */
void led_init(LED *led, void *port, uint32_t pin, bool active_high) {
    if (!led) return;
    led->active_high = active_high;

#ifdef __ZEPHYR__
    /* In Zephyr, 'port' is expected to be a gpio_dt_spec* */
    const struct gpio_dt_spec *spec = (const struct gpio_dt_spec *)port;
    led->gpio = *spec;
    led->configured = false;

    if (!device_is_ready(led->gpio.port)) {
        /* Device not ready; leave unconfigured */
        return;
    }
    /* Configure as output, default OFF */
    int ret = gpio_pin_configure_dt(&led->gpio, GPIO_OUTPUT_INACTIVE);
    if (ret == 0) {
        led->configured = true;
        /* Ensure OFF according to polarity */
        if (active_high) {
            gpio_pin_set_dt(&led->gpio, 0);
        } else {
            gpio_pin_set_dt(&led->gpio, 1);
        }
    }
#else
    led->port = port;
    led->pin = pin;
#endif
}

/**
 * \brief Turn the LED on according to its active level.
 * \param led Handle of the LED to drive.
 * \return None
 */
void led_on(LED *led) {
    if (!led) return;
#ifdef __ZEPHYR__
    if (!led->configured) return;
    gpio_pin_set_dt(&led->gpio, led->active_high ? 1 : 0);
#else
    platform_gpio_write(led->port, led->pin, led->active_high ? 1 : 0);
#endif
}

/**
 * \brief Turn the LED off according to its active level.
 * \param led Handle of the LED to drive.
 * \return None
 */
void led_off(LED *led) {
    if (!led) return;
#ifdef __ZEPHYR__
    if (!led->configured) return;
    gpio_pin_set_dt(&led->gpio, led->active_high ? 0 : 1);
#else
    platform_gpio_write(led->port, led->pin, led->active_high ? 0 : 1);
#endif
}

/**
 * \brief Toggle the current state of the LED.
 * \param led Handle of the LED to toggle.
 * \return None
 */
void led_toggle(LED *led) {
    if (!led) return;
#ifdef __ZEPHYR__
    if (!led->configured) return;
    int v = gpio_pin_get_dt(&led->gpio);
    gpio_pin_set_dt(&led->gpio, v ? 0 : 1);
#else
    int current = platform_gpio_read(led->port, led->pin);
    platform_gpio_write(led->port, led->pin, current ? 0 : 1);
#endif
}
