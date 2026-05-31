#include "entities/Player.hpp"
#include "core/Physics.hpp"
#include "resources/AudioManager.hpp"
#define LOG(message) std::cout << message << std::endl

#define STEP_COUNTDOWN 0.45f

Player::Player(sf::RenderWindow& window) : inventory({10, 3}, "Player"), hotbar(5) {
    this->sprite = sf::RectangleShape(sf::Vector2f(50.f, 100.f));
    this->sprite.setFillColor(sf::Color::Blue);
    this->sprite.setPosition({window.getSize().x / 2 - 25.f, window.getSize().y / 2 - 50.f});
    this->view = sf::View({800.f, 300.f}, {800.f, 600.f});
    this->speed = 300.0f;
    this->reloading = false;
    this->hitbox = Hitbox(this->sprite.getPosition(), this->sprite.getSize());
    this->envHitbox = Hitbox(this->sprite.getPosition(), {this->sprite.getSize().x, this->sprite.getSize().y * 0.1f});
    this->equippedItem = nullptr;

    this->gunSprite = sf::RectangleShape(sf::Vector2f(70.f, 30.f));
    this->gunSprite.setFillColor(sf::Color::Red);
    this->gunSprite.setOrigin({0, this->gunSprite.getSize().y / 2});
    this->gunSprite.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getSize().x / 2, this->sprite.getPosition().y + this->sprite.getSize().y / 2));
}

void Player::update() {
    this->gunSprite.setPosition(sf::Vector2f(this->sprite.getPosition().x + this->sprite.getSize().x / 2, this->sprite.getPosition().y + this->sprite.getSize().y / 2));
    sf::Vector2i mousePos = Window::getMousePosFromCenter();
    this->gunSprite.setRotation(sf::degrees(std::atan2(mousePos.y, mousePos.x) / M_PI * 180.f));
}

void Player::draw(sf::RenderWindow& window) {
    this->view.setCenter({this->sprite.getPosition().x + 25.f, this->sprite.getPosition().y + 50.f});
    window.draw(this->sprite);
    window.draw(this->gunSprite);
}

void Player::setPosition(sf::Vector2f pos)
{
    this->sprite.setPosition(pos);
    
    this->hitbox.position = pos;
    this->hitbox.debugSprite.setPosition(this->hitbox.position);

    this->envHitbox.position = {pos.x, pos.y + this->sprite.getSize().y * 0.8f};
    this->envHitbox.debugSprite.setPosition(this->envHitbox.position);
}

