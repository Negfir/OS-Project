#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>

int s=0;
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
int inorder(struct node *root,int a)
{
    if (root != NULL)
    {
        if (a==root->key){
            printf("---I find %d\n",a);
            return 1;
        }
        else{
       // printf("I passed %d \n",root->key);

        inorder(root->left,a);

        inorder(root->right,a);}
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

// Driver Program to test above functions
int main()
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
s=678;


    struct timeval st, et;

    gettimeofday(&st,NULL);

    // print inoder traversal of the BST
//    int a=inorder(root,20);
//    if (a==1){
//        return 1;
//    }

    clock_t t;
    t = clock();

    //inorder(root,s);


    int pid,i,numOfProcess=2;
    for(i=0; i<numOfProcess; i++){
        pid = fork();    // <-------- You had a == not =
        if(pid == -1){   // <-------- You didn't have a open {, close }
            //error handling
        }
        else if(pid == 0){
            //child, do something
            int a;
            if (i%2==0){
                a=inorder(root->left,s);
                if(a==1){
                   // return 0;
                }
            }
            else{
                a=inorder(root->right,s);
                if(a==1){
                    //return 0;
                }
            }
            printf("Child %d\n",i );

            exit(0);
        }
        else{ //parent
            //i want to wait till all children are done before continue..how?
            //i tried wait(NULL), and waitpid(pid, NULL, 0)
            //but both wont work..?
            printf("started %d\n",i );
            //...........
            //something done in parent
        }
    }
    printf ("Waiting\n");
    // Need to wait for all
    for(i=0; i<numOfProcess; i++){
        wait(NULL);
        printf("done %d\n",i );
    }



    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("took %f seconds to execute \n", time_taken);

    long double a[4], b[4], loadavg;
    FILE *fp;
    char dump[50];

    struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
    printf("Memory usage: %ld bytes\n",r_usage.ru_maxrss);

    for(int i=0;i<10;i++)
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

    return 0;
}