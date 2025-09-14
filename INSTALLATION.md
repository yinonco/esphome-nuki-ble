# Nuki BLE ESPHome Component - Installation Guide

## Quick Start

### For Home Assistant with ESPHome Addon

1. **Access ESPHome addon**:
   - Go to Home Assistant → Supervisor → Add-ons
   - Open ESPHome addon
   - Click "Open Web UI"

2. **Copy the component files** to ESPHome addon's custom components directory:
   ```bash
   # On your Raspberry Pi, copy to:
   cp -r esphome/components/nuki_ble /usr/share/hassio/addons/core_esphome/custom_components/
   
   # Or via Home Assistant file editor:
   # Navigate to /usr/share/hassio/addons/core_esphome/custom_components/
   # Create folder 'nuki_ble' and copy all files from esphome/components/nuki_ble/
   ```

3. **Create your configuration**:
   - In ESPHome Web UI, click "New Device"
   - Choose "Continue" (don't use the wizard)
   - Copy the content from `nuki_ble_example.yaml`
   - Edit the device_name, device_id, and pin_code values

4. **Set up secrets** (optional):
   - Create a `secrets.yaml` file in ESPHome addon directory
   - Copy content from `secrets.yaml.template` and fill in your values

### For Standalone ESPHome Installation

1. **Copy the component files** to your ESPHome custom components directory:
   ```bash
   cp -r esphome/components/nuki_ble /path/to/esphome/custom_components/
   ```

2. **Create your secrets file**:
   ```bash
   cp secrets.yaml.template secrets.yaml
   # Edit secrets.yaml with your actual values
   ```

3. **Use the example configuration**:
   ```bash
   cp nuki_ble_example.yaml your_nuki_config.yaml
   # Edit your_nuki_config.yaml with your settings
   ```

4. **Compile and upload**:
   ```bash
   esphome run your_nuki_config.yaml
   ```

## Home Assistant ESPHome Addon - Detailed Steps

### Method 1: Using Home Assistant File Editor (Recommended)

1. **Install File Editor addon** (if not already installed):
   - Go to Home Assistant → Supervisor → Add-ons
   - Install "File Editor" addon
   - Start the addon

2. **Create the custom components directory**:
   - Open File Editor
   - Navigate to `/usr/share/hassio/addons/core_esphome/`
   - Create folder `custom_components` if it doesn't exist
   - Create folder `nuki_ble` inside `custom_components`

3. **Copy component files**:
   - Copy all files from `esphome/components/nuki_ble/` to `/usr/share/hassio/addons/core_esphome/custom_components/nuki_ble/`
   - You should have these files in the nuki_ble folder:
     - `__init__.py`
     - `manifest.yaml`
     - `nuki_ble.h`
     - `nuki_ble.cpp`
     - `nuki_ble.py`

### Method 2: Using SSH/Terminal

1. **Enable SSH addon** in Home Assistant
2. **Connect via SSH** to your Raspberry Pi
3. **Run these commands**:
   ```bash
   # Create custom components directory
   mkdir -p /usr/share/hassio/addons/core_esphome/custom_components
   
   # Copy the component files (adjust path to your files)
   cp -r /path/to/your/ESPHOMENUKI/esphome/components/nuki_ble /usr/share/hassio/addons/core_esphome/custom_components/
   ```

### Method 3: Using Samba Share (if enabled)

1. **Access your Home Assistant files** via network share
2. **Navigate to** `/usr/share/hassio/addons/core_esphome/custom_components/`
3. **Copy the nuki_ble folder** with all its files

### Creating Your Nuki Device Configuration

1. **Open ESPHome Web UI**:
   - Go to Home Assistant → Supervisor → Add-ons → ESPHome
   - Click "Open Web UI"

2. **Create new device**:
   - Click "New Device"
   - Choose "Continue" (skip the wizard)
   - Give your device a name (e.g., "nuki-ble-lock")

3. **Add the configuration**:
   - Copy the content from `nuki_ble_example.yaml`
   - Replace the device_name, device_id, and pin_code with your values
   - Set `pairing_mode: "app"` to avoid conflicts with your Nuki Bridge
   - Update WiFi credentials if using secrets

4. **Save and compile**:
   - Click "Save"
   - Click "Install" to compile and upload to your ESP32

## Important Notes

### ESP32-S3-WROOM-1-N8 Specific Settings

The configuration is already optimized for your ESP32-S3-WROOM-1-N8 board:
- Board: `esp32-s3-devkitc-1` (compatible with your board)
- Framework: Arduino (recommended for BLE stability)
- Upload speed: 921600 (optimal for S3)

### Getting Your Nuki Device ID

1. Open the Nuki app on your phone
2. Select your lock
3. Go to Settings → Lock Settings
4. Look for "Device ID" or "Lock ID"
5. Use this number in your configuration

### Pairing Modes

The component supports different pairing modes:

- **`app`** (default): Pairs as a Nuki app - **recommended** if you have a Nuki Bridge
- **`bridge`**: Pairs as a Nuki Bridge
- **`fob`**: Pairs as a Nuki Fob

**Important**: If you already have a Nuki Bridge, use `pairing_mode: "app"` to avoid conflicts. This allows both your bridge and ESP32 to work simultaneously.

### Pairing Process

1. **First time setup**:
   - Put your Nuki lock in pairing mode (hold button 10 seconds until LED ring lights up)
   - Flash the ESPHome configuration
   - The ESP32 will automatically detect and pair with the lock

2. **Subsequent uses**:
   - No re-pairing needed - credentials are stored persistently
   - Just power on and the lock will connect automatically

### Pin Code Setup

If you have a pin code set on your Nuki lock:
1. Add `pin_code: "your-pin-code"` to your configuration
2. This is required for most lock operations
3. The pin code is stored securely on the ESP32

## Troubleshooting

### Common Issues

1. **"No nuki in pairing mode found"**
   - Ensure lock is in pairing mode (LED ring should be lit)
   - Check device ID is correct
   - Verify BLE is working

2. **Connection drops**
   - Check battery level (should be > 20%)
   - Monitor RSSI sensor for signal strength
   - Ensure lock is within range (typically 10-20 meters)

3. **Lock operations fail**
   - Verify pin code is set correctly
   - Check lock battery level
   - Ensure lock is not jammed

### Debug Mode

To enable debug logging, add these build flags to your configuration:
```yaml
esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino
  build_flags:
    - DEBUG_NUKI_CONNECT
    - DEBUG_NUKI_COMMUNICATION
```

## File Structure

```
ESPHOMENUKI/
├── esphome/
│   └── components/
│       └── nuki_ble/
│           ├── __init__.py
│           ├── manifest.yaml
│           ├── nuki_ble.h
│           ├── nuki_ble.cpp
│           └── nuki_ble.py
├── nuki_ble_example.yaml
├── secrets.yaml.template
├── platformio.ini
├── test_component.py
└── README.md
```

## Support

- Original library: https://github.com/I-Connect/NukiBleEsp32
- ESPHome documentation: https://esphome.io/
- Nuki BLE API documentation: https://developer.nuki.io/page/nuki-smart-lock-api-2/2/

## License

MIT License - compatible with ESPHome and the original NukiBleEsp32 library.
