#include "environment/GameMap.hpp"
#include "environment/Door.hpp"
#include "environment/Structure.hpp"
#include "looting/LootContainer.hpp"
#include <fstream>

#include <iostream>
#define LOG(msg) std::cout << msg << std::endl

void GameMap::loadEnvironment(const std::string& path) {
    LOG("Loading environment from " << path);

    std::ifstream in(path, std::ios_base::binary);

    if (!in) {
        LOG("GameMap::loadEnvironment(): File not found");
        return;
    }

    Door::pool.clear();
    LootContainer::pool.clear();
    Structure::pool.clear();

    // door: constructor already adds to the pool
    // Lootbox: .create(...) handles constructor + pool
    // same for Structure

    LOG("Loading doors");

    size_t doorCount = 0;
    std::vector<size_t> targetDoorIds;
    in.read(reinterpret_cast<char*>(&doorCount), sizeof(size_t));
    targetDoorIds.reserve(doorCount);
    for (size_t i = 0; i < doorCount; ++i) {
        size_t id = 0, targetDoorId = 0;
        sf::Vector2f pos, size;
        in.read(reinterpret_cast<char*>(&id), sizeof(size_t));
        in.read(reinterpret_cast<char*>(&targetDoorId), sizeof(size_t));
        in.read(reinterpret_cast<char*>(&pos), sizeof(sf::Vector2f));
        in.read(reinterpret_cast<char*>(&size), sizeof(sf::Vector2f));

        Hitbox hb(pos, size, true);

        new Door(id, hb);
        targetDoorIds.push_back(targetDoorId);
    }
    // assing *targetDoor
    for (size_t i = 0; i < doorCount; ++i) {
        for (size_t j = 0; j < doorCount; ++j) {
            if (Door::pool[j]->id != targetDoorIds[i]) {
                continue;
            }
            Door::pool[i]->targetDoor = Door::pool[j];
        }
    }

    LOG("Loading lootboxes");

    size_t lootboxCount = 0;
    in.read(reinterpret_cast<char*>(&lootboxCount), sizeof(size_t));
    for (size_t i = 0; i < lootboxCount; ++i) {
        size_t type = 0;
        in.read(reinterpret_cast<char*>(&type), sizeof(size_t));
        sf::Vector2f pos;
        in.read(reinterpret_cast<char*>(&pos), sizeof(sf::Vector2f));

        LootContainer* newLootbox = LootContainer::create(type, pos, sf::Vector2f{100.f, 75.f}, 125.f);

        size_t rowCnt = 0;
        in.read(reinterpret_cast<char*>(&rowCnt), sizeof(size_t));
        for (size_t row = 0; row < rowCnt; ++row) {
            size_t yCnt = 0; // idk how to name it
            in.read(reinterpret_cast<char*>(&yCnt), sizeof(size_t));

            for (size_t slot = 0; slot < yCnt; ++slot) {
                ItemType itemType;
                size_t itemAmount = 0;

                in.read(reinterpret_cast<char*>(&itemType), sizeof(ItemType));
                in.read(reinterpret_cast<char*>(&itemAmount), sizeof(size_t));
                Item* newItem;

                if (itemType == ItemType::NONE) {
                    newItem = nullptr;
                } else {
                    newItem = new Item(itemType, itemAmount);
                }

                newLootbox->getInventory()->setItem({static_cast<unsigned int>(row), static_cast<unsigned int>(slot)}, newItem);
            }
        }
    }

    LOG("Loading structures");

    size_t structureCount = 0;
    in.read(reinterpret_cast<char*>(&structureCount), sizeof(size_t));
    for (size_t i = 0; i < structureCount; ++i) {
        size_t type = 0;
        sf::Vector2f pos;

        in.read(reinterpret_cast<char*>(&type), sizeof(size_t));
        in.read(reinterpret_cast<char*>(&pos), sizeof(sf::Vector2f));

        Structure::create(type, pos);
    }

    in.close();

    LOG("Environment loaded from " << path);
}