void Player::move(sf::Vector2f delta, const GameMap* map)
{
    // First, move the player, then check if it collides with anything
    this->sprite.move({delta.x, 0});

    this->hitbox.position = this->sprite.getPosition();
    this->hitbox.debugSprite.setPosition(this->hitbox.position);

    this->envHitbox.position = {this->hitbox.position.x, this->hitbox.position.y + this->sprite.getSize().y - this->envHitbox.size.y};
    this->envHitbox.debugSprite.setPosition(this->envHitbox.position);

    // Check for collisions with chunks
    for(int i = 0; i < map->chunks1L.size(); i++)
    {
        const sf::Vector2f cp(map->chunks1L[i].position.x + (float)CHUNK_SIZE/2.0 * (float)SCALE, map->chunks1L[i].position.y + (float)CHUNK_SIZE/2.0 * (float)SCALE); // Chunk center positiom
        const sf::Vector2f dist = cp - this->sprite.getPosition(); // Player distance to chunk
        
        if(std::sqrt(dist.x*dist.x + dist.y*dist.y) < 2500) // Ignore chunks that are far away
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                for (int y = 0; y < CHUNK_SIZE; y++)
                {
                    if(!map->chunks1L[i].hitbox[y][x])
                        continue;

                    const sf::Vector2f tilePos(map->chunks1L[i].position.x + x * TILE_SIZE * SCALE, map->chunks1L[i].position.y + y * TILE_SIZE * SCALE);

                    const float tileLeft   = map->chunks1L[i].position.x + x * TILE_SIZE * SCALE;
                    const float tileTop    = map->chunks1L[i].position.y + y * TILE_SIZE * SCALE;
                    const float tileRight  = tileLeft + TILE_SIZE * SCALE;
                    const float tileBottom = tileTop  + TILE_SIZE * SCALE;

                    const float playerLeft   = this->envHitbox.position.x;
                    const float playerTop    = this->envHitbox.position.y;
                    const float playerRight  = playerLeft + this->envHitbox.size.x;
                    const float playerBottom = playerTop  + this->envHitbox.size.y;

                    if (playerRight <= tileLeft || playerLeft >= tileRight ||
                        playerBottom <= tileTop || playerTop >= tileBottom)
                        continue;

                    if(delta.x > 0)
                    {
                        delta.x = 0;
                        this->sprite.setPosition({tilePos.x - this->sprite.getSize().x, this->sprite.getPosition().y});
                    } else if(delta.x < 0)
                    {
                        delta.x = 0;
                        this->sprite.setPosition({tilePos.x + TILE_SIZE * SCALE, this->sprite.getPosition().y});
                    }
                }
            }
        }
    }

    for(int i = 0; i < map->chunks2L.size(); i++)
    {
        const sf::Vector2f cp(map->chunks2L[i].position.x + (float)CHUNK_SIZE/2.0 * (float)SCALE, map->chunks2L[i].position.y + (float)CHUNK_SIZE/2.0 * (float)SCALE); // Chunk center positiom
        const sf::Vector2f dist = cp - this->sprite.getPosition(); // Player distance to chunk
        
        if(std::sqrt(dist.x*dist.x + dist.y*dist.y) < 2500) // Ignore chunks that are far away
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                for (int y = 0; y < CHUNK_SIZE; y++)
                {
                    if(!map->chunks2L[i].hitbox[y][x])
                        continue;

                    const sf::Vector2f tilePos(map->chunks2L[i].position.x + x * TILE_SIZE * SCALE, map->chunks2L[i].position.y + y * TILE_SIZE * SCALE);

                    const float tileLeft   = map->chunks2L[i].position.x + x * TILE_SIZE * SCALE;
                    const float tileTop    = map->chunks2L[i].position.y + y * TILE_SIZE * SCALE;
                    const float tileRight  = tileLeft + TILE_SIZE * SCALE;
                    const float tileBottom = tileTop  + TILE_SIZE * SCALE;

                    const float playerLeft   = this->envHitbox.position.x;
                    const float playerTop    = this->envHitbox.position.y;
                    const float playerRight  = playerLeft + this->envHitbox.size.x;
                    const float playerBottom = playerTop  + this->envHitbox.size.y;

                    if (playerRight <= tileLeft || playerLeft >= tileRight ||
                        playerBottom <= tileTop || playerTop >= tileBottom)
                        continue;

                    if(delta.x > 0)
                    {
                        delta.x = 0;
                        this->sprite.setPosition({tilePos.x - this->sprite.getSize().x, this->sprite.getPosition().y});
                    } else if(delta.x < 0)
                    {
                        delta.x = 0;
                        this->sprite.setPosition({tilePos.x + TILE_SIZE * SCALE, this->sprite.getPosition().y});
                    }
                }
            }
        }
    }

    // TODO this is scuffed
    this->sprite.move({0, delta.y});

    this->hitbox.position = this->sprite.getPosition();
    this->hitbox.debugSprite.setPosition(this->hitbox.position);

    this->envHitbox.position = {this->hitbox.position.x, this->hitbox.position.y + this->sprite.getSize().y - this->envHitbox.size.y};
    this->envHitbox.debugSprite.setPosition(this->envHitbox.position);

    // Check for collisions with chunks
    for(int i = 0; i < map->chunks1L.size(); i++)
    {
        const sf::Vector2f cp(map->chunks1L[i].position.x + (float)CHUNK_SIZE/2.0 * (float)SCALE, map->chunks1L[i].position.y + (float)CHUNK_SIZE/2.0 * (float)SCALE); // Chunk center positiom
        const sf::Vector2f dist = cp - this->sprite.getPosition(); // Player distance to chunk
        
        if(std::sqrt(dist.x*dist.x + dist.y*dist.y) < 2500) // Ignore chunks that are far away
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                for (int y = 0; y < CHUNK_SIZE; y++)
                {
                    if(!map->chunks1L[i].hitbox[y][x])
                        continue;

                    const sf::Vector2f tilePos(map->chunks1L[i].position.x + x * TILE_SIZE * SCALE, map->chunks1L[i].position.y + y * TILE_SIZE * SCALE);

                    const float tileLeft   = map->chunks1L[i].position.x + x * TILE_SIZE * SCALE;
                    const float tileTop    = map->chunks1L[i].position.y + y * TILE_SIZE * SCALE;
                    const float tileRight  = tileLeft + TILE_SIZE * SCALE;
                    const float tileBottom = tileTop  + TILE_SIZE * SCALE;

                    const float playerLeft   = this->envHitbox.position.x;
                    const float playerTop    = this->envHitbox.position.y;
                    const float playerRight  = playerLeft + this->envHitbox.size.x;
                    const float playerBottom = playerTop  + this->envHitbox.size.y;

                    if (playerRight <= tileLeft || playerLeft >= tileRight ||
                        playerBottom <= tileTop || playerTop >= tileBottom)
                        continue;

                    if(delta.y > 0)
                    {
                        delta.y = 0;
                        this->sprite.setPosition({this->sprite.getPosition().x, tilePos.y - this->hitbox.size.y});
                    } else if(delta.y < 0)
                    {
                        delta.y = 0;
                        this->sprite.setPosition({this->sprite.getPosition().x, tilePos.y + TILE_SIZE * SCALE - (this->hitbox.size.y - this->envHitbox.size.y)});
                    }
                }
            }
        }
    }

    for(int i = 0; i < map->chunks2L.size(); i++)
    {
        const sf::Vector2f cp(map->chunks2L[i].position.x + (float)CHUNK_SIZE/2.0 * (float)SCALE, map->chunks2L[i].position.y + (float)CHUNK_SIZE/2.0 * (float)SCALE); // Chunk center positiom
        const sf::Vector2f dist = cp - this->sprite.getPosition(); // Player distance to chunk
        
        if(std::sqrt(dist.x*dist.x + dist.y*dist.y) < 2500) // Ignore chunks that are far away
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                for (int y = 0; y < CHUNK_SIZE; y++)
                {
                    if(!map->chunks2L[i].hitbox[y][x])
                        continue;

                    const sf::Vector2f tilePos(map->chunks2L[i].position.x + x * TILE_SIZE * SCALE, map->chunks2L[i].position.y + y * TILE_SIZE * SCALE);

                    const float tileLeft   = map->chunks2L[i].position.x + x * TILE_SIZE * SCALE;
                    const float tileTop    = map->chunks2L[i].position.y + y * TILE_SIZE * SCALE;
                    const float tileRight  = tileLeft + TILE_SIZE * SCALE;
                    const float tileBottom = tileTop  + TILE_SIZE * SCALE;

                    const float playerLeft   = this->envHitbox.position.x;
                    const float playerTop    = this->envHitbox.position.y;
                    const float playerRight  = playerLeft + this->envHitbox.size.x;
                    const float playerBottom = playerTop  + this->envHitbox.size.y;

                    if (playerRight <= tileLeft || playerLeft >= tileRight ||
                        playerBottom <= tileTop || playerTop >= tileBottom)
                        continue;

                    if(delta.y > 0)
                    {
                        delta.y = 0;
                        this->sprite.setPosition({this->sprite.getPosition().x, tilePos.y - this->hitbox.size.y});
                    } else if(delta.y < 0)
                    {
                        delta.y = 0;
                        this->sprite.setPosition({this->sprite.getPosition().x, tilePos.y + TILE_SIZE * SCALE - (this->hitbox.size.y - this->envHitbox.size.y)});
                    }
                }
            }
        }
    }

    this->hitbox.position = this->sprite.getPosition();
    this->hitbox.debugSprite.setPosition(this->hitbox.position);

    this->envHitbox.position = {this->hitbox.position.x, this->hitbox.position.y + this->sprite.getSize().y - this->envHitbox.size.y};
    this->envHitbox.debugSprite.setPosition(this->envHitbox.position);

    //for(int i = 0; i < Hitbox::solidHitboxPool.size(); i++)
    //{
        // Hitbox* hb = Hitbox::solidHitboxPool[i];
        // if(this->sprite.getPosition().y <= (hb->position.y + hb->size.y) && (this->sprite.getPosition().y + this->hitbox.size.y) >= hb->position.y)
        // {
        //     if(delta.x < 0 && this->sprite.getPosition().x < (hb->position.x + hb->size.x) && this->sprite.getPosition().x > hb->position.x)
        //         this->sprite.setPosition({hb->position.x + hb->size.x + 1, this->sprite.getPosition().y});
        //     if(delta.x > 0 && (this->sprite.getPosition().x + this->hitbox.size.x) > hb->position.x && this->sprite.getPosition().x < (hb->position.x + hb->size.x))
        //         this->sprite.setPosition({hb->position.x - this->hitbox.size.x - 1, this->sprite.getPosition().y});
        // }
        // if(this->sprite.getPosition().x <= (hb->position.x + hb->size.x) && (this->sprite.getPosition().x + this->hitbox.size.x) >= hb->position.x)
        // {
        //     if(delta.y < 0 && this->sprite.getPosition().y < (hb->position.y + hb->size.y) && this->sprite.getPosition().y > hb->position.y)
        //         this->sprite.setPosition({this->sprite.getPosition().x, hb->position.y + hb->size.y + 1});
        //     if(delta.y > 0 && (this->sprite.getPosition().y + this->hitbox.size.y) > hb->position.y && this->sprite.getPosition().y < (hb->position.y + hb->size.y))
        //         this->sprite.setPosition({this->sprite.getPosition().x, hb->position.y - this->hitbox.size.y - 1});
        // }
    //}

    this->hitbox.position = this->sprite.getPosition();
    this->hitbox.debugSprite.setPosition(this->hitbox.position);

    this->envHitbox.position = {this->hitbox.position.x, this->hitbox.position.y + this->sprite.getSize().y * 0.8f};
    this->envHitbox.debugSprite.setPosition(this->envHitbox.position);

    if (delta != sf::Vector2f{0.f, 0.f}) {
        if (stepCountdown <= 0.f) {
            stepCountdown = STEP_COUNTDOWN;
            AudioManager::playSound("footstep");
        } else {
            stepCountdown -= Physics::deltaTime;
        }
    } else {
        stepCountdown = 0.f;
    }

}