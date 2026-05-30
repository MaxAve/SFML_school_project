#pragma once
#include "environment/TileMapChunk.hpp"
#include <string>


class GameMap {
public:
    size_t mapWidth, mapHeight; // in chunks

    std::vector<TileMapChunk> chunks1L; // normal tiles
    std::vector<TileMapChunk> chunks2L; // f.e. tall grass

    sf::Vector2f firstPos; // upper left corner

    // adds the stuff saved in the files to pools
    static void loadEnvironment(const std::string& path);

    static void saveEnvironment(const std::string& path);

    GameMap(sf::Vector2f _firstPos, const std::string& _path1L, const std::string& _path2L, size_t _chunksWidth, size_t _chunksHeight);
    GameMap(size_t _chunksWidth, size_t _chunksHeight);

    // temp
    void generateMap();

    void loadChunksFromFile(std::vector<TileMapChunk>& chunks, const std::string& path); 

    void loadChunksToFile(const std::vector<TileMapChunk>& chunks, const std::string& path);

    void draw(sf::RenderWindow& target, sf::Shader& shader, sf::Vector2f playerPos, bool debugMode);
};