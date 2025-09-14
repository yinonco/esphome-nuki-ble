# Fixed: "Platform not found: 'lock.nuki_ble'" Error

## ✅ **Problem Solved**

The error was caused by incorrect component structure. The component now has the correct structure:

```
nuki_ble/
├── __init__.py          # Python package marker
├── manifest.yaml        # Component metadata with dependencies
├── nuki_ble.h          # C++ header file
├── nuki_ble.cpp        # C++ implementation
└── nuki_ble.py         # Python configuration schema
```

## 🔧 **What Was Fixed**

1. **Added dependencies to manifest.yaml** - This tells ESPHome about the NukiBleEsp32 library
2. **Corrected file structure** - Removed incorrect nested directories
3. **Proper Python module structure** - ESPHome can now find the component

## 🚀 **How to Use**

1. **Upload the `nuki_ble/` folder** to your GitHub repository
2. **Update your YAML** with the correct GitHub URL:
   ```yaml
   external_components:
     - source:
         type: git
         url: https://github.com/YOUR_USERNAME/esphome-nuki-ble.git
         ref: main
       components: [nuki_ble]
   ```

3. **Use the component**:
   ```yaml
   lock:
     - platform: nuki_ble
       device_name: "ESP32-Nuki"
       device_id: 12345678
       pin_code: "1234"
       pairing_mode: "app"
   ```

## 🎯 **Key Points**

- ✅ **Dependencies are now properly declared** in manifest.yaml
- ✅ **Component structure follows ESPHome standards**
- ✅ **All required files are present**
- ✅ **App pairing mode won't affect your Nuki Bridge**

The component should now work correctly!
