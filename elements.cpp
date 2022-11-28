#include <SFML/Graphics.hpp>
#include <iostream>
#include "elements.h"
Button::Button(Texture * texture, float height) {
    float scale = height / texture->getSize().y;
    button.setSize(Vector2f(texture->getSize().x * scale, texture->getSize().y * scale));
    button.setTexture(texture);
}
void Button::setPosition(sf::Vector2f point) {
    button.setPosition(point);
}
void Button::drawTo(sf::RenderWindow &window) {
    window.draw(button);
}
Vector2f Button::getSize() {
    return button.getSize();
}
bool Button::isMouseOver(sf::RenderWindow &window) {
    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;

    int btnPosX = button.getPosition().x;
    int btnPosY = button.getPosition().y;

    int btnxPosWidth = button.getPosition().x + button.getSize().x;
    int btnyPosHeight = button.getPosition().y + button.getSize().y;

    if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
        return true;
    }
    return false;
}
NumBox::NumBox(Texture * texture, float height, Color textColor, bool sel) {
    float scale = height / texture->getSize().y;
    background.setSize(Vector2f(texture->getSize().x * scale, texture->getSize().y * scale));
    background.setTexture(texture);
    textbox.setCharacterSize(height/2);
    textbox.setColor(textColor);
    isSelected = sel;
    if(isSelected)
        textbox.setString("|");
    else
        textbox.setString("");
}
void NumBox::setSelected(bool sel) {
    isSelected = sel;

    if (!sel) {
        std::string t = text.str();
        std::string newT = "";
        for (int i = 0; i < t.length(); i++) {
            newT += t[i];
        }
        textbox.setString(newT);
    }
}
void NumBox::clear() {
    textbox.setString("");
    text.str("");
}
int NumBox::getNum() {
    int res = 0;
    bool negative = false;
    for(char letter : text.str()){
        if (letter == 45) negative = true;
        else
            res = res * 10 + letter - 48;
    }
    if(negative) res = res * -1;
    return res;
}
void NumBox::setFont(sf::Font &fonts) {
    textbox.setFont(fonts);
}
void NumBox::setLimit(bool ToF) {
    hasLimit = ToF;
}
void NumBox::setLimit(bool ToF, int lim) {
    hasLimit = ToF;
    limit = lim - 1;
}
void NumBox::setPosition(sf::Vector2f point) {
    background.setPosition(point);
    textbox.setPosition(Vector2f(point.x + 10, point.y + background.getSize().y / 5));
}
void NumBox::drawTo(sf::RenderWindow &window) {
    window.draw(background);
    window.draw(textbox);
}
Vector2f NumBox::getSize() {
    return background.getSize();
}
bool NumBox::isMouseOver(sf::RenderWindow &window) {
    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;

    int btnPosX = background.getPosition().x;
    int btnPosY = background.getPosition().y;

    int btnxPosWidth = background.getPosition().x + background.getSize().x;
    int btnyPosHeight = background.getPosition().y + background.getSize().y;

    if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
        return true;
    }
    return false;
}
void NumBox::typedOn(sf::Event input){
    if (isSelected) {
        int charTyped = input.text.unicode;
        std::cout << charTyped << std::endl;
        if (charTyped == 45 || (charTyped >= 48 && charTyped <= 57) || charTyped == DELETE_KEY) {
            if (hasLimit) {
                if (text.str().length() <= limit) {
                    inputLogic(charTyped);
                }
                else if (text.str().length() > limit && charTyped == DELETE_KEY) {
                    deleteLastChar();
                }
            }
            else {
                inputLogic(charTyped);
            }
        }
    }
}
void NumBox::deleteLastChar(){
    std::string t = text.str();
    std::string newT = "";
    for (int i = 0; i < t.length() - 1; i++) {
        newT += t[i];
    }
    text.str("");
    text << newT;
    textbox.setString(text.str() + "|");
}
void NumBox::inputLogic(int charTyped) {
    if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
        text << static_cast<char>(charTyped);
    }
    else if (charTyped == DELETE_KEY) {
        if (text.str().length() > 0) {
            deleteLastChar();
        }
    }
    textbox.setString(text.str() + "|");
}

void NumBox::setTexture(Texture * texture) {
    background.setTexture(texture);
}

DataBadge::DataBadge(Texture *texture, Font & font, int height) {
    float scale = (float)height / (float)texture->getSize().y;
    background.setSize(Vector2f(texture->getSize().x * scale, texture->getSize().y * scale));
    background.setTexture(texture);
    text.setCharacterSize(height/2);
    text.setColor(Color().White);
    text.setFont(font);
    this->setData(0);
}

void DataBadge::drawTo(RenderWindow &window) {
    window.draw(background);
    window.draw(text);
}

void DataBadge::setPosition(Vector2f point) {
    background.setPosition(point);
    text.setPosition(Vector2f(point.x + 70, point.y + background.getSize().y / 5));
}

void DataBadge::setData(int data) {
    text.setString(std::to_string(data));
}

Vector2f DataBadge::getPosition() {
    return background.getPosition();
}

Vector2f DataBadge::getSize() {
    return background.getSize();
}
