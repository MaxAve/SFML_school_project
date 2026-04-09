#include "../../include/environment/TileMapChunk.hpp"

sf::Texture TileMapChunk::tilesetAtlas;

TileMapChunk::TileMapChunk(sf::Vector2i position)
{
    this->mesh.setPrimitiveType(sf::PrimitiveType::Triangles);
    this->mesh.resize(CHUNK_WIDTH * CHUNK_HEIGHT * 6);

    for (unsigned int i = 0; i < CHUNK_WIDTH; ++i)
    {
        for (unsigned int j = 0; j < CHUNK_HEIGHT; ++j)
        {
            // get a pointer to the current tile's quad
            sf::Vertex* quad = &this->mesh[(i + j * CHUNK_WIDTH) * 6];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * TILE_SIZE * SCALE + position.x, j * TILE_SIZE * SCALE + position.y);
            quad[1].position = sf::Vector2f((i + 1) * TILE_SIZE * SCALE + position.x, j * TILE_SIZE * SCALE + position.y);
            quad[2].position = sf::Vector2f(i * TILE_SIZE * SCALE + position.x, (j + 1) * TILE_SIZE * SCALE + position.y);

            quad[3].position = sf::Vector2f((i + 1) * TILE_SIZE * SCALE + position.x, (j + 1) * TILE_SIZE * SCALE + position.y);
            quad[4].position = sf::Vector2f((i + 1) * TILE_SIZE * SCALE + position.x, j * TILE_SIZE * SCALE + position.y);
            quad[5].position = sf::Vector2f(i * TILE_SIZE * SCALE + position.x, (j + 1) * TILE_SIZE * SCALE + position.y);
        }
    }
}

bool TileMapChunk::updateTextures()
{
    for (unsigned int i = 0; i < CHUNK_WIDTH; ++i)
    {
        for (unsigned int j = 0; j < CHUNK_HEIGHT; ++j)
        {
            // get the current tile number
            int tileNumber = this->tiles[j][i];

            // find its position in the tileset texture
            int tu = tileNumber % (tilesetAtlas.getSize().x / TILE_SIZE);
            int tv = tileNumber / (tilesetAtlas.getSize().x / TILE_SIZE);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &this->mesh[(i + j * CHUNK_WIDTH) * 6];

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * TILE_SIZE, tv * TILE_SIZE);
            quad[1].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, tv * TILE_SIZE);
            quad[2].texCoords = sf::Vector2f(tu * TILE_SIZE, (tv + 1) * TILE_SIZE);

            quad[3].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, (tv + 1) * TILE_SIZE);
            quad[4].texCoords = sf::Vector2f((tu + 1) * TILE_SIZE, tv * TILE_SIZE);
            quad[5].texCoords = sf::Vector2f(tu * TILE_SIZE, (tv + 1) * TILE_SIZE);
        }
    }

    return true;
}

void TileMapChunk::draw(sf::RenderWindow& window)
{
    window.draw(this->mesh, &TileMapChunk::tilesetAtlas);
}