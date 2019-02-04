#include<stdio.h>
#include<stdlib.h>
#include <windows.h>
#include <sys/time.h>
#include <tchar.h>
//#include <strsafe.h>

#define MAX_THREADS 3
#define BUF_SIZE 255
int s;
struct timeval t0;
struct timeval t1;
float elapsed;

struct node {
    int key;
    struct node *left, *right;
};


struct node *newNode(int item) {
    struct node *temp = (struct node *) malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

// A utility function to do inorder traversal of BST
int inorder(struct node *root) {
    if (root != NULL) {
        if (root->key == s) {
            printf("i find %d\n", s);
            return s;
        } else {
            inorder(root->left);
            inorder(root->right);
            // return NULL;
        }
    }
}

/* A utility function to insert a new node with given key in BST */
struct node *insert(struct node *node, int key) {
    /* If the tree is empty, return a new node */
    if (node == NULL) return newNode(key);

    /* Otherwise, recur down the tree */
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    /* return the (unchanged) node pointer */
    return node;
}


float timedifference_msec(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec) * 1000.0f + (t1.tv_usec) / 1000.0f;
}


int main(int argc, char **argv) {
    printf("jhjr %s %s\n",argv[1],argv[2]);

    struct node *root = NULL;
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 20);
    insert(root, 40);
    insert(root, 70);
    insert(root, 60);
    insert(root, 80);

    char* m=argv[1];
    FILE *f = fopen(m, "r");
    int number = 0;
    int sum = 0; /* the sum of numbers in the file */

    while (fscanf(f, "%d,", &number) > 0) // parse %d followed by ','
    {
        insert(root, number);
        // printf("%d\n", number); // instead of sum you could put your numbers in an array
    }

    fclose(f);


    s = 237;


    clock_t t;
    t = clock();
    printf("I am process %s ", argv[2]);
    inorder(root);


    t = clock() - t;
    double time_taken = ((double) t) / CLOCKS_PER_SEC; // in seconds

    printf("Took %f seconds to execute \n", time_taken);
    return 0;
}