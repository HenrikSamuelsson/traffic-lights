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

void led_on(LED *led) {
    if (!led) return;
#ifdef __ZEPHYR__
    if (!led->configured) return;
    gpio_pin_set_dt(&led->gpio, led->active_high ? 1 : 0);
#else
    platform_gpio_write(led->port, led->pin, led->active_high ? 1 : 0);
#endif
}

void led_off(LED *led) {
    if (!led) return;
#ifdef __ZEPHYR__
    if (!led->configured) return;
    gpio_pin_set_dt(&led->gpio, led->active_high ? 0 : 1);
#else
    platform_gpio_write(led->port, led->pin, led->active_high ? 0 : 1);
#endif
}

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
