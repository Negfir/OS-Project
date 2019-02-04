#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#define NUM_THREADS
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>

 int s=0;
int ack=0;
struct node
{
    int key;
    struct node *left, *right;
};

int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
    return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}
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
            ack=1;
            return s;
        }
        else{
            inorder(root->left);
            inorder(root->right);
            return NULL;
        }
    }
}
    int *binorder(struct node *root)
    {

            inorder(root);


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




int main (int argc, char *argv[])
{
    pthread_t threads[4];
    struct node *root = NULL;
    root = insert(root, 50000);

    FILE* f = fopen("output.txt", "r");
    int number = 0;
    int sum = 0; /* the sum of numbers in the file */

    while( fscanf(f, "%d,", &number) > 0 ) // parse %d followed by ','
    {
        insert(root, number);
        // printf("%d\n", number); // instead of sum you could put your numbers in an array
    }

    fclose(f);

    struct timeval st, et;

    gettimeofday(&st,NULL);
s=678;
//binorder(root);
    clock_t r;
    r = clock();
    //s=290;
    int t;
    for(t=0; t<4; t++){
        printf("In main: creating thread %ld\n", t);
        if (t==0){
        pthread_create(&threads[t], NULL, binorder, (void *)root->left->left);}
        else if(t==1){
            pthread_create(&threads[t], NULL, binorder, (void *)root->right->right);
        }
        else if(t==2){
            pthread_create(&threads[t], NULL, binorder, (void *)root->right->left);
        }
        else{
            pthread_create(&threads[t], NULL, binorder, (void *)root->left->right);
        }
        pthread_join(threads[t], NULL);
    }


    gettimeofday(&et,NULL);


    r = clock() - r;
    double time_taken = ((double)r)/CLOCKS_PER_SEC; // in seconds
    printf("took %f seconds to execute \n", time_taken);

    long double a[4], b[4], loadavg;
    FILE *fp;
    char dump[50];

    struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
    printf("Memory usage: %ld bytes\n",r_usage.ru_maxrss);

    for(int i=0;i<20;i++)
    {
        fp = fopen("/proc/stat","r");
        fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3]);
        fclose(fp);
        sleep(1);

        fp = fopen("/proc/stat","r");
        fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3]);
        fclose(fp);

        loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));
        printf("The current CPU utilization is : %Lf\n",loadavg);
    }

    pthread_exit(0);

    //return 0;
}

