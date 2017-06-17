#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

typedef struct Node* NodePtr;
struct Node {
	int val;
	char color;
	NodePtr prnt;
	NodePtr left, right;
};

typedef struct RBT* RBTPtr;
struct RBT {
 	NodePtr root;
	NodePtr nil;
};

NodePtr node_alloc(RBTPtr tree, int newval) {
	NodePtr self = (NodePtr)malloc(sizeof(struct Node));
	self->val = newval;
	self->color = 'R';
	self->prnt = tree->nil;
	self->left = tree->nil;
	self->right = tree->nil;
	return self;
}

int total = 0;			//The total number of Nodes
int nb = 0;			//The number of Black Nodes
int insert = 0;
int delete = 0;
int miss = 0;

RBTPtr rbt_alloc() {
	RBTPtr tree = (RBTPtr)malloc(sizeof(struct RBT));
	NodePtr temp;
	temp = tree->nil = (NodePtr)malloc(sizeof(struct Node));
	temp->prnt = temp->left = temp->right = temp;
	temp->color = 'B';
	tree->root = tree->nil;
	return tree;
}

NodePtr tree_Search(RBTPtr self, NodePtr tree, int n){
	if((tree == self->nil) || (n == tree->val))
	{
		if(tree == self->nil)
		{
			//printf("%d is not found!\n", n);
			return tree;
		}
		return tree;
	}
	
	if(n < tree->val)
		return tree_Search(self, tree->left,n);
	else
		return tree_Search(self, tree->right,n); 
}

void Left_Rotate(RBTPtr tree, NodePtr n)
{
	NodePtr y;

	y = n->right;
	n->right = y->left;

	if(y->left != tree->nil)
		y->left->prnt = n;

	y->prnt = n->prnt;

	if(n->prnt == tree->nil)
		tree->root = y;
	else if(n == n->prnt->left)
		n->prnt->left = y;
	else
		n->prnt->right = y;

	y->left = n;
	n->prnt = y;
}

void Right_Rotate(RBTPtr self, NodePtr n)
{
	NodePtr x;

	x = n->left;
	n->left = x->right;

	if(x->right != self->nil)
		x->right->prnt = n;

	x->prnt = n->prnt;
	
	if(n->prnt == self->nil)
		self->root = x;
	else if(n == n->prnt->right)
		n->prnt->right = x;
	else
		n->prnt->left = x;

	x->right = n;
	n->prnt = x;
}

void RB_insert_fixup(RBTPtr self, NodePtr n)
{
	NodePtr y;
	while(n->prnt->color == 'R')
	{
		if(n->prnt == n->prnt->prnt->left)
		{
			y = n->prnt->prnt->right;
			if(y->color == 'R')
			{
				n->prnt->color = 'B';
				y->color = 'B';
				n->prnt->prnt->color = 'R';
				n = n->prnt->prnt;
			}
			else 
			{
				if(n == n->prnt->right)
				{
					n = n->prnt;
					Left_Rotate(self, n);
				}
				n->prnt->color = 'B';
				n->prnt->prnt->color = 'R';
				Right_Rotate(self, n->prnt->prnt);
			}
		}
		else
		{
			y = n->prnt->prnt->left;
			if(y->color == 'R')
			{
				n->prnt->color = 'B';
				y->color = 'B';
				n->prnt->prnt->color = 'R';
				n = n->prnt->prnt;
			}
			else 
			{
				if(n == n->prnt->left)
				{
					n = n->prnt;
					Right_Rotate(self, n);
				}
				n->prnt->color = 'B';
				n->prnt->prnt->color = 'R';
				Left_Rotate(self, n->prnt->prnt);
			}			
		}
	}
	self->root->color = 'B';
}

void RB_insert(RBTPtr self, NodePtr n) {
	NodePtr y = self->nil;
	NodePtr x = self->root;

	while(x != self->nil)
	{
		y = x;
		if(n->val < x->val)
			x = x->left;
		else
			x = x->right;
	}

	n->prnt = y;
	if(y == self->nil)
		self->root = n;
	else if(n->val < y->val)
		y->left = n;
	else
		y->right = n;

	n->left = self->nil;
	n->right = self->nil;
	n->color = 'R';
	RB_insert_fixup(self, n);
	insert++;
} 

