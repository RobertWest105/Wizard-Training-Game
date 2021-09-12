#include "Collider.h"

bool Collider::isColliding(sf::Vector2f pos, sf::Sprite& sp){
    sf::Vector2f otherPos = sp.getPosition();
    float widthOther = sp.getGlobalBounds().width;
    float heightOther = sp.getGlobalBounds().height;
    float width = this->sprite.getGlobalBounds().width;
    float height = this->sprite.getGlobalBounds().height;

    if(pos.x + width >= otherPos.x && pos.x <= otherPos.x + widthOther && pos.y + height >= otherPos.y && pos.y <= otherPos.y + heightOther){
        return true;
    }else return false;
}
