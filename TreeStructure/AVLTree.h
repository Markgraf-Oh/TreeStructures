#pragma once
#include "BinarySearchTree.h"
#include "AVLTreeNode.h"

template<typename T>
class AVLTree :
	public BinarySearchTree<T>
{
public:
	AVLTree(int (*init_rule)(const T& left, const T& right));

	AVLTree(T init_root_data, int (*init_rule)(const T& left, const T& right));

public:
	//Create new node with given data and Add that node into Tree. returns true if succeed
	virtual bool AddData(T newdata) override;

	/**	Find node which has given data.
	* @param target_data	data to search
	* @param start_node		the starting point of searching. use root of this Tree.
	* @param search_return	reperence of Node pointer value. This value will be set as a pointer to found node.
	* @return	returns true if found data
	*/
	//virtual bool FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const override;

	//find node with data and pop it from the tree
	virtual BinaryTreeNode<T>* PopData(T target_data) override;

	//find node with data and delete it from the tree
	virtual bool DeleteData(T target_data) override;


protected:
	//Add new node to the tree.
	virtual bool AddNode(BinaryTreeNode<T>* new_node) override;

	//insert given node under the target_node.  returns true if succeed.
	//virtual bool InsertNode(BinaryTreeNode<T>* target_node, BinaryTreeNode<T>* insert_node) override;

	/**	pop node out from the tree. without delete it.
	* @return	pointer of poped node.
	*/
	virtual BinaryTreeNode<T>* PopNode(BinaryTreeNode<T>* target_node) override;

	//pop node out from the tree. and delete it. returns true if succeed.
	virtual bool DeleteNode(BinaryTreeNode<T>* target_node) override;

public:
	static AVLTreeNode<T>* Cast(BinaryTreeNode<T>* target);

protected:
	AVLTreeNode<T>* BalanceTree(AVLTreeNode<T>* target);

	AVLTreeNode<T>* RotateTree(AVLTreeNode<T>* rotating_point, Direction rotating_direction);
};

template<typename T>
inline AVLTree<T>::AVLTree(int(*init_rule)(const T& left, const T& right)) : BinarySearchTree<T>(init_rule, true)
{

}

template<typename T>
inline AVLTree<T>::AVLTree(T init_root_data, int(*init_rule)(const T& left, const T& right))
	: BinarySearchTree<T>(init_rule, true)
{
	this->root = new AVLTreeNode<T>(init_root_data);
	this->IncreaseNodeCount();
}

template<typename T>
inline bool AVLTree<T>::AddData(T newdata)
{
	AVLTreeNode<T>* new_node = new AVLTreeNode<T>(newdata);
	if (AddNode(new_node)) return true;

	delete new_node;
	new_node = nullptr;
	return false;
}

template<typename T>
inline BinaryTreeNode<T>* AVLTree<T>::PopData(T target_data)
{
	BinaryTreeNode<T>* data_node = nullptr;
	
	if(!this->FindData(target_data, this->root, data_node)) return nullptr;

	if (Cast(data_node) == nullptr) return nullptr;

	return PopNode(data_node);
}

template<typename T>
inline bool AVLTree<T>::DeleteData(T target_data)
{
	BinaryTreeNode<T>* data_node = nullptr;

	if (!this->FindData(target_data, this->root, data_node)) return false;

	if (Cast(data_node) == nullptr) return false;

	return DeleteNode(data_node);
}

template<typename T>
inline bool AVLTree<T>::AddNode(BinaryTreeNode<T>* new_node)
{
	AVLTreeNode<T>* CheckPoint = Cast(new_node);
	if (CheckPoint == nullptr) return false;
	
	bool result =  BinarySearchTree<T>::AddNode(new_node);

	if (result)
	{
		CheckPoint = Cast(CheckPoint->GetParent());

		while (CheckPoint != nullptr)
		{
			int diff = CheckPoint->ResetHeight();
			CheckPoint = BalanceTree(CheckPoint);
			if (CheckPoint == nullptr || diff == 0) break;
			CheckPoint = Cast(CheckPoint->GetParent());
		}
	}
	
	return result;
}

