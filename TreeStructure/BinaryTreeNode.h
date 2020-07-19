#pragma once

enum class Direction
{
	Left,
	Right,
	Both,
	None
};

/**	이진트리노드
*	Simple Node for Binary Trees like : Binary Search Tree, Heap Tree, AVL Tree, atc
*/
template<typename T>
class BinaryTreeNode
{
	template<typename U> friend class BinaryTree;
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

	/**	주어진 방향에 붙어있는 자식 노드의 포인터를 반환합니다.
	*	Get the point of child node attached on given direction
	* @return	pointer of child node including nullptr. if Direction is incorrect, returns nullptr
	*/
	BinaryTreeNode<T>* GetChild(Direction dir);

	/**	붙어있는 자식노드의 수를 return합니다.
	*	returns number of child nodes attached on this node.
	*/
	int GetNumberofChilden();
	
	/**	이 노드가 부모의 어느쪽 가지에 붙어있는지 구합니다.
	*	Get the direction of the branch which this node is attached. 
	* @return	Direction::Left or Diretion::Right. if there is no parent node, returns Direction::None
	*/
	Direction GetDirectionFrom();
	
protected:
	/**	해당 노드의 데이터를 수정합니다.
	*	rewrite the data of this node
	* @remark	이 함수는 사용 시 트리 구조가 꼬이게 할 수 있습니다. 따라서 항상 Pop으로 분리 시킨뒤 사용하고, 수정한뒤 다시 넣으십시오.
				this function could damage the rule of tree structure. so pop this node from tree before use it.
	*/
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
inline BinaryTreeNode<T>* BinaryTreeNode<T>::GetChild(Direction dir)
{
	switch (dir)
	{
	case Direction::Left :
		return child_left;
	case Direction::Right :
		return child_right;
	default:
		return nullptr;
	}
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
inline Direction BinaryTreeNode<T>::GetDirectionFrom()
{
	if (parent == nullptr) return Direction::None;
	if (parent->child_left == this) return Direction::Left;
	if (parent->child_right == this) return Direction::Right;

	return Direction::None;
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
