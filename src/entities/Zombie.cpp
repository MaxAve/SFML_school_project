#include "entities/Zombie.hpp"

std::vector<Zombie*> Zombie::pool;

float randomFloat(float, float);

Zombie::Zombie(sf::Vector2f position, Player* targetPlayer) : soundTimer{randomFloat(1.f, 10.f)}, sound(AudioManager::soundBuffer.at("zombie_1")), texture("resources/textures/spritesheets/zombie1.png"), sprite(texture) {
    this->velocity = sf::Vector2f(0, 0);
    this->speed = 100.0f + (float)(rand() % 50);
    this->displacementVelocity = sf::Vector2f(0, 0);
    this->bulletPushVelocity = sf::Vector2f(0, 0);
    this->targetPlayer = targetPlayer;
    this->healthBar = HealthBar(100);
    this->hitbox = Hitbox(sf::Vector2f(50.f, 100.f));
    this->envHitbox = Hitbox(this->hitbox.position, {this->hitbox.size.x, this->hitbox.size.y * 0.1f});
    this->hurtTimer = 0.0f;
    this->isAggro = false;

    this->hitbox.debugSprite.setOutlineThickness(1);
    this->hitbox.debugSprite.setFillColor(sf::Color::Transparent);
    this->hitbox.debugSprite.setSize(this->hitbox.size);
    this->envHitbox.debugSprite.setOutlineThickness(1);
    this->envHitbox.debugSprite.setFillColor(sf::Color::Transparent);
    this->envHitbox.debugSprite.setSize(this->envHitbox.size);

    // Each zombie has a 20% chance to spawn as a fast zombie
    if(rand() % 5 != 0)
    {
        this->animation = Animation("resources/textures/spritesheets/zombie1.png", {4, 4, 1, 0}, 4, {32, 32});
    }
    else
    {
        this->animation = Animation("resources/textures/spritesheets/zombie2.png", {4, 4, 1, 0}, 8, {32, 32});
        this->speed *= 3.f;
        this->healthBar = HealthBar(200);
    }

    this->sprite = sf::Sprite(animation.spriteSheet);
    animation.setDefaultSprite(&(this->sprite));
    this->sprite.setPosition(position);
    this->sprite.setScale({4, 4});
    this->sprite.setOrigin({16, 0});

    animation.setAnimationCycle(1);

    Zombie::pool.push_back(this);

    sound.setRelativeToListener(true);
    sound.setMinDistance(200.f);
    sound.setAttenuation(4.f);
}

void Zombie::pushAwayFromOthers() {
    for (int i = 0; i < Zombie::pool.size(); i++) {
        Zombie::pool[i]->displacementVelocity.x = 0;
        Zombie::pool[i]->displacementVelocity.y = 0;
        for (int j = 0; j < Zombie::pool.size(); j++) {
            if (i == j)
                continue;

            float d = Utils::distance(Zombie::pool[i]->sprite.getPosition(), Zombie::pool[j]->sprite.getPosition());
            if (d < MIN_CROWD_DISTANCE) {
                float angle = std::atan2(Zombie::pool[i]->sprite.getPosition().y - Zombie::pool[j]->sprite.getPosition().y,
                                         Zombie::pool[i]->sprite.getPosition().x - Zombie::pool[j]->sprite.getPosition().x);
                Zombie::pool[i]->displacementVelocity.x += std::cos(angle) * std::pow(MIN_CROWD_DISTANCE - d, 2) * CROWD_DISPLACMENET_STRENGTH;
                Zombie::pool[i]->displacementVelocity.y += std::sin(angle) * std::pow(MIN_CROWD_DISTANCE - d, 2) * CROWD_DISPLACMENET_STRENGTH;
            }
        }
    }
}

