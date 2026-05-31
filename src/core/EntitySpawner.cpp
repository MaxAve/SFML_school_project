#include "core/EntitySpawner.hpp"

void EntitySpawner::attemptSpawnZombiesWithinPlayerRadius(Player* player, GameMap* map)
{
    int c = 0;
    for (int i = 0; i < Zombie::pool.size(); i++) {
        if(Utils::distance(Zombie::pool[i]->sprite.getPosition(), player->sprite.getPosition()) > 2000.0f)
        {
            Zombie::pool[i]->healthBar.setHealth(0);
            c++;
        }
    }
    std::cout << "[LOG] Deleted " << c << " entities out of range.\n";

    if(Zombie::pool.size() > 40)
    {
        std::cout << "[LOG] Cancelling spawn attempt: too many enemies active.\n";
        return;
    }

    for(int i = 0; i < map->chunks2L.size(); i++)
    {
        const sf::Vector2f dist = map->chunks2L[i].position - player->sprite.getPosition(); // Player distance to chunk
        
        if(std::sqrt(dist.x*dist.x + dist.y*dist.y) < 2000) // Ignore chunks that are far away + only every 4th chunk will have zombies
        {
            if((rand() % 3 == 0))
            {
                std::cout << "[LOG] Attempting to spawn enemies in chunk " << map->chunks2L[i].position.x << "," << map->chunks2L[i].position.y << "\n";
                if(map->chunks2L[i].enemiesSpawned)
                    continue;

                for (int x = 0; x < CHUNK_SIZE; x++)
                {
                    for (int y = 0; y < CHUNK_SIZE; y++)
                    {
                        if(map->chunks2L[i].hitbox[y][x])
                            continue;
                        if(rand() % 20 == 0)
                            new Zombie(sf::Vector2f(map->chunks2L[i].position.x + x * TILE_SIZE * SCALE + ((float)(TILE_SIZE * SCALE) / 2.0f) - 16, map->chunks2L[i].position.y + y * TILE_SIZE * SCALE + ((float)(TILE_SIZE * SCALE) / 2.0f) - 16), player);
                    }
                }
            }

            map->chunks2L[i].enemiesSpawned = true; // Chunks that failed to spawn enemies should not re-attempt to spawn them
        }
    }
}