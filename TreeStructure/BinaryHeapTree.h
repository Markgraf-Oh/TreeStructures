#pragma once
#include "BinaryTree.h"
#include <queue>

/* 힙트리
 *@param init_rule	the fuction which gets superior and minor value, and returns
 *					+1 if they aligned correctly,
 *					0 if they are same,
 *					-1 if they need to be aligned.
 */
template<typename T>
class BinaryHeapTree :
	protected BinaryTree<T>
{
public:
	BinaryHeapTree(int (*init_rule)(const T& superior, const T& minor));

	BinaryHeapTree(T init_root_data, int (*init_rule)(const T& superior, const T& minor));

protected:
	BinaryTreeNode<T>* node_end = nullptr;

protected://상속함수
	//Create new node with given data and Add that node into Tree. returns true if succeed
	virtual bool AddData(T newdata) override;

	/**	Find node which has given data.
	* @param target_data	data to search
	* @param start_node		the starting point of searching. use root of this Tree.
	* @param search_return	reperence of Node pointer value. This value will be set as a pointer to found node.
	* @return	returns true if found data
	*/
	virtual bool FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const override;

	/** empty place holder
	*/
	virtual BinaryTreeNode<T>* PopData(T target_data) override;

	/** empty place holder
	*/
	virtual bool DeleteData(T target_data) override;

	virtual bool DeleteNode(BinaryTreeNode<T>* target_node) override;

public:
	//get front
	BinaryTreeNode<T>* GetFront() const;
	//pop front
	bool PopFront();
	//get back
	const BinaryTreeNode<T>* GetBack() const;
	//pop back
	bool PopBack();
	//push
	BinaryTreeNode<T>* Push(T data);
	
	void ShowTree();

protected:
	//shiftup
	void ShiftUp(BinaryTreeNode<T>* target_node);

	//heapup
	void ShiftDown(BinaryTreeNode<T>* target_node);
	
	//addend
	BinaryTreeNode<T>* AddEnd(T data);
};

template<typename T>
inline BinaryHeapTree<T>::BinaryHeapTree(int(*init_rule)(const T& superior, const T& minor))
	: BinaryTree<T>(init_rule, false)
{
}

template<typename T>
inline BinaryHeapTree<T>::BinaryHeapTree(T init_root_data, int(*init_rule)(const T& superior, const T& minor))
	: BinaryTree<T>(init_root_data, init_rule, false)
{
	node_end = this->root;
}


template<typename T>
inline bool BinaryHeapTree<T>::AddData(T newdata)
{
	return false;
}

