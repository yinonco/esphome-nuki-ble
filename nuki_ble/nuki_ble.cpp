#include "nuki_ble.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nuki_ble {

static const char *const TAG = "nuki_ble";

void NukiBleComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Nuki BLE...");
  
  // Initialize scanner
  scanner_ = new BleScanner::Scanner();
  scanner_->initialize();
  
  // Initialize Nuki lock
  nuki_lock_ = new Nuki::NukiLock(device_name_.c_str(), device_id_);
  nuki_lock_->registerBleScanner(scanner_);
  nuki_lock_->initialize();
  
  // Set up event handler
  static NukiBleEventHandler event_handler(this);
  nuki_lock_->setEventHandler(&event_handler);
  
  // Save pin code if provided
  if (!pin_code_.empty()) {
    nuki_lock_->savePincode(pin_code_.c_str());
  }
  
  initialized_ = true;
  ESP_LOGCONFIG(TAG, "Nuki BLE setup complete");
}

void NukiBleComponent::loop() {
  if (!initialized_) {
    return;
  }
  
  // Update scanner
  scanner_->update();
  
  // Update sensors periodically
  uint32_t now = millis();
  if (now - last_update_ > update_interval_) {
    update_sensors();
    last_update_ = now;
  }
  
  // Small delay to prevent overwhelming the system
  delay(10);
}

void NukiBleComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Nuki BLE Lock:");
  ESP_LOGCONFIG(TAG, "  Device Name: %s", device_name_.c_str());
  ESP_LOGCONFIG(TAG, "  Device ID: %u", device_id_);
  ESP_LOGCONFIG(TAG, "  Pin Code: %s", pin_code_.empty() ? "Not set" : "Set");
  ESP_LOGCONFIG(TAG, "  Pairing Mode: %s", pairing_mode_.c_str());
  
  LOG_LOCK("", "Nuki BLE Lock", this);
  LOG_SENSOR("  ", "Battery Level", battery_level_sensor_);
  LOG_SENSOR("  ", "Battery Voltage", battery_voltage_sensor_);
  LOG_SENSOR("  ", "RSSI", rssi_sensor_);
  LOG_BINARY_SENSOR("  ", "Door Sensor", door_sensor_);
  LOG_BINARY_SENSOR("  ", "Keypad Sensor", keypad_sensor_);
  LOG_BINARY_SENSOR("  ", "Ring Sensor", ring_sensor_);
  LOG_TEXT_SENSOR("  ", "Lock State", lock_state_sensor_);
  LOG_TEXT_SENSOR("  ", "Last Action", last_action_sensor_);
  LOG_BUTTON("  ", "Unlock Button", unlock_button_);
  LOG_BUTTON("  ", "Lock Button", lock_button_);
  LOG_BUTTON("  ", "Unlatch Button", unlatch_button_);
  LOG_NUMBER("  ", "Auto Unlock Time", auto_unlock_time_number_);
  LOG_SWITCH("  ", "Auto Unlock Switch", auto_unlock_switch_);
  LOG_SWITCH("  ", "Auto Lock Switch", auto_lock_switch_);
}

void NukiBleComponent::control(const lock::LockCall &call) {
  if (!initialized_) {
    ESP_LOGW(TAG, "Component not initialized");
    return;
  }
  
  if (call.get_state().has_value()) {
    auto state = *call.get_state();
    
    switch (state) {
      case lock::LOCK_STATE_LOCKED:
        ESP_LOGI(TAG, "Locking Nuki");
        nuki_lock_->lockAction(Nuki::LockAction::Lock);
        break;
        
      case lock::LOCK_STATE_UNLOCKED:
        ESP_LOGI(TAG, "Unlocking Nuki");
        nuki_lock_->lockAction(Nuki::LockAction::Unlock);
        break;
        
      case lock::LOCK_STATE_UNLOCKING:
        ESP_LOGI(TAG, "Unlatching Nuki");
        nuki_lock_->lockAction(Nuki::LockAction::Unlatch);
        break;
        
      default:
        ESP_LOGW(TAG, "Unknown lock state: %d", state);
        break;
    }
  }
}

