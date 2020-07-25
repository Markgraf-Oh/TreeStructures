#pragma once

#include "BinaryTree.h"


/* 이진 탐색 트리
 *@param init_rule	the fuction which gets left and right value, and returns
 *					+1 if they aligned correctly,
 *					0 if they are same,
 *					-1 if they need to be aligned.
 */
template<typename T>
class BinarySearchTree :
	public BinaryTree<T>
{
public:
	
	BinarySearchTree(int (*init_rule)(const T& left, const T& right), bool IsSet = false);

	/**	@param init_rule	the fuction which gets left and right value, and returns +1 if they aligned correctly, 0 if they are same, -1 if they need to be aligned.
	*/
	BinarySearchTree(T init_root_data, int (*init_rule)(const T& left, const T& right), bool IsSet = false);

public:
	//Create new node with given data and Add that node into Tree. returns true if succeed
	virtual bool AddData(T newdata) override;
	
	/**	Find node which has given data.
	* @param target_data	data to search
	* @param start_node		the starting point of searching. use root of this Tree.
	* @param search_return	reperence of Node pointer value. This value will be set as a pointer to found node.
	* @return	returns true if found data
	*/
	virtual bool FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const override;

	//find node with data and pop it from the tree
	virtual BinaryTreeNode<T>* PopData(T target_data) override;
	
	//find node with data and delete it from the tree
	virtual bool DeleteData(T target_data) override;
protected:
	//Add new node to the tree.
	virtual bool AddNode(BinaryTreeNode<T>* new_node) override;

	//insert given node under the target_node.  returns true if succeed.
	virtual bool InsertNode(BinaryTreeNode<T>* target_node, BinaryTreeNode<T>* insert_node) override;

	/**	pop node out from the tree. without delete it.
	* @return	pointer of poped node.
	*/
	virtual BinaryTreeNode<T>* PopNode(BinaryTreeNode<T>* target_node) override;

	//pop node out from the tree. and delete it. returns true if succeed.
	virtual bool DeleteNode(BinaryTreeNode<T>* target_node) override;

	
};

template<typename T>
BinarySearchTree<T>::BinarySearchTree(int (*init_rule)(const T& left, const T& right), bool IsSet)
	: BinaryTree<T>(init_rule, IsSet)
{

}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(T init_root_data, int (*init_rule)(const T& left, const T& right), bool IsSet)
	: BinaryTree<T>(init_root_data, init_rule, IsSet)
{

}

template<typename T>
inline bool BinarySearchTree<T>::AddData(T newdata)
{
	BinaryTreeNode<T>* new_node = new BinaryTreeNode<T>(newdata);

	if (AddNode(new_node)) return true;
	
	delete new_node;
	new_node = nullptr;
	return false;
}



template<typename T>
inline bool BinarySearchTree<T>::FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const
{
	if (start_node == nullptr) return false;

	int comp = this->rule(target_data, start_node->GetData());
	
	if (comp == 0)
	{
		search_return = start_node;
		return true;
	}

	return FindData(target_data, ((comp > 0) ? start_node->GetChild(Direction::Left) : start_node->GetChild(Direction::Right)),  search_return);
}

template<typename T>
inline BinaryTreeNode<T>* BinarySearchTree<T>::PopData(T target_data)
{
	BinaryTreeNode<T>* data_node = nullptr;

	if (!FindData(target_data, this->root, data_node)) return nullptr;

	return PopNode(data_node);
}

template<typename T>
inline bool BinarySearchTree<T>::DeleteData(T target_data)
{
	BinaryTreeNode<T>* data_node = nullptr;

	if (!FindData(target_data, this->root, data_node)) return false;

	return DeleteNode(data_node);
}

template<typename T>
inline bool BinarySearchTree<T>::DeleteNode(BinaryTreeNode<T>* target_node)
{
	BinaryTreeNode<T>* poped_node = PopNode(target_node);
	if (poped_node == nullptr)
	{
		return false;
	}
	delete poped_node;
	poped_node = nullptr;
	return true;
}

