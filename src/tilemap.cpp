#include "Tilemap.hpp"

TileMap::TileMap(const std::string& texturePath, sf::Vector2u windowSize, unsigned int scaleFactor)
    : scaleFactor_(scaleFactor) {
    tileTexture_.loadFromFile(texturePath);

    tileTexture_.setRepeated(true);
    sf::Vector2u tileSize = tileTexture_.getSize() * scaleFactor_;

    tilemap_.setPrimitiveType(sf::Quads);

    int tilesX = windowSize.x / tileSize.x + 1;
    int tilesY = windowSize.y / tileSize.y + 1;

    for (int y = 0; y < tilesY; ++y) {
        for (int x = 0; x < tilesX; ++x) {
            sf::Vertex topLeft(sf::Vector2f(x * tileSize.x, y * tileSize.y), sf::Vector2f(0, 0));
            sf::Vertex topRight(sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y), sf::Vector2f(tileSize.x, 0));
            sf::Vertex bottomRight(sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y), sf::Vector2f(tileSize.x, tileSize.y));
            sf::Vertex bottomLeft(sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y), sf::Vector2f(0, tileSize.y));

            tilemap_.append(topLeft);
            tilemap_.append(topRight);
            tilemap_.append(bottomRight);
            tilemap_.append(bottomLeft);
        }
    }
}

void TileMap::draw(sf::RenderWindow& window) {
    sf::RenderStates states;
    states.texture = &tileTexture_;
    window.draw(tilemap_, states);
}
