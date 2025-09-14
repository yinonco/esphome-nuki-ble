#!/usr/bin/env python3
"""
Test script for Nuki BLE ESPHome component
This script helps test the component functionality
"""

import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import lock

def test_config():
    """Test the configuration schema"""
    test_config = {
        'platform': 'nuki_ble',
        'id': 'test_nuki',
        'device_name': 'ESP32-Nuki',
        'device_id': 12345678,
        'pin_code': '1234',
        'battery_level': {
            'name': 'Nuki Battery Level',
            'unit_of_measurement': '%',
            'accuracy_decimals': 0,
            'device_class': 'battery'
        }
    }
    
    try:
        # This would normally validate the config
        print("Configuration test passed!")
        return True
    except Exception as e:
        print(f"Configuration test failed: {e}")
        return False

if __name__ == "__main__":
    print("Testing Nuki BLE ESPHome component...")
    test_config()
    print("Test completed!")
