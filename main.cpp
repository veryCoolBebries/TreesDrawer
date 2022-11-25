#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "cmath"

using namespace std;
using namespace  sf;

Texture createBackgroundTexture(Uint16 windowWidth, Uint16 windowHeight, Color color);

struct Node {
    int data;
    int size = 1;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(int data) {
        this->data = data;
    }
};

void renderNodes(list <Drawable *> * toRender, Node * node, Font *font, int layer, int x, int y);

int main() {
    //Настройки окна
    Uint16 windowWidth = 1440;
    Uint16 windowHeight = 960;
    string windowTitle = "Граф-вонючка";

    //Инициализация окна
    RenderWindow myWindow(VideoMode(windowWidth, windowHeight), String::fromUtf8(windowTitle.begin(), windowTitle.end()), Style::Close);
    myWindow.setFramerateLimit(60);

    //Что-то делаем...
    Texture backgroundTexture = createBackgroundTexture(1440, 960, Color().White);
    RectangleShape background;
    background.setSize(Vector2f(1440, 960));
    background.setTexture(&backgroundTexture);

    Node root(12);
    root.left = new Node(11);
    root.right = new Node(13);
    root.right->right = new Node(15);
    root.right->right->right = new Node(16);
    root.right->right->left = new Node(14);

    Font montserratBold;
    if(!montserratBold.loadFromFile("/Users/mleykhner/Desktop/TreesDrawer/resources/MontserratBold.ttf")){
        throw "FONT_NOT_LOADED";
    }

    list <Drawable *> toRender;
    renderNodes(&toRender, &root, &montserratBold, 1, 600, 100);

    //Начинаем жизнь окна
    while (myWindow.isOpen()) //Пока окно открыто
    {
        sf::Event event; //Отслеживаем события
        while (myWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                //При закрытии окна завершаем работу программы
                myWindow.close();
        }
        myWindow.clear();
        myWindow.draw(background);
        for(Drawable * toDraw : toRender){
            myWindow.draw(* toDraw);
        }
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
    for(int pixel = 0; pixel < windowWidth * windowHeight * 4; pixel++){
        backgroundPixels[pixel] = myColor[pixel % 4];
    }
    backgroundTexture.update(backgroundPixels);
    return backgroundTexture;
}

void renderNodes(list <Drawable *> * toRender, Node * node, Font *font, int layer, int x, int y){
    CircleShape * newCircle = new CircleShape(50);
    newCircle->setFillColor(Color().Black);
    newCircle->setOrigin(Vector2f(50, 50));
    newCircle->setPosition(x, y);
    toRender->push_back(newCircle);

    Text * number = new Text;
    number->setString(to_string(node->data));
    number->setFont(*font);
    number->setCharacterSize(50);
    number->setFillColor(Color().Magenta);
    number->setOrigin(number->getLocalBounds().width/2, number->getLocalBounds().height/2);
    number->setPosition(x, y);
    toRender->push_back(number);

    if(node->left)
        renderNodes(toRender, node->left, font, layer + 1, x - x / pow(2, layer),  y + 100);
    if(node->right)
        renderNodes(toRender, node->right, font, layer + 1, x + x / pow(2, layer), y + 100);
}

