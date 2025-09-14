import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import lock, sensor, binary_sensor, text_sensor, button, number, switch
from esphome.const import (
    CONF_ID,
    CONF_DEVICE_NAME,
    CONF_DEVICE_ID,
    CONF_PIN_CODE,
    CONF_BATTERY_LEVEL,
    CONF_BATTERY_VOLTAGE,
    CONF_RSSI,
    CONF_DOOR_SENSOR,
    CONF_KEYPAD_SENSOR,
    CONF_RING_SENSOR,
    CONF_LOCK_STATE_SENSOR,
    CONF_LAST_ACTION_SENSOR,
    CONF_UNLOCK_BUTTON,
    CONF_LOCK_BUTTON,
    CONF_UNLATCH_BUTTON,
    CONF_AUTO_UNLOCK_TIME_NUMBER,
    CONF_AUTO_UNLOCK_SWITCH,
    CONF_AUTO_LOCK_SWITCH,
)

CODEOWNERS = ["@esphome/core"]
DEPENDENCIES = ["lock"]

nuki_ble_ns = cg.esphome_ns.namespace("nuki_ble")
NukiBleComponent = nuki_ble_ns.class_("NukiBleComponent", lock.Lock, cg.Component)

CONF_DEVICE_NAME = "device_name"
CONF_DEVICE_ID = "device_id"
CONF_PIN_CODE = "pin_code"

CONFIG_SCHEMA = (
    lock.LOCK_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(NukiBleComponent),
            cv.Required(CONF_DEVICE_NAME): cv.string,
            cv.Required(CONF_DEVICE_ID): cv.uint32_t,
            cv.Optional(CONF_PIN_CODE): cv.string,
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                unit_of_measurement="%",
                accuracy_decimals=0,
                device_class="battery",
            ),
            cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement="V",
                accuracy_decimals=2,
                device_class="voltage",
            ),
            cv.Optional(CONF_RSSI): sensor.sensor_schema(
                unit_of_measurement="dBm",
                accuracy_decimals=0,
                device_class="signal_strength",
            ),
            cv.Optional(CONF_DOOR_SENSOR): binary_sensor.binary_sensor_schema(
                device_class="door",
            ),
            cv.Optional(CONF_KEYPAD_SENSOR): binary_sensor.binary_sensor_schema(
                device_class="battery",
            ),
            cv.Optional(CONF_RING_SENSOR): binary_sensor.binary_sensor_schema(
                device_class="connectivity",
            ),
            cv.Optional(CONF_LOCK_STATE_SENSOR): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_LAST_ACTION_SENSOR): text_sensor.text_sensor_schema(),
            cv.Optional(CONF_UNLOCK_BUTTON): button.button_schema(),
            cv.Optional(CONF_LOCK_BUTTON): button.button_schema(),
            cv.Optional(CONF_UNLATCH_BUTTON): button.button_schema(),
            cv.Optional(CONF_AUTO_UNLOCK_TIME_NUMBER): number.number_schema(
                min_value=0,
                max_value=255,
                step=1,
            ),
            cv.Optional(CONF_AUTO_UNLOCK_SWITCH): switch.switch_schema(),
            cv.Optional(CONF_AUTO_LOCK_SWITCH): switch.switch_schema(),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await lock.register_lock(var, config)

    cg.add(var.set_device_name(config[CONF_DEVICE_NAME]))
    cg.add(var.set_device_id(config[CONF_DEVICE_ID]))
    
    if CONF_PIN_CODE in config:
        cg.add(var.set_pin_code(config[CONF_PIN_CODE]))

    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level_sensor(sens))

    if CONF_BATTERY_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(var.set_battery_voltage_sensor(sens))

    if CONF_RSSI in config:
        sens = await sensor.new_sensor(config[CONF_RSSI])
        cg.add(var.set_rssi_sensor(sens))

    if CONF_DOOR_SENSOR in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_DOOR_SENSOR])
        cg.add(var.set_door_sensor(sens))

    if CONF_KEYPAD_SENSOR in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_KEYPAD_SENSOR])
        cg.add(var.set_keypad_sensor(sens))

    if CONF_RING_SENSOR in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_RING_SENSOR])
        cg.add(var.set_ring_sensor(sens))

    if CONF_LOCK_STATE_SENSOR in config:
        sens = await text_sensor.new_text_sensor(config[CONF_LOCK_STATE_SENSOR])
        cg.add(var.set_lock_state_sensor(sens))

    if CONF_LAST_ACTION_SENSOR in config:
        sens = await text_sensor.new_text_sensor(config[CONF_LAST_ACTION_SENSOR])
        cg.add(var.set_last_action_sensor(sens))

    if CONF_UNLOCK_BUTTON in config:
        btn = await button.new_button(config[CONF_UNLOCK_BUTTON])
        cg.add(var.set_unlock_button(btn))

    if CONF_LOCK_BUTTON in config:
        btn = await button.new_button(config[CONF_LOCK_BUTTON])
        cg.add(var.set_lock_button(btn))

    if CONF_UNLATCH_BUTTON in config:
        btn = await button.new_button(config[CONF_UNLATCH_BUTTON])
        cg.add(var.set_unlatch_button(btn))

    if CONF_AUTO_UNLOCK_TIME_NUMBER in config:
        num = await number.new_number(config[CONF_AUTO_UNLOCK_TIME_NUMBER])
        cg.add(var.set_auto_unlock_time_number(num))

    if CONF_AUTO_UNLOCK_SWITCH in config:
        sw = await switch.new_switch(config[CONF_AUTO_UNLOCK_SWITCH])
        cg.add(var.set_auto_unlock_switch(sw))

    if CONF_AUTO_LOCK_SWITCH in config:
        sw = await switch.new_switch(config[CONF_AUTO_LOCK_SWITCH])
        cg.add(var.set_auto_lock_switch(sw))
