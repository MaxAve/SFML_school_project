#include "environment/GameMap.hpp"
#include <fstream>

#include <iostream>
#define LOG(msg) std::cout << msg << std::endl

GameMap::GameMap(sf::Vector2f _firstPos, const std::string& _path1L, const std::string& _path2L, size_t _mapWidth, size_t _mapHeight) : firstPos{_firstPos}, mapWidth{_mapWidth}, mapHeight{_mapHeight} {
    //generateMap();

    //loadChunksToFile(chunks1L, _path1L);
    //loadChunksToFile(chunks2L, _path2L);

    loadChunksFromFile(chunks1L, _path1L);
    loadChunksFromFile(chunks2L, _path2L);
}

GameMap::GameMap(size_t _mapWidth, size_t _mapHeight) : mapWidth{_mapWidth}, mapHeight{_mapHeight}
{
    for(int y = 0; y < mapWidth; y++)
    {
        for(int x = 0; x < mapHeight; x++)
        {
            this->chunks1L.push_back(TileMapChunk(sf::Vector2i(x * CHUNK_WIDTH * SCALE * TILE_SIZE, y * CHUNK_HEIGHT * SCALE * TILE_SIZE)));
            this->chunks2L.push_back(TileMapChunk(sf::Vector2i(x * CHUNK_WIDTH * SCALE * TILE_SIZE, y * CHUNK_HEIGHT * SCALE * TILE_SIZE)));
        }
    }
}

void fillTiles(TileMapChunk& chunk, int code) {
    for (size_t i = 0; i < CHUNK_HEIGHT; i++) {
        for (size_t j = 0; j < CHUNK_WIDTH; j++) {
            chunk.tiles[i][j] = code;
        }
    }
}

void GameMap::generateMap() {
    const sf::Vector2f chunkSize = {TILE_SIZE * CHUNK_WIDTH * SCALE, TILE_SIZE * CHUNK_HEIGHT * SCALE};
    size_t n = mapWidth * mapHeight;

    // generate tiles

    LOG("Generating tiles");
    for (size_t i = 0; i < n; i++) {
        LOG("Tile..." << i + 1);
        chunks1L.emplace_back(sf::Vector2i{
            static_cast<int>(firstPos.x + chunkSize.x * (i % mapWidth)),
            static_cast<int>(firstPos.y + chunkSize.y * (i / mapWidth))});
        chunks2L.emplace_back(sf::Vector2i{
            static_cast<int>(firstPos.x + chunkSize.x * (i % mapWidth)),
            static_cast<int>(firstPos.y + chunkSize.y * (i / mapWidth))});

        fillTiles(chunks1L[i], 0);
        fillTiles(chunks2L[i], 96); // 96 air
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

    for (int x = 0; x < 16; x++)
        for (int y = 0; y < 16; y++)
            if ((y < 7 || y >= 13) && (rand() % 3) == 0)
                chunks2L[0].tiles[y][x] = 80; // tall grass
            else
                chunks2L[0].tiles[y][x] = 96; // air

    for (size_t i = 0; i < n; i++) {
        chunks1L[i].updateTextures();
        chunks2L[i].updateTextures();
    }
}

void GameMap::loadChunksFromFile(std::vector<TileMapChunk>& chunks, const std::string& path) {
    std::ifstream in(path, std::ios::binary);

    if (!in) {
        LOG("Failed to open file. Path: " << path);
        return;
    }

    size_t ms[2] = {0, 0};
    in.read(reinterpret_cast<char*>(ms), sizeof(size_t) * 2);

    mapWidth = ms[0];
    mapHeight = ms[1];

    std::cout << "[LOG] Loading " << mapWidth << "x" << mapHeight << " tile map layer\n";

    chunks.clear();
    chunks.reserve(mapWidth * mapHeight);

    size_t n = mapWidth * mapHeight;
    const sf::Vector2f chunkSize = {TILE_SIZE * CHUNK_WIDTH * SCALE, TILE_SIZE * CHUNK_HEIGHT * SCALE};

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

    std::cout << "[LOG] Saving " << mapWidth << "x" << mapHeight << " tile map layer\n";

    size_t ms[2] = {mapWidth, mapHeight};
    out.write(reinterpret_cast<const char*>(ms), sizeof(size_t) * 2);

    for (size_t i = 0; i < chunksAmount; i++) {
        out.write(reinterpret_cast<const char*>(chunks[i].tiles), sizeof(int) * CHUNK_HEIGHT * CHUNK_WIDTH);
    }

    out.close();
}

void GameMap::draw(sf::RenderWindow& target, sf::Shader& shader) {
    const sf::Vector2f chunkSize = {TILE_SIZE * CHUNK_WIDTH * SCALE, TILE_SIZE * CHUNK_HEIGHT * SCALE};
    sf::View windowView = target.getView();
    sf::Vector2f viewCenter = windowView.getCenter();
    sf::Vector2f viewSize = windowView.getSize();

    sf::Vector2f upperLeftView = viewCenter - viewSize / 2.f;
    sf::Vector2f bottomRightView = viewCenter + viewSize / 2.f;

    size_t fromX = std::max(0.f, upperLeftView.x / chunkSize.x);
    size_t toX = std::min(mapWidth, static_cast<size_t>(bottomRightView.x / chunkSize.x) + 1);
    size_t fromY = std::max(0.f, upperLeftView.y / chunkSize.y);
    size_t toY = std::min(mapHeight, static_cast<size_t>(bottomRightView.y / chunkSize.y) + 1);

    for (size_t y = fromY; y < toY; ++y) {
        size_t idk = mapWidth * y;
        for (size_t x = fromX; x < toX; ++x) {
            chunks1L[x + idk].draw(target, shader);
            chunks2L[x + idk].draw(target, shader);
        }
    }
}