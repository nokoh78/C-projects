#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

 struct node {
    char name[12];
    char surname[12];
    int idx;
    struct node *next;
};
 typedef struct node  node_t;

 void printlist (node_t *head){

     node_t *temporary = head;

     while (temporary != NULL){
         printf("%d %s %s\n", temporary-> idx, temporary-> name, temporary-> surname);
         temporary = temporary-> next;
     }

 }

node_t *createNode (int IDX, char n[], char sn[]){
    node_t *nowy = malloc(sizeof(node_t));
    nowy-> idx = IDX;
    strcpy(nowy-> name,n);
    strcpy(nowy-> surname,sn);
    nowy-> next = NULL;
    return nowy;
}
node_t *insert_first_position (node_t **head, node_t *node_to_insert ) {
    node_to_insert -> next = *head;
    *head = node_to_insert;
    return node_to_insert;
}
void insert_not_first (node_t *node_before, node_t *node_to_insert){
node_to_insert->next = node_before->next;
node_to_insert ->next = node_to_insert;

}
node_t *seaching (node_t *head, int IDX){
    node_t *temp;
    temp = head;
    while (temp != NULL){
        if (temp->idx == IDX) return temp;
        temp = temp->next;
    }
    return NULL;
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
int stringtoint (char s[]){
    int num=0;

    for (int i=0;i< strlen(s) - 2;i++) {
        num*=10;
        num  += (s[i] - 48);
    }

    return num;
}
int lenghtOflist (node_t *head){
    node_t *current = head;
    int lenght = 1;
    while (current->next != NULL){
        lenght++;
        current = current->next;
    }
    return lenght;
}
void insert_myVersion (node_t *head, int idx, char name[], char surname[]){
    node_t *tmp = createNode (idx, name, surname);
if (tmp->idx < head->idx)
    insert_first_position(&head, tmp);
else {
    node_t *current = head;
    while (current-> next != NULL && current->next->idx < tmp->idx)
        current = current->next;
    insert_not_first (current, tmp);
}
}
void sortedInsert(node_t** head_ref, int idx, char name[], char surname[])
{
    node_t* current;
    node_t* new_node = createNode(idx, name, surname);
    if (*head_ref == NULL
        || (*head_ref)->idx
               >= new_node->idx) {
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else {
        current = *head_ref;
        while (current->next != NULL
               && current->next->idx < new_node->idx) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void shuffle (int arr[], int size){
srand (time(NULL));
for (int i=size-1; i>0; i--){
    int j = rand()% (i+1);
    swap (&arr[i], &arr[j]);
}
}
void deleteNode(node_t** head_ref, int key)
{
    node_t *temp = *head_ref, *prev;
    if (temp != NULL && temp->idx == key) {
        *head_ref = temp->next; 
        free(temp); 
        return;
    }
 
    while (temp != NULL && temp->idx != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;
 
    prev->next = temp->next;
 
    free(temp);
}
void fillingarr (node_t *head,int arr[], int size ){
    node_t *temp = head;
    for (int i=0;i<size;i++){
        arr[i] = temp->idx;
        temp = temp->next;
    }
}
int main() {

    node_t *head = NULL;
    int *arr = malloc(1000000*sizeof(int)); 
    double time_taken;
    char name[14];
    char surname[14];
    char idx_string[14];
    int idx;
    char line[40];
    int len = 40;
    clock_t S_time;
    int new[10000];
    
    FILE *add = fopen ("BIG_wyniki_LL_adding_elements_pojedyncze.txt", "w");
    FILE *del = fopen ("BIG_wyniki_LL_deleting_elements_pojedyncze.txt", "w");
    FILE *sea = fopen ("BIG_wyniki_LL_searching_elements_pojedyncze.txt", "w");
    FILE *read = fopen("baza_imion.txt", "r");

    if (read == NULL){
            printf("Error opening file\n");
            exit(1);
    }
    for (int i=0;i<100;i++){
        S_time = clock();
       for (int j=0;j<10000;j++){
            fgets(line, len, read);
            split (line, name , surname, idx_string, " ");
            idx = stringtoint(idx_string);
            arr[i*10000+j] = idx;
            new[j] = idx;
            sortedInsert (&head, idx, name, surname);
        }
        S_time = clock()-S_time;
        time_taken = ((double)S_time)/CLOCKS_PER_SEC;
        printf( "%d %f\n",i, time_taken);
        fprintf(add, "%f\n", time_taken);

        S_time = clock();
        for (int j=0;j<10000;j++)
            seaching(head, new[j]);
        S_time = clock()-S_time;
        time_taken = ((double)S_time)/CLOCKS_PER_SEC;
        printf( "%d %f\n",i, time_taken);
        fprintf(sea, "%f\n", time_taken);
    }
    fclose (sea);
    fclose(add);
    fclose(read);
printf("usuwanie\n");




    int list_lenght = lenghtOflist(head);
    printf("%d\n", list_lenght);
    //printlist(head);



shuffle(arr, list_lenght);

//for (int i=0;i<100000;i++)
//printf("%d %d\n ", i, arr[i]);
for (int i=0;i<100;i++){
    S_time = clock();
    for (int j=0;j<10000;j++){
        deleteNode(&head, arr[i*10000+j]);
    }
    S_time = clock() - S_time;
    time_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf( "%d %f %d\n",i, time_taken, lenghtOflist(head));
    fprintf(del, "%f\n", time_taken);
}

printlist(head);
fclose(del);
free(arr);
    return 0;
}