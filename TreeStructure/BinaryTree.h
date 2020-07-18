#pragma once
#include "BinaryTreeNode.h"

template<typename T>
class BinaryTree
{
public:
	BinaryTree(int (*init_rule)(const T& super, const T& minor), bool IsSet = false);

	BinaryTree(T init_root_data, int (*init_rule)(const T& super, const T& minor), bool IsSet = false);

	virtual ~BinaryTree();
public:
	const bool IsSet;

	
protected:
	
	BinaryTreeNode<T>* root = nullptr;

	int (*rule)(const T& left, const T& right);

	int node_count = 0;


public:
	void RemoveSubTree(BinaryTreeNode<T>*& sub_root);

	int GetNodeCount();

	BinaryTreeNode<T>* GetRoot();

	//add data;
	virtual bool AddData(T newdata) = 0;

	//find data;
	virtual bool FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const = 0;

	//find node with data and pop it from the tree
	virtual BinaryTreeNode<T>* PopData(T target_data) = 0;

	virtual bool DeleteData(T target_data) = 0;

protected:
	int GetDepth(BinaryTreeNode<T>* start_node);

	//pop node out from the tree. without delete it.
	virtual BinaryTreeNode<T>* PopNode(BinaryTreeNode<T>* target_node) = 0;

	virtual bool DeleteNode(BinaryTreeNode<T>* target_node) = 0;

};

template<typename T>
inline BinaryTree<T>::BinaryTree(int(*init_rule)(const T& super, const T& minor), bool IsSet) : rule(init_rule), IsSet(IsSet)
{
	
}

template<typename T>
inline BinaryTree<T>::BinaryTree(T init_root_data, int(*init_rule)(const T& super, const T& minor), bool IsSet) : rule(init_rule), IsSet(IsSet)
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
	if (start_node = nullptr) return false;

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
	// -1�� �ƴ� 0�� �ϴ� ������, start_node = root �϶� ��Ȯ�� �� ������ ������ ����

	int left = GetDepth(start_node->child_left);
	int right = GetDepth(start_node->child_right);

	return left > right ? left+1 : right+1;
}
