#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

//左旋,x的右结点成为其父结点
static void rbt_left_rotate(struct rbtree *T, struct rbtree_node *x)
{
	if(x->right != T->NIL && T->root->p == T->NIL)	//左旋的前提条件
	{
		struct rbtree_node *y = x->right;
		x->right = y->left;

		if(y->left != T->NIL)
		{
			y->left->p = x;
		}
		y->p = x->p;

		if(x->p == T->NIL)
		{
			T->root = y;
		}
		else if(x == x->p->left)
		{
			x->p->left = y;
		}
		else
		{
			x->p->right = y;
		}

		y->left = x;
		x->p = y;
	}
}

//右旋,y的左结点成为其父结点
static void rbt_right_rotate(struct rbtree *T, struct rbtree_node *y)
{
	if(y->left != T->NIL && T->root->p == T->NIL)	//右旋的前提条件
	{
		struct rbtree_node *x = y->left;
		y->left = x->right;

		if(x->right != T->NIL)
		{
			x->right->p = y;
		}
		x->p = y->p;

		if(y->p == T->NIL)
		{
			T->root = x;
		}
		else if(y == y->p->right)
		{
			y->p->right = x;
		}
		else
		{
			y->p->left = x;
		}

		x->right = y;
		y->p = x;
	}

}

//寻找x子树最小值结点的指针
static struct rbtree_node* rbt_minimum(struct rbtree *T, struct rbtree_node* x)
{
	while(x->left != T->NIL)
	{
		x = x->left;
	}

	return x;
}

//寻找x子树最大值结点的指针
static struct rbtree_node* rbt_maximum(struct rbtree *T, struct rbtree_node* x)
{
	while(x->right != T->NIL)
	{
		x = x->right;
	}

	return x;
}

//用v子树替代u子树
static void rbt_transplant(struct rbtree *T, struct rbtree_node *u, struct rbtree_node * v)
{
	if(u->p == T->NIL)
	{
		T->root = v;
	}
	else if(u == u->p->left)
	{
		u->p->left = v;
	}
	else
	{
		u->p->right = v;
	}
	v->p = u->p;
}

//插入后的调整, 对应三种情况
static void rbt_insert_fixup(struct rbtree *T, struct rbtree_node *z)
{
	while(z->p->color == RED)
	{
		if(z->p == z->p->p->left)
		{
			struct rbtree_node *y = z->p->p->right;
			if(y->color == RED)
			{
				z->p->color = BLACK;			//case1
				y->color = BLACK;				//case1
				z->p->p->color = RED;			//case1
				z = z->p->p;					//case1
			}
			else if(z == z->p->right)
			{
				z = z->p;						//case2
				rbt_left_rotate(T, z);			//case2
			}
			else
			{
				z->p->color = BLACK;			//case3
				z->p->p->color = RED;			//case3
				rbt_right_rotate(T, z->p->p);	//case3
			}

		}
		else	//以上的对称代码
		{
			struct rbtree_node *y = z->p->p->left;
			if(y->color == RED)
			{
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else if(z == z->p->left)
			{
				z = z->p;
				rbt_right_rotate(T, z);
			}
			else
			{
				z->p->color = BLACK;
				z->p->p->color = RED;
				rbt_left_rotate(T, z->p->p);
			}
		}
	}

	T->root->color = BLACK;
}

//删除后的调整,对应四种情况
static void rbt_delete_fixup(struct rbtree *T, struct rbtree_node *x)
{
	while(x != T->root && x->color == BLACK)
	{
		if(x == x->p->left)
		{
			struct rbtree_node *w = x->p->right;
			if(w->color == RED)
			{
				w->color = BLACK;				//case1
				x->p->color = RED;				//case1
				rbt_left_rotate(T, x->p);		//case1
				w = x->p->right;				//case1
			}
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;					//case2
				x = x->p;						//case2
			}
			else if(w->right->color == BLACK)
			{
				w->left->color = BLACK;			//case3
				w->color = RED;					//case3
				rbt_right_rotate(T, w);			//case3
				w = x->p->right;				//case3
			}
			else
			{
				w->color = x->p->color;			//case4
				x->p->color = BLACK;			//case4
				w->right->color = BLACK;		//case4
				rbt_left_rotate(T, x->p);		//case4
				x = T->root;
			}
		}
		else	//以上的对称代码
		{
			struct rbtree_node *w = x->p->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->p->color = RED;
				rbt_right_rotate(T, x->p);
				w = x->p->left;
			}
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->p;
			}
			else if(w->left->color == BLACK)
			{
				w->right->color = BLACK;
				w->color = RED;
				rbt_left_rotate(T, w);
				w = x->p->left;
			}
			else
			{
				w->color = x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				rbt_right_rotate(T, x->p);
				x = T->root;
			}
		}
	}

	x->color = BLACK;
}

