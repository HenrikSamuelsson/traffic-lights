#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __ZEPHYR__  /* Zephyr */
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LED {
#ifdef __ZEPHYR__
    struct gpio_dt_spec gpio;  /* bound to a DT alias (e.g., led0) */
    bool configured;
#else
    void *port;
    uint32_t pin;
#endif
    bool active_high; /* true if logical "1" turns LED on (false for active-low boards) */
} LED;

/* Initialize an LED instance.
 * Zephyr: pass a pointer to a gpio_dt_spec as 'port' and set 'pin' to 0 (ignored).
 * Non-Zephyr: pass a platform port handle and pin number.
 */
void led_init(LED *led, void *port, uint32_t pin, bool active_high);

/** Drive LED ON */
void led_on(LED *led);

/** Drive LED OFF */
void led_off(LED *led);

/** Toggle LED state */
void led_toggle(LED *led);

#ifdef __cplusplus
}
#endif

#endif /* LED_H */
