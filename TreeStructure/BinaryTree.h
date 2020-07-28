#pragma once
#include "BinaryTreeNode.h"

/**	place holder to force subclasses to overrdie certaion non virtual fuctions.
*/
template<typename T>
class BinaryTreePlaceHolder
{
protected:
	virtual bool AddNode(BinaryTreeNode<T>* new_node) = 0;

	virtual bool InsertNode(BinaryTreeNode<T>* target_node, BinaryTreeNode<T>* insert_node) = 0;

	virtual BinaryTreeNode<T>* PopNode(BinaryTreeNode<T>* target_node) = 0;
};

/**	Base Class for all Binary Trees.
*/
template<typename T>
class BinaryTree : public BinaryTreePlaceHolder<T>
{
public:
	BinaryTree(int (*init_rule)(const T& superior, const T& minor), bool IsSet = false);

	BinaryTree(T init_root_data, int (*init_rule)(const T& superior, const T& minor), bool IsSet = false);

	virtual ~BinaryTree();
public:
	/**	if this value is true, this tree cann't have duplicated data.
		default value is false
	*/
	const bool IsSet;

	
protected:
	
	BinaryTreeNode<T>* root = nullptr;

	int (*rule)(const T& superior, const T& minor);

	int node_count = 0;


public:
	/** remove this node and every other nodes under it
	* @param sub_root	reperence of pointer variable of target node. this variable will set as nullptr after delete.
	* @remark	this functions is used in Destructor of Binary Tree Class
	*/
	void RemoveSubTree(BinaryTreeNode<T>*& sub_root);

	// get number of nodes in this tree.
	int GetNodeCount();

	BinaryTreeNode<T>* GetRoot();

	//Create new node with given data and Add that node into Tree. returns true if succeed
	virtual bool AddData(T newdata) = 0;

	/**	Find node which has given data.
	* @param target_data	data to search
	* @param start_node		the starting point of searching. use root of this Tree.
	* @param search_return	reperence of Node pointer value. This value will be set as a pointer to found node.
	* @return	returns true if found data
	*/
	virtual bool FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const = 0;

	//find node with data and pop it from the tree
	virtual BinaryTreeNode<T>* PopData(T target_data) = 0;

	//find node with data and delete it from the tree. returns true if succeed
	virtual bool DeleteData(T target_data) = 0;

protected:	

	// this fuction increase node_count, and always return true
	virtual bool AddNode(BinaryTreeNode<T>* new_node) override;

	// this fuction increase node_count, and always return true
	virtual bool InsertNode(BinaryTreeNode<T>* target_node, BinaryTreeNode<T>* insert_node) override;

	// this fuction decrease node_count, and always return nullptr
	virtual BinaryTreeNode<T>* PopNode(BinaryTreeNode<T>* target_node) override;

	//pop node out from the tree. and delete it. returns ;true if succeed.
	virtual bool DeleteNode(BinaryTreeNode<T>* target_node) = 0;

	int GetDepth(BinaryTreeNode<T>* start_node);

	/**	attach two nodes with given direction
	* @param target_parent	node which will become a parent of target_child
	* @param target_child	node to attach
	* @param dir			direction to attach
	*/
	static bool AttachNode(BinaryTreeNode<T>* target_parent, BinaryTreeNode<T>* target_child, Direction dir);

	/** Detach child node on given direction and returns pointer of child node.
	* @return	pointer of detached child. if failed, nullptr
	* @remark	this fuction doesn't delete the detached child node.
	*/
	static BinaryTreeNode<T>* DetachChild(BinaryTreeNode<T>* target_parent, Direction child_selector);

	/** Detach target_child node from parent and returns pointer of parent node.
	* @return	pointer of detached parent node. if failed, nullptr
	* @remark	this fuction doesn't delete the detached child node.
	*/
	static BinaryTreeNode<T>* DetachFromParent(BinaryTreeNode<T>* target_child);

	bool SwapWithChild(BinaryTreeNode<T>* target_parent, Direction child_selector);

	static bool SwapDataPtr(BinaryTreeNode<T>* target1, BinaryTreeNode<T>* target2);

	void IncreaseNodeCount() { ++node_count; };
	void DecreaseNodeCount() { if(node_count >0 ) --node_count; };
};

template<typename T>
inline BinaryTree<T>::BinaryTree(int(*init_rule)(const T& superior, const T& minor), bool IsSet) : rule(init_rule), IsSet(IsSet)
{

}

template<typename T>
inline BinaryTree<T>::BinaryTree(T init_root_data, int(*init_rule)(const T& superior, const T& minor), bool IsSet) : rule(init_rule), IsSet(IsSet)
{
	root = new BinaryTreeNode<T>(init_root_data);
	node_count = 1;
}

template<typename T>
inline BinaryTree<T>::~BinaryTree()
{
	RemoveSubTree(root);
}

