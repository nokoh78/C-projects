#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
typedef struct treenode
{
    int value;
    char name[14];
    char sname[14];
    struct treenode *left;
    struct treenode *right;
    int height;
}treenode;

void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int height(treenode *item)
{
    if (item == NULL)
        return 0;
    return item->height;
}

int max(int a, int b)
{
    return (a > b)? a : b;
}


treenode *createnode(int value, char *cname, char *csname){
    treenode *node = malloc(sizeof(treenode));
    node->value   = value;
    node->left   = NULL;
    node->right  = NULL;
    strcpy(node ->name, cname);
    strcpy(node ->sname, csname);
    node->height = 1;
    return(node);
}

treenode *rr(treenode *top)
{
    treenode *bottom = top ->left;
    treenode *T2 = bottom ->right;
//rotating
    bottom ->right = top;
    top ->left = T2;
    //obliczanie maxymalnej wysokosci dla nowo uporzadkowanego drzewa
    top ->height = max(height(top ->left), height(top ->right))+1;
    bottom ->height = max(height(bottom ->left), height(bottom ->right))+1;

    //Return rotated tree 
    return bottom;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
treenode *lr(treenode *x)
{
    struct treenode *y = x->right;
    struct treenode *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    // Return new root
    return y;
}
//fnc that inform about waga including side of overweight
int balance(treenode *node)
{
    if (node == NULL)
        return 0;
    return height(node ->left) - height(node ->right);
}

treenode *insert(treenode *root, int value, char *name, char *sname)
{
    if (root == NULL)
        return(createnode(value, name, sname));

    if (value < root ->value)
        root ->left  = insert(root ->left, value, name, sname);
    else if (value > root ->value)
        root ->right = insert(root ->right, value, name, sname);
    else
        return root;

    root ->height = 1 + max(height(root ->left),
                           height(root ->right));

    //sprawdzanie czy drzewo jest zbalansowane
    int waga = balance(root);

    if (waga > 1 && value < root ->left->value)
        return rr(root);
    if (waga < -1 && value > root ->right->value)
        return lr(root);
    if (waga > 1 && value > root ->left->value){
        root ->left =  lr(root ->left);
        return rr(root);
    }
    if (waga < -1 && value < root->right->value){
        root->right = rr(root ->right);
        return lr(root);
    }
    return root;
}



void printtabs(int numtabs){
    for (int i = 0; i < numtabs; i++){
        printf("\t");
    }
}

void printtree_lv(treenode *root, int level){
    //printf("check\n");
    if (root == NULL){
        printtabs(level);
        printf("END\n");
        return;
    }
    printtabs(level);
    printf("value = %d\n", root->value);
    printtabs(level);
    printf("name = %s\n", root->name);
    printtabs(level);
    printf("sname = %s\n", root->sname);
    printtabs(level);
    printf("left\n");

    printtree_lv(root->left, level+1);
    printtabs(level);
    printf("right\n");

    printtree_lv(root->right, level+1);

}
void printtree(treenode* root){
    printtree_lv(root, 0);
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


bool findnum(treenode *root, int value){
    if (root == NULL) return false;
    if (root->value == value) return true;
    if (value < root->value){
        return findnum(root->left, value);
    } else {
        return findnum(root->right, value);
    }
}



//test
int isBalanced(treenode* root){
    if(root == NULL) return 1; //if node is empty it means it is balanced
    int leftHeight, rightHeight;

    leftHeight = height(root->left);
    rightHeight = height(root->right);

    if (abs(leftHeight - rightHeight) <= 1 && isBalanced(root->left) && isBalanced(root->right)) return 1; // order here matters because if height is not correct then
        //we won't even check if children are also balanced
    else return 0;
//test

}


treenode *treecreating(FILE *f, FILE *out, FILE *sz, char *a, char *b, char *c, treenode *root, int *t){
    char line[40];
    int len = 40;
    int num;
    int i = 0;
    clock_t time;
    clock_t szuk;
    double time_taken;
    double cz_szuk;

    time = clock();
    while (fgets(line, len, f)) {
        /*if ((i+1)%1000 == 0){
            time = clock() - time;
            time_taken = ((double)time)/CLOCKS_PER_SEC;
            fprintf(out, "%f\n", time_taken);
            //printf("%d\n", isBalanced(root));
            //wyszukiwanie i elementów
            shuffle(t, i);
            //printtree(root);
            szuk = clock();
            for (int j = 0; j < i; ++j) {
                if (findnum(root, t[j]) != 1){
                    printf("Brak %d\n", t[j]);
                }

            }
            szuk = clock() - szuk;
            cz_szuk = ((double)szuk)/CLOCKS_PER_SEC;
            fprintf(sz, "%f\n", cz_szuk);
            //koniec wyszukiwania elementów

            time = clock();
        }*/
        split(line, a, b, c, " ");
        num = atoi(c);
        t[i] = num;
        //printf("%s %s %d \n", a, b, num);
        root = insert(root, num, a, b);
        //printf("%d\n", i);
        i++;
    }
    printf("Tree created.\n");
    return root;
}

void freeing(treenode *root){
    if (root == NULL){
        return;
    }
    freeing(root ->right);
    freeing(root ->left);
    free(root);
}


treenode *replacement(treenode *root){
    treenode *temp = root;
    while (temp && temp ->left != NULL){
        temp = temp ->left;
    }
    return temp;
}

treenode *deleteNode(treenode *root, int value)
{
    if (root == NULL)
        return root;

    if ( value < root->value )
        root->left = deleteNode(root->left, value);

    else if( value > root->value )
        root->right = deleteNode(root->right, value);

    else
    {
        if( (root->left == NULL) || (root->right == NULL) )
        {
            treenode *temp = root->left ? root->left :
                                root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else
        {
            treenode *temp = replacement(root->right);

            root->value = temp->value;
            strcpy(root ->name, (temp)->name);
            strcpy(root ->sname, (temp)->sname);

            root->right = deleteNode(root->right, temp->value);
        }
    }
    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left),
                           height(root->right));

    int waga = balance(root);

    if (waga > 1 && balance(root->left) >= 0)
        return rr(root);
    if (waga > 1 && balance(root->left) < 0)
    {
        root->left =  lr(root->left);
        return rr(root);
    }
    if (waga < -1 && balance(root->right) <= 0)
        return lr(root);
    if (waga < -1 && balance(root->right) > 0)
    {
        root->right = rr(root->right);
        return lr(root);
    }

    return root;
}


int lvl(treenode *root, int i){
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


int main()
{
    int size = 500000;
    char a[20];
    char b[20];
    char c[20];
    char *filename = malloc(128);
    char *dodawanie = malloc(128);
    char *wyszukiwanie = malloc(128);
    char *usuwanie = malloc(128);
    for (int iterator = 0; iterator < 1; ++iterator) {
        printf("%d\n", iterator);
        int *t = calloc(size, sizeof(int));
        treenode *root = NULL;
        sprintf(filename, "../../baza_imion%d.txt",iterator);
        sprintf(dodawanie, "WC dodawanie AVL %d.txt",iterator);
        sprintf(wyszukiwanie, "WC szukanie AVL %d.txt",iterator);
        sprintf(usuwanie, "WC usuwanie %d.txt",iterator);

        FILE *f = fopen(filename, "r");
        FILE *fp = fopen(dodawanie, "w");
        FILE *sz = fopen(wyszukiwanie, "w");
        //pomiar dodawania i szukania
        root = treecreating(f, fp, sz, a, b, c, root, t);
        //printf("%d\n", root->height);
        lvl(root, 0);
        //printtree(root);
        //
        printf("Balans: %d\n", balance(root));
        fclose(f);
        fclose(fp);
        fclose(sz);
        //printtree(root);
        //pomiar usuwania
        FILE *plik = fopen(usuwanie, "w");
        double time_taken;
        clock_t time_t;
        shuffle(t, size);
        time_t = clock();

        for (int i = 0; i < size; ++i) {
            root = deleteNode(root, t[i]);
            if ((i+1)%1000 == 0){
                time_t = clock() - time_t;
                time_taken = ((double)time_t) / CLOCKS_PER_SEC;
                fprintf(plik, "%f\n", time_taken);
                time_t = clock();
            }
        }

        fclose(plik);


        free(t);
        freeing(root);
        //printtree(root);
    }
    free(wyszukiwanie);
    free(filename);
    free(dodawanie);
    free(usuwanie);
    return 0;
}