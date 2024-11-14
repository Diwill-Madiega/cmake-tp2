#include <SFML/Graphics.hpp>

class Circle {
public:
    // Constructor that takes radius and color
    Circle(float radius, const sf::Color& color) {
        m_circle.setRadius(radius);
        m_circle.setFillColor(color);
        m_circle.setOrigin(radius, radius);  // Set origin to the center for easier positioning
    }

    // Function to set the position of the circle
    void setPosition(float x, float y) {
        m_circle.setPosition(x, y);
    }

    // Function to draw the circle on the window
    void draw(sf::RenderWindow& window) {
        window.draw(m_circle);
    }

private:
    sf::CircleShape m_circle;
};