NodePtr tree_Min(RBTPtr self, NodePtr n){
	while(n->left != self->nil)
		n = n->left;
	return n;
}

NodePtr tree_Max(RBTPtr self, NodePtr n)
{
	while(n->right != self->nil)
		n = n->right;
	return n;
}

void RB_transplant(RBTPtr self, NodePtr tree, NodePtr n){
	if(tree->prnt == self->nil)
		self->root = n;
	else if(tree == tree->prnt->left)
		tree->prnt->left = n;
	else
		tree->prnt->right = n;
	
	n->prnt = tree->prnt;
}

void RB_delete_fixup(RBTPtr self, NodePtr x)
{
	NodePtr w;
	while((x != self->root) && (x->color == 'B'))
	{
		if(x == x->prnt->left)
		{
			w = x->prnt->right;
			if(w->color == 'R')
			{
				w->color = 'B';
				x->prnt->color = 'R';
				Left_Rotate(self,x->prnt);
				w = x->prnt->right;
			}
			if((w->left->color == 'B') && (w->right->color == 'B'))
			{
				w->color = 'R';
				x = x->prnt;
			}
			else
			{
				if(w->right->color == 'B')
				{
					w->left->color = 'B';
					w->color = 'R';
					Right_Rotate(self,w);
					w = x->prnt->right;
				}
				w->color = x->prnt->color;
				x->prnt->color = 'B';
				w->right->color = 'B';
				Left_Rotate(self,x->prnt);
				x = self->root;
			}
		}
		else
		{
			w = x->prnt->left;
			if(w->color == 'R')
			{
				w->color = 'B';
				x->prnt->color = 'R';
				Right_Rotate(self,x->prnt);
				w = x->prnt->left;
			}
			if((w->left->color == 'B') && (w->right->color == 'B'))
			{
				w->color = 'R';
				x = x->prnt;
			}
			else
			{
				if(w->left->color == 'B')
				{
					w->right->color = 'B';
					w->color = 'R';
					Left_Rotate(self,w);
					w = x->prnt->left;
				}
				w->color = x->prnt->color;
				x->prnt->color = 'B';
				w->left->color = 'B';
				Right_Rotate(self,x->prnt);
				x = self->root;
			}
		}
	}
	x->color = 'B';
}

void RB_delete(RBTPtr self, NodePtr n){
	NodePtr x, y, w, wp;
	char origincolor;

	if(n == self->nil)
	{
		miss++;
		return;
	}
	y = n;
	origincolor = y->color;

	if(n->left == self->nil)
	{
		x = n->right;
		RB_transplant(self,n,n->right);
	}
	else if(n->right == self->nil)
	{
		x = n->left;
		RB_transplant(self,n,n->left);
	}
	else
	{
		y = tree_Min(self, n->right);
		origincolor = y->color;
		w = y->prnt->right;
		x = y->right;
		if(y->prnt == n)
			x->prnt = y;
		else
		{
			RB_transplant(self,y,y->right);
			y->right = n->right;
			y->right->prnt = y;
		}
		RB_transplant(self,n,y);
		y->left = n->left;
		y->left->prnt = y;
		y->color = n->color;
	}
	if(origincolor == 'B')
		RB_delete_fixup(self,x);
	delete++;

}

void rbt_inorder(RBTPtr self, NodePtr tree) {
	if (tree == self->nil)
		return;
	else {
		rbt_inorder(self, tree->left);
		printf("%d %c\n", tree->val, tree->color);
		rbt_inorder(self, tree->right);
	}
}

void rbt_total(RBTPtr self, NodePtr tree) {
	
	if (tree == self->nil)
		return;
	else {
		rbt_total(self, tree->left);
		total++;
		rbt_total(self, tree->right);
	}

}

void rbt_count_nb(RBTPtr self, NodePtr tree) {

	if (tree == self->nil)
		return;
	else {
		rbt_count_nb(self, tree->left);
		if(tree->color == 'B')
			nb++;
		rbt_count_nb(self, tree->right);
	}
}

int rbt_count_bh(RBTPtr self){
	NodePtr x = self->root;
	int bh = 0;

	if(x == self->nil)
		return 0;

	while((x->left != self->nil) || (x->right != self->nil))
	{
		if(x->color == 'B')
			bh++;
	
		if(x->left != self->nil)
			x = x->left;
		else
			x = x->right;
	}

	if(x->color == 'B')
		bh++;

	return bh;
}

