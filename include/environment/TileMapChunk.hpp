#pragma once
#include "resources/Textures.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_set>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define TILE_SIZE 16
#define CHUNK_SIZE 16
#define SCALE 4 // By how much the textures are to be scaled up

class TileMapChunk
{
public:
    static sf::Texture tilesetAtlas;
    static inline const std::unordered_set<int> tilesWithHitbox = {
        64, 65, 66, 67, 68, 80, 81, 82, 83, 84, 96, 97, 98, 99, 100, 112, 113, 114, // Bush
        128, 144, 160, // Brick wall
        115, 116, 129, 130, 131, 132, 145, 147, 148, 161, 162, 163, // Fence
    };

    int tiles[CHUNK_HEIGHT][CHUNK_WIDTH]; // Tile IDs (0-255)
    bool hitbox[CHUNK_HEIGHT][CHUNK_WIDTH];
    sf::VertexArray mesh;
    sf::Vector2f position;

    TileMapChunk(sf::Vector2i position);

    bool updateTextures(); // Update textures based on the tile IDs stored
    void draw(sf::RenderWindow& window, sf::Shader& shader, bool drawHitboxes=false);
};