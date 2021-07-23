#include "Fireball.h"

#include<SFML/Graphics.hpp>

Fireball::Fireball(sf::Texture *tex, sf::IntRect texPos, int own, sf::Vector2f pos, sf::Vector2f dir, int startPow){
    this->ownerPlayerNum = own;

    this->power = startPow;
    this->toBeDestroyed = false;

    sprite.setTexture(*tex);
    this->direction = dir;
    sprite.setPosition(pos);
    sprite.setTextureRect(texPos);
    sprite.setScale(0.75f, 0.75f);

    //Change fireball to blue if p1 fired it, red by default
    if(this->ownerPlayerNum == 1) {
        this->sprite.setColor(sf::Color::Blue);
    }
}

Fireball::~Fireball(){
    delete this;
}

void Fireball::update(std::vector<sf::Sprite> mountains, sf::Vector2u windowSize){
    sprite.move(direction);

    sf::Vector2f destination = sprite.getPosition() + direction;
    for(auto m : mountains){
        if(isColliding(destination, m)){
            //If fireball is colliding with a mountain, destroy it
            toBeDestroyed = true;
            break;
        }
    }

    //Reduce fireball power by 1 for each frame it exists and if power hits 0, destroy it
    this->power--;
    if (this->power <= 0) {
        this->toBeDestroyed = true;
    }

    //If fireball goes off-screen, destroy it
    if (this->sprite.getPosition().x < 0 || this->sprite.getPosition().y < 0 || this->sprite.getPosition().x > windowSize.x || this->sprite.getPosition().y > windowSize.y) {
        this->toBeDestroyed = true;
    }
}

sf::Sprite Fireball::getSprite(){
    return this->sprite;
}

int Fireball::getPower(){
    return this->power;
}

int Fireball::getOwnerPlayerNum(){
    return this->ownerPlayerNum;
}

bool Fireball::getToBeDestroyed(){
    return this->toBeDestroyed;
}

void Fireball::setToBeDestroyed(bool val){
    this->toBeDestroyed = val;
}

