#ifndef OUR_DRIVERS_OUR_DRIVER_H_
#define OUR_DRIVERS_OUR_DRIVER_H_

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * our,led-sensor
 * ---------------
 * sensor_sample_fetch(dev);                      // turns the LED ON
 * sensor_channel_get(dev, SENSOR_CHAN_ALL, &v);  // turns the LED OFF
 *
 * Custom extension API (Task 2)
 * ------------------------------
 * our_led_sensor_reset_count() is specific to this driver and is not
 * part of the standard Sensor API vtable, so it is called directly by
 * name instead of through the sensor_* functions.
 */

/**
 * @brief Read how many times the LED has been turned on since the
 *        last reset, then reset that counter to zero.
 *
 * @param dev   The our,led-sensor device instance.
 * @param count Out-parameter. On success it receives the count as it
 *              was before the reset.
 *
 * @retval 0       on success.
 * @retval -EINVAL if dev or count is NULL.
 */
int our_led_sensor_reset_count(const struct device *dev, uint32_t *count);

#ifdef __cplusplus
}
#endif

#endif /* OUR_DRIVERS_OUR_DRIVER_H_ */