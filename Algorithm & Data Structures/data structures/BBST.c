#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>

struct TNode
{
    int data;
    struct TNode* left;
    struct TNode* right;
};

struct TNode* newNode(int data);

struct TNode* sortedArrayToBST(int *arr, int start, int end)
{
    if (start > end)
        return NULL;

    int mid = (start + end)/2;
    struct TNode *root = newNode(arr[mid]);

    root->left =  sortedArrayToBST(arr, start, mid-1);

    root->right = sortedArrayToBST(arr, mid+1, end);

    return root;
}

struct TNode* newNode(int data)
{
    struct TNode* node = (struct TNode*)
            malloc(sizeof(struct TNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void split(char *string, char *a, char *b, char *c, char *oddzielnik){
    char *p = strtok(string, oddzielnik);
    //printf("%s\n", p);
    strcpy(a, p);
    p = strtok(NULL, oddzielnik);
    //printf("%s\n", p);
    strcpy(b, p);
    p = strtok(NULL, oddzielnik);
    strcpy(c, p);
}
int lvl(struct TNode *root, int i){
    if (root ->right == NULL && root ->left == NULL){
        printf("%d ", i);
    }
    else{
        if (root->right != NULL)
            lvl(root->right, i+1);
        if (root->left != NULL)
            lvl(root->left, i+1);
    }
}

int findnum(struct TNode *root, int value){
    if (root == NULL) return 0;
    if (root->data == value) return 1;
    if (value < root->data){
        return findnum(root->left, value);
    } else {
        return findnum(root->right, value);
    }
}
int main()
{
    char a[20];
    char b[20];
    char c[20];
    char *filename = malloc(128);

    for (int i = 0; i < 20; ++i) {
        int iter = 0;
        int *t = malloc(1000 * (i+1)*sizeof(int));
        char line[40];
        int len = 40;
        sprintf(filename, "../wc/wc_%d.txt", i);
        FILE *f = fopen(filename, "r");
        int num;
        while (fgets(line, len, f)) {
            split(line, a, b, c, " ");
            num = atoi(c);
            t[iter] = num;
            iter++;
        }
        int n = 1000 * (i+1);
        struct TNode *root = sortedArrayToBST(t, 0, n - 1);
        double time_taken;
        clock_t time;
        time = clock();
        for (int j = 0; j < iter; ++j) {
            if (findnum(root, t[j]) == 0)
                printf("nie ma elementu\n");
        }
        time = clock() - time;
        time_taken = ((double)time)/CLOCKS_PER_SEC;
        printf("%f\n", time_taken);
        //lvl(root, 1);
        free(t);
    }

    return 0;
}
