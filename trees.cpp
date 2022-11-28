#include <iostream>
#include "trees.h"

Node::Node(int data) {
	left = right = 0;
	this->data = data;
	size = 1;
}
//случайная вставка в дерево
Node* _randomInsert(Node* previous, int data, Tree* tree = 0);
//пересчёт размера узла
int _fixSize(Node* previous);
//вставка узла в корень
Node* _insertRoot(Node* previous, int data);
//поворот влево вокруг узла
Node* _rotateLeft(Node* previous);
//поворот вправо вокруг узла
Node* _rotateRight(Node* previous);
//поиск минимального элемента в поддереве узла
Node* _findMinElement(Node* root);
//получение размера узла
int _getSize(Node* node);
//симметричный обход дерева
void _symmetricOrder(Node* root);
//удаление узла дерева
Node* _deleteElement(Node* current, int data, Tree * tree);
//получение высоты поддерева узла
int _getHeight(Node* root);
//поиск элемента по значению в поддереве узла
Node* _find(Node* root, int data);
//получение суммы путей от узла до всех четных узлов его поддерева
int _getSumPathsToEvenNodes(Node* node, int& sum, Tree* tree);

int getSize(const Tree tree)
{
	return _getSize(tree.root);
}

void symmetricOrder(const Tree tree)
{
	return _symmetricOrder(tree.root);
}

Node* insert(Tree& tree, int data)
{
	srand(time(NULL));
	if (tree.root == 0) return tree.root = _randomInsert(tree.root, data, &tree);
	else {
		try {
			find(tree, data);
		}
		catch(char const * ex){
			return _randomInsert(tree.root, data, &tree);
		}
		throw "Already exist!";
	}
}

void deleteElement(Tree& tree, int data)
{
	if (!&tree || tree.root == 0) throw "Tree is empty!";
	_deleteElement(tree.root, data, &tree);
	return;
}

Node* find(Tree& tree, int data)
{
	if (!&tree || tree.root == 0) throw "Tree is empty";
	Node* element = _find(tree.root, data);
	if (!element) throw "Element not found!";
	return element;
}

int getHeight(const Tree tree)
{
	return _getHeight(tree.root);
}

int getSumPathsToEvenNodes(Tree tree)
{
	if (tree.root == nullptr) return 0;
	int sum = 0;
	return _getSumPathsToEvenNodes(tree.root, sum, &tree);
}

Node* _randomInsert(Node* previous, int data, Tree* tree)
{
	
	if (!previous) return new Node(data);
	if (rand() % (previous->size + 1) == 0) {
		if (previous == tree->root) {
			tree->root = _insertRoot(previous, data);
			return tree->root;
		}
		return _insertRoot(previous, data);
	}
	else if (data < previous->data) previous->left = _randomInsert(previous->left, data, tree);
	else if (data > previous->data) previous->right = _randomInsert(previous->right, data, tree);
	_fixSize(previous);
	return previous;
}

int _fixSize(Node* previous)
{
	return previous->size = _getSize(previous->left) + _getSize(previous->right) + 1;
}

int _getSize(Node* node)
{
	if (!node) return 0; //Каво?
	else return _getSize(node->right) + _getSize(node->left) + 1;
}

Node* _insertRoot(Node* previous, int data)
{
	if (!previous) return new Node(data);
	if (data < previous->data) {
		previous->left = _insertRoot(previous->left, data);
		return _rotateRight(previous);
	}
	else
	{
		previous->right = _insertRoot(previous->right, data);
		return _rotateLeft(previous);
	}
}

Node* _rotateLeft(Node* previous)
{
	Node* current = previous->right;
	if (!current) return previous;
	previous->right = current->left;
	current->left = previous;
	current->size = previous->size;
	_fixSize(previous);
	return current;
}

Node* _rotateRight(Node* previous)
{
	Node* current = previous->left;
	if (!current) return previous;
	previous->left = current->right;
	current->right = previous;
	current->size = previous->size;
	_fixSize(previous);
	return current;
}

void _symmetricOrder(Node* root)
{
	if (!root) return;
	_symmetricOrder(root->left);
	std::cout << " " << root->data;
	_symmetricOrder(root->right);
}

Node* _findMinElement(Node* root)
{
	Node* current = root;
	while (current->left) {
		current = current->left;
	}
	return current;
}

Node* _deleteElement(Node* current, int data, Tree* tree)
{
	if (current->data == data) {
		if (current->left == 0 && current->right == 0) {
			if (current == tree->root) tree->root = 0;
			delete current;
			return NULL;
		}
        if (current->left == 0) {
            if (current == tree->root) tree->root = current->right;
            return current->right;
        }
        if (current->right == 0){
                    if (current == tree->root) tree->root = current->left;
                    return current->left;
        }
		Node* minElementInRightSubtree = _findMinElement(current->right);
		current->data = minElementInRightSubtree->data;
		current->right = _deleteElement(current->right, minElementInRightSubtree->data, tree);
		return current;
	}
	if (data < current->data) {
		if (current->left == 0) {
			throw "Element not found!";
		}
		current->left = _deleteElement(current->left, data, tree);
		return current;
	}
	if (data > current->data) {
		if (current->right == 0) {
			throw "Element not found!";
			return current;
		}
		current->right = _deleteElement(current->right, data,tree);
		return current;
	}

}

Node* _find(Node* root, int data)
{
	Node* current = root;
	while (current) {
		if (data == current->data) return current;
		else if (data < current->data) current = current->left;
		else current = current->right;
	}
	return NULL;
}

int _getHeight(Node* root)
{
	if (root == 0) return 0;
	else {
		int right = _getHeight(root->right);
		int left = _getHeight(root->left);
		return 1 + (right > left ? right : left);
	};
}

int _getSumPathsToEvenNodes(Node* node, int &sum, Tree* tree)
{
	if (!node) return sum;
	if (node->data % 2 == 0) sum += _getHeight(tree->root) - _getHeight(node);
	_getSumPathsToEvenNodes(node->left, sum, tree);
	_getSumPathsToEvenNodes(node->right, sum, tree);
	return sum;
}


