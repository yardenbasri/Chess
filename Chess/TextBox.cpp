#include "TextBox.h"

TextBox::TextBox()
{
}

TextBox::TextBox(int size, sf::Color color, bool sel)
{
	textbox.setCharacterSize(size);
	//textbox.setColor(color);
	textbox.setFillColor(color);
	isSelected = sel;
	if (sel)
		textbox.setString("_");
	else
		textbox.setString("");
}

void TextBox::SetFont(sf::Font& font)
{
	textbox.setFont(font);
}

void TextBox::SetPosition(sf::Vector2f pos)
{
	textbox.setPosition(pos);
}

void TextBox::SetLimit(bool hasLimit)
{
	this->hasLimit = hasLimit;
}

void TextBox::SetLimit(bool hasLimit, int limit)
{
	this->hasLimit = hasLimit;
	this->limit = limit;
}

void TextBox::SetSelected(bool selected)
{
	this->isSelected = selected;
	if (!selected)
	{
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length(); i++)
		{
			newT += t[i];
		}
		textbox.setString(newT);
	}
}

std::string TextBox::GetText()
{
	return text.str();
}

void TextBox::DrawTo(sf::RenderWindow& window)
{
	window.draw(textbox);
}

void TextBox::TypedOn(sf::Event input)
{
	if (isSelected)
	{
		int charTyped = input.text.unicode;
		if (charTyped < 128) {
			if (hasLimit)
			{
				if (text.str().length() < limit)
				{
					inputLogic(charTyped);
				}
				else if (text.str().length() >= limit && charTyped == DELETE_KEY)
				{
					deleteLastCharacter();
				}
			}
			else
				inputLogic(charTyped);

				
				
		}
	}
}

void TextBox::inputLogic(int charTyped)
{
	if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
		text << static_cast<char>(charTyped);
	else if (charTyped == DELETE_KEY)
	{
		if (text.str().length() > 0)
		{
			deleteLastCharacter();
		}
	}
	textbox.setString(text.str() + "_");
}

void TextBox::deleteLastCharacter()
{
	std::string t = text.str();
	std::string newT = "";
	for (int i = 0; i < t.length() - 1; i++)
	{
		newT += t[i];
	}
	text.str("");
	text << newT;

	textbox.setString(text.str());
}
