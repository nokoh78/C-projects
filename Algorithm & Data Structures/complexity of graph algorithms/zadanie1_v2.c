#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct stack{
    int value;
    struct stack *next;
};

struct stack *add_stack(struct stack *head, int value){
    struct stack *x = malloc(sizeof(*x));
    x->value = value;
    x->next = head;
    return x;
}

struct stack *pop (struct stack *head){
    if(head == NULL)
        return NULL;
    struct stack *tmp = head->next;
    free(head);
    return tmp;
}
int nbh(int **matrix, int vertex, int n){
    for (int i = 0; i < n; ++i) {
        if (matrix[vertex][i] != 0){
            return i;
        }
    }
    return -1;
}

void eulerian_circut(int **matrix, int n, int print){
    //tworzenie tablic do przechowywania danych
    //int *path = calloc(n, sizeof(int));
    struct stack *path = NULL;
    struct stack *stos = NULL;
    //we start with vertex v0
    stos = add_stack(stos, 0);
    while (stos != NULL) {
        int current = stos -> value;
        int next = nbh(matrix, current, n);
        if (next == -1){
            path = add_stack(path, current);
            stos = pop(stos);
        }
        else{
            stos = add_stack(stos, next);
            //remove edge
            matrix[current][next] = 0;
            matrix[next][current] = 0;
        }
    }
    //printing
    if (print == 1) {
        while (path != NULL) {
            printf("%d ", path ->value);
            path = pop(path);
        }
    }
    else {
        while (path != NULL) {
            path = pop(path);
        }
    }
    
}



int is_even (int **matrix, int n){
    int counter;
    for (int i=0;i<n;i++){
        counter = 0;
        for (int j=0;j<n;j++)
            if (matrix[i][j] == 1)
                counter++;        
        if (counter%2 == 1)
            return 0;
        
    }
    return 1;
}
void adding (int x, int *set, int **matrix, int n){
    set[x] = 1;
    for (int i=0;i<n;i++){
        if (matrix[x][i] == 1 && set[i] == 0){
            
            adding(i,set,matrix,n);
        }
    }
}

int is_connected (int **matrix, int n){
    int set[n];
    for (int i=0;i<n;i++)
        set[i] = 0;
    adding(0, set, matrix, n);
    for (int i=0;i<n;i++){
        if (set[i] == 0){
            printf("%d\n",i);
            return 0;
        }
    }
    
    return 1;
}
int split(char *string, char *a, char *b, char *oddzielnik){
    if (strcmp(string, "\n") == 0 || strlen(string) <= 1){
        return 0;
    }
    char *p = strtok(string, oddzielnik);
    strcpy(a, p);
    p = strtok(NULL, oddzielnik);
    strcpy(b, p);
    p = strtok(NULL, oddzielnik);
    if (p != NULL)
        return 0;
    return 1;
}

int main (){
    
    char filename[100];
    char line[20];
    int len = 20;
    double sat_lvl[] = {0.2, 0.3, 0.4, 0.6, 0.8, 0.95};
    int number_of_edges;
    int number_of_sat_lvl = 6;
    int trials = 5;
    int measure_points = 7;
    clock_t start, end;

    double **results = malloc(sizeof(double *) * measure_points);
    for (int i=0;i<measure_points;i++)
        results[i] = calloc(number_of_sat_lvl, sizeof(double));
    
    FILE *f;

    for (int i=0;i<trials;i++){
        for (int j=0;j<measure_points;j++){
            for (int k=0;k<number_of_sat_lvl;k++){
                printf("%d %d %d ", i, j, k);
                sprintf(filename, "C:\\Users\\matri\\OneDrive - ProData\\polibuda\\ADS\\raport 4\\grafy_eulerian_circut\\Graph_%d_%d00_%.0f.txt", i, j+1, sat_lvl[k]*100);
                //printf("%s\n", filename);
                f = fopen(filename, "r");
                if (f == NULL){
                    printf("error opening file\n");
                    return -1;
                }

                fgets(line, len, f);
                int number_of_vertices = atoi(line);
        
                //allocating memory for nbh matrix
                int **nbh_matrix;
                nbh_matrix = malloc(sizeof(int *) * number_of_vertices);
                for (int l = 0; l < number_of_vertices; ++l) 
                    nbh_matrix[l] = calloc(number_of_vertices, sizeof(int));
            
                //filling matrix
                number_of_edges = number_of_vertices*(number_of_vertices-1)*sat_lvl[k];
                //printf("%d\n", number_of_edges);
                int node_1, node_2;
                char a[8], b[8];
                while (fgets(line, len, f)){
                    //printf("%s\n", line);
                    split(line, a, b, " ");
                    node_1 = atoi(a)-1;
                    node_2 = atoi(b)-1;
                    nbh_matrix[node_1][node_2] = 1;
                    nbh_matrix[node_2][node_1] = 1;
                }
                
                //checking correctness
                if (is_connected(nbh_matrix, number_of_vertices) == 0){
                    printf("in %s graph is not connected\n", filename);
                    break;
                }
            
                if (is_even(nbh_matrix, number_of_vertices) == 0) {
                    printf("in %s at least one vertex has odd degree\n", filename);
                    break;
                }
                

                //creating euler cycle
                start = clock();
                eulerian_circut(nbh_matrix, number_of_vertices, 0);
                end = clock();
                results[j][k] += ((double) (end - start)) / CLOCKS_PER_SEC;

                //deleting matrix
                for (int l = 0; l <number_of_vertices; l++) 
                    free(nbh_matrix[l]);
                free(nbh_matrix);
                fclose(f);
            }
        }
    }

    for (int i=0;i<measure_points;i++){
        for (int j=0;j<number_of_sat_lvl;j++){
            results[i][j]/=trials;
        }
    }
    
    f = fopen("results.txt", "w");
    fprintf(f, "%d %d\n", measure_points, number_of_sat_lvl);
    //printing and saving results
    for (int i=0;i<measure_points;i++){
        for (int j=0;j<number_of_sat_lvl;j++){
            printf("%f ", results[i][j]);
            fprintf(f, "%f ", results[i][j]);
        }
        printf("\n");
        fprintf(f, "\n");
    }
    fclose(f);
    for (int i=0;i<measure_points;i++)
        free(results[i]);
    free(results);
    
    return 0;
}
