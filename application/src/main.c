#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void) {
#if DT_NODE_HAS_STATUS(DT_ALIAS(ext_green_led), okay)
  const struct gpio_dt_spec ledG =
      GPIO_DT_SPEC_GET(DT_ALIAS(ext_green_led), gpios); // P1.04 (active-low)
  const struct gpio_dt_spec ledA =
      GPIO_DT_SPEC_GET(DT_ALIAS(ext_amber_led), gpios); // P1.05 (active-low)
  const struct gpio_dt_spec ledR =
      GPIO_DT_SPEC_GET(DT_ALIAS(ext_red_led), gpios); // P1.06 (active-low)

  printk("LED G port=%s pin=%d flags=0x%lx\n", ledG.port->name, ledG.pin,
         (unsigned long)ledG.dt_flags);
  printk("LED A port=%s pin=%d flags=0x%lx\n", ledA.port->name, ledA.pin,
         (unsigned long)ledA.dt_flags);
  printk("LED R port=%s pin=%d flags=0x%lx\n", ledR.port->name, ledR.pin,
         (unsigned long)ledR.dt_flags);

  /* Configure as plain OUTPUTs (keep your raw test style) */
  int rc = 0;
  rc |= gpio_pin_configure(ledG.port, ledG.pin, GPIO_OUTPUT);
  rc |= gpio_pin_configure(ledA.port, ledA.pin, GPIO_OUTPUT);
  rc |= gpio_pin_configure(ledR.port, ledR.pin, GPIO_OUTPUT);
  printk("cfg rc=%d\n", rc);

  /* Blink by forcing PHYSICAL level with *_raw() (ignore active-low) */
  /* Green: your original pattern (1500 ms on, 700 ms off) */
  /* Amber/Red: simple different cadences so you can tell them apart */
  uint32_t g_on = 1500, g_off = 700;
  uint32_t a_on = 700, a_off = 300;
  uint32_t r_on = 500, r_off = 500;

  /* Start all OFF (HIGH for active-low wiring) */
  gpio_pin_set_raw(ledG.port, ledG.pin, 1);
  gpio_pin_set_raw(ledA.port, ledA.pin, 1);
  gpio_pin_set_raw(ledR.port, ledR.pin, 1);

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
      gpio_pin_set_raw(ledG.port, ledG.pin,
                       g_on_phase ? 0 : 1); // LOW=ON, HIGH=OFF
    }

    /* Amber */
    if ((!a_on_phase && ta >= a_off) || (a_on_phase && ta >= a_on)) {
      a_on_phase = !a_on_phase;
      ta = 0;
      gpio_pin_set_raw(ledA.port, ledA.pin, a_on_phase ? 0 : 1);
    }

    /* Red */
    if ((!r_on_phase && tr >= r_off) || (r_on_phase && tr >= r_on)) {
      r_on_phase = !r_on_phase;
      tr = 0;
      gpio_pin_set_raw(ledR.port, ledR.pin, r_on_phase ? 0 : 1);
    }
  }
#else
#warning "Alias ext_green_led missing; check overlay filename/content"
  printk("DT alias missing\n");
  return 0;
#endif
}
