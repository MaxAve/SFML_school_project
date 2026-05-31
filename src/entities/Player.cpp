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
    this->envHitbox = Hitbox(this->sprite.getPosition(), {this->sprite.getSize().x, this->sprite.getSize().y / 5.f});
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
    this->sprite.move(delta);

    for(int i = 0; i < map->chunks1L.size(); i++)
    {
        sf::Vector2f cp(map->chunks1L[i].position.x + (float)CHUNK_SIZE/2.0 * (float)SCALE, map->chunks1L[i].position.y + (float)CHUNK_SIZE/2.0 * (float)SCALE);
        sf::Vector2f dist = cp - this->sprite.getPosition();
        
        if(std::sqrt(dist.x*dist.x + dist.y*dist.y) < 2500)
        {
            bool collision = false;

            for(int x = 0; x < CHUNK_SIZE; x++)
            {
                if(collision)
                    break;
                for(int y = 0; y < CHUNK_SIZE; y++)
                {
                    if(collision)
                        break;

                    if(map->chunks1L[i].hitbox[y][x])
                    {
                        const sf::Vector2f p(map->chunks1L[i].position.x + x * TILE_SIZE * SCALE, map->chunks1L[i].position.y + y * TILE_SIZE * SCALE);

                        if((this->envHitbox.position.x + this->envHitbox.size.x) >= p.x && (this->envHitbox.position.x + this->envHitbox.size.x) <= (p.x + TILE_SIZE * SCALE)
                        && (this->envHitbox.position.y + this->envHitbox.size.y) >= p.y && this->envHitbox.position.y <= (p.y + TILE_SIZE * SCALE)
                        )
                        {
                            if(delta.x > 0)
                            {
                                this->sprite.setPosition({p.x - this->envHitbox.size.x, this->sprite.getPosition().y});
                                collision = true;
                            }
                            if(delta.y > 0)
                            {
                                this->sprite.setPosition({this->sprite.getPosition().x, p.y - this->sprite.getSize().y});
                                collision = true;
                            }
                            if(delta.x < 0)
                            {
                                this->sprite.setPosition({p.x, this->sprite.getPosition().y});
                                collision = true;
                            }
                            if(delta.y < 0)
                            {
                                this->sprite.setPosition({this->sprite.getPosition().x, p.y});
                                collision = true;
                            }
                        }
                    }
                }
            }
        }
    }

    // for(int i = 0; i < map->chunks2L.size(); i++)
    // {
    //     sf::Vector2f cp(map->chunks2L[i].position.x + (float)CHUNK_SIZE/2.0 * (float)SCALE, map->chunks2L[i].position.y + (float)CHUNK_SIZE/2.0 * (float)SCALE);
    //     sf::Vector2f dist = cp - this->sprite.getPosition();
    //     if(std::sqrt(dist.x*dist.x + dist.y*dist.y) < 2500)
    //     {
    //         for(int x = 0; x < CHUNK_SIZE; x++)
    //         {
    //             for(int y = 0; y < CHUNK_SIZE; y++)
    //             {
    //                 if(map->chunks1L[i].hitbox[y][x])
    //                 {
    //                     Hitbox hb(sf::Vector2f(map->chunks1L[i].position.x + (float)(x * TILE_SIZE * SCALE), map->chunks1L[i].position.y  + (float)(y * TILE_SIZE * SCALE)), sf::Vector2f(TILE_SIZE * SCALE, TILE_SIZE * SCALE));

    //                     if(this->envHitbox.touching(&hb))
    //                     {
    //                         std::cout << "waaaaa " << x << "\n";
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }

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