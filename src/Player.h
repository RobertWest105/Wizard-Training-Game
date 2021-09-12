#ifndef PLAYER_H
#define PLAYER_H

#include<SFML/Graphics.hpp>
#include<vector>

#include "Collider.h"
#include "Fireball.h"

class Fireball;

class Player : private Collider {

	int maxHP;
	int hp;
	bool dead;

	int playerNumber;

	sf::Text hpText;

	std::vector<Fireball> fireballs;
	int shootTimer;
	int ammo;

	public:
	    Player(sf::Texture* texture, sf::Font* textFont, sf::Vector2f pos, int playerNumber);

	    void shoot(sf::Texture* fireballTexture, sf::Vector2f direction);

	    void update(Player* enemy, std::vector<sf::Sprite> mountains, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Vector2u windowSize);

	    void checkFireballCollision(Fireball& fb);

	    bool checkMountainCollision();

	    void takeDamage(int dmg);

	    void destroyFireballs();

	    void reset(sf::Vector2f startPos);

	    void clampPosition(sf::Vector2u windowSize);

	    sf::Sprite getSprite();

	    std::vector<Fireball>& getFireballs();

	    int getPlayerNumber();

	    bool getDead();

	    sf::Text getHpText();

	    void setHpText();

};

#endif // PLAYER_H
