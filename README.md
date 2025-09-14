# ESPHome Nuki BLE Component

A custom ESPHome component for direct BLE communication with Nuki Smart Locks without requiring a Nuki Bridge.

## Features

- Direct BLE communication with Nuki Smart Lock
- Real-time lock state monitoring
- Battery level monitoring
- Door sensor status
- Keypad battery status
- Ring to open functionality
- Lock/unlock/unlatch controls
- Auto-lock and auto-unlock settings
- Last action tracking

## Hardware Requirements

- ESP32-S3-WROOM-1-N8 (or compatible ESP32 board)
- Nuki Smart Lock v2 or v3

## Installation

### Method 1: External Component (Recommended)

Add this to your ESPHome configuration:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/YOUR_USERNAME/esphome-nuki-ble.git
      ref: main
    components: [nuki_ble]
```

### Method 2: Manual Installation

1. Clone this repository
2. Copy the `nuki_ble` folder to your ESPHome custom components directory
3. Restart ESPHome

## Configuration

### Basic Configuration

```yaml
lock:
  - platform: nuki_ble
    device_name: "ESP32-Nuki"
    device_id: 12345678  # Your Nuki device ID
    pin_code: "1234"     # Your Nuki pin code (optional)
    pairing_mode: "app"  # Pair as app (won't affect your Nuki Bridge)
```

### Pairing Modes

- **`app`** (default): Pairs as a Nuki app - **recommended** if you have a Nuki Bridge
- **`bridge`**: Pairs as a Nuki Bridge
- **`fob`**: Pairs as a Nuki Fob

**Important**: If you already have a Nuki Bridge, use `pairing_mode: "app"` to avoid conflicts.

### Full Configuration Example

```yaml
esphome:
  name: nuki-ble-lock
  friendly_name: Nuki BLE Lock

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

external_components:
  - source:
      type: git
      url: https://github.com/YOUR_USERNAME/esphome-nuki-ble.git
      ref: main
    components: [nuki_ble]

lock:
  - platform: nuki_ble
    id: nuki_lock
    device_name: "ESP32-Nuki"
    device_id: 12345678
    pin_code: "1234"
    pairing_mode: "app"  # Won't affect your Nuki Bridge
    
    # Sensors
    battery_level:
      name: "Nuki Battery Level"
    battery_voltage:
      name: "Nuki Battery Voltage"
    rssi:
      name: "Nuki RSSI"
    
    # Binary sensors
    door_sensor:
      name: "Nuki Door"
    keypad_sensor:
      name: "Nuki Keypad Battery"
    ring_sensor:
      name: "Nuki Ring to Open"
    
    # Text sensors
    lock_state_sensor:
      name: "Nuki Lock State"
    last_action_sensor:
      name: "Nuki Last Action"
    
    # Buttons
    unlock_button:
      name: "Nuki Unlock"
    lock_button:
      name: "Nuki Lock"
    unlatch_button:
      name: "Nuki Unlatch"
    
    # Numbers
    auto_unlock_time_number:
      name: "Nuki Auto Unlock Time"
      min_value: 0
      max_value: 255
      step: 1
    
    # Switches
    auto_unlock_switch:
      name: "Nuki Auto Unlock"
    auto_lock_switch:
      name: "Nuki Auto Lock"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

api:
  encryption:
    key: !secret api_key

ota:
  password: !secret ota_password
```

## Getting Your Nuki Device ID

1. Open the Nuki app
2. Go to your lock settings
3. Look for the device ID in the lock information
4. Use this ID in your ESPHome configuration

## Pairing Process

1. Flash the ESPHome configuration to your ESP32
2. Put your Nuki lock in pairing mode (hold button for 10 seconds until LED ring lights up)
3. The ESP32 will automatically detect and pair with the lock
4. Credentials are stored persistently, so no re-pairing is needed after restarts

## Available Sensors

- **Battery Level**: Lock battery percentage
- **Battery Voltage**: Lock battery voltage
- **RSSI**: Signal strength
- **Door Sensor**: Door open/closed status
- **Keypad Battery**: Keypad battery status
- **Ring Sensor**: Ring to open status
- **Lock State**: Current lock state (Locked/Unlocked/Unlatched/etc.)
- **Last Action**: Last performed action

## Available Controls

- **Lock Button**: Lock the door
- **Unlock Button**: Unlock the door
- **Unlatch Button**: Unlatch the door
- **Auto Unlock Switch**: Enable/disable auto unlock
- **Auto Lock Switch**: Enable/disable auto lock
- **Auto Unlock Time**: Set auto unlock timeout

## Troubleshooting

### No Nuki Found
- Ensure your Nuki lock is in pairing mode
- Check that the device ID is correct
- Verify BLE is working on your ESP32

### Connection Issues
- Check RSSI sensor for signal strength
- Ensure lock battery is not critically low
- Try restarting both devices

### Pin Code Required
- Some operations require a pin code
- Set the pin_code in your configuration
- Pin code is stored securely on the ESP32

## Dependencies

This component uses the [NukiBleEsp32](https://github.com/I-Connect/NukiBleEsp32) library for BLE communication.

## License

MIT License - see the original library for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## Support

- Original library: https://github.com/I-Connect/NukiBleEsp32
- ESPHome documentation: https://esphome.io/
- Nuki BLE API documentation: https://developer.nuki.io/page/nuki-smart-lock-api-2/2/