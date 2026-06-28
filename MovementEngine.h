#pragma once
#include <iostream>

// Dimensions matching our Prince of Persia grid architecture
const int TILE_SIZE_PIXELS = 32; 

enum PlayerState {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_STOPPING_SKID,
    STATE_LEAPING,
    STATE_HANGING
};

struct PlayerCharacter {
    // Exact pixel coordinates on the screen
    float pixel_x = 32.0f;
    float pixel_y = 128.0f;

    // Velocities applied per frame step
    float vel_x = 0.0f;
    float vel_y = 0.0f;

    // State mechanics
    PlayerState current_state = STATE_IDLE;
    int animation_frame = 0;
    int state_timer = 0; // Tracks locked frame sequences (like an un-interruptible skid)
    bool facing_right = true;
};

class MovementEngine {
public:
    void update_player_physics(PlayerCharacter& player, const uint8_t* tile_map, int map_w, int map_h) {
        // 1. Get current grid indexes for boundary processing
        int grid_x = static_cast<int>(player.pixel_x) / TILE_SIZE_PIXELS;
        int grid_y = static_cast<int>(player.pixel_y) / TILE_SIZE_PIXELS;

        // 2. State-Machine Evaluation
        switch (player.current_state) {
            
            case STATE_IDLE:
                player.vel_x = 0.0f;
                // Check if the floor beneath disappears
                if (!check_collision(grid_x, grid_y + 2, tile_map, map_w, map_h)) {
                    player.current_state = STATE_LEAPING; // Fall straight down
                    player.vel_y = 4.0f; 
                }
                break;

            case STATE_RUNNING:
                player.vel_x = player.facing_right ? 4.0f : -4.0f;
                
                // Edge/Ledge Check: Look ahead to see if the player is running off a cliff
                int look_ahead_x;
                look_ahead_x = player.facing_right ? (static_cast<int>(player.pixel_x + 16) / TILE_SIZE_PIXELS) 
                                                   : (static_cast<int>(player.pixel_x - 16) / TILE_SIZE_PIXELS);
                
                if (!check_collision(look_ahead_x, grid_y + 2, tile_map, map_w, map_h)) {
                    // Transition automatically to a skid jump preparation or edge stop
                    player.current_state = STATE_STOPPING_SKID;
                    player.state_timer = 6; // Force 6 frames of skidding animation
                }
                break;

            case STATE_STOPPING_SKID:
                // Decelerate and lock inputs
                player.vel_x *= 0.5f; 
                player.state_timer--;
                if (player.state_timer <= 0) {
                    player.vel_x = 0.0f;
                    player.current_state = STATE_IDLE;
                }
                break;

            case STATE_LEAPING:
                // Apply a simple locked arc or gravitational pull down
                player.vel_y += 0.5f; // Gravity step
                
                // Ledge Grab Raycast: If passing near a solid tile corner, lock to STATE_HANGING
                int hands_x;
                hands_x = player.facing_right ? (static_cast<int>(player.pixel_x + 16) / TILE_SIZE_PIXELS)
                                              : (static_cast<int>(player.pixel_x - 16) / TILE_SIZE_PIXELS);
                int hands_y = static_cast<int>(player.pixel_y) / TILE_SIZE_PIXELS;

                // If hand hits a wall tile that is empty above it -> Catch Ledge!
                if (check_collision(hands_x, hands_y, tile_map, map_w, map_h) && 
                    !check_collision(hands_x, hands_y - 1, tile_map, map_w, map_h)) {
                    player.current_state = STATE_HANGING;
                    player.vel_x = 0.0f;
                    player.vel_y = 0.0f;
                    // Snap explicitly to the ledge alignment
                    player.pixel_x = static_cast<float>(hands_x * TILE_SIZE_PIXELS - (player.facing_right ? 16 : -16));
                    player.pixel_y = static_cast<float>(hands_y * TILE_SIZE_PIXELS);
                    std::cout << "[Engine Event] Caught Ledge Edge Safely!" << std::endl;
                }
                break;

            case STATE_HANGING:
                // Completely static until an upward or downward action command is registered
                player.vel_x = 0.0f;
                player.vel_y = 0.0f;
                break;
        }

        // 3. Execute Vector Translation
        player.pixel_x += player.vel_x;
        player.pixel_y += player.vel_y;
    }

private:
    bool check_collision(int x, int y, const uint8_t* map, int w, int h) {
        if (x < 0 || x >= w || y < 0 || y >= h) return true; // Treat boundaries as solid walls
        return map[y * w + x] == 1; // 1 represents a physical platform tile block
    }
};
