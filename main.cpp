#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "cmath"
#include "trees.h"
#include "elements.h"
#include "functions.h"

using namespace std;
using namespace  sf;

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

    //Загружаем файлы ресурсов TODO: Вызывать испключения если файл не загрузился
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
    srand(std::time(NULL));
    for(int i = 0; i < 15; i++){
        insert(myTree, random() % 100);
    }

    //Инициализируем необходимые переменные
    int verticalDistance = 200;
    int horizontalDistance = 400;
    bool dragging = false;
    int mousePositionX = 0;
    int mousePositionY = 0;
    int xOffset = 220;
    int yOffset = 200;

    //Создаём очередь рендеринга и элементы управления
    list <Drawable *> renderQueue;
    renderTree(&renderQueue, myTree, &montserratBold, 500, 200, xOffset, yOffset, horizontalDistance, verticalDistance);

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
                        renderTree(&renderQueue, myTree, &montserratBold, 500, 200, xOffset, yOffset, horizontalDistance, verticalDistance);

                    }
                    else if(deleteButton.isMouseOver(myWindow)) {
                        std::cout << "delete: " << numberInput.getNum() << "\n";
                        try{
                            deleteElement(myTree->root, numberInput.getNum());
                            numberInput.clear();
                            clearRenderQueue(&renderQueue);
                            renderTree(&renderQueue, myTree, &montserratBold, 500, 200, xOffset, yOffset, horizontalDistance, verticalDistance);
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
                case sf::Event::MouseWheelMoved:
                    horizontalDistance += event.mouseWheel.delta * -10;
                    if(horizontalDistance < 50) horizontalDistance = 50;
                    clearRenderQueue(&renderQueue);
                    renderTree(&renderQueue, myTree, &montserratBold, 500, 200, xOffset, yOffset, horizontalDistance, verticalDistance);
                    break;
                case sf::Event::TextEntered:
                    numberInput.typedOn(event);
                    break;
                default:
                    break;
            }

        }

        if (dragging){
            int deltaX = Mouse::getPosition(myWindow).x - mousePositionX;
            int deltaY = Mouse::getPosition(myWindow).y - mousePositionY;
            xOffset += deltaX;
            yOffset += deltaY;

            clearRenderQueue(&renderQueue);
            renderTree(&renderQueue, myTree, &montserratBold, 500, 200, xOffset, yOffset, horizontalDistance, verticalDistance);

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