template<typename T>
inline bool BinarySearchTree<T>::AddNode(BinaryTreeNode<T>* new_node)
{
	if (new_node == nullptr) return false;

	BinaryTreeNode<T>* current_node = nullptr;
	BinaryTreeNode<T>* next_node = BinaryTree<T>::root;

	if (next_node == nullptr)
	{
		this->root = new_node;
		return true;
	}
	while (next_node != nullptr)
	{
		current_node = next_node;
		switch (this->rule(new_node->GetData(), current_node->GetData()))
		{
		case 1:
			next_node = current_node->GetChild(Direction::Left);
			break;
		case 0:
			if (!BinaryTree<T>::IsSet) return false;
			if (current_node->GetChild(Direction::Left) != nullptr)
			{
				return InsertNode(current_node, new_node);
			}
			next_node = current_node->GetChild(Direction::Left);
			break;
		case -1:
			next_node = current_node->GetChild(Direction::Right);
		}
	}

	if (this->rule(new_node->GetData(), current_node->GetData()) >= 0)
	{
		BinaryTree<T>::AttachNode(current_node, new_node, Direction::Left);
	}
	else
	{
		BinaryTree<T>::AttachNode(current_node, new_node, Direction::Right);
	}
	
	return __super::AddNode(new_node);
}

template<typename T>
inline bool BinarySearchTree<T>::InsertNode(BinaryTreeNode<T>* target_node, BinaryTreeNode<T>* insert_node)
{	
	//미완
	if (target_node == nullptr || insert_node == nullptr) return false;
	
	if (target_node->GetParent() != nullptr)
	{
		BinaryTree<T>::AttachNode(target_node->GetParent(), insert_node, target_node->GetDirectionFrom());
	}
	else if (target_node == BinaryTree<T>::root)
	{
		this->root = insert_node;
		BinaryTree<T>::DetachFromParent(insert_node);
	}		
	else return false;

	Direction new_target_direction = (BinaryTree<T>::rule(insert_node->GetData(), target_node->GetData()) >= 0) ? 
		Direction::Left : Direction::Right;
	
	BinaryTree<T>::AttachNode(target_node->GetParent(), insert_node, new_target_direction);
	
	return __super::InsertNode(target_node, insert_node);
	
}


template<typename T>
inline BinaryTreeNode<T>* BinarySearchTree<T>::PopNode(BinaryTreeNode<T>* target_node)
{
	if (target_node == nullptr) return nullptr;

	BinaryTreeNode<T>* replace_node = nullptr;
	switch (target_node->GetNumberofChilden())
	{
	case 0:
		break;
	case 1:
		replace_node = (target_node->GetChild(Direction::Left) == nullptr) ?
			target_node->GetChild(Direction::Right) : target_node->GetChild(Direction::Left);

		break;
	case 2:
		if (this->rule(target_node->GetChild(Direction::Left)->GetData(), target_node->GetData()) == 0)
		{
			replace_node = target_node->GetChild(Direction::Left);
		}
		else
		{
			replace_node = target_node->GetChild(Direction::Right);
			while (replace_node->GetChild(Direction::Left) != nullptr)
			{
				replace_node = replace_node->GetChild(Direction::Left);
			}
			if (replace_node->GetChild(Direction::Right) != nullptr)
			{
				BinaryTree<T>::AttachNode(replace_node->GetParent(), replace_node->GetChild(Direction::Right), Direction::Left);
			}
			BinaryTree<T>::AttachNode(replace_node, target_node->GetChild(Direction::Right), Direction::Right);
		}
		break;
	}

	if (target_node->GetParent() != nullptr)
	{
		BinaryTree<T>::AttachNode(target_node->GetParent(), replace_node, target_node->GetDirectionFrom());
	}
	if (target_node == this->root) this->root = nullptr;

	return __super::PopNode(target_node);
}
