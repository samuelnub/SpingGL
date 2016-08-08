#pragma once
#ifndef TOOLS_BINARY_SEARCH_TREE_H
#define TOOLS_BINARY_SEARCH_TREE_H

#include <iostream>
#include <math.h>

//binary search tree class template, courtesy of https://gist.github.com/mgechev/5911348
template <class T>
struct Node
{
	T data;
	Node *left;
	Node *right;

	inline Node(T data)
	{
		this->data = data;
	}

	inline Node(T data, Node<T> left, Node<T> right)
	{
		this->data = data;
		this->left = left;
		this->right = right;
	}
};

template <class T>
class BST
{
private:
	Node<T> *_root;

public:
	inline void add(T data)
	{
		if (this->_root != nullptr)
		{
			this->addHelper(this->_root, data);
		}
		else
		{
			this->_root = new Node<T>(data);
		}
	}

	inline void print()
	{
		this->printHelper(this->_root);
	}

	inline int nodeCount()
	{
		return this->nodeCountHelper(this->_root);
	}

	inline int height()
	{
		return this->heightHelper(this->_root);
	}

	inline void printMaxPath()
	{
		this->printMaxPathHelper(this->_root);
	}

	inline bool deleteData(T data)
	{
		return this->deleteDataHelper(NULL, this->_root, data);
	}

protected:
	void addHelper(Node<T> *root, T data);
	void printHelper(Node<T> *root);
	int nodeCountHelper(Node<T> *root);
	int heightHelper(Node<T> *root);
	void printMaxPathHelper(Node<T> *root);
	bool deleteDataHelper(Note<T> *root, Node<T> *current, T data);

};

#endif