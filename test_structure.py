# Test the component structure
import os

def test_component_structure():
    """Test that the component has the required files"""
    component_dir = "nuki_ble"
    required_files = [
        "__init__.py",
        "manifest.yaml", 
        "nuki_ble.h",
        "nuki_ble.cpp",
        "nuki_ble.py"
    ]
    
    print("Checking component structure...")
    for file in required_files:
        file_path = os.path.join(component_dir, file)
        if os.path.exists(file_path):
            print(f"✅ {file}")
        else:
            print(f"❌ {file} - MISSING")
    
    print("\nComponent structure check complete!")

if __name__ == "__main__":
    test_component_structure()
