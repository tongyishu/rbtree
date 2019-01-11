#ifndef RBTREE_H
#define RBTREE_H
#define RED 0
#define BLACK 1

/*
 * 红黑树的结点结构
 */
typedef struct rbtree_node
{
	void *key;
	int color;
	struct rbtree_node *left, *right, *p;
}rbtree_node;

/*
 * 红黑树的结构
 */
typedef struct rbtree
{
	struct rbtree_node *root;
	struct rbtree_node *NIL;
}rbtree;

/*
 * 红黑树的初始化
 * @T 指向红黑树结构体的指针
 */
void rbtree_init(struct rbtree *T);

/*
 * 红黑树的销毁
 * @T 指向红黑树结构体的指针
 */
void rbtree_destroy(struct rbtree *T);

/*
 * 红黑树的插入
 * @T 指向红黑树结构体的指针
 * @key 要插入的数据
 * @compare 插入数据的比较函数指针,小于返回-1,等于返回0,大于返回1
 */
void rbtree_insert(struct rbtree *T, void *key, int (*compare)(void *, void *));

/*
 * 红黑树的删除
 * @T 指向红黑树结构体的指针
 * @key 要插入的数据
 * @compare 插入数据的比较函数指针,小于返回-1,等于返回0,大于返回1
 */
void rbtree_delete(struct rbtree *T, void *key, int (*compare)(void *, void *));

/*
 * 红黑树的查找
 * @T 指向红黑树结构体的指针
 * @key 要插入的数据
 * @compare 插入数据的比较函数指针,小于返回-1,等于返回0,大于返回1
 * 找到返回0,未找到返回-1
 */
int rbtree_search(struct rbtree *T, void *key, int (*compare)(void *, void *));

/*
 * 红黑树的销毁
 * @T 指向红黑树结构体的指针
 * 返回红黑树中的最小值的指针
 */
void* rbtree_minimum(struct rbtree *T);

/*
 * 红黑树的销毁
 * @T 指向红黑树结构体的指针
 * 返回红黑树中的最大值的指针
 */
void* rbtree_maximum(struct rbtree *T);

#endif
