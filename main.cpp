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

    // Check if the mouse is within the bounds of the button:
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

Texture createBackgroundTexture(Uint16 windowWidth, Uint16 windowHeight, Color color);
void renderNodes(list <Drawable *> * renderQueue, Node * node, Font *font, int halfWidth, int verticalDistribution, int layer = 1, int xOffset = 0, int yOffset = 0);
void clearRenderQueue(list <Drawable *> * renderQueue);

int main() {
    //Настройки окна
    Uint16 windowWidth = 1440;
    Uint16 windowHeight = 960;
    string windowTitle = "Граф-вонючка";
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;

    //Инициализация окна
    RenderWindow myWindow(VideoMode(windowWidth, windowHeight), String::fromUtf8(windowTitle.begin(), windowTitle.end()), Style::Close, settings);
    myWindow.setFramerateLimit(60);

    //Что-то делаем...
    Texture backgroundTexture = createBackgroundTexture(1440, 960, Color(230, 230, 230));
    RectangleShape background;
    background.setSize(Vector2f(1440, 960));
    background.setTexture(&backgroundTexture);

    Texture addNewButton_texture;
    addNewButton_texture.loadFromFile("/Users/mleykhner/Documents/TreesDrawer/resources/addNewNode.png");
    Texture deleteButton_texture;
    deleteButton_texture.loadFromFile("/Users/mleykhner/Documents/TreesDrawer/resources/deleteNode.png");


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

    int xOffset = 220;
    int yOffset = 200;

    list <Drawable *> renderQueue;
    renderNodes(&renderQueue, &root, &montserratBold, 500, 200, 1, xOffset, yOffset);
    Button addNewButton(&addNewButton_texture, 60);
    addNewButton.setPosition(Vector2f(20, 960 - addNewButton.getSize().y - 20));
    Button deleteButton(&deleteButton_texture, 60);
    deleteButton.setPosition(Vector2f(40 + addNewButton.getSize().x, 960 - addNewButton.getSize().y - 20));

    //Начинаем жизнь окна
    bool dragging = false;
    int mousePositionX = 0;
    int mousePositionY = 0;

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
                        std::cout << "addNew" << "\n";
                    }
                    else if(deleteButton.isMouseOver(myWindow)) {
                        std::cout << "delete" << "\n";
                    }
                    else {
                        dragging = true;
                        mousePositionX = Mouse::getPosition(myWindow).x;
                        mousePositionY = Mouse::getPosition(myWindow).y;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    dragging = false;
                    break;
            }

        }

        if (dragging){
            int deltaX = Mouse::getPosition(myWindow).x - mousePositionX;
            int deltaY = Mouse::getPosition(myWindow).y - mousePositionY;

            clearRenderQueue(&renderQueue);
            xOffset+=deltaX;
            yOffset+=deltaY;

            renderNodes(&renderQueue, &root, &montserratBold, 500, 200, 1, xOffset, yOffset);

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

void clearRenderQueue(list <Drawable *> * renderQueue){
    while(!renderQueue->empty()){
        delete *renderQueue->begin();
        renderQueue->pop_front();
    }
}