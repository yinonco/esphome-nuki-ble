# ESPHome Nuki BLE Component

This is a custom ESPHome component for Nuki Smart Lock BLE communication.

## Installation

Add to your ESPHome configuration:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/YOUR_USERNAME/esphome-nuki-ble.git
      ref: main
    components: [nuki_ble]
```

## Usage

```yaml
lock:
  - platform: nuki_ble
    device_name: "ESP32-Nuki"
    device_id: 12345678
    pin_code: "1234"
    pairing_mode: "app"
```
