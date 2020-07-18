#pragma once

enum class Direction
{
	Left,
	Right,
	Both
};

template<typename T>
class BinaryTreeNode
{
	template<typename U> friend class BinaryTree;
	template<typename U> friend class BinarySearchTree;
public:
	BinaryTreeNode();

	BinaryTreeNode(T initdata);

	virtual ~BinaryTreeNode();

public:
	
protected:
	
	T* data_ptr = nullptr;

	BinaryTreeNode<T> * parent = nullptr;

	BinaryTreeNode<T> * child_left = nullptr;

	BinaryTreeNode<T> * child_right = nullptr;

public:
	T GetData();

	BinaryTreeNode<T>* GetParent();

	BinaryTreeNode<T>* GetLeftChild();

	BinaryTreeNode<T>* GetRightChild();

	int GetNumberofChilden();
	
protected:
	void _SetData(T newdata);

};

template<typename T>
inline BinaryTreeNode<T>::BinaryTreeNode()
{
}

template<typename T>
inline BinaryTreeNode<T>::BinaryTreeNode(T initdata)
{
	data_ptr = new T(initdata);
}

template<typename T>
inline BinaryTreeNode<T>::~BinaryTreeNode()
{
	delete data_ptr;
	data_ptr = nullptr;
}

template<typename T>
inline T BinaryTreeNode<T>::GetData()
{
	return *data_ptr;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryTreeNode<T>::GetParent()
{
	return parent;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryTreeNode<T>::GetLeftChild()
{
	return child_left;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryTreeNode<T>::GetRightChild()
{
	return child_right;
}

template<typename T>
inline int BinaryTreeNode<T>::GetNumberofChilden()
{
	int num = 0;
	if (nullptr != child_left) num++;
	if (nullptr != child_right) num++;
	return num;
}

template<typename T>
inline void BinaryTreeNode<T>::_SetData(T newdata)
{
	if (data_ptr == nullptr)
	{
		data_ptr = new T(newdata);
		return;
	}
	*data_ptr = newdata;
}
