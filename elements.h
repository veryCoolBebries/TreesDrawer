//
// Created by Максим Лейхнер on 27.11.2022.
//
#pragma once
#include "SFML/Graphics.hpp"
#include <sstream>
using namespace sf;

#ifndef TREESDRAWER_ELEMENTS_H
#define TREESDRAWER_ELEMENTS_H

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Button {
public:
    Button(Texture * texture, float height);
    void setPosition(sf::Vector2f point);
    void drawTo(sf::RenderWindow &window);
    Vector2f getSize();
    bool isMouseOver(sf::RenderWindow &window);
private:
    sf::RectangleShape button;
};

class NumBox {
public:
    NumBox(Texture * texture, float height, Color textColor = Color().Black, bool sel = false);
    void setSelected(bool sel);
    void clear();
    int getNum();
    void setFont(sf::Font &fonts);
    void setLimit(bool ToF);
    void setLimit(bool ToF, int lim);
    void setPosition(sf::Vector2f point);
    void drawTo(sf::RenderWindow &window);
    Vector2f getSize();
    bool isMouseOver(sf::RenderWindow &window);
    void typedOn(sf::Event input);
    void setTexture(Texture * texture);

private:
    sf::Text textbox;
    std::ostringstream text;
    sf::RectangleShape background;
    bool isSelected = false;
    bool hasLimit = false;
    int limit = 0;
    void deleteLastChar();
    void inputLogic(int charTyped);
};

#endif //TREESDRAWER_ELEMENTS_H
