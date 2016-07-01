#include<iostream>
using namespace std;

#ifndef __BTREE_H__
#define __BTREE_H__

template<class K,int M=3>//��Ϊ����B����ÿ�����������ؼ��֣�
struct BNode
{
	BNode<K,M>* _parent;
	size_t _size;//Ԫ�ظ���
	K keys[M];//����subs����һ������
	BNode<K,M> *subs[M+1];//Ϊ��ʹ����Ԫ�ط���ʱ�Ƚ������Ž��������ٵ������ֲ�Խ��
	BNode()
		:_parent(NULL)
		,_size(0)
	{
		for (int i = 0; i <= M + 1; i++)
		{
			subs[i] = NULL;//ָ������ÿ��Ԫ�ض�Ϊ��ָ��,key����ʼ����������sub��ô��ʼ��������
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
		//˼·������Ԫ�صĴ�С�Լ�����ÿ������Ĺؼ����ж�Ҫ����һ��֧·�������������
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
		//����Ҳû�ҵ�,Ϊ��ʹ�ø�����·������ڵ㣬��˷���parent,����ڵ����parent��
	}


	bool Insert(const K& key)
	{
		//˼·�����޽�㣬������ڵ㣻���нڵ㣬�ҵ�����λ�ò����ڸ��ڵ��ϣ������ʱ���ϸýڵ��
		//Ԫ�ظ���_size=M,����ѣ����ϵ�����λ��������������Ȼ�������������������ֱ�������㡣
		if (_root == NULL)
		{
			_root = new Node;
			_root->keys[0] = key;
			_root->subs[0] = NULL;
			_root->_parent = NULL;
			_root->_size++;
			return true;
		}
		//���иýڵ��ˣ��򲻲����ظ��ڵ㣡����
		if (Find(key)._second != -1)//���ظ�
		{
			return false;
		}
		Node* cur = Find(key)._first;//!!!���ݲ��ҵ���λ��ֱ�Ӳ壬�����������
		Node* sub = NULL;
		int insertKey = key;
		while (1)
		{
			_Insert(cur, sub, insertKey);//�Ƚ���Ԫ��key���룬���ڸ�����ĺ���λ��
			if (cur->_size < M)//������������
			{
				return true;
			}
			//��Ҫ����		
			int index = 0;
			Node* tmp = new Node;
			int mid = (cur->_size-1) / 2;
			//����keyֵ
			for (int i = mid + 1; i < cur->_size; i++)
			{
				tmp->keys[index++] = cur->keys[i];
				tmp->_size++;
			}
			//�����ؼ��ֵ��±�
			for (int i = mid + 1; i <= cur->_size; i++)
			{
				tmp->subs[index++] = cur->subs[i];
				if (cur->subs[i])//������������
				{
					cur->subs[i]->_parent = tmp;
				}
			}
			cur->_size = (cur->_size - 1) / 2;//����cur����Ԫ�ظ���
			if (cur->_parent == NULL)
			{
				//������֧·����֧·Ϊcur,��֧·Ϊtmp����
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
				insertKey = cur->keys[mid];//�ò���midλ��Ԫ��
				sub = tmp;//�Ӵ����Ϊtmp
				cur = cur->_parent;//��������
			}
		}
		
	}


protected:
	void _Insert(Node* cur, Node* sub,const K& key)
	{
		size_t index = cur->_size;
		while (index >= 0 && cur->keys[index]>key)//Ϊ����key�鵽����λ�ã��Ƚ�Ԫ�������ƶ�1�����ȵ�����λ��
		{
			cur->keys[index+1] = cur->keys[index];
			cur->subs[index + 1] = cur->subs[index];//�±�ҲҪ����
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