#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int max (int a, int b){
    return (a>b)? a:b;
}


int knapsack_bf(int n, int w, int values[], int weights[]){
    if (n == 0 || w == 0)//base case
        return 0;
    if (weights[n - 1] > w)
        return knapsack_bf(n-1, w, values, weights);
    else
        return max(values[n-1] + knapsack_bf(n-1, w - weights[n-1], values, weights), knapsack_bf(n-1, w, values, weights));
}

int knapsack_dp_calc(int n, int w, int *values, int *weights, int **DP){
    if (n == 0 || w == 0){//base case
        //printf("Base case\n");
        return 0;}

    if (DP[n][w] != -1) {
        //printf("DP\n");
        return DP[n][w];
    }
    if (weights[n - 1] > w) {
        //printf("Za duza waga\n");
        DP[n][w] = knapsack_dp_calc(n - 1, w, values, weights, DP);
        return DP[n][w];
    }
    else {
        //printf("Obliczenia\n");
        DP[n][w] = max(values[n - 1] + knapsack_dp_calc(n - 1, w - weights[n - 1], values, weights, DP),knapsack_dp_calc(n - 1, w, values, weights, DP));
        return DP[n][w];
    }
}


int knapsack_dp_rec(int n, int w, int *values, int *weights){
    int **DP = malloc(sizeof(int*) * (n+1));
    for (int i = 0; i <= n; ++i) {
        DP[i] = malloc(sizeof(int) * (w+1));
    }
    //printf("test 1\n");
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= w; ++j) {
            DP[i][j] = -1;
        }
    }
    //printf("test 2\n");
    int max_value =  knapsack_dp_calc(n, w, values, weights, DP);
    //printf("koniec\n");
    for (int i = 0; i <= n; ++i) {
        free(DP[i]);
    }
    free(DP);
    return max_value;
}


int iter_dp_knapsack (int number_of_items, int max_weight, int values[], int weights[]){
    //allocating some memory
    int max_value;
    int temp;
    int **dp = malloc (sizeof (int *) * (number_of_items+1));
    for (int i=0;i<(number_of_items+1);i++)
        dp[i] = calloc ((max_weight+1), sizeof(int));

    //proper calculations
    for (int i=1;i<=number_of_items;i++){
        for (int j=1;j<=max_weight;j++){

            if (j - weights[i-1] < 0 )
                temp = 0;

            else
                temp = values[i-1] + dp[i-1][j-weights[i-1]];

            dp[i][j] = max(dp[i-1][j],temp );
        }
    }
    /*
    for (int i=0;i<number_of_items+1;i++){
        for (int j=0;j<max_weight+1;j++)
            printf("%d ", dp[i][j]);
        printf("\n");

    }
    */

    max_value = dp[number_of_items][max_weight];

    //freeing memory after calculations
    for (int i=0;i<(number_of_items+1);i++)
        free(dp[i]);
    free(dp);

    return max_value;
}

void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap2 (double *a, double *b){
    double temp = *a;
    *a = *b;
    *b = temp;
}

int partition(double *arr, int *arr2, int *arr3, int start, int end){
    double pivot = arr[(start+end)/2];
    swap2(&arr[end], &arr[(start+end)/2]);
    swap(&arr2[end], &arr2[(start+end)/2]);
    swap(&arr3[end], &arr3[(start+end)/2]);
    int i=(start-1);
    for(int j=start; j<= end-1; j++){
        if(arr[j] > pivot){
            i++;
            swap2(&arr[i], &arr[j]);
            swap(&arr2[i], &arr2[j]);
            swap(&arr3[i], &arr3[j]);
        }
    }
    swap2(&arr[i+1], &arr[end]);
    swap(&arr2[i+1], &arr2[end]);
    swap(&arr3[i+1], &arr3[end]);
    return i+1;
}

void quickSort(double *arr, int *arr2, int *arr3, int start, int end){

    while(start < end) {
        int mid = partition(arr, arr2, arr3, start, end);
        if (mid - start <= end-(mid+1)){
            quickSort(arr, arr2, arr3, start, mid);
            start = mid+1;
        }
        else{
            quickSort(arr, arr2, arr3, mid+1, end);
            end = mid;
        }
    }
}


double greedy_knapsack_low (int number_of_items, int max_weight, int values[], int weights[]){
    double profit[number_of_items];
    for (int i=0;i<number_of_items;i++)
        profit[i] =  (double)values[i] / weights[i];

    quickSort(profit, values, weights, 0, number_of_items-1);

    int c_weight = 0;
    double c_value = 0;
    int i=0;
    while ((c_weight += weights[i]) <= max_weight){

        c_value += values[i];
        i++;
    }
    return c_value;
}

