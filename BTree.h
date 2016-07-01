#include<iostream>
using namespace std;

#ifndef __BTREE_H__
#define __BTREE_H__

template<class K,int M=3>//设为三阶B树（每个数组三个关键字）
struct BNode
{
	BNode<K,M>* _parent;
	size_t _size;//元素个数
	K keys[M];//由于subs多了一个长度
	BNode<K,M> *subs[M+1];//为了使插入元素分裂时先讲该数放进数组中再调整而又不越界
	BNode()
		:_parent(NULL)
		,_size(0)
	{
		for (int i = 0; i <= M + 1; i++)
		{
			subs[i] = NULL;//指针数组每个元素都为空指针,key不初始化？？？？sub怎么初始化？？？
		}
	}
};

template<class K,class V>
struct Pair
{
	K _first;
	V _second;
	Pair(const K& key=K(),const V& value=V())
		:_first(key)
		, _second(value)
	{}
};


template<class K,int M=3>
class BTree
{
	typedef BNode<K,M> Node;
public:
	BTree()
		:_root(NULL)
	{}


	Pair<Node*,int> Find(const K& key)
	{
		if (_root == NULL)
			return Pair<Node*,int>(NULL,-1);
		//思路：根据元素的大小以及遍历每个数组的关键字判断要走哪一条支路，再逐层逐层查找
		Node* cur = _root;
		Node* parent = NULL;
		while (cur)
		{
			int index = 0;
			while (index < cur->_size)
			{
				if (cur->keys[index] > key)
				{
					break;
				}
				else if (cur->keys[index] < key)
				{
					index++;
				}
				else
				{
					return Pair<Node*, int>(parent, index);
				}
			}
			parent = cur;
			cur = cur->subs[index];
		}
		return Pair<Node*, int>(parent, -1);
		//找完也没找到,为了使得该情况下方便插入节点，因此返回parent,插入节点插在parent上
	}


	bool Insert(const K& key)
	{
		//思路：若无结点，插入根节点；若有节点，找到合适位置插入在根节点上，如果此时插上该节点后
		//元素个数_size=M,则分裂，向上调整中位数，若调整后仍然是这样，则继续调整，直到不满足。
		if (_root == NULL)
		{
			_root = new Node;
			_root->keys[0] = key;
			_root->subs[0] = NULL;
			_root->_parent = NULL;
			_root->_size++;
			return true;
		}
		//若有该节点了，则不插入重复节点！！！
		if (Find(key)._second != -1)//不重复
		{
			return false;
		}
		Node* cur = Find(key)._first;//!!!根据查找到的位置直接插，避免代码冗余
		Node* sub = NULL;
		int insertKey = key;
		while (1)
		{
			_Insert(cur, sub, insertKey);//先将该元素key插入，放在该数组的合适位置
			if (cur->_size < M)//插入后，无需分裂
			{
				return true;
			}
			//需要分裂		
			int index = 0;
			Node* tmp = new Node;
			int mid = (cur->_size-1) / 2;
			//拷贝key值
			for (int i = mid + 1; i < cur->_size; i++)
			{
				tmp->keys[index++] = cur->keys[i];
				tmp->_size++;
			}
			//拷贝关键字的下标
			for (int i = mid + 1; i <= cur->_size; i++)
			{
				tmp->subs[index++] = cur->subs[i];
				if (cur->subs[i])//有子链，链上
				{
					cur->subs[i]->_parent = tmp;
				}
			}
			cur->_size = (cur->_size - 1) / 2;//更新cur数组元素个数
			if (cur->_parent == NULL)
			{
				//分两条支路，左支路为cur,右支路为tmp链上
				_root = new Node;
				_root->keys[0] = cur->keys[mid];
				_root->subs[0] = cur;
				_root->subs[1] = tmp;
				_root->_size++;
				cur->_parent = _root;
				tmp->_parent = _root;
				return true;
			}
			else
			{			
				insertKey = cur->keys[mid];//该插入mid位置元素
				sub = tmp;//子串便成为tmp
				cur = cur->_parent;//继续调整
			}
		}
		
	}


protected:
	void _Insert(Node* cur, Node* sub,const K& key)
	{
		size_t index = cur->_size;
		while (index >= 0 && cur->keys[index]>key)//为便于key查到合适位置，先将元素往后移动1个长度到合适位置
		{
			cur->keys[index+1] = cur->keys[index];
			cur->subs[index + 1] = cur->subs[index];//下标也要更新
			index--;
		}	
		cur->keys[index+1] = key;
		cur->subs[index + 2] = sub;
		if (sub)
		{
			sub->_parent = cur;
		}
		cur->_size++;
	}
private:
	Node* _root;
};
#endif //__BTREE_H__