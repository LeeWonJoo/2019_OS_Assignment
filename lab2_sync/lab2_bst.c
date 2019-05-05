/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32170896, 32163395, 32162925
*	    Student name : Kim EunJin, Lee WonJoo, Yun SeulGi
*
*	lab2_bst.c :
*	    - thread-safe bst code.
*	    - coarse-grained, fine-grained lock code
*
*	Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"

/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree* tree)
{
    // You need to implement lab2_node_print_inorder function.
	int node_cnt = 0;
	node_cnt = lab2_node_inorder(tree->root);
	return node_cnt;
}

int lab2_node_inorder(lab2_node* node)
{
	static int node_count = 0;
	if(node) {
		node_count++;
		lab2_node_inorder(node->left);
		printf("%d ", node->key);
		lab2_node_inorder(node->right);
	}
	return node_count;
}


/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree* lab2_tree_create()
{
    // You need to implement lab2_tree_create function.
	lab2_tree* newTree = (lab2_tree*)malloc(sizeof(lab2_tree));
	int check = pthread_mutex_init(&(newTree->mutex), NULL);
	assert(check == 0); //checking success
	newTree->root = NULL;
	return newTree;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree* tree)
{
    // You need to implement lab2_tree_delete function.
	pthread_mutex_destroy(&(tree->mutex));
	free(tree);
	return;
}


/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node* lab2_node_create(int key)
{
	// You need to implement lab2_node_create function.
	lab2_node* newNode = (lab2_node*)malloc(sizeof(lab2_node));
	int check = pthread_mutex_init(&(newNode->mutex), NULL);
	assert(check == 0); //checking success
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->key = key;

	return newNode;
}


/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_node* node  : bst node which you want to remove. 
 *  @return                     : status (success or fail)
 */
int lab2_node_delete(lab2_node* node)
{
	// You need to implement lab2_node_delete function.
	pthread_mutex_destroy(&(node->mutex));
	free(node);
	return LAB2_SUCCESS;
}


/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert(lab2_tree* tree, lab2_node* new_node)
{
	// You need to implement lab2_node_insert function.
	lab2_node* nextNode = tree->root;
	lab2_node* currNode;
	int isLeft = 1;

	if(tree->root == NULL) {
		tree->root = new_node;
	}
	else {
		while(nextNode != NULL) {
			currNode = nextNode;
			if(currNode->key == new_node->key) {
				lab2_node_delete(new_node);
				return LAB2_ERROR;
			}
			else if(currNode->key < new_node->key) {
				nextNode = currNode->left;
				isLeft = 1;
			}
			else {
				nextNode = currNode->right;
				isLeft = 0;
			}
		}//breaks when nextNode == NULL ->now time to insert!!
		if(isLeft) {
			currNode->left = new_node;
		}
		else {
			currNode->right = new_node;
		}
	}
	return LAB2_SUCCESS;
}


/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree* tree, lab2_node* new_node)
{
	// You need to implement lab2_node_insert_fg function.
	lab2_node* nextNode = tree->root;
	lab2_node* currNode;
	int isLeft = 1;

	if(tree->root == NULL) {
		tree->root = new_node;
	}
	else {
		while(1) {
			currNode = nextNode;
			//make lock and check this node
			pthread_mutex_lock(&(currNode->mutex));
			if(currNode->key == new_node->key) {
				lab2_node_delete(new_node);
				pthread_mutex_unlock(&(currNode->mutex));
				return LAB2_ERROR;
			}
			else if(currNode->key < new_node->key) {
				nextNode = currNode->left;
				isLeft = 1;
			}
			else {
				nextNode = currNode->right;
				isLeft = 0;
			}
			if(nextNode != NULL)
				pthread_mutex_unlock(&(currNode->mutex));
			else
				break;
		}//breaks when nextNode == NULL
		//now time to insert!!
		//we still have lock for currNode
		if(isLeft) {
			currNode->left = new_node;
		}
		else {
			currNode->right = new_node;
		}
		//now unlocks currNode
		pthread_mutex_unlock(&(currNode->mutex));
	}
	return LAB2_SUCCESS;
}