void GameMap::saveEnvironment(const std::string& path) {
    LOG("Loading Environment to " << path);

    std::ofstream out(path, std::ios_base::binary);

    if (!out) {
        LOG("GameMap::saveEnvironment(): File not found");
        return;
    }

    LOG("Saving doors");

    size_t doorCount = Door::pool.size();
    out.write(reinterpret_cast<const char*>(&doorCount), sizeof(size_t));
    for (auto* door : Door::pool) {
        out.write(reinterpret_cast<const char*>(&door->id), sizeof(size_t)); // id
        size_t targetDoorId = door->targetDoor ? door->targetDoor->id : 0;
        out.write(reinterpret_cast<const char*>(&targetDoorId), sizeof(size_t));                // target id
        out.write(reinterpret_cast<const char*>(&door->hitbox.position), sizeof(sf::Vector2f)); // position
        out.write(reinterpret_cast<const char*>(&door->hitbox.size), sizeof(sf::Vector2f));     // size
    }

    LOG("Saving lootboxes");

    size_t lootboxCount = LootContainer::pool.size();
    out.write(reinterpret_cast<const char*>(&lootboxCount), sizeof(size_t));
    for (auto* lootbox : LootContainer::pool) {
        out.write(reinterpret_cast<const char*>(&lootbox->type), sizeof(size_t)); // type
        sf::Vector2f pos = lootbox->getPosition();
        out.write(reinterpret_cast<const char*>(&pos), sizeof(sf::Vector2f)); // position

        auto& inventorySlots = *(lootbox->getInventorySlots());
        size_t rowCnt = inventorySlots.size();
        out.write(reinterpret_cast<const char*>(&rowCnt), sizeof(size_t));

        for (auto& row : inventorySlots) {
            size_t yCnt = row.size();
            out.write(reinterpret_cast<const char*>(&yCnt), sizeof(size_t));
            for (auto& slot : row) {
                ItemType itemType;
                size_t itemAmount = 0;

                if (!slot.getItem()) {
                    itemType = ItemType::NONE;
                    itemAmount = 0;
                } else {
                    itemType = slot.getItem()->getType();
                    itemAmount = slot.getItem()->getAmount();
                }

                out.write(reinterpret_cast<const char*>(&itemType), sizeof(ItemType)); // type
                out.write(reinterpret_cast<const char*>(&itemAmount), sizeof(size_t)); // amount
            }
        }
    }

    LOG("Saving structures");

    size_t structureCount = Structure::pool.size();
    out.write(reinterpret_cast<const char*>(&structureCount), sizeof(size_t));
    for (auto* structure : Structure::pool) {
        size_t type = structure->getType();
        sf::Vector2f pos = structure->getPosition();

        out.write(reinterpret_cast<const char*>(&type), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(&pos), sizeof(sf::Vector2f));
    }

    out.close();

    LOG("Environment loaded to " << path);
}

GameMap::GameMap(sf::Vector2f _firstPos, const std::string& _path1L, const std::string& _path2L, size_t _mapWidth, size_t _mapHeight) : firstPos{_firstPos}, mapWidth{_mapWidth}, mapHeight{_mapHeight} {
    // generateMap();
    // loadChunksToFile(chunks1L, _path1L);
    // loadChunksToFile(chunks2L, _path2L);

    loadChunksFromFile(chunks1L, _path1L);
    loadChunksFromFile(chunks2L, _path2L);
}

GameMap::GameMap(size_t _mapWidth, size_t _mapHeight) : mapWidth{_mapWidth}, mapHeight{_mapHeight} {
    for (int y = 0; y < mapWidth; y++) {
        for (int x = 0; x < mapHeight; x++) {
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
    // TODO this is broken

    // const sf::Vector2f chunkSize = {TILE_SIZE * CHUNK_WIDTH * SCALE, TILE_SIZE * CHUNK_HEIGHT * SCALE};
    // sf::View windowView = target.getView();
    // sf::Vector2f viewCenter = windowView.getCenter();
    // sf::Vector2f viewSize = windowView.getSize();

    // sf::Vector2f upperLeftView = viewCenter - viewSize / 2.f;
    // sf::Vector2f bottomRightView = viewCenter + viewSize / 2.f;

    // size_t fromX = std::max(0.f, upperLeftView.x / chunkSize.x);
    // size_t toX = std::min(mapWidth, static_cast<size_t>(bottomRightView.x / chunkSize.x) + 1);
    // size_t fromY = std::max(0.f, upperLeftView.y / chunkSize.y);
    // size_t toY = std::min(mapHeight, static_cast<size_t>(bottomRightView.y / chunkSize.y) + 1);

    // for (size_t y = fromY; y < toY; ++y) {
    //     size_t idk = mapWidth * y;
    //     for (size_t x = fromX; x < toX; ++x) {
    //         chunks1L[x + idk].draw(target, shader);
    //         chunks2L[x + idk].draw(target, shader);
    //     }
    // }

    for (int i = 0; i < chunks1L.size(); i++) {
        chunks1L[i].draw(target, shader);
    }

    for (int i = 0; i < chunks2L.size(); i++) {
        chunks2L[i].draw(target, shader);
    }
}