#pragma once

#include "BinaryTreeNode.h"
#include "BinaryTree.h"

template<typename T>
class AVLTreeNode :
	public BinaryTreeNode<T>
{
public:
	AVLTreeNode(T initdata);

protected:
	//Height of subtree
	int height = 1;

public:
	/**	returns balance factor. (height of right subtree - height of left subtree)
	*/
	int GetBalanceFactor();

	AVLTreeNode<T>* GetChild(Direction dir);

	int GetHeight();

	/** Calculate collect height by comparing two subtree's heights.
	*@return	difference between new height value and old one.(new-old)
	*/
	int ResetHeight();

	/** Manually change the height value of node
	*/
	void _SetHeightManually(int new_height)
	{
		height = new_height;
	};
};

template<typename T>
inline AVLTreeNode<T>::AVLTreeNode(T initdata) : BinaryTreeNode<T>(initdata)
{

}

template<typename T>
inline int AVLTreeNode<T>::GetBalanceFactor()
{
	int left = 0;
	if (GetChild(Direction::Left) != nullptr) left = GetChild(Direction::Left)->GetHeight();
	int right = 0;
	if (GetChild(Direction::Right) != nullptr) right = GetChild(Direction::Right)->GetHeight();
	return right - left;
}

template<typename T>
inline AVLTreeNode<T>* AVLTreeNode<T>::GetChild(Direction dir)
{
	if (dir >= Direction::None) return nullptr;
	AVLTreeNode<T>* child = nullptr;
	child = dynamic_cast<AVLTreeNode<T>*>(((dir == Direction::Left) ? this->child_left : this->child_right));
	return child;
}

template<typename T>
inline int AVLTreeNode<T>::GetHeight()
{
	return height;
}

template<typename T>
inline int AVLTreeNode<T>::ResetHeight()
{
	int tempheight = height;
	int left = 0;
	if (GetChild(Direction::Left) != nullptr) left = GetChild(Direction::Left)->GetHeight();
	int right = 0;
	if (GetChild(Direction::Right) != nullptr) right = GetChild(Direction::Right)->GetHeight();
	height = std::max(left, right) + 1;
	return height - tempheight;
}