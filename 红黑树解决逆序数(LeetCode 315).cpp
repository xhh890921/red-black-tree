
#include <stdio.h>

#define BLACK 0
#define RED 1
struct RBNode{
	int value; //保存基本数据 
	int color; //节点颜色，BLACK和RED，默认BLACK 
	RBNode *left; //指向左子树 
	RBNode *right; //指向右子树 
	RBNode *parent; //指向父节点 
	int size; //以该节点为根的树，包含了多少个子节点
	int cnt; //相同节点的个数
	//传入节点保存的数值，并初始化各个变量
	RBNode(int _value){
		value = _value;
		color = BLACK;
		left = NULL;
		right = NULL;
		parent = NULL;
		size = 1;
		cnt = 1;
	}
};



//更新节点node的size，使节点size保持正确的数值
void update_node_size(RBNode *node){
	int size = node->cnt; //设置size等于node-count
	if (node->left){ //如果节点有左子树
		size += node->left->size; //将size累加左子树的大小
	}
	if (node->right){ //如果节点有右子树
		size += node->right->size; //size累加右子树的大小
	}
	node->size = size; //更新node-size为size
}




//将传入的x节点进行左旋与当前树的根节点，在旋转的过程中，更新根节点 
void left_rotate(RBNode *x, RBNode *&root){
    RBNode *y = x->right; //使用指针y，保存旋转节点x的右孩子
    x->right = y->left; //将X的右指针指向Y的左子树 
    if (y->left){ //如果Y的左子树不空，即贝塔子树存在
    	y->left->parent = x; //将贝塔的父节点指针指向X 
    }
    y->parent = x->parent; //Y的父节点更新为X的父节点
    if (!x->parent){ //如果X恰好就是根节点
    	root = y; //将Y保存到root指针中
    }
    else{ //如果X不是根节点
    	if (x == x->parent->left){ //X是其父节点的左子树
    		x->parent->left = y; //更新其父节点的左指针
    	}
    	else{ //如果X是右子树
    		x->parent->right = y; //将它父节点的右指针指向Y
    	}
    }
    x->parent = y; //将X的父节点指针指向Y
    y->left = x; //Y的左指针指向X
    //更新x和y包含子孙节点的数量size
    //先更新旋转后得到的孩子节点x的属性，再更新的根节点y的属性
	update_node_size(x);
    update_node_size(y);
}


//将传入的x节点进行右旋与当前树的根节点，在旋转的过程中，更新根节点 
void right_rotate(RBNode *x, RBNode *&root){
	RBNode *y = x->left; //使用Y保存X的左孩子
    x->left = y->right; //更新X的左指针，指向Y的右孩子
    if (y->right){ //更新贝塔的父节点指针 
    	y->right->parent = x;
    }
    y->parent = x->parent; //Y的父节点更新为X的父节点
    if (!x->parent){ //如果X恰好就是根节点
    	root = y; //将Y保存到root指针中
    }
    else{ //如果X不是根节点
    	if (x == x->parent->left){ //X是其父节点的左子树
    		x->parent->left = y; //更新其父节点的左指针
    	}
    	else{ //如果X是右子树
    		x->parent->right = y; //将它父节点的右指针指向Y
    	}
    }
    x->parent = y; //将X的父节点指针指向Y
    y->right = x; //将Y的右孩子指向X
    //更新X和Y的子孙节点数量size
	update_node_size(x);
	update_node_size(y);
}



//获取叔叔节点，函数传入node，返回node的叔叔节点指针 
RBNode *uncle_node(RBNode *node){
	RBNode *parent = node->parent; //设置parent指向node的父节点 
	RBNode *grandparent = parent->parent; //grandparent指向node的祖父节点
	if (parent == grandparent->left){ //如果node的父节点是它祖父节点的左孩子
		return grandparent->right; //叔叔是右孩子
	}
	return grandparent->left; //叔叔是左孩子
}



