#define DT_DRV_COMPAT our_led_sensor

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <our_driver/our_driver.h>

LOG_MODULE_REGISTER(our_led_sensor, CONFIG_OUR_LED_SENSOR_LOG_LEVEL);

/* Immutable, DTS-derived settings */
struct our_driver_config {
	struct gpio_dt_spec led;
};

/* Runtime mutable state */
struct our_driver_data {
	uint8_t led_state; /* 1 = ON, 0 = OFF */
	uint32_t activation_count;  /* times the LED has been turned ON */
};

static int our_driver_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	const struct our_driver_config *cfg = dev->config;
	struct our_driver_data *data = dev->data;
	int ret;

	ARG_UNUSED(chan);

	ret = gpio_pin_set_dt(&cfg->led, 1);
	if (ret < 0) {
		LOG_ERR("Failed to turn LED on (%d)", ret);
		return ret;
	}

	data->led_state = 1;
	data->activation_count++;
	LOG_DBG("sample_fetch: LED turned ON (activation #%u)", data->activation_count);

	return 0;
}

static int our_driver_channel_get(const struct device *dev, enum sensor_channel chan,
				   struct sensor_value *val)
{
	const struct our_driver_config *cfg = dev->config;
	struct our_driver_data *data = dev->data;
	int ret;

	ARG_UNUSED(chan);

	ret = gpio_pin_set_dt(&cfg->led, 0);
	if (ret < 0) {
		LOG_ERR("Failed to turn LED off (%d)", ret);
		return ret;
	}

	data->led_state = 0;

	val->val1 = data->led_state;
	val->val2 = 0;

	LOG_DBG("channel_get: LED turned OFF");

	return 0;
}

int our_led_sensor_reset_count(const struct device *dev, uint32_t *count)   
{                                                                           
	struct our_driver_data *data;                                           

	if (dev == NULL || count == NULL) {                                     
		return -EINVAL;                                                     
	}                                                                       

	data = dev->data;                                                       

	*count = data->activation_count;                                        
	data->activation_count = 0;                                             

	LOG_DBG("Activation count read (%u) and reset", *count);                

	return 0;                                                               
}                 

int our_led_sensor_set_count(const struct device *dev, uint32_t value)
{
	if (dev == NULL) {
		return -EINVAL;
	}

	struct our_driver_data *data = dev->data;

	data->activation_count = value;
	return 0;
}

static DEVICE_API(sensor, our_driver_api) = {
	.sample_fetch = our_driver_sample_fetch,
	.channel_get = our_driver_channel_get,
};

static int our_driver_init(const struct device *dev)
{
	const struct our_driver_config *cfg = dev->config;
	int ret;

	if (!gpio_is_ready_dt(&cfg->led)) {
		LOG_ERR("LED GPIO device not ready");
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("Failed to configure LED GPIO (%d)", ret);
		return ret;
	}

	return 0;
}

#define OUR_DRIVER_DEFINE(inst)                                                  \
	static struct our_driver_data data_##inst;                               \
	static const struct our_driver_config cfg_##inst = {                     \
		.led = GPIO_DT_SPEC_INST_GET(inst, led_gpios),                   \
	};                                                                        \
	DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL,                        \
			      &data_##inst, &cfg_##inst,                          \
			      POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,          \
			      &our_driver_api)

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)