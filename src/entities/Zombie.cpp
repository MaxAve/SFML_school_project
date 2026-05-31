#include "entities/Zombie.hpp"

std::vector<Zombie*> Zombie::pool;

float randomFloat(float, float);

Zombie::Zombie(sf::Vector2f position, Player* targetPlayer) : soundTimer{randomFloat(1.f, 10.f)}, sound(AudioManager::soundBuffer.at("zombie_1")), texture("resources/textures/spritesheets/zombie1.png"), sprite(texture) {
    // this->sprite = sf::RectangleShape(sf::Vector2f(50.f, 100.f));
    // this->sprite.setPosition(position);
    // this->sprite.setFillColor(sf::Color(255, 100, 100));
    // this->sprite.setOutlineThickness(2.0f);
    // this->sprite.setOutlineColor(sf::Color(100, 0, 0));

    this->speed = 100.0f + (float)(rand() % 50);
    this->velocity = sf::Vector2f(0, 0);
    this->displacementVelocity = sf::Vector2f(0, 0);
    this->bulletPushVelocity = sf::Vector2f(0, 0);
    this->targetPlayer = targetPlayer;
    this->healthBar = HealthBar(100);
    this->hitbox = Hitbox(sf::Vector2f(50.f, 100.f));
    this->hurtTimer = 0.0f;

    this->animation = Animation("resources/textures/spritesheets/zombie1.png", {4, 4, 1, 0}, 4, {32, 32});

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

void Zombie::updateAll(sf::Vector2f playerPos) {
    Zombie::pushAwayFromOthers();
    for (int i = 0; i < Zombie::pool.size(); i++) {
        Zombie::pool[i]->update(playerPos);
        if (Zombie::pool[i]->healthBar.currentHealth == 0) {
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

void Zombie::update(sf::Vector2f playerPos) {
    // Move towards player
    float angle = std::atan2(this->targetPlayer->sprite.getPosition().y - this->sprite.getPosition().y,
                             this->targetPlayer->sprite.getPosition().x - this->sprite.getPosition().x);
    this->velocity.x = std::cos(angle) * this->speed;
    this->velocity.y = std::sin(angle) * this->speed;

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
            this->animation.setAnimationCycle(1);
            this->sprite.setColor(sf::Color(255, 255, 255, 255));
        }
    }

    if (Utils::distance(this->sprite.getPosition(), this->targetPlayer->sprite.getPosition()) > 60.0f) {
        this->sprite.move({(this->velocity.x + this->displacementVelocity.x + this->bulletPushVelocity.x) * Physics::deltaTime,
                           (this->velocity.y + this->displacementVelocity.y + this->bulletPushVelocity.y) * Physics::deltaTime});
    }

    this->healthBar.setPosition({this->sprite.getPosition().x + 25, this->sprite.getPosition().y - 10});

    this->hitbox.position = this->sprite.getPosition();

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