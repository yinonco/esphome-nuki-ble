#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/lock/lock.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/button/button.h"
#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"

#include "NukiBle.h"
#include "BleScanner.h"

namespace esphome {
namespace nuki_ble {

class NukiBleComponent : public lock::Lock, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_device_name(const std::string &device_name) { device_name_ = device_name; }
  void set_device_id(uint32_t device_id) { device_id_ = device_id; }
  void set_pin_code(const std::string &pin_code) { pin_code_ = pin_code; }
  void set_pairing_mode(const std::string &pairing_mode) { pairing_mode_ = pairing_mode; }
  
  // Sensors
  void set_battery_level_sensor(sensor::Sensor *battery_level_sensor) { battery_level_sensor_ = battery_level_sensor; }
  void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) { battery_voltage_sensor_ = battery_voltage_sensor; }
  void set_rssi_sensor(sensor::Sensor *rssi_sensor) { rssi_sensor_ = rssi_sensor; }
  
  // Binary sensors
  void set_door_sensor(binary_sensor::BinarySensor *door_sensor) { door_sensor_ = door_sensor; }
  void set_keypad_sensor(binary_sensor::BinarySensor *keypad_sensor) { keypad_sensor_ = keypad_sensor; }
  void set_ring_sensor(binary_sensor::BinarySensor *ring_sensor) { ring_sensor_ = ring_sensor; }
  
  // Text sensors
  void set_lock_state_sensor(text_sensor::TextSensor *lock_state_sensor) { lock_state_sensor_ = lock_state_sensor; }
  void set_last_action_sensor(text_sensor::TextSensor *last_action_sensor) { last_action_sensor_ = last_action_sensor; }
  
  // Buttons
  void set_unlock_button(button::Button *unlock_button) { unlock_button_ = unlock_button; }
  void set_lock_button(button::Button *lock_button) { lock_button_ = lock_button; }
  void set_unlatch_button(button::Button *unlatch_button) { unlatch_button_ = unlatch_button; }
  
  // Numbers
  void set_auto_unlock_time_number(number::Number *auto_unlock_time_number) { auto_unlock_time_number_ = auto_unlock_time_number; }
  
  // Switches
  void set_auto_unlock_switch(switch_::Switch *auto_unlock_switch) { auto_unlock_switch_ = auto_unlock_switch; }
  void set_auto_lock_switch(switch_::Switch *auto_lock_switch) { auto_lock_switch_ = auto_lock_switch; }

 protected:
  void control(const lock::LockCall &call) override;
  
  void update_lock_state();
  void update_sensors();
  void handle_nuki_event(Nuki::EventType event_type);
  
  std::string device_name_;
  uint32_t device_id_;
  std::string pin_code_;
  std::string pairing_mode_;
  
  Nuki::NukiLock *nuki_lock_;
  BleScanner::Scanner *scanner_;
  
  // Sensors
  sensor::Sensor *battery_level_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *rssi_sensor_{nullptr};
  
  // Binary sensors
  binary_sensor::BinarySensor *door_sensor_{nullptr};
  binary_sensor::BinarySensor *keypad_sensor_{nullptr};
  binary_sensor::BinarySensor *ring_sensor_{nullptr};
  
  // Text sensors
  text_sensor::TextSensor *lock_state_sensor_{nullptr};
  text_sensor::TextSensor *last_action_sensor_{nullptr};
  
  // Buttons
  button::Button *unlock_button_{nullptr};
  button::Button *lock_button_{nullptr};
  button::Button *unlatch_button_{nullptr};
  
  // Numbers
  number::Number *auto_unlock_time_number_{nullptr};
  
  // Switches
  switch_::Switch *auto_unlock_switch_{nullptr};
  switch_::Switch *auto_lock_switch_{nullptr};
  
  bool initialized_{false};
  uint32_t last_update_{0};
  uint32_t update_interval_{5000}; // 5 seconds
};

class NukiBleEventHandler : public Nuki::SmartlockEventHandler {
 public:
  explicit NukiBleEventHandler(NukiBleComponent *component) : component_(component) {}
  
  void notify(Nuki::EventType eventType) override {
    if (component_) {
      component_->handle_nuki_event(eventType);
    }
  }
  
 private:
  NukiBleComponent *component_;
};

}  // namespace nuki_ble
}  // namespace esphome
