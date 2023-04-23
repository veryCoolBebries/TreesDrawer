#pragma once
struct Node {
	int data;
	int size;
	Node* left;
	Node* right;
	Node(int data);
};
struct Tree {
	Node* root = 0;
};
//получение размера дерева
int getSize(const Tree tree);
//симметричный обход дерева
void symmetricOrder(const Tree tree);
//вставка нового узла в дерево
Node* insert(Tree &tree, int data);
//удаление узла дерева по значению
void deleteElement(Tree &tree, int data);
//поиск узла дерева по значению
Node* find(Tree& tree, int data);
//получение высоты дерева
int getHeight(const Tree tree);
//получение суммы длин путей до всех чётных вершин
int getSumPathsToEvenNodes(Tree tree);
