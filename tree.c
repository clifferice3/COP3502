#include <stdlib.h>
#include <stdio.h>

typedef struct node_t {
   int data;
   struct node_t *left, *right;
} node_t;


void traverse_tree(node_t *head)
{
  if(head==NULL)
    return;
  traverse_tree(head->left);
  printf("%d ", head->data);
  traverse_tree(head->right);
}

int find_min(node_t *head)
{
   node_t *curr_node = head;
   int min_val;
   while(curr_node != NULL)
   {
      min_val = curr_node->data;
      curr_node = curr_node->left;
   }
   return min_val;
}

node_t *delete(int data, node_t *head)
{
   node_t *parent_node = NULL,
      	*curr_node = head;

   while(curr_node != NULL)
   {
      if(curr_node->data != data)
      {
	 		parent_node = curr_node;
	 		if(data > curr_node->data)
	  		  	curr_node = curr_node->right;
		 	else
	 		   curr_node = curr_node->left;
	    
      }
      else
      {
			 node_t **link_to_set;
	 		 node_t *new_val;

	 		node_t dummy_val;
	 		dummy_val.left=NULL;
	 		dummy_val.right = NULL;
	 		if(parent_node==NULL)
	   		 parent_node=&dummy_val;

	 		if(parent_node->right == curr_node)
	    		link_to_set = &parent_node->right;
	 		else
	    		link_to_set = &parent_node->left;

	 if(curr_node->right == NULL && curr_node->left == NULL) // leaf node
	 {
	    new_val = NULL;
	    free(curr_node);
	 }
	 else if(curr_node->right == NULL) // Only one branch
	 {
	    new_val = curr_node->left;
	    free(curr_node);
	 }
	 else if(curr_node->left == NULL) //Only one branch
	 {
	    new_val = curr_node->right;
	    free(curr_node);
	 }
	 else //Two Branches
	 {
	    int min_val = find_min(curr_node->right);
	    printf("min_val:%d\n",min_val);
	    new_val = curr_node;
	    curr_node->data = min_val;
	    curr_node->right=delete(min_val, curr_node->right);
	 }
	 *link_to_set = new_val;
	 if(parent_node==&dummy_val)
	    head = dummy_val.left;
	 return head;
      }
   }
   return head;
}

node_t *insert(int data, node_t *head)
{
   node_t *curr_node = head,
      *new_node = malloc(sizeof(node_t));
   new_node->right = NULL;
   new_node->left = NULL;
   new_node->data = data;
   if(head==NULL)
   {
      return new_node;
   }
   while(1)
   {
	 
      if(data > curr_node->data)
      {
	 if(curr_node->right==NULL)
	 {
	    curr_node->right=new_node;
	    return head;
	 }
	 else 
	 {
	    curr_node = curr_node->right;
	 }
      }
      else
      {
	 if(curr_node->left==NULL)
	 {
	    curr_node->left=new_node;
	    return head;
	 }
	 else 
	 {
	    curr_node = curr_node->left;
	 }

      }
   }
}

int count_max_levels(node_t *head)
{
   if(head == NULL)
      return 0;
   int left_depth = count_max_levels(head->left);
   int right_depth = count_max_levels(head->right);
   
   if(left_depth > right_depth)
   {
      return left_depth + 1;
   }
   else
   {
      return right_depth+1;
   }
}

int main()
{
   node_t *head = NULL;
   head=insert(5, head);
   head=insert(9, head);
   head=insert(6, head);
   head=insert(7, head);
   head=insert(16, head);
   
   traverse_tree(head);
   printf("\n");

   head = delete(12, head);
   traverse_tree(head);
   printf("\n");

   printf("Max Depth:%d\n", count_max_levels(head));
}
