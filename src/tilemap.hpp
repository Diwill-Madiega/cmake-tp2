#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>

class TileMap {
public:
    TileMap(const std::string& texturePath, sf::Vector2u windowSize, unsigned int scaleFactor);

    void draw(sf::RenderWindow& window);

private:
    sf::Texture tileTexture_;
    sf::VertexArray tilemap_;
    unsigned int scaleFactor_;
};

#endif // TILEMAP_HPP
