//This program contains functions for Binary Search Tree */

#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef RED_BLACK_TREE
//Constant for red-black tree
#define BLACK 0
#define RED 1
#endif

struct node
{
  int data;
  struct node *left;
  struct node *right;
#ifdef RED_BLACK_TREE
  int color;          //add this field for red-black tree implementation
#endif
};

struct node *lookup(struct node *bst, int target)
{
	if (bst == NULL) return NULL;
	if (bst->data == target)
		return bst;
	else if (bst->data > target)
		return lookup(bst->left,target);
	else 
		return lookup(bst->right,target);

}

int minValue(struct node *bst)
{
	if (bst->left == NULL) return bst->data;
	return minValue(bst->left);
}

int maxValue(struct node *bst)
{
	if (bst->right == NULL) return bst->data;
	return maxValue(bst->right);
}

int maxDepth(struct node *bst)
{
	int lDepth, rDepth;

	if (bst == NULL)
	{
		return 0;
	}

	lDepth = maxDepth(bst->left); 
	rDepth = maxDepth(bst->right);

	if (lDepth > rDepth)
	{
		return (lDepth + 1);
	}
	else
	{
		return (rDepth + 1);
	}
}

void printPathsRecur(node *node, int path[], int pathLength)
{
   if (node == NULL) return;
   
   //append this node to the path array
   path[pathLength] = node->data;
   pathLength++;
   
   if (node->left == NULL && node->right == NULL)   //We reach the leaf
   {  
       for (int i = 0; i < pathLength; i++)
       {
           printf("%d->",path[i]);
       }
	   printf("\n");
   }
   else
   {
      printPathsRecur(node->left, path, pathLength);
	  printPathsRecur(node->right, path, pathLength);
   }
}

void printPaths(node *node)
{
   int path[100];
   printPathsRecur(node, path, 0);
}

struct node *newNode(int new_data)
{
   node *new_Node = (node *)malloc(sizeof(node));
   new_Node->data = new_data;
   new_Node->left = NULL;
   new_Node->right = NULL;
   return new_Node;
}

#ifdef RED_BLACK_TREE
//These operations are for red-black tree
int isRed(struct node* bst)
{
   if (bst == NULL) return false;
   return (bst->color == RED);
}

struct node *rotateLeft(struct node* bst)
{
   struct node *x = bst->right;
   bst->right = x->left;
   x->left = bst;
   x->color = bst->color;
   bst->color = RED;
   return x;
}

struct node *rotatRight(struct node* bst)
{
   struct node *x = bst->left;
   bst->left = x->right;
   x->right = bst;
   x->color = bst->color;
   bst->color = RED;
   return x;
}

struct node *flipColor(struct node* bst)
{
   bst->color = RED;
   bst->left->color = BLACK;
   bst->right->color = BLACK;

   return bst;
}
#endif

struct node *insert(struct node *bst, int data)
{
   if (bst == NULL)
   {
     return(newNode(data));
   } 
   else
   {
     if (bst->data > data)
     {
	   bst->left = insert(bst->left, data);
     }
     else  //(bst->data <= data)
     {
	   bst->right = insert(bst->right, data);
     }

#ifdef RED_BLACK_TREE
	 //Add the following codes to rebalance red-black tree
	 if (!isRed(bst->left) && isRed(bst->right))    //right link is red but left link is black 
		bst = rotateLeft(bst);
	 if (isRed(bst->left) && isRed(bst->left->left))  //2 consecutive red links on the left
	    bst = rotateRight(bst);
     	 if (isRed(bst->left) && isRed(bst->right))      //split 4 nodes
	    bst = flipColors(bst);
#endif
     return (bst);  //return the new root pointer
   }
}

struct node *findParent(struct node* root, struct node* child)
{
	if (root == NULL || root == child) return NULL;

	if (root->left->data == child->data || root->right->data == child->data) 
		return root;
	if (root->data > child->data)
		return findParent(root->left,child);
	else if (root->data < child->data)
		return findParent(root->right,child);  
}

