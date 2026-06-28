import os
import struct

def package_game(template_engine_path, output_app_path, asset_folder):
    """
    Fuses the pre-compiled C++ Engine template with the project asset package.
    """
    print(f"[*] Compiling Standalone App: {output_app_path}")
    
    # 1. Read the pre-compiled engine binary
    if not os.path.exists(template_engine_path):
        raise FileNotFoundError(f"Core Engine Template missing at: {template_engine_path}")
        
    with open(template_engine_path, "rb") as engine_file:
        engine_binary = engine_file.read()
    
    # 2. Gather assets and pack them into a byte-stream payload
    payload = bytearray()
    
    # Example asset: A flat 2D tile map array (10 rows x 16 cols)
    # Prince of Persia screen structure: 0=Empty, 1=Floor, 2=Ledge, 3=Spikes
    sample_map = [
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,2,0,0,0,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
    ]
    
    # Pack map integers into raw binary data
    map_bytes = struct.pack(f"{len(sample_map)}B", *sample_map)
    payload.extend(map_bytes)
    
    # 3. Append the Magic Marker and Payload Size to the very end of the file
    # This acts as a map for the C++ engine to find where the data starts
    payload_size = len(payload)
    magic_marker = b"MAKEGAME" # 8-byte unique identifier
    
    footer = struct.pack("<I", payload_size) + magic_marker
    
    # 4. Merge all components into a single standalone app file
    with open(output_app_path, "wb") as standalone_app:
        standalone_app.write(engine_binary) # The executable runtime
        standalone_app.write(payload)       # The map/asset payload data
        standalone_app.write(footer)        # The structural pointer footer
        
    print(f"[+] Standalone App Successfully Written! Size: {os.path.getsize(output_app_path)} bytes")

# Test run parameters
if __name__ == "__main__":
    # For initial layout testing, we assume engine template is compiled
    try:
        package_game("engine_template.exe", "MyCustomGame.exe", "assets/")
    except Exception as e:
        print(f"[-] Packaging failed: {e}. Build engine template first.")
