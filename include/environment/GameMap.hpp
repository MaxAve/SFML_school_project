#pragma once
#include "environment/TileMapChunk.hpp"
#include <string>


class GameMap {

    const size_t mapWidth, mapHeight; // in chunks

    std::vector<TileMapChunk> chunks1L; // normal tiles
    std::vector<TileMapChunk> chunks2L; // f.e. tall grass

    sf::Vector2f firstPos; // upper left corner

public:
    GameMap(sf::Vector2f _firstPos, const std::string& _path1L, const std::string& _path2L, size_t _chunksWidth, size_t _chunksHeight);

    // temp
    void generateMap();

    void loadChunksFromFile(std::vector<TileMapChunk>& chunks, const std::string& path); 

    void loadChunksToFile(const std::vector<TileMapChunk>& chunks, const std::string& path);

    // TODO: Make it O(1)
    void draw(sf::RenderWindow& target, sf::Shader& shader);
};