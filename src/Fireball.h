#ifndef FIREBALL_H
#define FIREBALL_H

#include<SFML/Graphics.hpp>

#include "Collider.h"

class Fireball : private Collider {
    int ownerPlayerNum;

    int power;
    bool toBeDestroyed;

    sf::Vector2f direction;

    public:
        Fireball(sf::Texture *tex, sf::IntRect texPos, int own, sf::Vector2f pos, sf::Vector2f dir, int startPow);
        ~Fireball();

        void update(std::vector<sf::Sprite> mountains, sf::Vector2u windowSize);

        sf::Sprite getSprite();

        int getOwnerPlayerNum();

        int getPower();

        bool getToBeDestroyed();

        void setToBeDestroyed(bool val);

};

#endif

