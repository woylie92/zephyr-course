#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *led_sensor = DEVICE_DT_GET_ANY(our_led_sensor);

    if (led_sensor == NULL) {
        LOG_ERR("No our,led-sensor node found in devicetree");
        return 0;
    }

    if (!device_is_ready(led_sensor)) {
        LOG_ERR("our_led_sensor device not ready");
        return 0;
    }

    LOG_INF("Blink loop disabled, use the shell: sensor fetch / read / info");

    k_sleep(K_FOREVER);

    return 0;
}