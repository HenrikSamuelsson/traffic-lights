#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void) {
#if DT_NODE_HAS_STATUS(DT_ALIAS(ext_green_led), okay)
  const struct gpio_dt_spec ledTG =
      GPIO_DT_SPEC_GET(DT_ALIAS(ext_green_led), gpios);
  const struct gpio_dt_spec ledTA =
      GPIO_DT_SPEC_GET(DT_ALIAS(ext_amber_led), gpios);
  const struct gpio_dt_spec ledTR =
      GPIO_DT_SPEC_GET(DT_ALIAS(ext_red_led), gpios);
  const struct gpio_dt_spec ledPW =
      GPIO_DT_SPEC_GET(DT_ALIAS(pedestrian_white_led), gpios);
  const struct gpio_dt_spec ledPG =
      GPIO_DT_SPEC_GET(DT_ALIAS(pedestrian_green_led), gpios);
  const struct gpio_dt_spec ledPR =
      GPIO_DT_SPEC_GET(DT_ALIAS(pedestrian_red_led), gpios);

  /* Configure as plain OUTPUTs. */
  int rc = 0;
  rc |= gpio_pin_configure(ledTG.port, ledTG.pin, GPIO_OUTPUT);
  rc |= gpio_pin_configure(ledTA.port, ledTA.pin, GPIO_OUTPUT);
  rc |= gpio_pin_configure(ledTR.port, ledTR.pin, GPIO_OUTPUT);
  rc |= gpio_pin_configure(ledPW.port, ledPW.pin, GPIO_OUTPUT);
  rc |= gpio_pin_configure(ledPG.port, ledPG.pin, GPIO_OUTPUT);
  rc |= gpio_pin_configure(ledPG.port, ledPR.pin, GPIO_OUTPUT);

  /* Blink by forcing PHYSICAL level with *_raw() (ignore active-low) */
  /* Green: your original pattern (1500 ms on, 700 ms off) */
  /* Amber/Red: simple different cadences so you can tell them apart */
  uint32_t g_on = 1500, g_off = 700;
  uint32_t a_on = 700, a_off = 300;
  uint32_t r_on = 500, r_off = 500;

  /* Start all OFF (HIGH for active-low wiring) */
  gpio_pin_set_raw(ledTG.port, ledTG.pin, 1);
  gpio_pin_set_raw(ledTA.port, ledTA.pin, 1);
  gpio_pin_set_raw(ledTR.port, ledTR.pin, 1);
  gpio_pin_set_raw(ledPW.port, ledPW.pin, 1);
  gpio_pin_set_raw(ledPG.port, ledPG.pin, 1);
  gpio_pin_set_raw(ledPR.port, ledPR.pin, 1);

  uint32_t tg = 0, ta = 0, tr = 0;
  bool g_on_phase = false, a_on_phase = false, r_on_phase = false;

  while (1) {
    k_msleep(50);
    tg += 50;
    ta += 50;
    tr += 50;

    /* Green */
    if ((!g_on_phase && tg >= g_off) || (g_on_phase && tg >= g_on)) {
      g_on_phase = !g_on_phase;
      tg = 0;
      gpio_pin_set_raw(ledTG.port, ledTG.pin, g_on_phase ? 0 : 1);
      gpio_pin_set_raw(ledTG.port, ledPG.pin, g_on_phase ? 0 : 1);
    }

    /* Test run amber and pedestrian white. */
    if ((!a_on_phase && ta >= a_off) || (a_on_phase && ta >= a_on)) {
      a_on_phase = !a_on_phase;
      ta = 0;
      gpio_pin_set_raw(ledTA.port, ledTA.pin, a_on_phase ? 0 : 1);
      gpio_pin_set_raw(ledPW.port, ledPW.pin, a_on_phase ? 0 : 1);
    }

    /* Test red LEDS. */
    if ((!r_on_phase && tr >= r_off) || (r_on_phase && tr >= r_on)) {
      r_on_phase = !r_on_phase;
      tr = 0;
      gpio_pin_set_raw(ledTR.port, ledTR.pin, r_on_phase ? 0 : 1);
      gpio_pin_set_raw(ledPR.port, ledPR.pin, r_on_phase ? 0 : 1);
    }
  }
#else
#warning "Alias ext_green_led missing; check overlay filename/content"
  printk("DT alias missing\n");
  return 0;
#endif
}