template<typename T>
inline bool BinaryHeapTree<T>::FindData(const T& target_data, BinaryTreeNode<T>* start_node, BinaryTreeNode<T>*& search_return) const
{
	return false;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryHeapTree<T>::PopData(T target_data)
{
	return NULL;
}

template<typename T>
inline bool BinaryHeapTree<T>::DeleteData(T target_data)
{
	return false;
}

template<typename T>
inline bool BinaryHeapTree<T>::DeleteNode(BinaryTreeNode<T>* target_node)
{
	return false;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryHeapTree<T>::GetFront() const
{
	return this->root;
}

template<typename T>
inline bool BinaryHeapTree<T>::PopFront()
{
	if (this->root == nullptr) return false;
	if (this->root != this->node_end)
	{
		BinaryTree<T>::SwapDataPtr(this->root, this->node_end);
	}
	if (PopBack())
	{
		ShiftDown(this->root);
		return true;
	}
	return false;
}

template<typename T>
inline const BinaryTreeNode<T>* BinaryHeapTree<T>::GetBack() const
{
	return node_end;
}

template<typename T>
inline bool BinaryHeapTree<T>::PopBack()
{
	
	if (node_end == nullptr) return false;

	if (node_end == this->root)
	{
		BinaryTreeNode<T>* poped = this->DetachFromParent(this->root);
		delete poped;
		poped = nullptr;
		this->DecreaseNodeCount();
		node_end = nullptr;
		this->root = nullptr;
		return true;
	}
	//finding next node_end
	BinaryTreeNode<T>* next_end = node_end;

	while (next_end->GetDirectionFrom() == Direction::Left)
	{
		next_end = next_end->GetParent();
	}

	if (next_end == this->root)
	{
		while (next_end->GetChild(Direction::Right) != nullptr)
		{
			next_end = next_end->GetChild(Direction::Right);
		}		
	}
	else
	{
		next_end = next_end->GetParent()->GetChild(Direction::Left);
		while (next_end->GetChild(Direction::Right) != nullptr)
		{
			next_end = next_end->GetChild(Direction::Right);
		}
	}
		
	BinaryTreeNode<T>* poped = this->DetachFromParent(node_end);
	delete node_end;
	node_end = next_end;
	this->DecreaseNodeCount();
	return true;
}

template<typename T>
inline BinaryTreeNode<T>* BinaryHeapTree<T>::Push(T data)
{
	BinaryTreeNode<T>* new_node = AddEnd(data);
	ShiftUp(new_node);
	if (new_node != nullptr) this->IncreaseNodeCount();
	return new_node;
}

template<typename T>
inline void BinaryHeapTree<T>::ShowTree()
{
	std::queue<BinaryTreeNode<T>*> BFSQ;
	BFSQ.push(this->root);
	while (!BFSQ.empty())
	{
		if (BFSQ.front()->GetChild(Direction::Left) != nullptr)
		{
			BFSQ.push(BFSQ.front()->GetChild(Direction::Left));
		}
		if (BFSQ.front()->GetChild(Direction::Right) != nullptr)
		{
			BFSQ.push(BFSQ.front()->GetChild(Direction::Right));
		}
		std::cout << BFSQ.front()->GetData() << std::endl;
		BFSQ.pop();
	}
}

template<typename T>
inline void BinaryHeapTree<T>::ShiftUp(BinaryTreeNode<T>* target_node)
{
	if (target_node == nullptr) return;
	while ((target_node->GetParent() != nullptr) &&
		   (this->rule(target_node->GetParent()->GetData(), target_node->GetData()) < 0))
	{
		BinaryTree<T>::SwapDataPtr(target_node->GetParent(), target_node);
		target_node = target_node->GetParent();
	}
}

template<typename T>
inline void BinaryHeapTree<T>::ShiftDown(BinaryTreeNode<T>* target_node)
{
	if (target_node == nullptr) return;
	while (target_node->GetNumberofChilden() >= 1)
	{
		Direction swap_dir = Direction::None;
		if (target_node->GetNumberofChilden() == 1)
		{
			swap_dir = (target_node->GetChild(Direction::Left) != nullptr) ?
				Direction::Left : Direction::Right;
		}
		else
		{
			swap_dir =
				(this->rule(target_node->GetChild(Direction::Left)->GetData(), target_node->GetChild(Direction::Right)->GetData()) > 0) ?
				Direction::Left : Direction::Right;
		}

		if (this->rule(target_node->GetData(), target_node->GetChild(swap_dir)->GetData()) < 0)
		{
			BinaryTree<T>::SwapDataPtr(target_node, target_node->GetChild(swap_dir));
			target_node = target_node->GetChild(swap_dir);
		}
		else break;
	}
}

template<typename T>
inline BinaryTreeNode<T>* BinaryHeapTree<T>::AddEnd(T data)
{
	BinaryTreeNode<T>* next_end = nullptr;
	Direction next_dir = Direction::None;

	if (this->GetNodeCount() == 0)
	{
		next_end = new BinaryTreeNode<T>(data);
		this->node_end = next_end;
		this->root = next_end;
		return next_end;
	}

	BinaryTreeNode<T>* current_node = node_end;

	while (current_node->GetDirectionFrom() == Direction::Right)
	{
		current_node = current_node->GetParent();
	}
	

	if (current_node == this->root)
	{
		while (current_node->GetChild(Direction::Left) != nullptr)
		{
			current_node = current_node->GetChild(Direction::Left);
		}
		next_dir = Direction::Left;
	}
	else
	{
		current_node = current_node->GetParent();

		if (current_node->GetChild(Direction::Right) == nullptr)
		{
			next_dir = Direction::Right;
		}
		else
		{
			current_node = current_node->GetChild(Direction::Right);
			while (current_node->GetChild(Direction::Left) != nullptr)
				current_node = current_node->GetChild(Direction::Left);
			next_dir = Direction::Left;
		}
	}

	next_end = new BinaryTreeNode<T>(data);
	if (this->AttachNode(current_node, next_end, next_dir))
	{
		node_end = next_end;
	}
	else
	{
		delete next_end;
		next_end = nullptr;
	}
	return next_end;
}