template<typename T>
inline BinaryTreeNode<T>* AVLTree<T>::PopNode(BinaryTreeNode<T>* target_node)
{
	if (dynamic_cast<AVLTreeNode<T>*>(target_node) == nullptr) return nullptr;

	BinaryTreeNode<T>* replace_node = nullptr;
	//_PopNode_FindReplace(target_node, replace_node);
	BinarySearchTree<T>::_PopNode_FindReplace(target_node, replace_node);

	AVLTreeNode<T>* CheckPoint = Cast(replace_node->GetParent());

	BinarySearchTree<T>::_PopNode_SwapAndDetach(target_node, replace_node);
	
	AVLTreeNode<T>* AVL_replacer = dynamic_cast<AVLTreeNode<T>*>(replace_node);
	if (AVL_replacer != nullptr)
	{
		AVL_replacer->_SetHeightManualy(dynamic_cast<AVLTreeNode<T>*>(target_node)->GetHeight());
	}
	
	//reset heights
	while (CheckPoint != nullptr)
	{
		int diff = CheckPoint->ResetHeight();
		CheckPoint = BalanceTree(CheckPoint);
		if (CheckPoint == nullptr || diff == 0) break;
		CheckPoint = Cast(CheckPoint->GetParent());
	}

	this->DecreaseNodeCount();

	return replace_node;
}

template<typename T>
inline bool AVLTree<T>::DeleteNode(BinaryTreeNode<T>* target_node)
{
	if (Cast(target_node) == nullptr) return false;

	return __super::DeleteNode(target_node);
}

template<typename T>
inline AVLTreeNode<T>* AVLTree<T>::Cast(BinaryTreeNode<T>* target)
{
	return dynamic_cast<AVLTreeNode<T>*>(target);
}

template<typename T>
inline AVLTreeNode<T>* AVLTree<T>::BalanceTree(AVLTreeNode<T>* target)
{
	if (target == nullptr) return nullptr;

	AVLTreeNode<T>* replace = target;
	if (target->GetBalanceFactor() >= 2) 
	{
		//오른쪽이 무겁다
		if (target->GetChild(Direction::Right)->GetBalanceFactor() < 0)
		{
			//RL
			RotateTree(target->GetChild(Direction::Right), Direction::Right);
		}
		//RR
		replace = RotateTree(target, Direction::Left);
	}
	else if (target->GetBalanceFactor() <= -2) 
	{
		//왼쪽이 무겁다
		if (target->GetChild(Direction::Left)->GetBalanceFactor() > 0)
		{
			//LR
			RotateTree(target->GetChild(Direction::Left), Direction::Left);
		}
		//LL
		replace = RotateTree(target, Direction::Right);
	}
	return replace;
}

template<typename T>
inline AVLTreeNode<T>* AVLTree<T>::RotateTree(AVLTreeNode<T>* rotating_point, Direction rotating_direction)
{
	if (rotating_direction >= Direction::None)
	{
		std::cerr << LINE_INFO << std::endl << "Direction input error" << int(rotating_direction) << std::endl;
		return rotating_point;
	}

	Direction opposite = (rotating_direction == Direction::Left) ? Direction::Right : Direction::Left;

	AVLTreeNode<T>* child = rotating_point->GetChild(opposite);
	if (child == nullptr)
	{
		std::cerr << LINE_INFO << std::endl << "Can't rotate because there is no child node at oppsite side." << int(opposite) << std::endl;
		return rotating_point;
	}	


	AVLTreeNode<T>* grand_child = Cast(BinaryTree<T>::DetachChild(child, rotating_direction));

	Direction parent_direction = rotating_point->GetDirectionFrom();
	
	AVLTreeNode<T>* parent = Cast(BinaryTree<T>::DetachFromParent(rotating_point));;
	
	BinaryTree<T>::DetachFromParent(child);
	
	bool attachment_error = false;
	attachment_error = attachment_error ||
		!(BinaryTree<T>::AttachNode(child, rotating_point, rotating_direction));

	if(grand_child != nullptr)
		attachment_error = attachment_error ||
		!(BinaryTree<T>::AttachNode(rotating_point, grand_child, opposite));

	if (parent_direction < Direction::None)
	{
		attachment_error = attachment_error ||
			!(BinaryTree<T>::AttachNode(parent, child, parent_direction));
	}
	else if (rotating_point == this->root)
	{
		this->root = child;
	}

	if (attachment_error)
	{
		std::cerr << LINE_INFO << std::endl << "Node Attachment Error." << std::endl;
	}

	if (rotating_point != nullptr) rotating_point->ResetHeight();
	if (child != nullptr) child->ResetHeight();
	if (parent != nullptr) parent->ResetHeight();
	
	return child;
}