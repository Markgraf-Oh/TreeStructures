#pragma once
#include <queue>
#include "BinaryTree.h"

template<typename T>
class BinarySearchTree :
	public BinaryTree<T>
{
public:
	BinarySearchTree(int (*init_rule)(const T& left, const T& right), bool IsSet = false);

	BinarySearchTree(T init_root_data, int (*init_rule)(const T& left, const T& right), bool IsSet = false);

public:
	//add data;
	//만약 중복이 허용 된다면, 동일한 데이터는 왼쪽 방향으로 1열로 늘어선다.
	virtual bool AddData(T newdata) override;
		
	//find data;
	virtual bool FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const override;

	//find node with data and pop it from the tree
	virtual BinaryTreeNode<T>* PopData(T target_data) override;

	virtual bool DeleteData(T target_data) override;

protected:
	//pop node out from the tree. without delete it.
	virtual BinaryTreeNode<T>* PopNode(BinaryTreeNode<T>* target_node) override;

	virtual bool DeleteNode(BinaryTreeNode<T>* target_node) override;

	virtual bool InsertNode(BinaryTreeNode<T>* target_node, BinaryTreeNode<T>* insert_node);
		
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
	BinaryTreeNode<T>* current_node = nullptr;
	BinaryTreeNode<T>* next_node = BinaryTree<T>::root;
	if (next_node == nullptr)
	{
		this->root = new BinaryTreeNode<T>(newdata);
		return true;
	}
	while (next_node != nullptr)
	{
		current_node = next_node;
		switch (this->rule(newdata, current_node->GetData()))
		{
		case 1:
			next_node = current_node->GetChild(Direction::Left);
			break;
		case 0:
			if (!BinaryTree<T>::IsSet) return false;
			if (current_node->GetChild(Direction::Left) != nullptr)
			{
				return InsertNode(current_node, new BinaryTreeNode<T>(newdata));
			}
			next_node = current_node->GetChild(Direction::Left);
			break;
		case -1:
			next_node = current_node->GetChild(Direction::Right);
		}
	}

	if (this->rule(newdata, current_node->GetData()) >= 0)
	{
		BinaryTree<T>::AttachNode(current_node, new BinaryTreeNode<T>(newdata), Direction::Left);
	}
	else
	{
		BinaryTree<T>::AttachNode(current_node, new BinaryTreeNode<T>(newdata), Direction::Right);
	}
	this->node_count++;
	return true;
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

	this->node_count--;
	return target_node;
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
	
	this->node_count++;

	return true;
}
