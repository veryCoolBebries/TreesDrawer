//
// Created by Максим Лейхнер on 27.11.2022.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "trees.h"
using namespace sf;
using namespace std;
#ifndef TREESDRAWER_FUNCTIONS_H
#define TREESDRAWER_FUNCTIONS_H
Texture createBackgroundTexture(Uint16 windowWidth, Uint16 windowHeight, Color color);
void renderNodes(list <Drawable *> * renderQueue, Node * node, Font *font, int x, int y, int xOffset, int yOffset, int horizontalDistribution, int verticalDistribution, int layer);
void renderTree(list <Drawable *> * renderQueue, Tree * tree, Font *font, int x, int y, int xOffset = 220, int yOffset = 200, int horizontalDistribution = 400, int verticalDistribution = 150, int layer = 1);
void clearRenderQueue(list <Drawable *> * renderQueue);
#endif //TREESDRAWER_FUNCTIONS_H