void Zombie::updateAll(sf::Vector2f playerPos, const GameMap* map) {
    Zombie::pushAwayFromOthers();
    for (int i = 0; i < Zombie::pool.size(); i++) {
        Zombie::pool[i]->update(playerPos, map);
        if (Zombie::pool[i]->healthBar.currentHealth <= 0) {
            Particle::spawnBloodParticles(Zombie::pool[i]->sprite.getPosition(), 35, 500);
            Zombie::pool.erase(Zombie::pool.begin() + i);
            i--;
        }
    }
}

void Zombie::drawAll(sf::RenderWindow& window) {
    for (int i = 0; i < Zombie::pool.size(); i++) {
        Zombie::pool[i]->draw(window);
    }
}

void Zombie::move(sf::Vector2f delta, const GameMap* map)
{
    this->sprite.move({delta.x, 0});

    this->hitbox.position = {this->sprite.getPosition().x - 16, this->sprite.getPosition().y + 16};
    this->hitbox.debugSprite.setPosition(this->hitbox.position);

    this->envHitbox.position = {this->hitbox.position.x, this->hitbox.position.y + this->hitbox.size.y - this->envHitbox.size.y};
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
                        this->sprite.setPosition({tilePos.x - this->hitbox.size.x + 16, this->sprite.getPosition().y});
                    } else if(delta.x < 0)
                    {
                        delta.x = 0;
                        this->sprite.setPosition({tilePos.x + TILE_SIZE * SCALE + 16, this->sprite.getPosition().y});
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
                        this->sprite.setPosition({tilePos.x - this->hitbox.size.x + 16, this->sprite.getPosition().y});
                    } else if(delta.x < 0)
                    {
                        delta.x = 0;
                        this->sprite.setPosition({tilePos.x + TILE_SIZE * SCALE + 16, this->sprite.getPosition().y});
                    }
                }
            }
        }
    }

    // TODO this is scuffed
    this->sprite.move({0, delta.y});

    this->hitbox.position = {this->sprite.getPosition().x - 16, this->sprite.getPosition().y + 16};
    this->hitbox.debugSprite.setPosition(this->hitbox.position);

    this->envHitbox.position = {this->hitbox.position.x, this->hitbox.position.y + this->hitbox.size.y - this->envHitbox.size.y};
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
                        this->sprite.setPosition({this->sprite.getPosition().x, tilePos.y - this->hitbox.size.y - 16});
                    } else if(delta.y < 0)
                    {
                        delta.y = 0;
                        this->sprite.setPosition({this->sprite.getPosition().x, tilePos.y + TILE_SIZE * SCALE - (this->hitbox.size.y - this->envHitbox.size.y) - 16});
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
                        this->sprite.setPosition({this->sprite.getPosition().x, tilePos.y - this->hitbox.size.y - 16});
                    } else if(delta.y < 0)
                    {
                        delta.y = 0;
                        this->sprite.setPosition({this->sprite.getPosition().x, tilePos.y + TILE_SIZE * SCALE - (this->hitbox.size.y - this->envHitbox.size.y) - 16});
                    }
                }
            }
        }
    }

    this->hitbox.position = {this->sprite.getPosition().x - 16, this->sprite.getPosition().y + 16};
    this->hitbox.debugSprite.setPosition(this->hitbox.position);

    this->envHitbox.position = {this->hitbox.position.x, this->hitbox.position.y + this->hitbox.size.y - this->envHitbox.size.y};
    this->envHitbox.debugSprite.setPosition(this->envHitbox.position);

    this->healthBar.setPosition({this->sprite.getPosition().x + 25, this->sprite.getPosition().y - 10});
}

