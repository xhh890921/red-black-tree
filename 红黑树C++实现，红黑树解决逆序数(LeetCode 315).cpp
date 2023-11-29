
#include <stdio.h>

#define BLACK 0
#define RED 1
struct RBNode{
	int value; //����������� 
	int color; //�ڵ���ɫ��BLACK��RED��Ĭ��BLACK 
	RBNode *left; //ָ�������� 
	RBNode *right; //ָ�������� 
	RBNode *parent; //ָ�򸸽ڵ� 
	int size; //�Ըýڵ�Ϊ�������������˶��ٸ��ӽڵ�
	int cnt; //��ͬ�ڵ�ĸ���
	//����ڵ㱣�����ֵ������ʼ����������
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



//���½ڵ�node��size��ʹ�ڵ�size������ȷ����ֵ
void update_node_size(RBNode *node){
	int size = node->cnt; //����size����node-count
	if (node->left){ //����ڵ���������
		size += node->left->size; //��size�ۼ��������Ĵ�С
	}
	if (node->right){ //����ڵ���������
		size += node->right->size; //size�ۼ��������Ĵ�С
	}
	node->size = size; //����node-sizeΪsize
}




//�������x�ڵ���������뵱ǰ���ĸ��ڵ㣬����ת�Ĺ����У����¸��ڵ� 
void left_rotate(RBNode *x, RBNode *&root){
    RBNode *y = x->right; //ʹ��ָ��y��������ת�ڵ�x���Һ���
    x->right = y->left; //��X����ָ��ָ��Y�������� 
    if (y->left){ //���Y�����������գ���������������
    	y->left->parent = x; //�������ĸ��ڵ�ָ��ָ��X 
    }
    y->parent = x->parent; //Y�ĸ��ڵ����ΪX�ĸ��ڵ�
    if (!x->parent){ //���Xǡ�þ��Ǹ��ڵ�
    	root = y; //��Y���浽rootָ����
    }
    else{ //���X���Ǹ��ڵ�
    	if (x == x->parent->left){ //X���丸�ڵ��������
    		x->parent->left = y; //�����丸�ڵ����ָ��
    	}
    	else{ //���X��������
    		x->parent->right = y; //�������ڵ����ָ��ָ��Y
    	}
    }
    x->parent = y; //��X�ĸ��ڵ�ָ��ָ��Y
    y->left = x; //Y����ָ��ָ��X
    //����x��y��������ڵ������size
    //�ȸ�����ת��õ��ĺ��ӽڵ�x�����ԣ��ٸ��µĸ��ڵ�y������
	update_node_size(x);
    update_node_size(y);
}


//�������x�ڵ���������뵱ǰ���ĸ��ڵ㣬����ת�Ĺ����У����¸��ڵ� 
void right_rotate(RBNode *x, RBNode *&root){
	RBNode *y = x->left; //ʹ��Y����X������
    x->left = y->right; //����X����ָ�룬ָ��Y���Һ���
    if (y->right){ //���±����ĸ��ڵ�ָ�� 
    	y->right->parent = x;
    }
    y->parent = x->parent; //Y�ĸ��ڵ����ΪX�ĸ��ڵ�
    if (!x->parent){ //���Xǡ�þ��Ǹ��ڵ�
    	root = y; //��Y���浽rootָ����
    }
    else{ //���X���Ǹ��ڵ�
    	if (x == x->parent->left){ //X���丸�ڵ��������
    		x->parent->left = y; //�����丸�ڵ����ָ��
    	}
    	else{ //���X��������
    		x->parent->right = y; //�������ڵ����ָ��ָ��Y
    	}
    }
    x->parent = y; //��X�ĸ��ڵ�ָ��ָ��Y
    y->right = x; //��Y���Һ���ָ��X
    //����X��Y������ڵ�����size
	update_node_size(x);
	update_node_size(y);
}



//��ȡ����ڵ㣬��������node������node������ڵ�ָ�� 
RBNode *uncle_node(RBNode *node){
	RBNode *parent = node->parent; //����parentָ��node�ĸ��ڵ� 
	RBNode *grandparent = parent->parent; //grandparentָ��node���游�ڵ�
	if (parent == grandparent->left){ //���node�ĸ��ڵ������游�ڵ������
		return grandparent->right; //�������Һ���
	}
	return grandparent->left; //����������
}



//�ӽڵ�node��ʼ���Ͻ��к���������� 
void fix_double_red(RBNode *node, RBNode *&root){
	while (1){ //��������������whileѭ��
		if(node == root) { //�����ǰ����ڵ��ָ��node�Ѿ�ָ����ڵ�
			node->color = BLACK; //��ǰ�ڵ�����Ϊ��ɫ
			return; //����
		}
		RBNode *parent = node->parent; //ָ��ǰ�ڵ㸸�ڵ�
		if(parent->color == BLACK){ //�����Ϊ��ɫ
			return; //�����˫���������������
		}
		RBNode *grandparent = node->parent->parent; //ָ���游�ڵ�
		RBNode *uncle = uncle_node(node); //ָ������ڵ�
		
		if (uncle && uncle->color == RED){ //���uncle��Ϊ�գ����Ǻ�ɫ��
			parent->color = BLACK; //�����ڵ�������ڵ�����Ϊ��ɫ
			uncle->color = BLACK;
			grandparent->color = RED; //�游�ڵ�����Ϊ��ɫ
			node = grandparent; //nodeָ���游�ڵ�
		}
		else{ //���uncleΪ�գ�����uncleΪ��ɫ
			//��parent��������ʱ��������2
			if (parent == grandparent->left){
				if (node == parent->left){ //node������
					parent->color = BLACK; //Ⱦɫ��һ������
					grandparent->color = RED;
					right_rotate(grandparent, root);
				}
				else{ //node���Һ���
					node->color = BLACK; //��ҪȾɫ��������ת
					grandparent->color = RED;
					left_rotate(parent, root);
					right_rotate(grandparent, root);
				}
			}
			else{ //����3
				//node���Һ�����ҪȾɫ��һ������
				if (node == parent->right){
					parent->color = BLACK;
					grandparent->color = RED;
					left_rotate(grandparent, root);
				}
				else{ //��ҪȾɫ��������ת
					node->color = BLACK;
					grandparent->color = RED;
					right_rotate(parent, root);
					left_rotate(grandparent, root);
				}
			}
			return; //����˳���2�򳡾�3�ĵ����󣬺����������˱��ε���
		}
	}
}


//���������ĸ��ڵ�root�������ڵ�node����node���뵽������У������Ӧ�ĵ���
void node_insert(RBNode *&root, RBNode *node){
	RBNode *ptr = root; //����ptrָ����ڵ�root
	while(ptr != node){ //ptr��������node�Ĳ���λ��
		ptr->size++; //��ptrָ��Ľڵ����������1
		//�����ǰ����Ľڵ�ֵ�������ڱ����Ľڵ�ֵ
		if (node->value == ptr->value){
			ptr->cnt++; //��ptrָ��Ľڵ������count��1
			return;
		}
		//�������ڵ��ֵС�����ڱ����Ľڵ�
		if (node->value < ptr->value){ 
			if (!ptr->left){ //�ڸýڵ�û������ʱ
				ptr->left = node; //ʹnode��Ϊptr������
				node->parent = ptr; //node�ĸ��ڵ�ָ��ptr 
			}
			ptr = ptr->left; //��ptrָ��ptr-left
		}
		//�������ڵ��ֵ�������ڱ����Ľڵ�
		else if (node->value > ptr->value){
			if (!ptr->right){ //�ýڵ�û���Һ���
				ptr->right = node; //node��Ϊ�����Һ���
				node->parent = ptr; //����node�ĸ��ڵ�
			}
			ptr = ptr->right; //ptrָ��ptr-right
		}
	}
	node->color = RED; //��node����ɫ�޸�Ϊ��ɫ
	fix_double_red(node, root); //����˫��ڵ�������������к�����ĵ��� 
}


//�����������ĸ��ڵ�root�����value�����������ж��ٸ�Ԫ�ر�valueС
int get_small_count(RBNode *root, int value){
	RBNode *node = root; //���ñ���nodeָ��root
	int small = 0; //����small�����valueС�Ľڵ����
	while(node){ //�Ӹ��ڵ�����ѭ������
		if (node->value == value){ //���value�͵�ǰ������nodeֵ���
			if (node->left){ //node�����������
				small += node->left->size; //ʹ��small�ۼ��������еĽڵ����
			}
			break; //����ѭ��
		}
		if (value < node->value){ //���valueС��nodeֵ
			node = node->left; //������������
		}
		else{ //���value����node��ֵ
			if (node->left){
				//��small�ۼ�node�������Ľڵ����
				small += node->left->size;
			}
			small += node->cnt; //��small�ۼӵ�ǰ�ڵ�ֵ�ĸ��� 
			node = node->right; //���Ҽ������� 
		}
	}
	return small; //��󷵻�small
}



#include <algorithm>
#include <vector>
using namespace std;

class Solution {
public:
    vector<int> countSmaller(vector<int>& nums) {
    	vector<RBNode *> nodes; //���������ڵ�ָ�������
    	vector<int> count(nums.size(), 0); //����������������
    	//�Ӻ���ǰ����ԭ����
    	for (int i = nums.size() - 1; i >= 0; i--){
    		//���������Ľڵ㣬�������nodes
    		nodes.push_back(new RBNode(nums[i]));
	    }
	    RBNode *root = nodes[0];
	    for (int i = 1; i < nodes.size(); i++){
	    	//��nodes[i]���뵽��rootΪ���������� 
	    	node_insert(root, nodes[i]);	    	
	    	//�����������нڵ�Ȳ���ڵ�С�Ľڵ�����
	    	count[i] = get_small_count(root, nodes[i]->value);
    	}
    	//ɾ��nodes�б���ĺ�����ڵ�
    	for (int i = 0; i < nodes.size(); i++){
	    	delete nodes[i];
	    }
	    //����count���飬�õ�ԭ�����������
	    reverse(count.begin(),count.end());
        return count; //����nums��������
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
