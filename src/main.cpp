#include<SFML/Graphics.hpp>
#include <iostream>

#include "Fireball.h"
#include "Player.h"

void game(){

}

int main()
{
    const int SCREEN_WIDTH = 512;
    const int WIDTH_TILES = SCREEN_WIDTH/32;
    const int SCREEN_HEIGHT = 512;
    const int HEIGHT_TILES = SCREEN_HEIGHT/32;

    const std::string level=
    "^^^^^^^^^^^^^^^^"
    "^..............^"
    "^...^^.........^"
    "^....^.........^"
    "^..............^"
    "^.......^......^"
    "^..............^"
    "^..............^"
    "^..............^"
    "^...^..........^"
    "^.......^^.....^"
    "^.......^^.....^"
    "^..............^"
    "^..............^"
    "^..............^"
    "^^^^^^^^^^^^^^^^";

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mage Training Grounds", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60);

    //Setup tiles
    sf::Texture mountainTexture;
    sf::Texture grassTexture;
    mountainTexture.loadFromFile("Images/Mountain.png");
    grassTexture.loadFromFile("Images/Grass.png");

    //Setup text font
	sf::Font font;
	font.loadFromFile("Images/Retro Gaming.ttf");
	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(16);
    gameOverText.setFillColor(sf::Color::White);

	//Setup players
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Images/Mage.png");
	sf::Vector2f p1StartPos = sf::Vector2f(50, (float)(window.getSize().y/2));
	sf::Vector2f p2StartPos = sf::Vector2f((float)(window.getSize().x - 3*playerTexture.getSize().x - 50), (float)(window.getSize().y/2));
	Player player1(&playerTexture, &font, p1StartPos, 1);
	Player player2(&playerTexture, &font, p2StartPos, 2);

	//Setup fireballs
	sf::Texture fireballsTexture;
	fireballsTexture.loadFromFile("Images/Fireball Spritesheet.png");

	auto getTile = [&](int x, int y){
        if(x >= 0 && x < WIDTH_TILES && y >= 0 && y < HEIGHT_TILES){
            return level[y*WIDTH_TILES + x];
        }else return ' ';
	};

	//Update window
	bool titleScreen = true;
	int winner = 0;
	while(window.isOpen()){
		sf::Event evnt;
		while(window.pollEvent(evnt)){
			switch(evnt.type){
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}

			if(titleScreen && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
                titleScreen = false;
			}else if(winner != 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
                //Restart game
                player1.reset(p1StartPos);
                player2.reset(p2StartPos);
                winner = 0;
			}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
                window.close();
			}
		}

		window.clear();

		if(titleScreen){
            sf::Text titleScreenText[3];

            titleScreenText[0].setString("WIZARD TRAINING");
            titleScreenText[0].setPosition(window.getSize().x/3, window.getSize().y/5);

            titleScreenText[1].setString("Controls:\n\nPlayer 1: WASD to move, Space to fire\nPlayer 2: Arrow Keys to move, Right Shift to fire\n\nPress Escape at any time to quit");
            titleScreenText[1].setPosition(window.getSize().x/18, 2*window.getSize().y/5);

            titleScreenText[2].setString("Press Enter to begin...");
            titleScreenText[2].setPosition(window.getSize().x/3, 4*window.getSize().y/5);

            for(sf::Text t : titleScreenText){
                t.setFont(font);
                t.setCharacterSize(16);
                t.setFillColor(sf::Color::White);
                window.draw(t);
            }
		}else{
            switch(winner){
            case 1:
            case 2:
                gameOverText.setString("Player " + std::to_string(winner) + " wins!\nPress enter to restart or escape to close");
                gameOverText.setPosition(window.getSize().x/10, window.getSize().y/2);
                window.draw(gameOverText);
                break;
            default:
                //Update game state
                std::vector<sf::Sprite> mountainSprites;

                //Draw tiles according to level
                for(int x = 0; x < WIDTH_TILES; x++){
                    for(int y = 0; y < HEIGHT_TILES; y++){
                        sf::Sprite tile;
                        bool isMountain = false;
                        switch(getTile(x, y)){
                        case '.':
                            tile.setTexture(grassTexture);
                            break;
                        case '^':
                            tile.setTexture(mountainTexture);
                            isMountain = true;
                            break;
                        default:
                            std::cout << "Tile not found" << std::endl;
                            break;
                        }
                        tile.setScale(2, 2);
                        tile.setPosition(x*32, y*32);
                        if(isMountain) mountainSprites.push_back(tile);
                        window.draw(tile);
                    }
                }

                if(!player1.getDead()){
                    player1.update(&player2, mountainSprites, sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::A, sf::Keyboard::Key::D, window.getSize());

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
                        //Check which side player 2 is on
                        if(player1.getSprite().getPosition().x < player2.getSprite().getPosition().x){ //Shoot right
                            player1.shoot(&fireballsTexture, sf::Vector2f(20.0f, 0.0f));
                        }
                        else if(player1.getSprite().getPosition().x > player2.getSprite().getPosition().x){ //Shoot left
                            player1.shoot(&fireballsTexture, sf::Vector2f(-20.0f, 0.0f));
                        }
                    }

                    window.draw(player1.getSprite());
                    window.draw(player1.getHpText());

                    for(int i = 0; i < player1.getFireballs().size(); i++){
                        window.draw(player1.getFireballs()[i].getSprite());
                    }
                }else{
                    //End game because player 2 won
                    winner = 2;
                }

                if(!player2.getDead()){
                    player2.update(&player1, mountainSprites, sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, window.getSize());

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)){
                        if(player1.getSprite().getPosition().x < player2.getSprite().getPosition().x){ //Shoot left
                            player2.shoot(&fireballsTexture, sf::Vector2f(-20.0f, 0.0f));
                        }
                        else if(player1.getSprite().getPosition().x > player2.getSprite().getPosition().x){ //Shoot right
                            player2.shoot(&fireballsTexture, sf::Vector2f(20.0f, 0.0f));
                        }
                    }

                    window.draw(player2.getSprite());
                    window.draw(player2.getHpText());

                    for(int i = 0; i < player2.getFireballs().size(); i++){
                        window.draw(player2.getFireballs()[i].getSprite());
                    }
                }else{
                    //End game because player 1 won
                    winner = 1;
                }
                break;
            }
		}

		window.display();

	}
    return 0;
}