int findPredecessor(struct node* bst, int key)   //Worst case run time: O((logn)^2)
{
	struct node* key_node = lookup(bst,key);   //Search for the key first (O(logn))

	if (key_node == NULL) return -1;     //indicate that the key is not found

	if (key_node->left != NULL)  //ease case, just found the max of the left subtree
	{
		return maxValue(key_node->left);     
	}
	else  //go up the parent and stop first time when "turn left".  If it has never turned left after reaching the node, then key is min
	{
		struct node *child = key_node;
		struct node *parent = findParent(bst,child);
		printf("parent->data is %d, child->data is %d\n",parent->data,child->data);

		while ((parent->data > child->data) && (parent != bst))    //first time to "turn left" happens when (parent->data < child->data)
		{
			child = parent;
			parent = findParent(bst,child);                    
		   // printf("parent->data is %d, child->data is %d\n",parent->data,child->data);
		}

		if (parent == bst)  //make one more comparison with the root
		{
			if (parent->data < key)
				return parent->data;
			else
				return -2;          //indicate the key is min
		}
		else
		{
			return parent->data;
		}
	}
}

struct node *deleteNode(struct node *bst, int key)   //Worst case run time: O(logn)
{
    //Search for the key node
	struct node* key_node = lookup(bst,key);   //O(logn)

	if (key_node == NULL) return bst;      //return the original bst since the key is not found

	struct node *parent;
	if (key_node->left == NULL && key_node->right == NULL)   //no child, just delete the key
	{
		parent = findParent(bst,key_node);    //O(logn)
		if (parent->data < key_node->data)
		{
			parent->right = NULL;
		}
		else
		{
			parent->left = NULL;
		}
		free(key_node);
	}
	else if ((key_node->left == NULL && key_node->right != NULL) ||
		     (key_node->left != NULL && key_node->right == NULL))   //one child (could be left or right child), needs to move the child up to the parent
	{
		parent = findParent(bst,key_node);     //O(logn)
		if (parent->data < key_node->data)     //key_node is the right child of the parent
		{
			if (key_node->left != NULL)
			    parent->right = key_node->left;
			else
				parent->right = key_node->right;
		}
		else     //key_node is the left child of the parent
		{
			if (key_node->left != NULL)
				parent->left = key_node->left;
			else
				parent->left = key_node->right;
		}
		free(key_node);
	}
	else  //2 children
	{
		//find its predecessor.  Since there are 2 children in this case, just find the maxValue of the left subtree
		//also find the parent of its predecessor for the preparation of delete the predecessor 
		struct node *pred_node, *parent_pred_node;
		if (key_node->left->right == NULL)  //predecessor is the left child of the key_node
		{
			pred_node = key_node->left;
			parent_pred_node = key_node;
		}
		else      //predecessor is the max value of the left subtree of the key_node
		{
          	  parent_pred_node = key_node->left;
		  while (parent_pred_node->right->right != NULL)   //find the parent_pred_node first.  Worst case run time for balanced tree: O(log n)
			parent_pred_node = parent_pred_node->right;

		  pred_node = parent_pred_node->right;
		}
			
		printf("Before swap, pred_node->data = %d, key_node->data = %d\n",pred_node->data,key_node->data);
		//swap the pred node data with the key node data
		int temp = pred_node->data;
		pred_node->data = key_node->data;
		key_node->data = temp;
		printf("After swap, pred_node->data = %d, key_node->data = %d\n",pred_node->data,key_node->data);
		
		//delete the pred_node
		if (pred_node->left == NULL)   //no left child, just delete the key.  Note in this case pred_node->right is always NULL
	    	{
			if (parent_pred_node == key_node)
				parent_pred_node->left = NULL;
			else
				parent_pred_node->right = NULL;
	    	}
		else   //if the prev_node has 1 left child, need to move up the child to its parent 
	    	{
			if (parent_pred_node == key_node)
				parent_pred_node->left = pred_node->left;
			else
				parent_pred_node->right = pred_node->left;
		}

		free(pred_node);
	}
	return bst;
}

void printTree(struct node *node)
{
  if (node == NULL)
     return; 
  printTree(node->left);
  printf("%d ",node->data);
  printTree(node->right);
}

