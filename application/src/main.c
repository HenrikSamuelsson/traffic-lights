#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void) {
#if DT_NODE_HAS_STATUS(DT_ALIAS(ext_green_led), okay)
  const struct gpio_dt_spec led =
      GPIO_DT_SPEC_GET(DT_ALIAS(ext_green_led), gpios); // P1.00

  printk("LED port name: %s, pin=%d, dt_flags=0x%lx\n", led.port->name, led.pin,
         (unsigned long)led.dt_flags);

  /* Configure as a plain OUTPUT, no initial level (raw test) */
  int rc = gpio_pin_configure(led.port, led.pin, GPIO_OUTPUT);
  printk("cfg rc=%d\n", rc);

  /* Blink by forcing PHYSICAL level with *_raw() (ignore active-low) */
  while (1) {
    gpio_pin_set_raw(led.port, led.pin, 0); // force LOW  -> LED ON
    k_msleep(700);
    gpio_pin_set_raw(led.port, led.pin, 1); // force HIGH -> LED OFF
    k_msleep(700);
  }
#else
#warning "Alias ext_green_led missing; check overlay filename/content"
  printk("DT alias missing\n");
  return 0;
#endif
}
