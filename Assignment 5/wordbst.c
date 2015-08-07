// Clifford Rice
// COP3502C
// April 8, 2013
// Program 5, a word binary tree.

// import necessary libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// define a wordlength constant
#define MAXLEN 30

// mandated struct
typedef struct node_t{
    char* word;
    struct node_t *left, *right;
} node_t;

// function prototypes
void inorder(node_t *head);
node_t* find(node_t *curr_node, char* key);
node_t* insert(char* data, node_t *head);
int height(node_t *head);
void handleOp(FILE* ifp, node_t* tree);
void freeTree(node_t* curr);

void inorder(node_t *head){

    // No printing if the pointer is uninitialized
    if(head==NULL)
        return;

    // Inorder traversal prints left-most to right-most item. (This item is between its left and right subtree)
    inorder(head->left);
    printf("%s ", head->word);
    inorder(head->right);
}

node_t* find(node_t* curr_node, char* key){

    // If the node we reach is null, return null indicating it is not in the tree
    if(curr_node == NULL){
        return curr_node;
    }

    // Compare and store the vales so we don't need to calculate this multiple times.
    int cmpval = strcmp(curr_node->word, key);

    if(cmpval==0){
        // If the values match, return this node
        return curr_node;
    } else if(cmpval<0){
        // If this value is lexicographically greater than the current node, continue to the right branch
        return find(curr_node->right, key);
    } else {
        // Otherwise, the value is less and the search should continue to the left branch
        return find(curr_node->left, key);
    }
}

node_t* insert(char* data, node_t* head){

    // Make a helper node and a new node containing the value we are looking to add
    node_t* curr_node = head,
            *new_node = malloc(sizeof(node_t));
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->word = malloc(sizeof(char)*MAXLEN);
    strcpy(new_node->word, data);

    // If the tree is empty, just return this node
    if(head == NULL){
        return new_node;
    }

    // Loop until something is returned
    while(1){
        // Check to see if the new node is lexicographically greater than this node's word
        if(strcmp(data, curr_node->word) > 0){
            // If the right node is empty, set it equal to this one and return the head. Otherwise continue down the right branch
            if(curr_node->right==NULL){
                curr_node->right=new_node;
                return head;
            } else {
                curr_node = curr_node->right;
            }
        } else {
            // If the left node is empty and the value is lesser, set it equal to the new node and return the head
            // Otherwise continue down the left subtree
            if(curr_node->left==NULL){
                curr_node->left=new_node;
                return head;
            } else {
                curr_node = curr_node->left;
            }
        }
    }
}

int height(node_t *head){
    // This node is empty and its subtree depth is 0
    if(head == NULL)
        return 0;

    // Calculate the left and right depths
    int left_depth = height(head->left);
    int right_depth = height(head->right);

    // Return the greater of the two depths plus this node
    if(left_depth > right_depth){
        return left_depth + 1;
    } else {
        return right_depth + 1;
    }
}

void handleOp(FILE* ifp, node_t* tree){
    // Set up variables for the operation choice and the potentially needed word. Fill the former.
    int decision;
    char key[MAXLEN];
    fscanf(ifp,"%d",&decision);

    if (decision == 1){
        // Get the word, insert it to the tree, print that it was.
        fscanf(ifp,"%s",key);
        insert(key, tree);
        printf("Inserted %s.\n", key);
    } else if(decision == 2){
        // Get the word, check to see if it is in the tree. Print if it was or wasn't.
        fscanf(ifp,"%s",key);
        if(find(tree, key)!=NULL){
            printf("%s is present.\n", key);
        } else {
            printf("%s is not present.\n", key);
        }
    } else if(decision == 3){
        // Return the height of the whole tree
        printf("The height is %d.\n", height(tree)-1);
    } else if(decision == 4){
        // Get the word, try to find it, if you find it, print the height. Otherwise say it wasn't found.
        fscanf(ifp,"%s",key);
        node_t* found = find(tree, key);
        if(found!=NULL){
            printf("The height of the subtree at %s is %d.\n", key, height(found)-1);
        } else {
            printf("%s is not present.\n", key);
        }
    } else if(decision == 5){
        // Print out the tree on one line and then add a newline
        inorder(tree);
        printf("\n");
    }
}

void freeTree(node_t* curr){
    // Don't do anything if the current node is NULL
    if(curr == NULL);
    else{
        // Free the left, the right, the current word, and then the current node.
        freeTree(curr->right);
        freeTree(curr->left);
        free(curr->word);
        free(curr);
    }
}

int main(){
    // Set up the file pointer and the empty tree
    FILE *ifp = fopen("words.txt", "r");
    node_t* tree = NULL;

    // Declare the needed variables
    int numWords, numOps, i;

    // Store the number of words
    fscanf(ifp,"%d",&numWords);

    // Add all of the words from the file to the tree
    for(i = 0; i < numWords; i++){
        char word[MAXLEN];
        fscanf(ifp, "%s", word);
        tree = insert(word, tree);
    }

    // Get the number of operations to be performed
    fscanf(ifp,"%d",&numOps);

    // Handle all of the operations
    for(i=0; i<numOps; i++){
        handleOp(ifp, tree);
    }

    // Free the binary tree
    freeTree(tree);

    // Close the file
    close(ifp);

    // Process returns 0
    return 0;
}
