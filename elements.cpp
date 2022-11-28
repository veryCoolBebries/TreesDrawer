#include <SFML/Graphics.hpp>
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
    for(char letter : text.str()){
        res = res * 10 + letter - 48;
    }
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
        if (charTyped < 128) {
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
