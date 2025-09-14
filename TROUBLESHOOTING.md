# Troubleshooting: Platform not found: 'lock.nuki_ble'

## Quick Fixes

### Option 1: Manual Installation (Recommended for testing)

1. **Copy the component files manually**:
   ```bash
   # For Home Assistant ESPHome addon:
   mkdir -p /usr/share/hassio/addons/core_esphome/custom_components
   cp -r esphome/components/nuki_ble /usr/share/hassio/addons/core_esphome/custom_components/
   ```

2. **Remove the external_components section** from your YAML:
   ```yaml
   # Comment out or remove this section:
   # external_components:
   #   - source:
   #       type: git
   #       url: https://github.com/YOUR_USERNAME/esphome-nuki-ble.git
   #       ref: main
   #     components: [nuki_ble]
   ```

3. **Restart ESPHome** and try again

### Option 2: Fix the GitHub URL

If you've uploaded to GitHub, update the URL in your YAML:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/YOUR_ACTUAL_USERNAME/esphome-nuki-ble.git
      ref: main
    components: [nuki_ble]
```

### Option 3: Use Local Path

If you're testing locally:

```yaml
external_components:
  - source:
      type: local
      path: ./esphome/components/nuki_ble
    components: [nuki_ble]
```

## Common Issues

1. **Wrong URL**: Make sure the GitHub URL is correct
2. **Component not uploaded**: Ensure all files are in the GitHub repository
3. **Missing files**: Check that all 5 files are present:
   - `__init__.py`
   - `manifest.yaml`
   - `nuki_ble.h`
   - `nuki_ble.cpp`
   - `nuki_ble.py`

## Test Configuration

Use this minimal configuration to test:

```yaml
esphome:
  name: nuki-test
  friendly_name: Nuki Test

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

# Remove external_components section for manual installation
# external_components:
#   - source:
#       type: git
#       url: https://github.com/YOUR_USERNAME/esphome-nuki-ble.git
#       ref: main
#     components: [nuki_ble]

lock:
  - platform: nuki_ble
    device_name: "ESP32-Nuki"
    device_id: 12345678
    pin_code: "1234"
    pairing_mode: "app"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

api:
  encryption:
    key: !secret api_key

ota:
  password: !secret ota_password
```
