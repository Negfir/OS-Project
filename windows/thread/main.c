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

struct node
{
    int key;
    struct node *left, *right;
};



// A utility function to create a new BST node
struct node *newNode(int item)
{
    struct node *temp =  (struct node *)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

// A utility function to do inorder traversal of BST
int inorder(struct node *root)
{
    if (root != NULL)
    {
        if(root->key==s){
            printf("i find %d\n",s);
            return s;
        }
        else{
        inorder(root->left);
        inorder(root->right);
        return NULL;
        }
    }
}

/* A utility function to insert a new node with given key in BST */
struct node* insert(struct node* node, int key)
{
    /* If the tree is empty, return a new node */
    if (node == NULL) return newNode(key);

    /* Otherwise, recur down the tree */
    if (key < node->key)
        node->left  = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    /* return the (unchanged) node pointer */
    return node;
}









DWORD Sum; /* data is shared by the thread(s) */
/* the thread runs in this separate function */
DWORD WINAPI Summation(LPVOID Param)
{
    DWORD Upper = *(DWORD*)Param;
    for (DWORD i = 0; i <= Upper; i++)
        Sum += i;
    return 0;
}


float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec ) * 1000.0f + (t1.tv_usec ) / 1000.0f;
}


int main(int argc, char *argv[])
{

    struct node *root = NULL;
    root = insert(root, 500000);



    FILE* f = fopen("output.txt", "r");
    int number = 0;
    int sum = 0; /* the sum of numbers in the file */

    while( fscanf(f, "%d,", &number) > 0 ) // parse %d followed by ','
    {
        insert(root, number);
       // printf("%d\n", number); // instead of sum you could put your numbers in an array
    }

    fclose(f);


    DWORD ThreadId;
    HANDLE ThreadHandle1;
    HANDLE ThreadHandle2;
    HANDLE  hThreadArray[4];
    s=237;
//    if (argc != 2) {
//        fprintf(stderr,"An integer parameter is required\n");
//        return -1;
//    }
//    Param = atoi(argv[1]);
//    if (Param < 0) {
//        fprintf(stderr,"An integer >= 0 is required\n");
//        return -1;
//    }
/* create the thread */




    clock_t t;
    t = clock();

    //inorder(root);

    hThreadArray[0] = CreateThread(
            NULL, /* default security attributes */
            0, /* default stack size */
            inorder, /* thread function */
            root->right->right, /* parameter to thread function */
            0, /* default creation flags */
            &ThreadId); /* returns the thread identifier */
    hThreadArray[1] = CreateThread(
            NULL, /* default security attributes */
            0, /* default stack size */
            inorder, /* thread function */
            root->left->left, /* parameter to thread function */
            0, /* default creation flags */
            &ThreadId); /* returns the thread identifier */
    hThreadArray[1] = CreateThread(
            NULL, /* default security attributes */
            0, /* default stack size */
            inorder, /* thread function */
            root->left->right, /* parameter to thread function */
            0, /* default creation flags */
            &ThreadId); /* returns the thread identifier */
    hThreadArray[1] = CreateThread(
            NULL, /* default security attributes */
            0, /* default stack size */
            inorder, /* thread function */
            root->right->left, /* parameter to thread function */
            0, /* default creation flags */
            &ThreadId); /* returns the thread identifier */




    WaitForMultipleObjects(4, hThreadArray, TRUE, INFINITE);

    // Close all thread handles and free memory allocations.

    for(int i=0; i<4; i++)
    {
        CloseHandle(hThreadArray[i]);

    }



    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    printf("took %f seconds to execute \n", time_taken);
    return 0;
}