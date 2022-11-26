#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <list>
#include "cmath"
#include "trees.h"

using namespace std;
using namespace  sf;

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Button {
public:
    Button(Texture * texture, float height) {
        float scale = height / texture->getSize().y;
        button.setSize(Vector2f(texture->getSize().x * scale, texture->getSize().y * scale));
        button.setTexture(texture);
    }

    void setPosition(sf::Vector2f point) {
        button.setPosition(point);
    }

    void drawTo(sf::RenderWindow &window) {
        window.draw(button);
    }

    Vector2f getSize(){
        return button.getSize();
    }

    bool isMouseOver(sf::RenderWindow &window) {
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
private:
    sf::RectangleShape button;
};
class NumBox {
public:
    NumBox(Texture * texture, float height, Color textColor = Color().Black, bool sel = false) {
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

    void setSelected(bool sel) {
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

    void clear(){
        textbox.setString("");
        text.str("");
    }

    int getNum(){
        int res = 0;
        for(char letter : text.str()){
            res = res * 10 + letter - 48;
        }
        return res;
    }

    void setFont(sf::Font &fonts) {
        textbox.setFont(fonts);
    }

    void setLimit(bool ToF) {
        hasLimit = ToF;
    }

    void setLimit(bool ToF, int lim) {
        hasLimit = ToF;
        limit = lim - 1;
    }

    void setPosition(sf::Vector2f point) {
        background.setPosition(point);
        textbox.setPosition(Vector2f(point.x + 10, point.y + background.getSize().y / 5));
    }

    void drawTo(sf::RenderWindow &window) {
        window.draw(background);
        window.draw(textbox);
    }

    Vector2f getSize(){
        return background.getSize();
    }

    bool isMouseOver(sf::RenderWindow &window) {
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

    void typedOn(sf::Event input) {
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

private:
    sf::Text textbox;
    std::ostringstream text;
    sf::RectangleShape background;
    bool isSelected = false;
    bool hasLimit = false;
    int limit = 0;

    void deleteLastChar() {
        std::string t = text.str();
        std::string newT = "";
        for (int i = 0; i < t.length() - 1; i++) {
            newT += t[i];
        }
        text.str("");
        text << newT;
        textbox.setString(text.str() + "|");
    }

    void inputLogic(int charTyped) {
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

};


Texture createBackgroundTexture(Uint16 windowWidth, Uint16 windowHeight, Color color);
void renderNodes(list <Drawable *> * renderQueue, Node * node, Font *font, int x, int y, int xOffset, int yOffset, int horizontalDistribution, int verticalDistribution, int layer);
void renderTree(list <Drawable *> * renderQueue, Tree * tree, Font *font, int x, int y, int xOffset = 220, int yOffset = 200, int horizontalDistribution = 400, int verticalDistribution = 150, int layer = 1);
void clearRenderQueue(list <Drawable *> * renderQueue);

int main() {
    //Настройки окна
    Uint16 windowWidth = 1440;
    Uint16 windowHeight = 960;
    string windowTitle = "Мудрое дерево";
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    //Инициализация окна
    RenderWindow myWindow(VideoMode(windowWidth, windowHeight), String::fromUtf8(windowTitle.begin(), windowTitle.end()), Style::Close, settings);
    myWindow.setFramerateLimit(60);

    //Создаём текстуру фона
    Texture backgroundTexture = createBackgroundTexture(windowWidth, windowHeight, Color(230, 230, 230));
    RectangleShape background;
    background.setSize(Vector2f(windowWidth, windowHeight));
    background.setTexture(&backgroundTexture);

    //Загружаем файлы ресурсов
    Texture addNewButton_texture;
    addNewButton_texture.loadFromFile("/Users/mleykhner/Documents/TreesDrawer/resources/addNewNode.png");
    Texture deleteButton_texture;
    deleteButton_texture.loadFromFile("/Users/mleykhner/Documents/TreesDrawer/resources/deleteNode.png");
    Texture searchButton_texture;
    searchButton_texture.loadFromFile("/Users/mleykhner/Documents/TreesDrawer/resources/searchNode.png");
    Texture textField_texture;
    textField_texture.loadFromFile("/Users/mleykhner/Documents/TreesDrawer/resources/textField.png");
    Font montserratBold;
    if(!montserratBold.loadFromFile("/Users/mleykhner/Documents/TreesDrawer/resources/MontserratBold.ttf")){
        throw "FONT_NOT_LOADED";
    }

    //Создаём дерево
    Tree * myTree = new Tree;
    myTree->root = new Node(10);

    //Инициализируем необходимые переменные
    bool dragging = false;
    int mousePositionX = 0;
    int mousePositionY = 0;
    int xOffset = 220;
    int yOffset = 200;

    //Создаём очередь рендеринга и кнопки
    list <Drawable *> renderQueue;
    renderTree(&renderQueue, myTree, &montserratBold, 500, 200);

    NumBox numberInput(&textField_texture, 60);
    numberInput.setPosition(Vector2f(20, windowHeight - numberInput.getSize().y - 20));
    numberInput.setFont(montserratBold);
    numberInput.setLimit(true, 3);

    Button addNewButton(&addNewButton_texture, 60);
    addNewButton.setPosition(Vector2f(40 + numberInput.getSize().x, windowHeight - addNewButton.getSize().y - 20));
    Button deleteButton(&deleteButton_texture, 60);
    deleteButton.setPosition(Vector2f(60 + numberInput.getSize().x + addNewButton.getSize().x, windowHeight - deleteButton.getSize().y - 20));
    Button searchButton(&searchButton_texture, 60);
    searchButton.setPosition(Vector2f(80 + numberInput.getSize().x + addNewButton.getSize().x + deleteButton.getSize().x, windowHeight - searchButton.getSize().y - 20));

    //Начинаем жизнь окна
    while (myWindow.isOpen()) //Пока окно открыто
    {
        sf::Event event; //Отслеживаем события
        while (myWindow.pollEvent(event))
        {
            switch (event.type){
                case sf::Event::Closed:
                    //При закрытии окна завершаем работу программы
                    myWindow.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (addNewButton.isMouseOver(myWindow)) {
                        std::cout << "addNew: " << numberInput.getNum() << "\n";
                        insert(myTree, numberInput.getNum());
                        numberInput.clear();
                        clearRenderQueue(&renderQueue);
                        renderTree(&renderQueue, myTree, &montserratBold, 500, 200, xOffset, yOffset);

                    }
                    else if(deleteButton.isMouseOver(myWindow)) {
                        std::cout << "delete: " << numberInput.getNum() << "\n";
                        try{
                            deleteElement(myTree->root, numberInput.getNum());
                            numberInput.clear();
                            clearRenderQueue(&renderQueue);
                            renderTree(&renderQueue, myTree, &montserratBold, 500, 200, xOffset, yOffset);
                        }catch(char * error){ //TODO: Починить отлов ошибки
                            cout << "error" << error << endl;
                            //TODO: Покрасить поле вводе
                        }
                    }
                    else if(numberInput.isMouseOver(myWindow)){
                        numberInput.setSelected(true);
                    }
                    else {
                        numberInput.setSelected(false);
                        dragging = true;
                        mousePositionX = Mouse::getPosition(myWindow).x;
                        mousePositionY = Mouse::getPosition(myWindow).y;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    dragging = false;
                    break;
                case sf::Event::TextEntered:
                    numberInput.typedOn(event);
                    break;
            }

        }

        if (dragging){
            int deltaX = Mouse::getPosition(myWindow).x - mousePositionX;
            int deltaY = Mouse::getPosition(myWindow).y - mousePositionY;
            xOffset += deltaX;
            yOffset += deltaY;

            clearRenderQueue(&renderQueue);
            renderTree(&renderQueue, myTree, &montserratBold, 500, 200, xOffset, yOffset);

            mousePositionX = Mouse::getPosition(myWindow).x;
            mousePositionY = Mouse::getPosition(myWindow).y;
        }

        myWindow.clear();

        myWindow.draw(background);
        for(Drawable * toDraw : renderQueue){
            myWindow.draw(* toDraw);
        }
        addNewButton.drawTo(myWindow);
        deleteButton.drawTo(myWindow);
        searchButton.drawTo(myWindow);
        numberInput.drawTo(myWindow);

        myWindow.display();
    }

    return 0;
}

Texture createBackgroundTexture(Uint16 windowWidth, Uint16 windowHeight, Color color){
    //Создаем текстуру фона
    Texture backgroundTexture;
    backgroundTexture.create(windowWidth, windowHeight);
    //Массив пикселей
    Uint8 * backgroundPixels = new Uint8 [windowHeight * windowWidth * 4];
    //Раскладываем цвет на составляющие
    Uint8 myColor[4] = {color.r, color.g, color.b, color.a};
    //Записываем цвет в каждый пиксель
    for(int pixel = 0; pixel < windowWidth * windowHeight * 4; pixel++){
        backgroundPixels[pixel] = myColor[pixel % 4];
    }
    //Обновляем текстуру новыми пикселями
    backgroundTexture.update(backgroundPixels);
    //Возвращяем текстуру
    return backgroundTexture;
}

void renderNodes(list <Drawable *> * renderQueue, Node * node, Font *font, int x, int y, int xOffset, int yOffset, int horizontalDistribution, int verticalDistribution, int layer){

    //Рисуем ветки
    if(node->left) {
        VertexArray * line = new VertexArray(sf::Lines);
        Vertex * pointOne = new Vertex();
        Vertex * pointTwo = new Vertex();
        pointOne->color = Color().Black;
        pointOne->position = Vector2f(x + xOffset, y + yOffset);
        pointTwo->color = Color().Black;
        pointTwo->position = Vector2f(x - horizontalDistribution / pow(2, layer - 1) + xOffset, y + verticalDistribution + yOffset);
        line->resize(2);
        line->append(*pointOne);
        line->append(*pointTwo);
        renderQueue->push_back(line);
    }
    if(node->right) {
        VertexArray * line = new VertexArray(sf::Lines);
        Vertex * pointOne = new Vertex();
        Vertex * pointTwo = new Vertex();
        pointOne->color = Color().Black;
        pointOne->position = Vector2f(x + xOffset, y + yOffset);
        pointTwo->color = Color().Black;
        pointTwo->position = Vector2f(x + horizontalDistribution / pow(2, layer - 1) + xOffset, y + verticalDistribution + yOffset);
        line->resize(2);
        line->append(*pointOne);
        line->append(*pointTwo);
        renderQueue->push_back(line);
    }

    //Рисуем узел
    CircleShape * newCircle = new CircleShape(50);
    newCircle->setOutlineThickness(5);
    newCircle->setOutlineColor(Color(217, 155, 102));
    newCircle->setOrigin(Vector2f(50, 50));
    newCircle->setPosition(x + xOffset, y + yOffset);
    renderQueue->push_back(newCircle);

    //Рисуем надпись
    Text * number = new Text;
    number->setString(to_string(node->data));
    number->setFont(*font);
    number->setCharacterSize(50);
    number->setFillColor(Color( 191, 90, 90));
    number->setOrigin(number->getLocalBounds().width/2, number->getLocalBounds().height/2);
    number->setPosition(x + xOffset, y + yOffset - 15);
    renderQueue->push_back(number);

    //Рекурсивно вызываем функцию для левого и правого ребёнка
    if(node->left)
        renderNodes(renderQueue, node->left, font, x - horizontalDistribution / pow(2, layer - 1), y + verticalDistribution, xOffset, yOffset, horizontalDistribution, verticalDistribution, layer + 1);
    if(node->right)
        renderNodes(renderQueue, node->right, font, x + horizontalDistribution / pow(2, layer - 1), y + verticalDistribution, xOffset, yOffset, horizontalDistribution, verticalDistribution, layer + 1);

}

void renderTree(list <Drawable *> * renderQueue, Tree * tree, Font *font, int x, int y, int xOffset, int yOffset, int horizontalDistribution, int verticalDistribution, int layer){
    renderNodes(renderQueue, tree->root, font, x, y, xOffset, yOffset, horizontalDistribution, verticalDistribution, layer);
}

void clearRenderQueue(list <Drawable *> * renderQueue){
    while(!renderQueue->empty()){
        delete *renderQueue->begin();
        renderQueue->pop_front();
    }
}