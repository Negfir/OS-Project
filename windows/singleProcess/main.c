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


static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void init(){
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double getCurrentValue(){
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
              (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}

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


int main(int argc, char *argv[]) {

    struct node *root = NULL;
    root = insert(root, 5000000);


    char* m="output.txt";
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

    inorder(root);


    t = clock() - t;
    double time_taken = ((double) t) / CLOCKS_PER_SEC; // in seconds

    printf("Took %f seconds to execute \n", time_taken);

    init();
    double d = getCurrentValue();
    printf("CPU Utilization is: %f percent\n", &d);

    return 0;
}