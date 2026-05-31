#include "core/Animation.hpp"

Animation::Animation()
{
    this->fps=0;
}

Animation::Animation(std::string spriteSheetPath, std::vector<int> animationLengths, int fps, sf::Vector2i spriteSize)
{
    this->fps = fps;
    if(!this->spriteSheet.loadFromFile(spriteSheetPath))
    {
        std::cout << "[ERR] Could not load sprite sheet " << spriteSheetPath << "\n";
    }
    this->animationLengths = animationLengths;
    this->spriteSize = spriteSize;
    this->timeSinceLastFrame = 0;

    this->setAnimationCycle(0);
}

void Animation::setAnimationCycle(int cycle)
{
    this->currentFrame = 0;
    this->currentCycle = cycle;
}

void Animation::update(sf::Sprite* targetSprite, float dt)
{
    if(targetSprite == nullptr)
        return;

    const float updateDelay = 1.0f / (float)this->fps;

    if(this->timeSinceLastFrame >= updateDelay)
    {
        int x = this->spriteSize.x * this->currentFrame;
        int y = this->spriteSize.y * this->currentCycle;

        targetSprite->setTextureRect(sf::IntRect({x, y}, {this->spriteSize.x, this->spriteSize.y}));

        this->currentFrame = (this->currentFrame + 1) % this->animationLengths[this->currentCycle];
        this->timeSinceLastFrame = 0;
    }

    this->timeSinceLastFrame += dt;
}

void Animation::setDefaultSprite(sf::Sprite *targetSprite)
{
    targetSprite->setTextureRect(sf::IntRect({0, 0}, {this->spriteSize.x, this->spriteSize.y}));
}