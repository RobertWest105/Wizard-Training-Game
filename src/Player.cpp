#include "Player.h"

#include<iostream>

Player::Player(sf::Texture* texture, sf::Font* textFont, sf::Vector2f pos, int playerNumber){
    this->maxHP = 20;
    this->hp = this->maxHP;
    this->hpText.setFont(*textFont);
    setHpText();
    this->hpText.setCharacterSize(16);
    this->hpText.setFillColor(sf::Color::White);

    this->dead = false;
    this->shootTimer = 40; //<40 means not ready to fire, >=40 means ready
    this->ammo = 5;

    this->sprite.setTexture(*texture);
    this->sprite.setScale(2, 2);
    this->sprite.setPosition(pos);
    this->hpText.setPosition(pos.x, pos.y + this->sprite.getGlobalBounds().height);

    this->playerNumber = playerNumber;
    if(this->playerNumber == 2) {
        this->sprite.setColor(sf::Color::Red);
    }
}

Player::~Player(){

}

void Player::shoot(sf::Texture* fireballTexture, sf::Vector2f direction){
    int startPower = 10;
    if (this->ammo > 0) {
        if (this->shootTimer >= 40) {
            //Choose which part of fireball spritesheet to use based on direction, then create it
            sf::Vector2u fireballSize = (*fireballTexture).getSize();
            fireballSize.x /= 8;
            fireballSize.y /= 8;
            sf::IntRect chosenFireball;
				if(direction.x > 0){
					chosenFireball = sf::IntRect(fireballSize.x*0, fireballSize.y*4, fireballSize.x, fireballSize.y);
				}
				else {
					chosenFireball = sf::IntRect(0, 0, fireballSize.x, fireballSize.y);
				}
				this->fireballs.push_back(new Fireball(fireballTexture, chosenFireball, this->playerNumber, this->sprite.getPosition(), direction, startPower));

				this->ammo--;
				this->shootTimer = 0;
			}
		}
		else this->ammo = 5;
}

void Player::update(Player* enemy, std::vector<sf::Sprite> mountains, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Vector2u windowSize){
    //Destroy any fireballs that're to be destroyed
    this->destroyFireballs();

    //Move
    int moveSpeed = 50;
    sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f); //diagonal movement is faster due to adding offsets
    if (sf::Keyboard::isKeyPressed(up)) {
        offset += sf::Vector2f(0.0f*moveSpeed, -0.1f*moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(right)) {
        offset += sf::Vector2f(0.1f*moveSpeed, 0.0f*moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(left)) {
        offset += sf::Vector2f(-0.1f*moveSpeed, 0.0f*moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(down)) {
        offset += sf::Vector2f(0.0f*moveSpeed, 0.1f*moveSpeed);
    }
    sf::Vector2f destination = this->sprite.getPosition() + offset;
    //if destination is not inside any mountain sprite, do the move, else don't
    bool invalidMove = false;
    for(auto m : mountains){
        invalidMove = invalidMove || isColliding(destination, m);
    }
    if(!invalidMove){
        this->sprite.move(offset);
        this->clampPosition(windowSize);
    }

    //Update HP text position
    this->hpText.setPosition(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height);

    //Less time to next shot
    if (this->shootTimer < 40) {
        this->shootTimer++;
    }

    //Update fireballs
    for (int i = 0; i < this->fireballs.size(); i++) {
        this->fireballs[i]->update(mountains, windowSize);
    }

    for(int i = 0; i < enemy->getFireballs().size(); i++){
        checkFireballCollision(enemy->getFireballs()[i]); //must only check collision with enemy's fireballs when they exist!
    }
}

void Player::checkFireballCollision(Fireball* fb){
    //First check that this player isn't the owner of this fireball
	if (fb->getOwnerPlayerNum() != this->playerNumber) {
        sf::Sprite fireballSprite = fb->getSprite();
		//If given fireball collides with this player, take damage equal to power
		if (isColliding(this->sprite.getPosition(), fireballSprite)) {
			takeDamage(fb->getPower());
			fb->setToBeDestroyed(true);
		}
	}
}

void Player::takeDamage(int dmg){
    this->hp -= dmg;
    //Tell players how much damage they took - To do: IN GAME
    //std::cout << "Player " << this->getPlayerNumber() << " took " << dmg << " damage!" << std::endl;
    //std::cout << "Player " << this->getPlayerNumber() << " now has " << this->hp << "HP" << std::endl;
    if (this->hp <= 0) {
        this->dead = true;
        std::cout << "Player " << this->getPlayerNumber() << " is dead!" << std::endl;
        //To do: remove player sprite and show win screen
    }else setHpText();
}

void Player::destroyFireballs(){
    for (int i = 0; i < this->fireballs.size(); i++) {
        if (this->fireballs[i]->getToBeDestroyed()) {
            this->fireballs.erase(this->fireballs.begin() + i);
        }
    }
}

void Player::reset(sf::Vector2f startPos){
    this->dead = false;
    this->hp = this->maxHP;
    this->shootTimer = 40;
    this->ammo = 5;
    this->fireballs.clear();
    setHpText();
    this->sprite.setPosition(startPos);
    this->hpText.setPosition(startPos.x, startPos.y + sprite.getGlobalBounds().height);
}

void Player::clampPosition(sf::Vector2u windowSize){
    if (this->sprite.getPosition().x < 0) { //Gone off left
        this->sprite.setPosition(0, this->sprite.getPosition().y);
    }
    if (this->sprite.getPosition().y < 0) { //Gone off up
        this->sprite.setPosition(this->sprite.getPosition().x, 0);
    }
    if (this->sprite.getPosition().x + std::max(this->sprite.getGlobalBounds().width, this->hpText.getGlobalBounds().width) > windowSize.x) { //Gone off right
        this->sprite.setPosition(windowSize.x - std::max(this->sprite.getGlobalBounds().width, this->hpText.getGlobalBounds().width), this->sprite.getPosition().y);
    }
    if (this->sprite.getPosition().y + this->sprite.getGlobalBounds().height + this->hpText.getCharacterSize() > windowSize.y) { //Gone off down
        this->sprite.setPosition(this->sprite.getPosition().x, windowSize.y - this->sprite.getGlobalBounds().height - this->hpText.getCharacterSize());
    }
}

sf::Sprite Player::getSprite(){
    return this->sprite;
}

std::vector<Fireball*> Player::getFireballs(){
    return this->fireballs;
}

int Player::getPlayerNumber(){
    return this->playerNumber;
}

bool Player::getDead(){
    return this->dead;
}

sf::Text Player::getHpText(){
    return this->hpText;
}

void Player::setHpText(){
    this->hpText.setString("HP: " + std::to_string(this->hp));
}
