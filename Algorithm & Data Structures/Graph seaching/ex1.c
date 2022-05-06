#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <Windows.h>

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

typedef struct node{
    int value;
    struct node *next;
} node_t;
typedef struct EL{
    int node_1;
    int node_2;
    struct EL *next;
} edgelist;
void push (node_t **head, int value){
    node_t * new_node = malloc(sizeof(node_t));
    new_node->value = value;
    node_t * temp = *head;
    new_node->next = temp;
    *head = new_node;
}
void printlist (node_t *head){
    node_t *current = head;
    while (current != NULL){
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}
void push_edge (edgelist ** head, int value1, int value2){
    edgelist *new_node = malloc(sizeof(edgelist));
    new_node->node_1 = value1;
    new_node->node_2 = value2;
    edgelist *temp = *head;
    new_node->next = temp;
    *head = new_node;
}
void creating(FILE *f, int n, int **adj_matrix, node_t ** adj_list, edgelist **e_list, int **incident_matrix ){
    char line[40];
    int len = 40;
    char a[8];
    char b[8];
    int node_1;
    int node_2;
    for (int i=0;i<n;i++) {
        fgets(line, len, f);
        split(line, a, b, " ");
        node_1 = atoi(a);
        node_2 = atoi(b);
        
        //adj matrix
        adj_matrix[node_1][node_2] = 1;
        adj_matrix[node_2][node_1] = 1;
        //adj list
        push(&adj_list[node_1], node_2);
        //edge list
        push_edge(e_list, node_1, node_2);
        //incident matrix
        incident_matrix[node_1][i] = 1;
        incident_matrix[node_2][i] = 1;
    }
}

void deleting_list (node_t ** head){
    node_t *current = *head;
    node_t *next;
    while (current != NULL){
        next = current->next; 
        free(current);
        current = next;
    }
    return;
}
void deleting_edge_list (edgelist **head){
    edgelist *current = *head, *next;
    while (current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}
int searching_edgelist (edgelist *head, int value1, int value2){
    edgelist *temp = head;
    while (temp != NULL){
        if (temp->node_1 == value1 && temp->node_2 == value2)
            return 1;
        temp = temp->next;
    }
    return 0;
}
int searching_adj_list_list (node_t *head, int value){
    node_t *current = head;
    while (current != NULL){
        if (current->value == value)
            return 1;
        current = current->next;
    }
    return 0;
}
int searching_adj_list (node_t ** matrix, int value1, int value2){
    if (searching_adj_list_list(matrix[value1], value2) == 1)
        return 1;
    return 0;
}
int is_corresponding (int **matrix,  int column, int value2){
    if (matrix[value2][column] == 1)
        return 1;
    return 0;
}
int searching_incident_matrix (int **matrix, int value1, int value2, int number_of_edges){

        for (int i=0;i<number_of_edges;i++){
            if (matrix[value1][i] == 1){
                if (matrix[value2][i] == 1)
                    return 1;
            }
        }
    return 0;
}
int searching_adj_matrix (int **matrix, int value1, int value2){
    if (matrix[value1][value2] == 1)
        return 1;
    return 0;
}

double get_cpu_time(){
    FILETIME a,b,c,d;
    if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
        return
            (double)(d.dwLowDateTime |
            ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    }else{
        return 0;
    }
}

int main() {
    char *filename = malloc(128);
    char line[16];
    int len = 16;
    int trials = 3;
    int measuring_points = 10;
    float level_of_saturation = 0.6; //needed to create incident matrix
    int number_of_edges;
    int number_of_tests;
    double *adj_matrix = calloc(sizeof(double), measuring_points);
    double *incidents_matrix = calloc(sizeof(double), measuring_points);
    double *edge_list = calloc(sizeof(double), measuring_points);
    double *adj_list = calloc(sizeof(double), measuring_points);
    FILE *f;
    edgelist *current;
    char a[8];
    char b[8];
    int node_1;
    int node_2;
    double start;
    double end;
    //clock_t time;
    for (int i = 0; i < trials; ++i) {
        for (int j = 0; j < measuring_points; j ++) {
            printf("%d %d\n", i,j);
            sprintf(filename, "G:\\ADS\\grafy\\zad1\\grafy_txt\\edgelist_%d.%d00.txt", i, j+1);
            //printf("%s\n", filename);

            f = fopen(filename, "r");
            if (f == NULL){
                printf("File opening error\n");
                return -1;
            }
            fgets(line, len, f);
            int n = atoi(line);
            //printf("%d\n", n);
            number_of_edges = n*(n-1)*level_of_saturation/2;
            number_of_tests = 4455;
            //printf("%d\n", number_of_edges);

            //section responsible for allocating memory for our computations.
            //adj matrix
            int **tab;
            tab = malloc(sizeof(int *) * n);
            for (int k = 0; k < n; ++k) {
                tab[k] = calloc(n, sizeof(int));
            }
            //ajd list
            node_t ** array_of_heads = malloc (sizeof( node_t *) * n);
            for (int k=0;k<n;k++){
                array_of_heads[k] = NULL;
            }
            //edge list
            edgelist *e_list = NULL;
            //incident matrix
            int **incident_matrix = malloc(sizeof(int*) * n);
            for (int k=0;k<n;k++){
                incident_matrix[k] = calloc (number_of_edges, sizeof(int));
            }
            //printf("tworzenie");
            //tworzenie
            creating(f, number_of_edges, tab, array_of_heads, &e_list, incident_matrix);
            
           //searching
           //printf("Edge");
           edgelist *tests = NULL;
            for (int i = 0; i < number_of_tests; i++)
            {
                //printf("%d  ", i);
                fgets(line, len, f);
                split(line, a, b, " ");
                node_1 = atoi(a);
                node_2 = atoi(b);
                push_edge(&tests, node_1, node_2);
            }
            printf("adj matrix\n");
            current = tests;
            while (current != NULL){
                start = get_cpu_time();
                searching_adj_matrix(tab, current->node_1, current->node_2);
                end = get_cpu_time();
                adj_matrix[j] += (end - start);
                current = current->next;
            }
            
            
            printf("adj list\n");
            start = get_cpu_time();
            current = tests;
            while (current != NULL){
                searching_adj_list(array_of_heads, current->node_1, current->node_2);
                current = current->next;
            }
            end = get_cpu_time();
            adj_list[j] += (end - start);

            printf("edge list\n");
            start = get_cpu_time();
            current = tests;
            while (current != NULL){
                searching_edgelist(e_list, current->node_1, current->node_2);
                current = current->next;
            }
            end = get_cpu_time();
            edge_list[j] += (end - start);

            printf("incident matrix\n");
            
            current = tests;
            while (current != NULL){
                start = get_cpu_time();
                searching_incident_matrix(incident_matrix, current->node_1, current->node_2, number_of_edges);
                end = get_cpu_time();
                incidents_matrix[j] += (end - start);
                current = current->next;
            }
            
            
           /*
           int a, b, c, d;
            for (int i=0;i<100;i++){  
            for (int j=i+1;j<100;j++){
            
            a = searching_edgelist (e_list, i, j);
            
            b = searching_adj_list (array_of_heads, i,j);
            
            c = searching_incident_matrix (incident_matrix, i, j, number_of_edges);
            
            d = searching_adj_matrix (tab, i, j);
            if (a != b || b!= c || c!= d)
                printf("xd");
            }}     
            */
            
            

            //uwalnianie
            fclose(f);
            //adj matrix
            for (int k = 0; k <n; ++k) {
                free(tab[k]);
            }
            free(tab);
            //adj list
            for (int k = 0;k<n;k++){
                deleting_list (&array_of_heads[k]);
            }
            free(array_of_heads);
            //egde list
            deleting_edge_list(&e_list);
            //incident matrix
            for (int k=0;k<n;k++){
                free(incident_matrix[k]);
            }
            free(incident_matrix);
            //tests list
            //deleting_edge_list(&tests);

        }
    }

    for (int i=0;i<measuring_points;i++){
        adj_matrix[i]/=trials;
        incidents_matrix[i]/=trials;
        edge_list[i]/=trials;
        adj_list[i]/=trials;
    }

    FILE *output = fopen("wyniki_koncowe.txt", "w");

    for (int i=0;i<measuring_points;i++){
        fprintf(output, "%f\n", adj_matrix[i]);
    }
    for (int i=0;i<measuring_points;i++){
        fprintf(output, "%f\n", incidents_matrix[i]);
    }
    for (int i=0;i<measuring_points;i++){
        fprintf(output, "%f\n", edge_list[i]);
    }
    for (int i=0;i<measuring_points;i++){
        fprintf(output, "%f\n", adj_list[i]);
    }

    fclose(output);
    free(filename);
    free(adj_list);
    free(adj_matrix);
    free(edge_list);
    free(incidents_matrix);
    
    return 0;
}