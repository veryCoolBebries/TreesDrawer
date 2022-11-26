#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "cmath"

using namespace std;
using namespace  sf;

struct Node {
    int data;
    int size = 1;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(int data) {
        this->data = data;
    }
};

Texture createBackgroundTexture(Uint16 windowWidth, Uint16 windowHeight, Color color);
void renderNodes(list <Drawable *> * renderQueue, Node * node, Font *font, int halfWidth, int verticalDistribution, int layer = 1, int xOffset = 0, int yOffset = 0);
void clearRenderQueue(list <Drawable *> renderQueue);

int main() {
    //Настройки окна
    Uint16 windowWidth = 1440;
    Uint16 windowHeight = 960;
    string windowTitle = "Граф-вонючка";

    //Инициализация окна
    RenderWindow myWindow(VideoMode(windowWidth, windowHeight), String::fromUtf8(windowTitle.begin(), windowTitle.end()), Style::Close);
    myWindow.setFramerateLimit(60);

    //Что-то делаем...
    Texture backgroundTexture = createBackgroundTexture(1440, 960, Color(230, 230, 230));
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
    if(!montserratBold.loadFromFile("/Users/mleykhner/Documents/TreesDrawer/resources/MontserratBold.ttf")){
        throw "FONT_NOT_LOADED";
    }

    list <Drawable *> renderQueue;
    renderNodes(&renderQueue, &root, &montserratBold, 500, 200, 1, 220, 200);

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
        for(Drawable * toDraw : renderQueue){
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

void renderNodes(list <Drawable *> * renderQueue, Node * node, Font *font, int halfWidth, int verticalDistribution, int layer, int xOffset, int yOffset){
    if(node->left) {
        VertexArray * line = new VertexArray(sf::Lines);
        Vertex * pointOne = new Vertex();
        Vertex * pointTwo = new Vertex();
        pointOne->color = Color().Black;
        pointOne->position = Vector2f(halfWidth + xOffset, verticalDistribution * (layer - 1) + yOffset - 50);
        pointTwo->color = Color().Black;
        pointTwo->position = Vector2f(halfWidth - halfWidth / pow(2, layer) + xOffset, verticalDistribution * layer + yOffset - 50);
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
        pointOne->position = Vector2f(halfWidth + xOffset, verticalDistribution * (layer - 1) + yOffset - 50);
        pointTwo->color = Color().Black;
        pointTwo->position = Vector2f(halfWidth + halfWidth / pow(2, layer) + xOffset, verticalDistribution * layer + yOffset - 50);
        line->resize(2);
        line->append(*pointOne);
        line->append(*pointTwo);
        renderQueue->push_back(line);
    }

    CircleShape * newCircle = new CircleShape(50);
    //newCircle->setFillColor(Color().Black);
    newCircle->setOutlineThickness(5);
    newCircle->setOutlineColor(Color(217, 155, 102));
    newCircle->setOrigin(Vector2f(50, 50));
    newCircle->setPosition(halfWidth + xOffset, verticalDistribution * (layer - 1) + yOffset - 50);
    renderQueue->push_back(newCircle);

    Text * number = new Text;
    number->setString(to_string(node->data));
    number->setFont(*font);
    number->setCharacterSize(50);
    number->setFillColor(Color( 191, 90, 90));
    number->setOrigin(number->getLocalBounds().width/2, number->getLocalBounds().height/2);
    number->setPosition(halfWidth + xOffset, verticalDistribution * (layer - 1) + yOffset - 63);
    renderQueue->push_back(number);

    if(node->left)
        renderNodes(renderQueue, node->left, font, halfWidth - halfWidth / pow(2, layer),  verticalDistribution, layer + 1, xOffset, yOffset);
    if(node->right)
        renderNodes(renderQueue, node->right, font, halfWidth + halfWidth / pow(2, layer),  verticalDistribution, layer + 1, xOffset, yOffset);
}

void clearRenderQueue(list <Drawable *> renderQueue){
    for(Drawable * toRender : renderQueue){
        delete toRender;
    }
}

void drawButtons(list <Drawable *> * renderQueue){

}