double greedy_knapsack_up (int number_of_items, int max_weight, int values[], int weights[]){
    double profit[number_of_items];
    for (int i=0;i<number_of_items;i++)
        profit[i] =  (double)values[i] / weights[i];

    quickSort(profit, values, weights, 0, number_of_items-1);

    int c_weight = 0;
    double c_value = 0;
    int i=0;
    while ((c_weight += weights[i]) <= max_weight){

        c_value += values[i];
        i++;
    }
    c_weight -= weights[i];
    c_value +=(double) (max_weight - c_weight) / weights[i] * values[i];
    return c_value;
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
    int trials = 10;
    char filename[100];
    char line[20];
    int len = 20;
    char a[8], b[8];
    int measure_points = 10;
    FILE *f;
    int number_of_items = 20000;
    int values[number_of_items];
    int weights[number_of_items];
    clock_t start, end;
    int capacity = 9000;
    double **results = malloc (sizeof(double*) * measure_points);
    double *precision = calloc(measure_points, sizeof(double));
    double *precision2 = calloc(measure_points, sizeof(double));
    double *precision3 = calloc(measure_points, sizeof(double));
    for (int i=0;i<measure_points;i++){
        results[i] = calloc (4, sizeof(double));
    }

    int max_weight = 6;
    for (int i=0;i<trials;i++)  {
        sprintf(filename,"../skrypt_conda/knapsack_%d.txt", i );
        f = fopen(filename, "r");
        if (!f ){
            printf("error opening file\n");
            return -1;
        }
        for (int j=0;j<number_of_items;j++) {
            fgets(line, len, f);
            split(line, a, b, " ");
            weights[j] = atoi(a);
            values[j] = atoi(b);
        }
    //sprawdzanie dokładności
    

        for (int j = 0; j < measure_points; ++j) {
            int iter = iter_dp_knapsack(20000, 100+j*50, values, weights);
            printf("%d %d\n", i, j);
            //printf("%f %f, %d\n", greedy_knapsack_low((j+1) * 2000, capacity, values, weights), greedy_knapsack_up((j+1) * 2000, capacity, values, weights), iter_dp_knapsack((j+1) * 2000, capacity, values, weights));
            precision[j] += greedy_knapsack_up(20000, 100+j*50, values, weights) - greedy_knapsack_low(20000, 100+j*50, values, weights) ;
            precision2[j] += iter - greedy_knapsack_low(20000, 100+j*50, values, weights); // zmieniam tutaj na low bo gdy było up to wyniki byly za dokladne - co za tym idzie odwracam znak
            precision3[j] += iter;
        }



        /*for (int j=0;j<10;j++){

            start = clock();
            knapsack_dp_rec( (j+1)*2000, capacity, values, weights);
            end = clock();
            results[j][0] += ((double) (end - start)) / CLOCKS_PER_SEC;

            start = clock();
            iter_dp_knapsack( (j+1)*2000, capacity, values, weights);
            end = clock();
            results[j][1] += ((double) (end - start)) / CLOCKS_PER_SEC;

            start = clock();
            greedy_knapsack( (j+1)*2000, capacity, values, weights);
            end = clock();
            results[j][2] += ((double) (end - start)) / CLOCKS_PER_SEC;
            printf("%d\n", j);

        }/*
        /*for (int j = 0; j < 10; j++)
        {
            start = clock();
            knapsack_bf( (j+1)*3, capacity, values, weights);
            end = clock();
            results[j][3] += ((double) (end - start)) / CLOCKS_PER_SEC;
        }*/

        fclose(f);
    }



    //printf("%f\n", greedy_knapsack( number_of_items, max_weight, values, weights));
    //printf("%d\n", knapsack_bf( number_of_items, max_weight, values, weights));
    //printf("%d\n", knapsack_dp_rec( number_of_items, max_weight, values, weights));
    //printf("%d\n", iter_dp_knapsack( number_of_items, max_weight, values, weights));
    /*
    for (int i=0;i<measure_points;i++){
        for (int j=0;j<4;j++){
            results[i][j]/=trials;
        }
    }

    f = fopen("results.txt", "w");
    //printing and saving results
    for (int i=0;i<measure_points;i++){
        for (int j=0;j<4;j++){
            printf("%f ", results[i][j]);
            fprintf(f, "%f ", results[i][j]);
        }
        printf("\n");
        fprintf(f, "\n");
    }
    fclose(f);
*/

    f = fopen("precision3.txt", "w");
    for (int i = 0; i < measure_points; i++)
    {
        precision[i] /= trials;
        precision2[i] /= trials;
        precision3[i] /= trials;
        printf("%f %f %f\n", precision[i], precision2[i], precision3[i]);
        fprintf(f, "%f %f %f\n", precision[i], precision2[i], precision3[i]);
    }
    

    
    for (int i=0;i<4;i++)   {
        free(results[i]);
    }
    free(results);
    free(precision);
    free(precision2);
    free(precision3);
    return 0;
}