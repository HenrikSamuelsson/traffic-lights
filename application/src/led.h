/** 
 * \file led.h
 * \brief Public LED abstraction for platform-agnostic control.
 * \details Doxygen documentation restored after refactor to make the API discoverable.
 */

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
void led_init(LED *led, void *port, uint32_t pin, bool active_high);

/** Drive LED ON */
/**
 * \brief Turn the LED on according to its active level.
 * \param led Handle of the LED to drive.
 * \return None
 */
void led_on(LED *led);

/** Drive LED OFF */
/**
 * \brief Turn the LED off according to its active level.
 * \param led Handle of the LED to drive.
 * \return None
 */
void led_off(LED *led);

/** Toggle LED state */
/**
 * \brief Toggle the current state of the LED.
 * \param led Handle of the LED to toggle.
 * \return None
 */
void led_toggle(LED *led);

#ifdef __cplusplus
}
#endif

#endif /* LED_H */