void rbt_print(RBTPtr self, NodePtr tree, int level) {
	if (tree->right != self->nil)
		rbt_print(self, tree->right, level + 1);
	for(int i = 0; i < level; i++)
		printf("    ");
	printf("%d%c\n", tree->val, tree->color);
	if (tree->left != self->nil)
		rbt_print(self, tree->left, level + 1);
}

NodePtr sucessor(RBTPtr self, NodePtr n)
{
	NodePtr y;

	if(n->right != self->nil)
		return tree_Min(self,n->right);
	y = n->prnt;
	while((y != self->nil) && (n == y->right))
	{
		n = y;
		y = y->prnt;
	}
	return y;
}

NodePtr predecessor(RBTPtr self, NodePtr n)
{
	NodePtr y;

	if(n->left != self->nil)
		return tree_Max(self,n->left);
	y = n->prnt;
	while((y != self->nil) && (n == y->left))
	{
		n = y;
		y = y->prnt;
	}
	return y;
}

void RB_Destroy(RBTPtr self, NodePtr tree)
{
	if(tree == self->nil)
		return;
	RB_Destroy(self, tree->left);
	RB_Destroy(self, tree->right); 
	RB_delete(self, tree);
}

NodePtr tree_find(RBTPtr self, NodePtr tree, int n){
	if(n == tree->val)
		return tree;
	
	if(n < tree->val)
	{
		if(tree->left == self->nil)
			return tree;
		else
			return tree_find(self, tree->left,n);
	}
	else
	{
		if(tree->right == self->nil)
			return tree;
		else
			return tree_find(self, tree->right,n);
	}
}

void main() {

	RBTPtr rbt = rbt_alloc();
	char filename[50];
	char file[100];

	DIR *dp;
	struct dirent *ep;
	dp = opendir("./rbtest");
	FILE *RBT, *out;

	char pre[512], suc[512], mid[512];

	NodePtr fndNode, preNode, sucNode;

	if(dp == NULL)
	{
		printf("Couldn't open the directory\n");
		exit(0);
	}
		
		
	int num;
	int bh = 0;

	while(ep = readdir(dp))
	{
		strcpy(filename,ep->d_name);
		strcpy(file, "./rbtest/");
		strcat(file, filename);
		
		if(!strcmp(filename,"input.txt"))
		{
			RBT = fopen(file, "rt");
		
			if(RBT != NULL)
			{
				while(fscanf(RBT, "%d", &num) != EOF)
				{
					if(num == 0)
					{
						rbt_print(rbt,rbt->root,0);
						break;
					}	
					else if(num > 0)
						RB_insert(rbt, node_alloc(rbt, num));
					else
					{
						num = num * (-1);
						RB_delete(rbt, tree_Search(rbt, rbt->root,num));
					}
				}
			}
			fclose(RBT);
		}

		else if(!strcmp(filename,"search.txt"))
		{
			RBT = fopen(file, "rt");
			out = fopen("./rbtest/output.txt", "wt");
			if((RBT != NULL) && (out != NULL))
			{
				while(fscanf(RBT, "%d", &num) != EOF)
				{
					if(num == 0)
						break;	
					else
					{
						fndNode = tree_find(rbt,rbt->root,num);
						if(fndNode->val == num)
						{
							sprintf(mid, "%d", num);
							preNode = predecessor(rbt,fndNode);
							sucNode = sucessor(rbt,fndNode);
						}
						else
						{
							strcpy(mid,"NIL");
							if(fndNode->val > num)
							{
								preNode = predecessor(rbt,fndNode);
								sucNode = fndNode;
							}
							else
							{
								preNode = fndNode;
								sucNode = sucessor(rbt,fndNode);
							}
						}

						if(preNode == rbt->nil)
							strcpy(pre,"NIL");
						else
							sprintf(pre, "%d", preNode->val);

						if(sucNode == rbt->nil)
							strcpy(suc,"NIL");
						else
							sprintf(suc, "%d", sucNode->val);
						printf("%s %s %s\n", pre, mid, suc);
						fprintf(out,"%s %s %s\n", pre, mid, suc);
					}	
				}
			}
	
			fclose(RBT);
			fclose(out);
		}
	}
	RB_Destroy(rbt, rbt->root);
	(void)closedir(dp);
}
