#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class ResourceManager {
private:
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, sf::Font> fonts;

public:
    void loadTextures(const std::unordered_map<std::string, std::string>& texturePaths);
    void loadFonts(const std::unordered_map<std::string, std::string>& fontPaths);

    std::shared_ptr<sf::Texture> getTexture(const std::string& name) const;
    const sf::Font& getFont(const std::string& name) const;
};

#endif // RESOURCE_MANAGER_HPP
