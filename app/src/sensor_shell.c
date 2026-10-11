#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

static const struct device *get_dev(const struct shell *sh)
{
	const struct device *dev = DEVICE_DT_GET_ANY(our_led_sensor);

	if (dev == NULL) {
		shell_error(sh, "No our_led_sensor device found");
		return NULL;
	}
	if (!device_is_ready(dev)) {
		shell_error(sh, "Device %s is not ready", dev->name);
		return NULL;
	}
	return dev;
}

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = get_dev(sh);

	if (dev == NULL) {
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(dev);

	if (ret < 0) {
		shell_error(sh, "sensor_sample_fetch failed: %d", ret);
		return ret;
	}
	shell_print(sh, "Sample fetched");
	return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = get_dev(sh);

	if (dev == NULL) {
		return -ENODEV;
	}

	struct sensor_value val;
	int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);

	if (ret < 0) {
		shell_error(sh, "sensor_channel_get failed: %d", ret);
		return ret;
	}
	shell_print(sh, "val1=%d val2=%d", val.val1, val.val2);
	return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = DEVICE_DT_GET_ANY(our_led_sensor);

	if (dev == NULL) {
		shell_error(sh, "No our_led_sensor device found");
		return -ENODEV;
	}
	shell_print(sh, "Device: %s", dev->name);
	shell_print(sh, "Ready:  %s", device_is_ready(dev) ? "yes" : "no");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
	SHELL_CMD(fetch, NULL, "Fetch a sample (LED on)", cmd_sensor_fetch),
	SHELL_CMD(read, NULL, "Read channel value (LED off)", cmd_sensor_read),
	SHELL_CMD(info, NULL, "Show device name and ready state", cmd_sensor_info),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED sensor commands", NULL);