#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

//比较函数,这里比较类型为 long
int compare(void *a, void *b)
{
	int _a = (long)a;
	int _b = (long)b;
	if(_a < _b) return -1;
	if(_a > _b) return 1;
	return 0;
}

//打印各结点及其父结点
void rbtree_print(struct rbtree *T, struct rbtree_node* x)
{
	if(x != T->NIL)
	{
		if(x->color == RED)
			printf("%ld(RED) -> %ld\n", (long)x->key, (long)x->p->key);
		else
			printf("%ld(BLACK) -> %ld\n", (long)x->key, (long)x->p->key);
		rbtree_print(T, x->left);
		rbtree_print(T, x->right);
	}
}

//测试数据来自《算法导论》
int main()
{

	rbtree rbt;
	rbtree_init(&rbt);
	long res;

	rbtree_insert(&rbt, (void*)26, compare);
	rbtree_insert(&rbt, (void*)17, compare);
	rbtree_insert(&rbt, (void*)41, compare);
	rbtree_insert(&rbt, (void*)14, compare);
	rbtree_insert(&rbt, (void*)21, compare);
	rbtree_insert(&rbt, (void*)30, compare);
	rbtree_insert(&rbt, (void*)47, compare);
	rbtree_insert(&rbt, (void*)10, compare);
	rbtree_insert(&rbt, (void*)16, compare);
	rbtree_insert(&rbt, (void*)19, compare);
	rbtree_insert(&rbt, (void*)23, compare);
	rbtree_insert(&rbt, (void*)28, compare);
	rbtree_insert(&rbt, (void*)38, compare);
	rbtree_insert(&rbt, (void*)7, compare);
	rbtree_insert(&rbt, (void*)12, compare);
	rbtree_insert(&rbt, (void*)15, compare);
	rbtree_insert(&rbt, (void*)20, compare);
	rbtree_insert(&rbt, (void*)35, compare);
	rbtree_insert(&rbt, (void*)39, compare);
	rbtree_insert(&rbt, (void*)3, compare);

	rbtree_print(&rbt, rbt.root);
	printf("\n");

	res = (long)rbtree_minimum(&rbt);
	printf("min : %ld\n", res);
	res = (long)rbtree_maximum(&rbt);
	printf("max : %ld\n", res);
	printf("\n");


	rbtree_delete(&rbt, (void*)41, compare);
	rbtree_delete(&rbt, (void*)47, compare);
	rbtree_delete(&rbt, (void*)10, compare);
	rbtree_delete(&rbt, (void*)16, compare);
	rbtree_delete(&rbt, (void*)19, compare);
	rbtree_delete(&rbt, (void*)23, compare);
	rbtree_delete(&rbt, (void*)28, compare);
	rbtree_delete(&rbt, (void*)38, compare);

	rbtree_print(&rbt, rbt.root);
	printf("\n");

	res = (long)rbtree_minimum(&rbt);
	printf("min : %ld\n", res);
	res = (long)rbtree_maximum(&rbt);
	printf("max : %ld\n", res);
	printf("\n");

	rbtree_destroy(&rbt);
	return 0;
}

