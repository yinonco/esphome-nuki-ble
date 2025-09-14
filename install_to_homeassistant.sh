#!/bin/bash
# Script to copy Nuki BLE component to ESPHome addon directory
# Run this script on your Raspberry Pi with Home Assistant

echo "Nuki BLE ESPHome Component Installer"
echo "===================================="

# Check if running on Home Assistant
if [ ! -d "/usr/share/hassio" ]; then
    echo "Error: This script is designed for Home Assistant with ESPHome addon"
    echo "Please run this on your Raspberry Pi with Home Assistant installed"
    exit 1
fi

# Create custom components directory
echo "Creating custom components directory..."
mkdir -p /usr/share/hassio/addons/core_esphome/custom_components

# Check if source directory exists
if [ ! -d "esphome/components/nuki_ble" ]; then
    echo "Error: Source directory 'esphome/components/nuki_ble' not found"
    echo "Please run this script from the directory containing the ESPHOMENUKI files"
    exit 1
fi

# Copy component files
echo "Copying Nuki BLE component files..."
cp -r esphome/components/nuki_ble /usr/share/hassio/addons/core_esphome/custom_components/

# Verify installation
if [ -d "/usr/share/hassio/addons/core_esphome/custom_components/nuki_ble" ]; then
    echo "✅ Installation successful!"
    echo ""
    echo "Component files installed to:"
    echo "/usr/share/hassio/addons/core_esphome/custom_components/nuki_ble/"
    echo ""
    echo "Next steps:"
    echo "1. Open ESPHome Web UI in Home Assistant"
    echo "2. Create a new device"
    echo "3. Use the configuration from nuki_ble_example.yaml"
    echo "4. Update device_name, device_id, and pin_code"
    echo "5. Save and install to your ESP32"
else
    echo "❌ Installation failed!"
    echo "Please check the error messages above"
    exit 1
fi