//查找后返回结点指针
static struct rbtree_node* rbt_search(struct rbtree *T, void *key, int (*compare)(void *, void *))
{
	struct rbtree_node *x = T->root;

	while(x != T->NIL)
	{
		if(compare(x->key, key) == -1)
		{
			x = x->right;
		}
		else if(compare(x->key, key) == 1)
		{
			x = x->left;
		}
		else
		{
			return x;
		}
	}
	return x;
}

//递归销毁
static void rbt_destory(struct rbtree *T, struct rbtree_node *x)
{
	if(x != T->NIL)
	{
		rbt_destory(T, x->left);
		rbt_destory(T, x->right);
		free(x);
	}
}

//红黑树的初始化
void rbtree_init(struct rbtree *T)
{
	T->NIL = (struct rbtree_node*)malloc(sizeof(struct rbtree_node));
	T->NIL->color = BLACK;

	//以下四个指针可以初始化任意值
	T->NIL->key = NULL;
	T->NIL->left = NULL;
	T->NIL->p = NULL;
	T->NIL->right = NULL;

	T->root = T->NIL;
}

//红黑树的销毁
void rbtree_destroy(struct rbtree *T)
{
	rbt_destory(T, T->root);
	if(T->NIL != NULL)
		free(T->NIL);
}

//红黑树的查找
int rbtree_search(struct rbtree *T, void *key, int (*compare)(void *, void *))
{
	struct rbtree_node *x = rbt_search(T, key, compare);
	if(x == T->NIL)
	{
		return -1;
	}
	return 0;
}

//红黑树的插入
void rbtree_insert(struct rbtree *T, void *key,int (*compare)(void *, void *))
{
	struct rbtree_node *y = T->NIL;
	struct rbtree_node *x = T->root;
	struct rbtree_node *z = (struct rbtree_node*)malloc(sizeof(struct rbtree_node));
	z->key = key;
	z->left = T->NIL;
	z->right = T->NIL;
	z->color = RED;

	while(x != T->NIL)
	{
		y = x;
		if(compare(z->key, x->key) == -1)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}

	z->p = y;
	if(y == T->NIL)
	{
		T->root = z;
	}
	else if(compare(z->key, y->key) == -1)
	{
		y->left = z;
	}
	else
	{
		y->right = z;
	}

	rbt_insert_fixup(T, z);
}

//红黑树的删除
void rbtree_delete(struct rbtree *T, void *key, int (*compare)(void *, void *))
{
	struct rbtree_node *z = rbt_search(T, key, compare);
	if(z == T->NIL)
		return;

	struct rbtree_node *y = z;
	struct rbtree_node *x = T->NIL;
	int y_original_color = y->color;

	if(z->left == T->NIL)
	{
		x = z->right;
		rbt_transplant(T, z, z->right);
	}
	else if(z->right == T->NIL)
	{
		x = z->left;
		rbt_transplant(T, z, z->left);
	}
	else
	{
		y = rbt_minimum(T, z->right);
		y_original_color = y->color;
		x = y->right;

		if(y->p == z)
		{
			x->p = y;
		}
		else
		{
			rbt_transplant(T, y, y->right);
			y->right = z->right;
			y->right->p = y;
		}

		rbt_transplant(T, z, y);
		y->left = z->left;
		y->left->p = y;
		y->color = z->color;
	}

	if(y_original_color == BLACK)
	{
		rbt_delete_fixup(T, x);
	}
}

//返回红黑树的最小值
void* rbtree_minimum(struct rbtree *T)
{
	struct rbtree_node *x = rbt_minimum(T, T->root);
	if(x == T->NIL)
		return NULL;
	return x->key;
}

//返回红黑树的最大值
void* rbtree_maximum(struct rbtree *T)
{
	struct rbtree_node *x = rbt_maximum(T, T->root);
	if(x == T->NIL)
		return NULL;
	return x->key;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
