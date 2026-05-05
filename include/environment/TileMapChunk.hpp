#pragma once
#include "resources/Textures.hpp"
#include <SFML/Graphics.hpp>

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define TILE_SIZE 16
#define SCALE 4 // By how much the textures are to be scaled up

class TileMapChunk
{
public:
    static sf::Texture tilesetAtlas;

    int tiles[16][16]; // Tile IDs (0-255)
    sf::VertexArray mesh;

    TileMapChunk(sf::Vector2i position);

    bool updateTextures(); // Update textures based on the tile IDs stored
    void draw(sf::RenderWindow& window, sf::Shader& shader);
};