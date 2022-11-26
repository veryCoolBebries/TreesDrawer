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
Node* randomInsert(Node* previous, int data, Tree* tree = 0);
int fixSize(Node* previous);
int getSize(Node* previous);
Node* insertRoot(Node* previous, int data);
Node* rotateLeft(Node* previous);
Node* rotateRight(Node* previous);
void symmetricOrder(Node* root);
Node* insert(Tree* root, int data);
Node* findMinElement(Node* root);
Node* deleteElement(Node* current, int data);
Node* find(Node* root, int data);
int getHeight(Node* root);
int getSumPathsToEvenNodes(Tree* tree);