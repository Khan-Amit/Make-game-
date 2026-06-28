#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

const int GRID_WIDTH = 16;
const int GRID_HEIGHT = 10;
const int MAP_SIZE = GRID_WIDTH * GRID_HEIGHT;

struct GameData {
    uint8_t tile_map[MAP_SIZE];
};

// Reads embedded binary payload from its own executable file
bool read_embedded_assets(const char* exe_path, GameData& out_data) {
    std::ifstream file(exe_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[-] Error opening self binary!" << std::endl;
        return false;
    }

    std::streamsize file_size = file.tellg();
    
    // Check if file contains at least our minimum footer (4 bytes size + 8 bytes marker = 12 bytes)
    if (file_size < 12) return false;

    // 1. Read footer marker from the last 12 bytes
    file.seekg(file_size - 12);
    uint32_t payload_size = 0;
    char magic_marker[8];

    file.read(reinterpret_cast<char*>(&payload_size), sizeof(payload_size));
    file.read(magic_marker, 8);

    // 2. Validate magic marker
    if (std::memcmp(magic_marker, "MAKEGAME", 8) != 0) {
        std::cout << "[!] No custom asset package bound. Running in IDE/Debug empty mode." << std::endl;
        return false; 
    }

    // 3. Locate and extract payload block
    std::streamoff payload_offset = file_size - 12 - payload_size;
    file.seekg(payload_offset);
    
    file.read(reinterpret_cast<char*>(out_data.tile_map), MAP_SIZE);
    
    std::cout << "[+] Embedded asset footprint detected and loaded into engine memory!" << std::endl;
    return true;
}

int main(int argc, char* argv[]) {
    GameData active_game_data = {};
    
    // Attempt self-unpacking sequence
    bool embedded_found = read_embedded_assets(argv[0], active_game_data);
    
    if (embedded_found) {
        // Print the extracted map inside the console to verify execution
        std::cout << "--- Loaded Grid Layout Map ---" << std::endl;
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            for (int x = 0; x < GRID_WIDTH; ++x) {
                std::cout << (int)active_game_data.tile_map[y * GRID_WIDTH + x] << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "[*] Loading default blank room state configuration." << std::endl;
    }
    
    std::cout << "\n[Engine Setup Finished] Standing by for execution window loop..." << std::endl;
    return 0;
}