/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree* tree, lab2_node* new_node)
{
	// You need to implement lab2_node_insert_cg function.
	lab2_node* nextNode = tree->root;
	lab2_node* currNode;
	int isLeft = 1;

	if(tree->root == NULL) {
		tree->root = new_node;
	}
	else {
		pthread_mutex_lock(&(tree->root->mutex));
		while(nextNode != NULL) {
			currNode = nextNode;
			if(currNode->key == new_node->key) {
				lab2_node_delete(new_node);
				pthread_mutex_unlock(&(tree->root->mutex));
				return LAB2_ERROR;
			}
			else if(currNode->key < new_node->key) {
				nextNode = currNode->left;
				isLeft = 1;
			}
			else {
				nextNode = currNode->right;
				isLeft = 0;
			}
		}//breaks when nextNode == NULL ->now time to insert!!
		if(isLeft) {
			currNode->left = new_node;
		}
		else {
			currNode->right = new_node;
		}
		pthread_mutex_unlock(&(tree->root->mutex));
	}
	return 0;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree* tree, int key)
{
	// You need to implement lab2_node_remove function.
	lab2_node* currNode = tree->root;
	lab2_node* pastNode = NULL;
	lab2_node* subNode = NULL; //for substitution
	lab2_node* p_subNode = NULL; //for substitution
​
	lab2_node* tmp = NULL;
​
	int isFound=0;
​
	if(tree->root == NULL) {
		return LAB2_ERROR;
	}
	else {
		while(currNode != NULL) {
			//make lock and check this node
			pthread_mutex_unlock(&(currNode->mutex));
			if(currNode->key == key) {
				isFound = 1;
				break;
			}
			pastNode = currNode;
			else {
				if(currNode->key < key) {
					currNode = currNode->left;
				}
				else {
					currNode = currNode->right;
				}
			}
			pthread_mutex_unlock(&(currNode->mutex));
		}//breaks when nextNode == NULL or currNode->kew == key
		//now time to delete!!
		
		if(!isFound) {
			//when nextNode == NULL
			//means, matching node not found
			return LAB2_ERROR;
		}
		else {
			//matching node is found!!
			//no need to check for any other node having same key value
			//when currNode has 2 child node,
			//we need to find for substitute node
			//--> smallest node from the right tree
			if((currNode->right != NULL) && (currNode->right != NULL)) {
				subNode = currNode->right;
				p_subNode = currNode;
				while(subNode->left != NULL) {
					p_subNode = subNode;
					subNode = subNode->left;
				}
				//now, change ONLY the key value!!
				currNode->key = subNode->key;
				//key value changed,
				//now the "subNode" will be the node we will delete(now no needed)
				currNode = subNode;
				pastNode = p_subNode;
			}
​
			//case when currNode have a child on left
			if(currNode->right == NULL) {
				tmp = currNode->left;
			}
			else { //incase if currNode has any child on right...
				tmp = currNode->right;
			}
​
			if(currNode == tree->root)
				tree->root = tmp;
			else{
				if(currNode == pastNode->left)
					pastNode->left = tmp;
				else
					pastNode->right = tmp;
			}
			//now deletes the node!!
			lab2_node_delete(currNode);
		}
	}
	return LAB2_SUCCESS;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree* tree, int key)
{
	// You need to implement lab2_node_remove_fg function.
	lab2_node* currNode = tree->root;
	lab2_node* pastNode = NULL;
	lab2_node* subNode = NULL; //for substitution
	lab2_node* p_subNode = NULL; //for substitution
​
	lab2_node* tmp = NULL;
​
	int isFound=0;
​
	if(tree->root == NULL) {
		return LAB2_ERROR;
	}
	else {
		while(currNode != NULL) {
			//make lock and check this node
			pthread_mutex_unlock(&(currNode->mutex));
			if(currNode->key == key) {
				isFound = 1;
				break;
			}
			pastNode = currNode;
			else {
				if(currNode->key < key) {
					currNode = currNode->left;
				}
				else {
					currNode = currNode->right;
				}
			}
			pthread_mutex_unlock(&(currNode->mutex));
		}//breaks when nextNode == NULL or currNode->kew == key
		//now time to delete!!
		
		if(!isFound) {
			//when nextNode == NULL
			//means, matching node not found
			return LAB2_ERROR;
		}
		else {
			//matching node is found!!
			//no need to check for any other node having same key value
			//when currNode has 2 child node,
			//we need to find for substitute node
			//--> smallest node from the right tree
			if((currNode->right != NULL) && (currNode->right != NULL)) {
				subNode = currNode->right;
				p_subNode = currNode;
				while(subNode->left != NULL) {
					p_subNode = subNode;
					subNode = subNode->left;
				}
				//now, change ONLY the key value!!
				currNode->key = subNode->key;
				//key value changed,
				//now the "subNode" will be the node we will delete(now no needed)
				currNode = subNode;
				pastNode = p_subNode;
			}
​
			//case when currNode have a child on left
			if(currNode->right == NULL) {
				tmp = currNode->left;
			}
			else { //incase if currNode has any child on right...
				tmp = currNode->right;
			}
​
			if(currNode == tree->root)
				tree->root = tmp;
			else{
				if(currNode == pastNode->left)
					pastNode->left = tmp;
				else
					pastNode->right = tmp;
			}
			//now deletes the node!!
			lab2_node_delete(currNode);
		}
	}
	return LAB2_SUCCESS;
}​


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree* tree, int key)
{
	// You need to implement lab2_node_remove_cg function.
	lab2_node* currNode = tree->root;
	lab2_node* pastNode = NULL;
	lab2_node* subNode = NULL; //for substitution
	lab2_node* p_subNode = NULL; //for substitution
​
	lab2_node* tmp = NULL;
​
	int isFound=0;
​
	if(tree->root == NULL) {
		return LAB2_ERROR;
	}
	else {
		while(currNode != NULL) {
			//make lock and check this node
			pthread_mutex_unlock(&(currNode->mutex));
			if(currNode->key == key) {
				isFound = 1;
				break;
			}
			pastNode = currNode;
			else {
				if(currNode->key < key) {
					currNode = currNode->left;
				}
				else {
					currNode = currNode->right;
				}
			}
			pthread_mutex_unlock(&(currNode->mutex));
		}//breaks when nextNode == NULL or currNode->kew == key
		//now time to delete!!
		
		if(!isFound) {
			//when nextNode == NULL
			//means, matching node not found
			return LAB2_ERROR;
		}
		else {
			//matching node is found!!
			//no need to check for any other node having same key value
			//when currNode has 2 child node,
			//we need to find for substitute node
			//--> smallest node from the right tree
			if((currNode->right != NULL) && (currNode->right != NULL)) {
				subNode = currNode->right;
				p_subNode = currNode;
				while(subNode->left != NULL) {
					p_subNode = subNode;
					subNode = subNode->left;
				}
				//now, change ONLY the key value!!
				currNode->key = subNode->key;
				//key value changed,
				//now the "subNode" will be the node we will delete(now no needed)
				currNode = subNode;
				pastNode = p_subNode;
			}
​
			//case when currNode have a child on left
			if(currNode->right == NULL) {
				tmp = currNode->left;
			}
			else { //incase if currNode has any child on right...
				tmp = currNode->right;
			}
​
			if(currNode == tree->root)
				tree->root = tmp;
			else{
				if(currNode == pastNode->left)
					pastNode->left = tmp;
				else
					pastNode->right = tmp;
			}
			//now deletes the node!!
			lab2_node_delete(currNode);
		}
	}
	return LAB2_SUCCESS;
}
