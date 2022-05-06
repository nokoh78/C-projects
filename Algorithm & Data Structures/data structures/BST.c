#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
typedef struct treenode{
    int value;
    char name[13];
    char sname[13];
    struct treenode *left;
    struct treenode *right;
} treenode;

treenode *createnode(int value, char *cname, char *csname){
    treenode* result = malloc(sizeof(treenode));
    if (result != NULL){
        result ->left = NULL;
        result ->right = NULL;
        result ->value = value;
        strcpy(result ->name, cname);
        strcpy(result ->sname, csname);
        //printf("%s\n", result ->sname);
    }
    return result;
}

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

bool insertnum(treenode **rootpoint, int value, char *name, char *sname){

    treenode *root = *rootpoint;

    if (root == NULL){
        //printf("%s %s %d\n", name, sname, value);
        //printf("insert\n");
        (*rootpoint) = createnode(value, name, sname);
        return true;
    }
    if (value == root->value){
        return false;
    }
    if (value < root->value){
        return insertnum(&(root->left), value, name, sname);
    }else {
        return insertnum(&(root->right), value, name, sname);
    }

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

treenode *replacement(treenode *root){
    treenode *temp = root;
    //printf("test 2\n");
    while (temp && temp ->left != NULL){
        temp = temp ->left;
    }
    return temp;
}


treenode *deleteNode(treenode **root, int key) {

    if (*root == NULL) return *root;

    if (key < (*root)->value)
        (*root)->left = deleteNode(&(*root)->left, key);
    else if (key > (*root)->value)
        (*root)->right = deleteNode(&(*root)->right, key);

    else {
        if ((*root)->left == NULL) {
            treenode *temp = (*root)->right;
            free(*root);
            return temp;
        } else if ((*root)->right == NULL) {
            treenode *temp = (*root)->left;
            free(*root);
            return temp;
        }

        treenode *temp = replacement((*root)->right);

        (*root)->value = temp->value;
        strcpy(temp ->name, (*root)->name);
        strcpy(temp ->sname, (*root)->sname);
        (*root)->right = deleteNode(&(*root)->right, temp->value);
    }
    return (*root);
}


void freeing(treenode *root){
    if (root == NULL){
        return;
    }
    freeing(root ->right);
    freeing(root ->left);
    free(root);
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

void treecreating(FILE *f, FILE *out, FILE *sz, char *a, char *b, char *c, treenode **root, int *t){
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
        if ((i+1)%1000 == 0){
            printf("%d\n", i);
            time = clock() - time;
            time_taken = ((double)time)/CLOCKS_PER_SEC;
            fprintf(out, "%f\n", time_taken);
            //wyszukiwanie i elementów
            shuffle(t, i);
            szuk = clock();
            for (int j = 0; j < i; ++j) {
               // printf("%d\n", j);
                if (findnum(*root, t[j]) != 1){
                    printf("Brak %d\n", t[j]);
                }
            }
            szuk = clock() - szuk;
            cz_szuk = ((double)szuk)/CLOCKS_PER_SEC;
            fprintf(sz, "%f\n", cz_szuk);
            //koniec wyszukiwania elementów

            time = clock();
        }
        split(line, a, b, c, " ");
        num = atoi(c);
        t[i] = num;
        //printf("%s %s %d \n", a, b, c);
        insertnum(root, num, a, b);
        //printf("%d\n", i);
        i++;
    }
}



void randdel(treenode **root, int *t, int size, FILE *plik){
    double czas_zabrany;
    clock_t czas;
    czas = clock();

    for (int i = 0; i < size; i++){
        printf("%d\n", t[i]);
        *root = deleteNode(root, t[i]);
        if ((i+1)%1000 == 0){
            czas = clock() - czas;
            czas_zabrany = ((double) czas) / CLOCKS_PER_SEC;
            fprintf(plik, "%f\n", czas_zabrany);
            czas = clock();
        }

    }
    //printtree(*root);
}

int main(){
    int size = 20000;
    char a[20];
    char b[20];
    char c[20];
    char *filename = malloc(128);
    char *dodawanie = malloc(128);
    char *wyszukiwanie = malloc(128);
    char *usuwanie = malloc(128);


    for (int iterator = 0; iterator < 5; ++iterator) {
        printf("%d\n", iterator);
        int *t = calloc(size, sizeof(int));

        //pomiar dodawania i szukania
        treenode *root = NULL;
        sprintf(filename, "G:\\ADS\\Projekt 2\\baza_imion_wc%d.txt",iterator);
        sprintf(dodawanie, "wyniki dodawania WC BST %d.txt",iterator);
        sprintf(wyszukiwanie, "wyniki wyszukiwania WC BST %d.txt",iterator);
        sprintf(usuwanie, "wyniki usuwania WC BST %d.txt",iterator);

        FILE *f = fopen(filename, "r");
        FILE *fp = fopen(dodawanie, "w");
        FILE *sz = fopen(wyszukiwanie, "w");
        treecreating(f, fp, sz, a, b, c, &root, t);
        fclose(f);
        fclose(fp);
        fclose(sz);
        printf("działa\n");
        //pomiar usuwania
        shuffle(t, size);
        printf("shuffle");
        FILE *plik = fopen(usuwanie, "w");
        //double time_taken;
        //clock_t time_t;
        //time_t = clock();
        randdel(&root, t, size, plik);
        //time_t = clock() - time_t;
        //time_taken = ((double) time_t) / CLOCKS_PER_SEC;
        //fprintf(plik, "%f\n", time_taken);
        fclose(plik);
        free(t);
        freeing(root);
    }
    free(wyszukiwanie);
    free(filename);
    free(dodawanie);
    free(usuwanie);

    return 0;
}