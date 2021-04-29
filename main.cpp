#include <iostream>
#include <random>
#include <ctime>
#include <vector>

#include <SFML/Graphics.hpp>

const int CELL_W = 10;
const int CELL_H = 10;
const int WINDOW_W = 1280;
const int WINDOW_H = 720;

struct Tile
{
    Tile(const sf::Vector2f& pos);
    sf::RectangleShape mShape;
    bool mAlive = false;
};

void DrawTiles(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles, sf::RenderWindow& window);
void Generate30(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles, const int& currentColumn);
void Generate110(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles, const int& currentColumn);
void RandomizeRow(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles);
void Reset(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles);

int main()
{
    srand((unsigned)time(0));
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Cellular Automata");

    sf::View view(sf::FloatRect(0, 0, WINDOW_W, WINDOW_H));
    window.setFramerateLimit(60);

    const auto white = sf::Color(255, 255, 255, 255);

    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    
    for (size_t i = 0; i < WINDOW_H; i += CELL_H)
    {
        std::vector<std::shared_ptr<Tile>> row;

        for (size_t j = 0; j < WINDOW_W; j += CELL_W)
        {
            row.push_back(std::make_shared<Tile>(sf::Vector2f(j, i)));
        }

        tiles.push_back(row);
    }
    
    RandomizeRow(tiles);

    int row = 0;
    int generation = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (row == (WINDOW_H / CELL_H) - 1)
                {
                    Reset(tiles);
                    row = 0;
                    RandomizeRow(tiles);
                }
                
                if (event.key.code == sf::Keyboard::Q)
                {
                    Generate110(tiles, row);
                    row++;
                }

                if (event.key.code == sf::Keyboard::W)
                {
                    Generate30(tiles, row);
                    row++;
                }

                if (event.key.code == sf::Keyboard::R)
                {
                    Reset(tiles);
                }

                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {


                for (auto &row : tiles)
                {
                    for (auto &col : row)
                    {
                        auto cord = window.mapCoordsToPixel(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                        if (col->mShape.getGlobalBounds().contains(sf::Vector2f(cord.x, cord.y)))
                        {
                            col->mAlive = true;
                        }
                    }
                }
            }
        }


        window.clear(white);
        
        DrawTiles(tiles, window);
        
        window.display();

        
    }

	
	return 0;
}

Tile::Tile(const sf::Vector2f& pos)
{
    mShape.setSize(sf::Vector2f(CELL_W, CELL_H));
    mShape.setFillColor(sf::Color::Red);
    mShape.setOutlineColor(sf::Color::Black);
    mShape.setOutlineThickness(1.0f);
    mShape.setPosition(pos);
}

void DrawTiles(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles, sf::RenderWindow& window)
{
    for (size_t i = 0; i < WINDOW_H / CELL_H; i++)
    {
        for (size_t j = 0; j < WINDOW_W / CELL_W; j++)
        {
            if (tiles[i][j]->mAlive)
            {
                tiles[i][j]->mShape.setFillColor(sf::Color::White);
            }
            else
            {
                tiles[i][j]->mShape.setFillColor(sf::Color::Red);
            }

            window.draw(tiles[i][j]->mShape);
        }
    }
}

void Generate30(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles, const int& row)
{
    for (size_t i = row; i < row + 1; i++)
    {
        for (size_t j = 0; j < (WINDOW_W / CELL_W); j++)
        {
            int left = 0, right = 0;

            if (j == 0)
            {
                left = (WINDOW_W / CELL_W) - 1;
                right = j + 1;
            }
            else if (j == (WINDOW_W / CELL_W) - 1)
            {
                left = j - 1;
                right = 0;
            }
            else
            {
                left = j - 1;
                right = j + 1;
            }

            if (tiles[i][j]->mAlive && tiles[i][right]->mAlive && tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = false;
            }
            else if (tiles[i][j]->mAlive && !tiles[i][right]->mAlive && tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = false;
            }
            else if (!tiles[i][j]->mAlive && tiles[i][right]->mAlive && tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = false;
            }
            else if (!tiles[i][j]->mAlive && !tiles[i][right]->mAlive && tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (tiles[i][j]->mAlive && tiles[i][right]->mAlive && !tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (tiles[i][j]->mAlive && !tiles[i][right]->mAlive && !tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (!tiles[i][j]->mAlive && tiles[i][right]->mAlive && !tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (!tiles[i][j]->mAlive && !tiles[i][right]->mAlive && !tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = false;
            }
        }
    }
}

void Generate110(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles, const int& row)
{
    for (size_t i = row; i < row + 1; i++)
    {
        for (size_t j = 0; j < (WINDOW_W / CELL_W); j++)
        {
            int left = 0, right = 0;

            if (j == 0)
            {
                left = (WINDOW_W / CELL_W) - 1;
                right = j + 1;
            }
            else if (j == (WINDOW_W / CELL_W) - 1)
            {
                left = j - 1;
                right = 0;
            }
            else
            {
                left = j - 1;
                right = j + 1;
            }

            if (tiles[i][j]->mAlive && tiles[i][right]->mAlive && tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = false;
            }
            else if (tiles[i][j]->mAlive && !tiles[i][right]->mAlive && tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (!tiles[i][j]->mAlive && tiles[i][right]->mAlive && tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (!tiles[i][j]->mAlive && !tiles[i][right]->mAlive && tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = false;
            }
            else if (tiles[i][j]->mAlive && tiles[i][right]->mAlive && !tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (tiles[i][j]->mAlive && !tiles[i][right]->mAlive && !tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (!tiles[i][j]->mAlive && tiles[i][right]->mAlive && !tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = true;
            }
            else if (!tiles[i][j]->mAlive && !tiles[i][right]->mAlive && !tiles[i][left]->mAlive)
            {
                tiles[i + 1][j]->mAlive = false;
            }
        }
    }
}

void RandomizeRow(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles)
{
    for (size_t j = 0; j < (WINDOW_W / CELL_W) ; j++)
    {
        auto n = (rand() % 100) ;
        (n % 2 == 0) ? tiles[0][j]->mAlive = true : tiles[0][j]->mAlive = false;
    }
    
}

void Reset(std::vector<std::vector<std::shared_ptr<Tile>>>& tiles)
{
    for (size_t i = 0; i < (WINDOW_H / CELL_H); i++)
    {
        for (size_t j = 0; j < (WINDOW_W / CELL_W); j++)
        {
            tiles[i][j]->mAlive = false;
        }
    }
}
