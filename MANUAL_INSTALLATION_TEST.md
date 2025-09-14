# Manual Installation Test

## Quick Test - Manual Installation

Since the external component is still not working, let's test with manual installation first:

### Step 1: Manual Installation

1. **Copy the component files** to ESPHome's custom components directory:
   ```bash
   # For Home Assistant ESPHome addon:
   mkdir -p /usr/share/hassio/addons/core_esphome/custom_components
   cp -r nuki_ble /usr/share/hassio/addons/core_esphome/custom_components/
   ```

2. **Use this test configuration** (without external_components):
   ```yaml
   esphome:
     name: nuki-test
     friendly_name: Nuki Test

   esp32:
     board: esp32-s3-devkitc-1
     framework:
       type: arduino

   # NO external_components section for manual installation

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

### Step 2: If Manual Installation Works

If manual installation works, then the issue is with the external component configuration. The problem might be:

1. **GitHub repository structure** - Make sure the `nuki_ble/` folder is at the repository root
2. **GitHub URL** - Make sure the URL is correct and accessible
3. **Component name** - Make sure it matches exactly

### Step 3: Debug External Component

If you want to use external components, try this debug version:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/YOUR_USERNAME/esphome-nuki-ble.git
      ref: main
    components: [nuki_ble]
    refresh: 0s  # Force refresh
```

## Alternative: Use Local Path

If you're testing locally, try:

```yaml
external_components:
  - source:
      type: local
      path: ./nuki_ble
    components: [nuki_ble]
```

## Next Steps

1. **Try manual installation first** to verify the component works
2. **If it works**, then debug the external component configuration
3. **If it doesn't work**, then there's an issue with the component code itself

Let me know which approach works!