template<typename T>
inline void BinaryTree<T>::RemoveSubTree(BinaryTreeNode<T>* &sub_root)
{
	if (sub_root == nullptr) return;

	RemoveSubTree(sub_root->child_left);
	RemoveSubTree(sub_root->child_right);
	delete sub_root;
	sub_root = nullptr;
	--node_count;
}

template<typename T>
inline int BinaryTree<T>::GetNodeCount()
{
	return node_count;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryTree<T>::GetRoot()
{
	return root;
}

template<typename T>
inline bool BinaryTree<T>::FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const
{
	if (start_node == nullptr) return false;

	int compare = rule(target_data, start_node->GetData());

	if (compare == 0)
	{
		search_return = start_node;
		return true;
	}
	
	bool left_search = FindData(target_data, start_node->child_left, search_return);
	bool right_search = FindData(target_data, start_node->child_right, search_return);

	return left_search || right_search;

}

template<typename T>
inline int BinaryTree<T>::GetDepth(BinaryTreeNode<T>* start_node)
{
	if (start_node == nullptr) return 0;
	// -1이 아닌 0로 하는 이유는, start_node = root 일때 정확히 몇 단인지 나오기 때문

	int left = GetDepth(start_node->child_left);
	int right = GetDepth(start_node->child_right);

	return left > right ? left+1 : right+1;
}

template<typename T>
inline bool BinaryTree<T>::AddNode(BinaryTreeNode<T>* new_node)
{
	++node_count;
	return true;
}

template<typename T>
inline bool BinaryTree<T>::InsertNode(BinaryTreeNode<T>* target_node, BinaryTreeNode<T>* insert_node)
{
	++node_count;
	return true;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryTree<T>::PopNode(BinaryTreeNode<T>* target_node)
{
	--node_count;
	return target_node;
}

template<typename T>
inline bool BinaryTree<T>::AttachNode(BinaryTreeNode<T>* target_parent, BinaryTreeNode<T>* target_child, Direction dir)
{
	if (target_parent == nullptr || target_child == nullptr) return false;
	if (target_child->parent != nullptr) return false;
	switch (dir)
	{
	case Direction::Left:
		if (target_parent->child_left != nullptr) return false;
		target_parent->child_left = target_child;
		break;
	case Direction::Right:
		if (target_parent->child_right != nullptr) return false;
		target_parent->child_right = target_child;
		break;
	default:
		return false;
	}
	target_child->parent = target_parent;
	return true;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryTree<T>::DetachChild(BinaryTreeNode<T>* target_parent, Direction child_selector)
{
	if (target_parent == nullptr) return nullptr;
	BinaryTreeNode<T>* target_child = nullptr;
	switch (child_selector)
	{
	case Direction::Left:
		target_child = target_parent->child_left;
		if (target_child == nullptr) break;
		target_parent->child_left = nullptr;
		target_child->parent = nullptr;
		break;
	case Direction::Right:
		target_child = target_parent->child_right;
		if (target_child == nullptr) break;
		target_parent->child_right = nullptr;
		target_child->parent = nullptr;
		break;
	case Direction::Both:
	case Direction::None:
	default:
		break;
	}

	return target_child;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryTree<T>::DetachFromParent(BinaryTreeNode<T>* target_child)
{
	if (target_child == nullptr || target_child->parent == nullptr) return nullptr;
	BinaryTreeNode<T>* result = target_child->parent;
	if(DetachChild(target_child->parent, target_child->GetDirectionFrom()) == target_child)
		return result;
	return nullptr;
}

template<typename T>
inline bool BinaryTree<T>::SwapWithChild(BinaryTreeNode<T>* target_parent, Direction child_selector)
{
	if (target_parent == nullptr) return false;

	BinaryTreeNode<T>* target_child = DetachChild(target_parent, child_selector);

	if (target_child == nullptr) return false;

	Direction opposite = (child_selector == Direction::Left) ?
		Direction::Right : Direction::Left;

	BinaryTreeNode<T>* target_child_sibling	= DetachChild(target_parent, opposite);
		
	BinaryTreeNode<T>* grand_child[2] = {DetachChild(target_child, Direction::Left), DetachChild(target_child, Direction::Right)};

	Direction parent_direction_from = target_parent->GetDirectionFrom();

	BinaryTreeNode<T>* grand_parent = DetachFromParent(target_parent);

	
	AttachNode(grand_parent, target_child, parent_direction_from);
	AttachNode(target_child, target_child_sibling, opposite);
	AttachNode(target_child, target_parent, child_selector);
	AttachNode(target_parent, grand_child[0], Direction::Left);
	AttachNode(target_parent, grand_child[1], Direction::Right);

	if (target_parent == this->root) this->root = target_child;
	
	return true;
}

template<typename T>
inline bool BinaryTree<T>::SwapDataPtr(BinaryTreeNode<T>* target1, BinaryTreeNode<T>* target2)
{
	if (target1 == nullptr || target2 == nullptr) return false;
	T* tempptr = target1->data_ptr;
	target1->data_ptr = target2->data_ptr;
	target2->data_ptr = tempptr;
	return true;
	
}