void printPreOrder(struct node *node)
{
	if (node == NULL)
		return;
	printf("%d ",node->data);
	printPreOrder(node->left);
	printPreOrder(node->right);
}

void printPostOrder(struct node *node)
{
	if (node == NULL)
		return;
	printPostOrder(node->left);
	printPostOrder(node->right);
	printf("%d ",node->data);
}

node* mirror(node *orig_node)
{
	if (orig_node == NULL) return NULL;

	node *temp;

	orig_node->left = mirror(orig_node->left);
	orig_node->right = mirror(orig_node->right);

    temp = orig_node->left;
	orig_node->left = orig_node->right; 
	orig_node->right = temp;

	return(orig_node);
}

void doubleTree(struct node* bst)
{
	if (bst == NULL) return;
	doubleTree(bst->left);
	doubleTree(bst->right);

	struct node *old_left;

	old_left = bst->left;
    bst->left = newNode(bst->data);
    bst->left->left = old_left;
}

int sameTree(struct node* a, struct node* b)
{
   if (a == NULL && b == NULL)
   {
     return true;
   }
   else if (a != NULL && b != NULL)  //both non-empty, compare
   {  
	  return (a->data == b->data &&
		      sameTree(a->left,b->left) &&
			  sameTree(a->right,b->right));
   }
   else
   {
	   return false;
   }
}

//Find the min. depth of a binary tree
int minDepth(struct node* root) 
{
  int left=INT_MAX, right=INT_MAX;
  if (root == NULL) return 0;

  if (root->left == NULL && root->right == NULL)
	  return 1;

  if (root->left != NULL)
	  left = minDepth(root->left) + 1;

  if (root->right != NULL)
	  right = minDepth(root->right) + 1;

  if (left < right)
	  return left;
  else
	  return right;
}

int isBST(struct node* bst)
{
	if (bst == NULL) return true;

	if (bst->left != NULL && minValue(bst->left) > bst->data)
		return false;
	else if (bst->right != NULL && maxValue(bst->right) <= bst->data)
		return false;
	else if (!(isBST(bst->left)) || !(isBST(bst->right)))
		return false;
	else
		return true;
}

struct node* lowestCommonAncestorBST(struct node* root, struct node* p, struct node* q) 
{ 
   struct node* curr = root;
   
   if (curr->data == p->data && curr->data == q->data)   //It is their parent 
      return curr;
   
   while (curr->data != p->data && curr->data != q->data)  //if one of p->val or q->val equal to curr->val, the other one must be its child
   {
       if ((curr->data > p->data && curr->data < q->data) ||   //one must be smaller than the common ancestor and the other one must be bigger than the common ancestor
           (curr->data < p->data && curr->data > q->data))
         break;
       else if (curr->data < p->data && curr->data < q->data)
         curr = curr->right;
       else
         curr = curr->left;
   }
   
   return curr;
}

struct node* buildTree()
{
   struct node *root = NULL;
   root = insert(root,3);
   root = insert(root,1);
   root = insert(root,5);
   root = insert(root,4);
   root = insert(root,0);
   root = insert(root,2);
   return root;
}

void TestBST()
{
	node* bst=buildTree();
	printPaths(bst); printf("\n");
	struct node *node1 = newNode(2);
	struct node *node5 = newNode(5);
	struct node *node_LCA = lowestCommonAncestorBST(bst,node1,node5);
	printf("The LCA of node %d and node %d is node %d\n",node1->data,node5->data,node_LCA->data);

	int min_depth = minDepth(bst);
	printf("The min. depth of the tree is %d",min_depth);
	
	int key = 5;
	int pred = findPredecessor(bst,key);
	printf("The predecessor of key %d is %d",key,pred);*/
	printPostOrder(bst);
	printPaths(bst); printf("\n");
	int result = isBST(bst);
	printf("Result is %d\n",result);
	
	int min_val = minValue(bst);
	int max_val = maxValue(bst);
	int depth = maxDepth(bst);
	printf("min_val = %d, max_val = %d, depth = %d\n",min_val,max_val,depth);
	
	mirror(bst);
	doubleTree(bst);
	printPaths(bst); printf("\n");
}
