#include "entities/EntityRenderer.hpp"

std::vector<std::pair<EntityType, void*>> EntityRenderer::entities;

bool EntityRenderer::compareEntities(std::pair<EntityType, void *> a, std::pair<EntityType, void *> b)
{
    int ya, yb;

    if(a.first == EntityType::PLAYER)
        ya = ((Player*)a.second)->hitbox.position.y;
    else if(a.first == EntityType::ZOMBIE)
        ya = ((Zombie*)a.second)->hitbox.position.y;

    if(b.first == EntityType::PLAYER)
        yb = ((Player*)b.second)->hitbox.position.y;
    else if(b.first == EntityType::ZOMBIE)
        yb = ((Zombie*)b.second)->hitbox.position.y;

    return ya < yb;
}

void EntityRenderer::sortEntities(Player* player, std::vector<Zombie*>& zombiePool)
{
    //std::cout << "sorting...\n";
    EntityRenderer::entities.resize(zombiePool.size() + 1);

    EntityRenderer::entities[0] = std::pair<EntityType, void*>(EntityType::PLAYER, (void*)player);
    for(int i = 0; i < zombiePool.size(); i++)
    {
        EntityRenderer::entities[i + 1] = std::pair<EntityType, void*>(EntityType::ZOMBIE, (void*)zombiePool[i]);
    }

    std::sort(entities.begin(), entities.end(),
        [](const std::pair<EntityType, void*>& a,
        const std::pair<EntityType, void*>& b) {
            return EntityRenderer::compareEntities(a, b);
    });
}

void EntityRenderer::drawAll(sf::RenderWindow& window, bool drawHitboxes)
{
    for(int i = 0; i < EntityRenderer::entities.size(); i++)
    {
        if(EntityRenderer::entities[i].first == EntityType::PLAYER)
        {
            ((Player*)EntityRenderer::entities[i].second)->draw(window);
            if(drawHitboxes)
            {
                ((Player*)EntityRenderer::entities[i].second)->hitbox.debugDraw(window);
                ((Player*)EntityRenderer::entities[i].second)->envHitbox.debugDraw(window, sf::Color::Yellow);
            }
        }
        else if(EntityRenderer::entities[i].first == EntityType::ZOMBIE)
        {
            ((Zombie*)EntityRenderer::entities[i].second)->draw(window);
            if(drawHitboxes)
                ((Zombie*)EntityRenderer::entities[i].second)->hitbox.debugDraw(window, sf::Color::Green);
        }
    }
}