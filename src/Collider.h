#ifndef COLLIDER_H
#define COLLIDER_H

#include<SFML/Graphics.hpp>

class Collider {
    public:
        sf::Sprite sprite;

        Collider();
        ~Collider();

    protected:
        bool isColliding(sf::Vector2f pos, sf::Sprite& sp);

};

#endif

