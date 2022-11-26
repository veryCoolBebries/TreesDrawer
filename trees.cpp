#include <iostream>
#include "trees.h"

Node::Node(int data) {
	left = right = 0;
	this->data = data;
	size = 1;
}

//int main() {
//	srand(time(NULL));
//	Tree* tree = new Tree;
//	tree->root = insert(tree, 0);
//	int hui[] = { 1,2,3,4,5,6 };
//	for (size_t i = 0; i < 6; i++)
//	{
//		//insert(tree, rand() % (10*(i+1)));
//		insert(tree, hui[i]); 
//	}
//	std::cout << "Size: " << getSize(tree->root) << std::endl;
//	Node* huinya = insert(tree, 10);
//	symmetricOrder(tree->root);
//	deleteElement(tree->root, 10);
//	std::cout << '/' << huinya->data << '/' << std::endl;
//	symmetricOrder(tree->root);
//	std::cout << "Height of tree: " << getHeight(tree->root) << std::endl;
//	std::cout << "Sum of paths to even node: " << getSumPathsToEvenNodes(tree);
//}

Node* randomInsert(Node* previous, int data, Tree* tree)
{
	
	if (!previous) return new Node(data);
	if /* (rand() % (previous->size + 1) == 0)*/(true) {
		if (previous == tree->root) {
			tree->root = insertRoot(previous, data);
			return tree->root;
		}
		return insertRoot(previous, data);
	}
	else if (data < previous->data) previous->left = randomInsert(previous->left, data, tree);
	else if (data > previous->data) previous->right = randomInsert(previous->right, data, tree);
	fixSize(previous);
	return previous;
}

int fixSize(Node* previous)
{
	return previous->size = getSize(previous->left) + getSize(previous->right) + 1;
}

int getSize(Node* previous)
{
	if (!previous) return 0;
	else return previous->size;
}

Node* insertRoot(Node* previous, int data)
{
	if (!previous) return new Node(data);
	if (data < previous->data) {
		previous->left = insertRoot(previous->left, data);
		return rotateRight(previous);
	}
	else
	{
		previous->right = insertRoot(previous->right, data);
		return rotateLeft(previous);
	}
}

Node* rotateLeft(Node* previous)
{
	Node* current = previous->right;
	if (!current) return previous;
	previous->right = current->left;
	current->left = previous;
	current->size = previous->size;
	fixSize(previous);
	return current;
}

Node* rotateRight(Node* previous)
{
	Node* current = previous->left;
	if (!current) return previous;
	previous->left = current->right;
	current->right = previous;
	current->size = previous->size;
	fixSize(previous);
	return current;
}

void symmetricOrder(Node* root)
{
	if (!root) return;
	symmetricOrder(root->left);
	std::cout << " " << root->data;
	symmetricOrder(root->right);
}

Node* insert(Tree* tree, int data)
{
	srand(time(NULL));
	return randomInsert(tree->root, data, tree);
}

Node* findMinElement(Node* root)
{
	Node* current = root;
	while (current->left) {
		current = current->left;
	}
	return current;
}

Node* deleteElement(Node* current, int data)
{
	if (current->data == data) {
		if (current->left == 0 && current->right == 0) {
			delete current;
			return NULL;
		}
		if (current->left == 0) return current->right;
		if (current->right == 0) return current->left;
		Node* minElementInRightSubtree = findMinElement(current->right);
		current->data = minElementInRightSubtree->data;
		current->right = deleteElement(current->right, minElementInRightSubtree->data);
		return current;
	}
	if (data < current->data) {
		if (current->left == 0) {
            throw "ELEMENT_NOT_FOUND";
			return current;
		}
		current->left = deleteElement(current->left, data);
		return current;
	}
	if (data > current->data) {
		if (current->right == 0) {
			throw "ELEMENT_NOT_FOUND";
			return current;
		}
		current->right = deleteElement(current->right, data);
		return current;
	}

}

Node* find(Node* root, int data)
{
	Node* current = root;
	while (current) {
		if (data == current->data) return current;
		if (data < current->data) current = current->left;
		if (data > current->data) current = current->right;
	}
	return NULL;
}

int getHeight(Node* root)
{
	if (root == 0) return 0;
	else {
		int right = getHeight(root->right);
		int left = getHeight(root->left);
		return 1 + (right > left ? right : left);
	};
}
int _getSumPathsToEvenNodes(Node* node, int &sum, Tree* tree)
{
	if (!node) return sum;
	if (node->data % 2 == 0) sum += getHeight(tree->root) - getHeight(node);
	_getSumPathsToEvenNodes(node->left, sum, tree);
	_getSumPathsToEvenNodes(node->right, sum, tree);
	return sum;
}
int getSumPathsToEvenNodes(Tree* tree)
{
	if (!tree) return 0;
	int sum = 0;
	_getSumPathsToEvenNodes(tree->root, sum, tree);
}

