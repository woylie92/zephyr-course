#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *led_sensor = DEVICE_DT_GET_ANY(our_led_sensor);
	struct sensor_value val;
	bool led_on = false;

	if (led_sensor == NULL) {
		LOG_ERR("No our,led-sensor node found in devicetree");
		return 0;
	}

	if (!device_is_ready(led_sensor)) {
		LOG_ERR("our_led_sensor device not ready");
		return 0;
	}

	while (1) {
		if (led_on) {
			sensor_channel_get(led_sensor, SENSOR_CHAN_ALL, &val); /* LED OFF */
			led_on = false;
			LOG_INF("LED state: OFF");
		} else {
			sensor_sample_fetch(led_sensor); /* LED ON */
			led_on = true;
			LOG_INF("LED state: ON");
		}

		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
	}

	return 0;
}