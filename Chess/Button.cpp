#include "Button.h"

Button::Button() 
{
}

Button::Button(std::string text, int charSize, sf::Vector2f size, sf::Color color, sf::Color bgColor, sf::Vector2f position)
{
	this->text.setString(text);
	this->text.setFillColor(sf::Color::Black);
	this->text.setCharacterSize(charSize);
	this->button.setSize(size);
	this->button.setFillColor(bgColor);
	position.x =  (position.x  - this->button.getLocalBounds().width/2);
	position.y = (position.y + this->button.getLocalBounds().height / 2);
	this->button.setPosition(position);
	float xPos = (position.x + this->button.getGlobalBounds().width + position.x) / 2 - this->text.getString().getSize() * 4.5;
	float yPos = (position.y + this->button.getGlobalBounds().height + position.y) / 2 - 10;
	this->text.setPosition({ xPos, yPos });
	font.loadFromFile("Assets/arial.ttf");
	this->text.setFont(font);
	this->btnColor = bgColor;
}




void Button::SetBackColor(sf::Color color)
{
	this->button.setFillColor(color);
}

void Button::SetTextColor(sf::Color color)
{
	this->text.setFillColor(color);
}



void Button::DrawTo(sf::RenderWindow& window)
{
	window.draw(button);
	window.draw(text);
}

bool Button::IsMouseHover(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	if(this->button.getGlobalBounds().contains(sf::Vector2f(mousePos)))
		return true;
	return false;
}

void Button::HandleMouseHover(sf::RenderWindow& window)
{
	if (IsMouseHover(window))
	{
		this->button.setFillColor(sf::Color::White);
	}
	else
		this->button.setFillColor(btnColor);
		

}
