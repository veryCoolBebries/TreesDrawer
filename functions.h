#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "trees.h"
using namespace sf;
using namespace std;
Texture createBackgroundTexture(Uint16 windowWidth, Uint16 windowHeight, Color color);
void renderNodes(list <Drawable *> * renderQueue, Node * node, Font *font, int x, int y, int xOffset, int yOffset, int horizontalDistribution, int verticalDistribution, Node * toHighlight, int layer);
void renderTree(list <Drawable *> * renderQueue, Tree * tree, Font *font, int x, int y, int xOffset = 220, int yOffset = 200, int horizontalDistribution = 400, int verticalDistribution = 150, Node * toHighlight = nullptr, int layer = 1);
void clearRenderQueue(list <Drawable *> * renderQueue);