void NukiBleComponent::update_lock_state() {
  if (!initialized_) {
    return;
  }
  
  Nuki::KeyTurnerState keyTurnerState;
  if (nuki_lock_->requestKeyTurnerState(&keyTurnerState) == Nuki::CmdResult::Success) {
    
    // Update lock state
    lock::LockState state;
    switch (keyTurnerState.lockState) {
      case Nuki::LockState::Uncalibrated:
        state = lock::LOCK_STATE_JAMMED;
        break;
      case Nuki::LockState::Locked:
        state = lock::LOCK_STATE_LOCKED;
        break;
      case Nuki::LockState::Unlocked:
        state = lock::LOCK_STATE_UNLOCKED;
        break;
      case Nuki::LockState::Unlatched:
        state = lock::LOCK_STATE_UNLOCKING;
        break;
      case Nuki::LockState::LockNGo:
        state = lock::LOCK_STATE_UNLOCKED;
        break;
      case Nuki::LockState::Unlatching:
        state = lock::LOCK_STATE_UNLOCKING;
        break;
      default:
        state = lock::LOCK_STATE_JAMMED;
        break;
    }
    this->publish_state(state);
    
    // Update sensors
    if (battery_level_sensor_) {
      battery_level_sensor_->publish_state(keyTurnerState.batteryCritical ? 0 : 100);
    }
    
    if (battery_voltage_sensor_) {
      battery_voltage_sensor_->publish_state(keyTurnerState.batteryVoltage / 1000.0f);
    }
    
    if (door_sensor_) {
      door_sensor_->publish_state(keyTurnerState.doorState == Nuki::DoorState::DoorClosed);
    }
    
    if (keypad_sensor_) {
      keypad_sensor_->publish_state(keyTurnerState.keypadBatteryCritical);
    }
    
    if (ring_sensor_) {
      ring_sensor_->publish_state(keyTurnerState.ringToOpen);
    }
    
    // Update text sensors
    if (lock_state_sensor_) {
      const char* state_text = "Unknown";
      switch (keyTurnerState.lockState) {
        case Nuki::LockState::Uncalibrated:
          state_text = "Uncalibrated";
          break;
        case Nuki::LockState::Locked:
          state_text = "Locked";
          break;
        case Nuki::LockState::Unlocked:
          state_text = "Unlocked";
          break;
        case Nuki::LockState::Unlatched:
          state_text = "Unlatched";
          break;
        case Nuki::LockState::LockNGo:
          state_text = "LockNGo";
          break;
        case Nuki::LockState::Unlatching:
          state_text = "Unlatching";
          break;
      }
      lock_state_sensor_->publish_state(state_text);
    }
    
    if (last_action_sensor_) {
      const char* action_text = "Unknown";
      switch (keyTurnerState.lastLockAction) {
        case Nuki::LockAction::Unlock:
          action_text = "Unlock";
          break;
        case Nuki::LockAction::Lock:
          action_text = "Lock";
          break;
        case Nuki::LockAction::Unlatch:
          action_text = "Unlatch";
          break;
        case Nuki::LockAction::LockNGo:
          action_text = "LockNGo";
          break;
        case Nuki::LockAction::LockNGoWithUnlatch:
          action_text = "LockNGoWithUnlatch";
          break;
        case Nuki::LockAction::FullLock:
          action_text = "FullLock";
          break;
        case Nuki::LockAction::FobAction1:
          action_text = "FobAction1";
          break;
        case Nuki::LockAction::FobAction2:
          action_text = "FobAction2";
          break;
        case Nuki::LockAction::FobAction3:
          action_text = "FobAction3";
          break;
      }
      last_action_sensor_->publish_state(action_text);
    }
  }
}

void NukiBleComponent::update_sensors() {
  update_lock_state();
}

void NukiBleComponent::handle_nuki_event(Nuki::EventType event_type) {
  ESP_LOGD(TAG, "Nuki event received: %d", event_type);
  
  switch (event_type) {
    case Nuki::EventType::KeyTurnerStatusUpdated:
      update_lock_state();
      break;
    default:
      break;
  }
}

}  // namespace nuki_ble
}  // namespace esphome