#define _CRT_SECURE_NO_WARNINGS 1
#include "BTree.h"

void BTreeTest()
{
	BTree<int,3> btree;
	int a[] = { 53, 75, 139, 49, 145, 36, 101 };
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		btree.Insert(a[i]);
	}
	btree.Find(5);
}
int main()
{
	BTreeTest();
	system("pause");
	return 0;
}