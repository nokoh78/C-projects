#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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

int is_correct(int v, int **graph, int path[], int pos){
    if (graph [ path[pos-1] ][ v ] == 0)
        return 0;
    for (int i = 0; i < pos; i++)
        if (path[i] == v)
            return 0;
    return 1;
}
  
int searching_ham_cycle(int **graph, int path[], int pos, int number_of_vertices, clock_t start){
    
    clock_t end = clock();
    if (((double) (end - start)) / CLOCKS_PER_SEC > 300 ){
        //printf("skip\n");
        return -1;
    }

    if (pos == number_of_vertices){
        if ( graph[ path[pos-1] ][ path[0] ] == 1 )
           return 1;
        else
          return 0;
    }
    for (int v = 1; v < number_of_vertices; v++){
        if (is_correct(v, graph, path, pos)){
            path[pos] = v;
            if (searching_ham_cycle (graph, path, pos+1, number_of_vertices, start) == 1)
                return 1;
            path[pos] = -1;
        }
    }
    return 0;
}
  

int hamiltonian_cycle(int **graph, int number_of_vertices, clock_t start){
    int test;
    int *path = malloc(sizeof(int) * number_of_vertices);
    for (int i = 0; i < number_of_vertices; i++)
        path[i] = -1;
    path[0] = 0;
    test = searching_ham_cycle(graph, path, 1, number_of_vertices, start);
    //printf("test = %d\n", test);
    if (test == 0){
        //printf("skip\n");
        return -1;
    }
    free(path);
    return 0;
}
  
int main (){
    
    char filename[100];
    char line[20];
    int len = 20;
    double sat_lvl[] = {0.2, 0.3, 0.4, 0.6, 0.8, 0.95};
    int number_of_edges;
    int number_of_sat_lvl = 6;
    int trials = 10;
    int measure_points = 10;
    int **skips = malloc(sizeof(int*) * measure_points);
    for (int i = 0; i < measure_points; i++)
    {
        skips[i] = calloc(number_of_sat_lvl, sizeof(int));
    }
    
    clock_t start, end;

    double **results = malloc(sizeof(double *) * measure_points);
    for (int i=0;i<measure_points;i++)
        results[i] = calloc(number_of_sat_lvl, sizeof(double));
    
    FILE *f;

    for (int i=0;i<trials;i++){
        for (int j=0;j<measure_points;j++){
            for (int k=0;k<number_of_sat_lvl;k++){
                printf("%d %d %d\n", i, j, k);
                sprintf(filename, "grafy_hamiltonian_circle/Graph_%d_%d_%.0f.txt", i, (j+1)*50, sat_lvl[k]*100);
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
            
                start = clock();
                //TODO
                if (hamiltonian_cycle(nbh_matrix, number_of_vertices, start) == -1){
                    skips[j][k]++;
                }
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
    
    f = fopen("results_ex2.csv", "w");
    fprintf(f, "measuring points: %d, saturation levels: %d, trial: %d\n", measure_points, number_of_sat_lvl, trials);
    //printing and saving results
    for (int i=0;i<measure_points;i++){
        for (int j=0;j<number_of_sat_lvl;j++){
            printf("%f ", results[i][j]);
            fprintf(f, "%f,", results[i][j]);
        }
        printf("\n");
        fprintf(f, "\n");
    }
    fclose(f);


     
    f = fopen("skips_ex2.csv", "w");
    fprintf(f, "measuring points: %d, saturation levels: %d, trial: %d\n", measure_points, number_of_sat_lvl, trials);
    //printing and saving results
    for (int i=0;i<measure_points;i++){
        for (int j=0;j<number_of_sat_lvl;j++){
            printf("%d ", skips[i][j]);
            fprintf(f, "%d,", skips[i][j]);
        }
        printf("\n");
        fprintf(f, "\n");
    }
    fclose(f);

    for (int i=0;i<measure_points;i++)
        free(results[i]);
    free(results);

    for (int i=0;i<measure_points;i++)
        free(skips[i]);
    free(skips);
    
    return 0;
}