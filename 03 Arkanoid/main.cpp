#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream> // For converting score to string

using namespace sf;

enum State { MENU, GAME, ABOUT };

int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(520, 450), "Arkanoid Pro!");
    app.setFramerateLimit(60);

    // Load Textures
    Texture t1, t2, t3, t4, tMenu;
    t1.loadFromFile("images/block01.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/ball.png");
    t4.loadFromFile("images/paddle.png");
    tMenu.loadFromFile("images/menu.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4), sMenu(tMenu);
    sMenu.setScale(520.f / tMenu.getSize().x, 450.f / tMenu.getSize().y);
    sPaddle.setPosition(300, 440);

    // Font and Text for Score
    Font font;
    font.loadFromFile("fonts/arial.ttf"); // Make sure you have a font file!
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);

    Sprite block[1000];
    int n = 0;
    int score = 0;

    // Initialize Blocks
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++) {
            block[n].setTexture(t1);
            block[n].setPosition(i * 43, j * 20);
            n++;
        }

    float dx = 6, dy = 5;
    float x = 300, y = 300;
    
    State currentState = MENU;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
            
            // Menu Navigation
            if (e.type == Event::KeyPressed) {
                if (currentState == MENU) {
                    if (e.key.code == Keyboard::P || e.key.code == Keyboard::S) currentState = GAME;
                    if (e.key.code == Keyboard::E) app.close();
                } 
                else {
                    // During Game: ESC or M goes back to Menu
                    if (e.key.code == Keyboard::Escape || e.key.code == Keyboard::M) {
                        currentState = MENU;
                    }
                }
            }
        }

        if (currentState == GAME) {
            // --- Game Logic ---
            x += dx;
            for (int i = 0; i < n; i++) {
                if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds())) {
                    block[i].setPosition(-100, 0); // Move block away
                    dx = -dx;
                    score += 10; // Increase score
                }
            }

            y += dy;
            for (int i = 0; i < n; i++) {
                if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds())) {
                    block[i].setPosition(-100, 0);
                    dy = -dy;
                    score += 10; // Increase score
                }
            }

            if (x < 0 || x > 520) dx = -dx;
            if (y < 0 || y > 450) dy = -dy;

            if (Keyboard::isKeyPressed(Keyboard::Right)) sPaddle.move(6, 0);
            if (Keyboard::isKeyPressed(Keyboard::Left)) sPaddle.move(-6, 0);

            if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) 
                dy = -(rand() % 5 + 2);

            sBall.setPosition(x, y);

            // Update Score Text
            std::stringstream ss;
            ss << "Score: " << score;
            scoreText.setString(ss.str());
        }

        // --- Drawing ---
        app.clear();

        if (currentState == MENU) {
            app.draw(sMenu);
        }
        else if (currentState == GAME) {
            app.draw(sBackground);
            app.draw(sBall);
            app.draw(sPaddle);
            for (int i = 0; i < n; i++) app.draw(block[i]);
            app.draw(scoreText); // Draw score on top of game
        }

        app.display();
    }

    return 0;
}
