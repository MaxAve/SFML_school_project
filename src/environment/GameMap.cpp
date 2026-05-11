#include "environment/GameMap.hpp"
#include <fstream>

#include <iostream>
#define LOG(msg) std::cout << msg << std::endl

GameMap::GameMap(sf::Vector2f _firstPos, const std::string& _path1L, const std::string& _path2L, size_t _mapWidth, size_t _mapHeight) : firstPos{_firstPos}, mapWidth{_mapWidth}, mapHeight{_mapHeight} {
    generateMap();

    // loadChunksFromFile(chunks1L, _path1L);
    // loadChunksFromFile(chunks2L, _path2L);
}

void fillTiles(TileMapChunk& chunk, int code) {
    for (size_t i = 0; i < CHUNK_HEIGHT; i++) {
        for (size_t j = 0; j < CHUNK_WIDTH; j++) {
            chunk.tiles[i][j] = code;
        }
    }
}

void GameMap::generateMap() {
    const sf::Vector2f chunkSize = {TILE_SIZE * CHUNK_WIDTH, TILE_SIZE * CHUNK_HEIGHT};
    size_t n = mapWidth * mapHeight;

    // generate tiles
    for (size_t i = 0; i < n; i++) {
        chunks1L.emplace_back(sf::Vector2i{
            static_cast<int>(firstPos.x + chunkSize.x * (i % mapWidth)),
            static_cast<int>(firstPos.y + chunkSize.y * (i / mapWidth))});
        chunks2L.emplace_back(sf::Vector2i{
            static_cast<int>(firstPos.x + chunkSize.x * (i % mapWidth)),
            static_cast<int>(firstPos.y + chunkSize.y * (i / mapWidth))});

        fillTiles(chunks1L[i], 0); // 0 nothing
        fillTiles(chunks2L[i], 96);
        
        chunks1L[i].updateTextures();
        chunks2L[i].updateTextures();
    }

    for (int x = 0; x < 16; x++)
        chunks1L[0].tiles[7][x] = 16;
    for (int x = 0; x < 16; x++)
        chunks1L[0].tiles[8][x] = 17;
    chunks1L[0].tiles[8][8] = 18;
    for (int x = 0; x < 16; x++)
        for (int y = 9; y < 13; y++)
            chunks1L[0].tiles[y][x] = 34;
    for (int x = 0; x < 16; x++)
        if (x % 2 == 0)
            chunks1L[0].tiles[10][x] = 33;
    for (int x = 0; x < 16; x++)
        chunks1L[0].tiles[13][x] = 64;
    chunks1L[0].updateTextures();

    for (int x = 0; x < 16; x++)
        for (int y = 0; y < 16; y++)
            if ((y < 7 || y >= 13) && (rand() % 3) == 0)
                chunks2L[0].tiles[y][x] = 80; // tall grass
            else
                chunks2L[0].tiles[y][x] = 96; // air
    chunks2L[0].updateTextures();
}

void GameMap::loadChunksFromFile(std::vector<TileMapChunk>& chunks, const std::string& path) {
    chunks.clear();
    chunks.reserve(mapWidth * mapHeight);
    std::ifstream in(path, std::ios::binary);

    if (!in) {
        LOG("Failed to open file. Path: " << path);
        return;
    }

    size_t n = mapWidth * mapHeight;
    const sf::Vector2f chunkSize = {TILE_SIZE * CHUNK_WIDTH, TILE_SIZE * CHUNK_HEIGHT};

    for (size_t i = 0; i < n; i++) {
        chunks.emplace_back(sf::Vector2i{
            static_cast<int>(firstPos.x + chunkSize.x * (i % mapWidth)),
            static_cast<int>(firstPos.y + chunkSize.y * (i / mapWidth))});

        in.read(reinterpret_cast<char*>(chunks[i].tiles), sizeof(int) * CHUNK_HEIGHT * CHUNK_WIDTH);
        chunks[i].updateTextures();
    }

    in.close();
}

void GameMap::loadChunksToFile(const std::vector<TileMapChunk>& chunks, const std::string& path) {
    std::ofstream out(path, std::ios::binary);

    if (!out) {
        LOG("Failed to open file. Path: " << path);
        return;
    }

    size_t chunksAmount = chunks.size();

    for (size_t i = 0; i < chunksAmount; i++) {
        out.write(reinterpret_cast<const char*>(chunks[i].tiles), sizeof(int) * CHUNK_HEIGHT * CHUNK_WIDTH);
    }

    out.close();
}

void GameMap::draw(sf::Vector2f playerPos, sf::RenderWindow& target, sf::Shader& shader) {
    for (size_t i = 0; i < (mapWidth * mapHeight); i++) {
        chunks1L[i].draw(target, shader);
        chunks2L[i].draw(target, shader);
    }
}