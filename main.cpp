#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "MovementEngine.h" // Include our newly created movement library

const int GRID_WIDTH = 16;
const int GRID_HEIGHT = 10;
const int MAP_SIZE = GRID_WIDTH * GRID_HEIGHT;

struct GameData {
    uint8_t tile_map[MAP_SIZE];
};

class MakeGameEngine {
private:
    bool is_running;
    GameData active_game_data;
    PlayerCharacter player;     // Added from MovementEngine
    MovementEngine physics_sys; // Added from MovementEngine

public:
    MakeGameEngine() : is_running(false) {}

    bool initialize(const char* exe_path) {
        std::cout << "[MakeGame Engine] Initializing In-House Core Engine..." << std::endl;
        is_running = true;
        
        // Load embedded level assets 
        read_embedded_assets(exe_path, active_game_data);
        return true;
    }

    void run() {
        // Simple fixed-timestep game frame update simulation loop
        while (is_running) {
            process_input();
            update_physics();
            render();
            
            // Temporary exit safety break so it doesn't loop infinitely in the terminal
            is_running = false; 
        }
    }

private:
    // --- THIS IS WHERE THE SAFE INPUT INJECTION SYSTEM LIVES ---
    void process_input() {
        std::cout << "[Engine] Processing Safe Input Commands..." << std::endl;
        
        // If locked in an un-interruptible state sequence (skidding, landing), ignore new actions
        if (player.current_state == STATE_STOPPING_SKID || player.current_state == STATE_HANGING) {
            std::cout << " -> Input locked during locked action state." << std::endl;
            return; 
        }

        // Example Action: Simulation of user pushing the 'Right arrow key'
        player.facing_right = true;
        player.current_state = STATE_RUNNING;
    }

    void update_physics() {
        // Safely evaluate step parameters
        physics_sys.update_player_physics(player, active_game_data.tile_map, GRID_WIDTH, GRID_HEIGHT);
    }

    void render() {
        std::cout << "[Engine Render Frame] Player Position - X: " << player.pixel_x << " Y: " << player.pixel_y << std::endl;
    }

    bool read_embedded_assets(const char* exe_path, GameData& out_data) {
        std::ifstream file(exe_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) return false;
        std::streamsize file_size = file.tellg();
        if (file_size < 12) return false;

        file.seekg(file_size - 12);
        uint32_t payload_size = 0;
        char magic_marker[8];
        file.read(reinterpret_cast<char*>(&payload_size), sizeof(payload_size));
        file.read(magic_marker, 8);

        if (std::memcmp(magic_marker, "MAKEGAME", 8) != 0) return false;

        file.seekg(file_size - 12 - payload_size);
        file.read(reinterpret_cast<char*>(out_data.tile_map), MAP_SIZE);
        return true;
    }
};

int main(int argc, char* argv[]) {
    MakeGameEngine engine;
    if (engine.initialize(argv[0])) {
        engine.run();
    }
    return 0;
}
