//
// Created by Максим Лейхнер on 27.11.2022.
//
#include <SFML/Graphics.hpp>
#include <list>
#include <cmath>
#include "trees.h"
using namespace sf;
using namespace std;

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