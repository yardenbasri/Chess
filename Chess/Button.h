#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

class Button
{
public:
	Button();
	Button(std::string text, int charSize, sf::Vector2f size, sf::Color color, sf::Color bgColor, sf::Vector2f position );
	
	void SetBackColor(sf::Color color);
	void SetTextColor(sf::Color color);
	
	void DrawTo(sf::RenderWindow& window);
	bool IsMouseHover(sf::RenderWindow& window);
	void HandleMouseHover(sf::RenderWindow& window);

	
private:
	sf::Text text;
	sf::RectangleShape button;
	sf::Font font;
	sf::Color btnColor;
	

};