//从节点node开始向上进行红黑树的修正 
void fix_double_red(RBNode *node, RBNode *&root){
	while (1){ //进入向上修正的while循环
		if(node == root) { //如果当前处理节点的指针node已经指向根节点
			node->color = BLACK; //当前节点设置为黑色
			return; //返回
		}
		RBNode *parent = node->parent; //指向当前节点父节点
		if(parent->color == BLACK){ //如果它为黑色
			return; //完成了双红调整，函数返回
		}
		RBNode *grandparent = node->parent->parent; //指向祖父节点
		RBNode *uncle = uncle_node(node); //指向叔叔节点
		
		if (uncle && uncle->color == RED){ //如果uncle不为空，且是红色的
			parent->color = BLACK; //将父节点与叔叔节点设置为黑色
			uncle->color = BLACK;
			grandparent->color = RED; //祖父节点设置为红色
			node = grandparent; //node指向祖父节点
		}
		else{ //如果uncle为空，或者uncle为黑色
			//当parent是左子树时，处理场景2
			if (parent == grandparent->left){
				if (node == parent->left){ //node是左孩子
					parent->color = BLACK; //染色和一次右旋
					grandparent->color = RED;
					right_rotate(grandparent, root);
				}
				else{ //node是右孩子
					node->color = BLACK; //需要染色和两次旋转
					grandparent->color = RED;
					left_rotate(parent, root);
					right_rotate(grandparent, root);
				}
			}
			else{ //场景3
				//node是右孩子需要染色和一次左旋
				if (node == parent->right){
					parent->color = BLACK;
					grandparent->color = RED;
					left_rotate(grandparent, root);
				}
				else{ //需要染色和两次旋转
					node->color = BLACK;
					grandparent->color = RED;
					right_rotate(parent, root);
					left_rotate(grandparent, root);
				}
			}
			return; //完成了场景2或场景3的调整后，红黑树就完成了本次调整
		}
	}
}


//传入红黑树的根节点root与待插入节点node，将node插入到红黑树中，完成相应的调整
void node_insert(RBNode *&root, RBNode *node){
	RBNode *ptr = root; //设置ptr指向根节点root
	while(ptr != node){ //ptr向下搜索node的插入位置
		ptr->size++; //将ptr指向的节点子孙个数加1
		//如果当前插入的节点值等于正在遍历的节点值
		if (node->value == ptr->value){
			ptr->cnt++; //将ptr指向的节点的数量count加1
			return;
		}
		//如果插入节点的值小于正在遍历的节点
		if (node->value < ptr->value){ 
			if (!ptr->left){ //在该节点没有左孩子时
				ptr->left = node; //使node成为ptr的左孩子
				node->parent = ptr; //node的父节点指向ptr 
			}
			ptr = ptr->left; //将ptr指向ptr-left
		}
		//如果插入节点的值大于正在遍历的节点
		else if (node->value > ptr->value){
			if (!ptr->right){ //该节点没有右孩子
				ptr->right = node; //node成为它的右孩子
				node->parent = ptr; //更新node的父节点
			}
			ptr = ptr->right; //ptr指向ptr-right
		}
	}
	node->color = RED; //将node的颜色修改为红色
	fix_double_red(node, root); //调用双红节点调整函数，进行红黑树的调整 
}


//函数传入树的根节点root与变量value，返回树中有多少个元素比value小
int get_small_count(RBNode *root, int value){
	RBNode *node = root; //设置变量node指向root
	int small = 0; //变量small保存比value小的节点个数
	while(node){ //从根节点向下循环遍历
		if (node->value == value){ //如果value和当前遍历的node值相等
			if (node->left){ //node如果有左子树
				small += node->left->size; //使用small累加左子树中的节点个数
			}
			break; //跳出循环
		}
		if (value < node->value){ //如果value小于node值
			node = node->left; //向左子树遍历
		}
		else{ //如果value大于node的值
			if (node->left){
				//将small累加node左子树的节点个数
				small += node->left->size;
			}
			small += node->cnt; //将small累加当前节点值的个数 
			node = node->right; //向右继续遍历 
		}
	}
	return small; //最后返回small
}



#include <algorithm>
#include <vector>
using namespace std;

class Solution {
public:
    vector<int> countSmaller(vector<int>& nums) {
    	vector<RBNode *> nodes; //保存红黑树节点指针的数组
    	vector<int> count(nums.size(), 0); //保存逆序数的数组
    	//从后向前遍历原数组
    	for (int i = nums.size() - 1; i >= 0; i--){
    		//构造红黑树的节点，并添加至nodes
    		nodes.push_back(new RBNode(nums[i]));
	    }
	    RBNode *root = nodes[0];
	    for (int i = 1; i < nodes.size(); i++){
	    	//将nodes[i]插入到以root为根的树堆中 
	    	node_insert(root, nodes[i]);	    	
	    	//计算树中已有节点比插入节点小的节点数量
	    	count[i] = get_small_count(root, nodes[i]->value);
    	}
    	//删除nodes中保存的红黑树节点
    	for (int i = 0; i < nodes.size(); i++){
	    	delete nodes[i];
	    }
	    //逆置count数组，得到原数组的逆序数
	    reverse(count.begin(),count.end());
        return count; //返回nums的逆序数
    }
};



int main(){
	int test[] = {5, 2, 5, 8, 1};
	vector<int> nums;
	for (int i = 0; i < 5; i++){
		nums.push_back(test[i]);
	}
	Solution solution;
	vector<int> count = solution.countSmaller(nums);
	for (int i = 0; i < count.size(); i++){
		printf("[%d]", count[i]);
	}
	printf("\n");
	return 0;
}