void Zombie::update(sf::Vector2f playerPos, const GameMap* map) {
    // Move towards player
    const sf::Vector2f dist = this->sprite.getPosition() - playerPos;
    if(std::sqrt(dist.x*dist.x + dist.y*dist.y) < 666 || this->isAggro)
    {
        float angle = std::atan2(this->targetPlayer->sprite.getPosition().y - this->sprite.getPosition().y,
                                this->targetPlayer->sprite.getPosition().x - this->sprite.getPosition().x);
        this->velocity.x = std::cos(angle) * this->speed;
        this->velocity.y = std::sin(angle) * this->speed;

        if(this->animation.getCurrentCycle() == 0 && this->hurtTimer <= 0.0001f)
            this->animation.setAnimationCycle(1);

        this->isAggro = true;
    }
    else
    {
        this->velocity.x = 0;
        this->velocity.y = 0;
        if(this->animation.getCurrentCycle() != 0)
            this->animation.setAnimationCycle(0);
    }

    this->bulletPushVelocity.x /= (1.0f + 5.f * Physics::deltaTime);
    if (std::abs(this->bulletPushVelocity.x) < 0.01f)
        this->bulletPushVelocity.x = 0.0f;
    this->bulletPushVelocity.y /= (1.0f + 5.f * Physics::deltaTime);
    if (std::abs(this->bulletPushVelocity.y) < 0.01f)
        this->bulletPushVelocity.y = 0.0f;

    if(this->hurtTimer > 0.0001f)
    {
        this->velocity.x = 0;
        this->velocity.y = 0;
        this->animation.setAnimationCycle(2);
        this->hurtTimer -= Physics::deltaTime;
        this->timeSinceBlink -= Physics::deltaTime;
        if(this->timeSinceBlink <= 0.0001f)
        {
            this->timeSinceBlink = 0.05f;
            if(this->sprite.getColor().a == 255)
                this->sprite.setColor(sf::Color(255, 255, 255, 100));
            else
                this->sprite.setColor(sf::Color(255, 255, 255, 255));
        }
        if(this->hurtTimer <= 0.0001f)
        {
            if(attackTimer > 0.0f)
                this->animation.setAnimationCycle(0);
            else
                this->animation.setAnimationCycle(1);
            this->sprite.setColor(sf::Color(255, 255, 255, 255));
        }
    }

    if (Utils::distance(this->sprite.getPosition(), this->targetPlayer->sprite.getPosition()) > 60.0f && attackTimer < 0.001f) {
        this->move({
            (this->velocity.x + this->displacementVelocity.x + this->bulletPushVelocity.x) * Physics::deltaTime,
            (this->velocity.y + this->displacementVelocity.y + this->bulletPushVelocity.y) * Physics::deltaTime}, map);
    }
    else
    {
        if(attackTimer < 0.001f)
        {
            this->targetPlayer->hpbar->changeHealth(-12);
            attackTimer = 2.0f;
            this->animation.setAnimationCycle(0);
        }
    }

    if(attackTimer > 0.0f)
    {
        attackTimer -= Physics::deltaTime;
        if(attackTimer <= 0.0f)
            this->animation.setAnimationCycle(1);
    }

    soundTimer -= Physics::deltaTime;
    if (soundTimer <= 0.f) {
        makeRandomSound(playerPos);
        soundTimer = randomFloat(5.f, 15.0f);
    }

    this->animation.update(&(this->sprite), Physics::deltaTime);

    if(this->targetPlayer->sprite.getPosition().x < this->sprite.getPosition().x)
        this->sprite.setScale({-std::abs(this->sprite.getScale().x), this->sprite.getScale().y});
    else
        this->sprite.setScale({std::abs(this->sprite.getScale().x), this->sprite.getScale().y});
}

void Zombie::makeRandomSound(sf::Vector2f playerPos) {
    // bit faster than vector
    std::array<std::string, 2> sounds = {
        "zombie_1",
        "zombie_2"};
    size_t randIdx = static_cast<size_t>(randomFloat(0.f, static_cast<float>(sounds.size())));

    sf::Vector2f delta = hitbox.position - playerPos;

    sound.setPosition({delta.x, delta.y, 0.f});

    sound.setBuffer(AudioManager::soundBuffer.at(sounds[randIdx]));
    sound.play();
}

void Zombie::draw(sf::RenderWindow& window) {
    window.draw(this->sprite);
    this->healthBar.draw(window);
}

sf::Vector2f Zombie::getHitboxPosition() {
    // TODO improve later
    return {64 + this->sprite.getPosition().x, 64 + this->sprite.getPosition().y};
}