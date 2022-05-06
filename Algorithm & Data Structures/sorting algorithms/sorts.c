#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void fillingArray (int t[],int x,int range)
{
    for (int i=0;i<x;i++)
        t[i]=rand()%range;
}
void filling_a_shape (int t[], int x)
{
    for (int i = 0; i < x/2; i++)
    {
        t[i] = 2*i + 1;
    }
    for (int i = x-1; i >= (x/2); i--)
    {
        t[i] = 2*(x-1 - i);
    }
}
void filling_v_shape (int t[], int x)
{
    for (int i = 0; i < (x/2); i++)
    {
        t[i] = (x-2*i - 1);
    }
    for (int i = x-1; i >= (x/2); i--)
    {
        t[i] = (t[x-i] + 1);
    }
    t[x/2] = 0;
}
void filling_constant (int t[], int x)
{
    for (int i=0;i<x;i++)
        t[i] = 0;
}
void filling_inc1 (int t[], int x)
{
    for (int i=0;i<x;i++)
        t[i] = i;
}
void filling_dec1 (int t[], int x)
{
    for (int i=0;i<x;i++)
        t[i] = x-i;
}
void copying (int t[], int copyy[],int n)
{
 for (int i=0;i<n;i++)
        copyy[i] = t[i];
}
void printing (int t[],int x)
{
   for(int i = 0; i < x; i++)
      printf("%d ", t[i]);
    printf("\n");
}
void swap2 (int *a, int *b){
int temp = *a;
*a = *b;
*b = temp;
}

void swap(int l[], int index, int pos)
{
    int temp = l[pos];
    l[pos] = l[index];
    l[index] = temp;
}
void heapify(int t[],int n, int i)
{
    int root = i;
    int l = 2 * i + 1;
    int r = l + 1;

    if (l < n && t[l] > t[root])
    {
        root = l;
    }
    if (r < n && t[r] > t[root])
    {
        root = r;
    }
    if (root != i)
    {
        swap(t, root, i);
        heapify(t, n, root);
    }
}
void merge(int t[],int first,int mid,int last)
{
    int size1 = mid-first+1, size2 = last - mid;
    int temp1[size1], temp2[size2];
    for (int i=0;i<size1;i++)
        temp1[i] = t[first+i];
    for (int i=0;i<size2;i++)
        temp2[i] = t[mid+1+i];
    int ch1=0,ch2=0,k=first;
    while (ch1<size1 && ch2 <size2)
    {
        if (temp1[ch1] < temp2[ch2])
        {
            t[k] = temp1[ch1];
            ch1++;
        }
        else
        {
            t[k] = temp2[ch2];
            ch2++;
        }
        k++;
    }
    while (ch1<size1)
    {
        t[k] = temp1[ch1];
        ch1++;
        k++;
    }
    while (ch2<size2)
    {
        t[k] = temp2[ch2];
        ch2++;
        k++;
    }
}
void insertion_sort(int l[],int n)
{
    int temp, j, i;
    for (i = 1; i < n; i++)
    {
        temp = l[i];
        j = i - 1;

        while (j >= 0 && l[j] > temp)
        {
         l[j + 1] = l[j];
         j -= 1;
        }
    l[j + 1] = temp;
    }
}
void selection_sort(int l[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int min_i = i;
        for (int j = i + 1; j < n; j++)
        {
            if (l[j] < l[min_i])
                min_i = j;
        }
        swap(l, min_i, i);
    }
}
void bubble_sort(int l[], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n-i-1; j++)
        {
            if (l[j] > l[j + 1])
            {
                swap(l, j+1, j);
            }
        }
    }
}
int partition(int *arr, int start, int end){
    int pivot = arr[(start+end)/2];
    swap2(&arr[end], &arr[(start+end)/2]);
    int i=(start-1);
    for(int j=start; j<= end-1; j++){
        if(arr[j] < pivot){
            i++;
            swap2(&arr[i], &arr[j]);
        }
    }
    swap2(&arr[i+1], &arr[end]);
    return i+1;
}

void quickSort(int *arr, int start, int end){
//    if (start < end){
//        //printf("DZIALAM\n");
//        int mid = partition(arr, start, end);
//
//        quickSort(arr, start, mid-1);
//        quickSort(arr, mid+1, end);
//    }
    while(start < end) {
        int mid = partition(arr, start, end);
        if (mid - start <= end-(mid+1)){
            quickSort(arr, start, mid);
            start = mid+1;
        }
        else{
            quickSort(arr, mid+1, end);
            end = mid;
        }
    }
}

void heap_sort(int t[], int n)
{
    int x;
    for (x = n/2 - 1; x > -1; x--)
    {
        heapify(t, n, x);
    }
    for (int i = n-1; i > 0; i--)
    {
        swap(t, i, 0);
        heapify(t, i, 0);
    }
}
void counting_sort (int t[], int n)
{
    int maks=t[0];
    for (int i=1;i<n;i++)
    {
        if (t[i]> maks)
            maks = t[i];
    }
    maks++;
    int indexes[maks+1];
    int sorted[n];
    for (int i=0;i<maks+1;i++)
        indexes[i] = 0;
    for (int i=0;i<n;i++)
        indexes[t[i]]++;
    for (int i=1;i<maks;i++)
        indexes[i]+=indexes[i-1];
    for (int i=0;i<n;i++)
    {
        sorted[indexes[t[i]]-1]  = t[i];
        indexes[t[i]]--;
    }
    for (int i=0;i<n;i++)
        t[i] = sorted[i];
}
void shell_sort(int t[], int n)
{
    int jmp;
    double tmp;
    tmp = sqrt((n - 1)/2);
    if (tmp < 1)
        tmp = 1;
    for (int i = tmp; i > 0; i--)
    {
        if (i == 1)
        {
            jmp = 1;
        }
        else
        {
            jmp = 2 * i * i + 1;
        }
        for (int j = jmp; j < n; j++)
        {
            int temp = t[j];
            int x;
            for (x = j; x >= jmp && t[x - jmp] > temp; x -= jmp)
            {
                swap(t, x, x-jmp);
            }
        }
    }
}
void merge_sort(int t[],int first,int last)
{
    if (first < last)
    {
        int mid = (first+last)/2;
        merge_sort(t,first,mid);
        merge_sort(t,mid+1,last);

        merge(t,first,mid,last);
    }
}
int main()
{
    int size = 170000;
    int range = 10000;
    int t[size];
    srand(time(NULL));
    printf("unsorted array:\n");
    fillingArray(t,size,range);
    //printing(t,size);
    clock_t S_time;
    double Stime_taken;
    int copyT[170000];

   /* FILE *f = fopen("dec1.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i=0; i<=230000; i+=10000)
    {
        size = i;
        filling_inc1 (t,size);
        copying(t,copyT,size);
        printing(copyT,size);
        S_time = clock();
        merge_sort(copyT, 0, size-1);
        S_time = clock()-S_time;
        Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
        printf("%f\n",Stime_taken);
        fprintf(f,"%f\n" , Stime_taken);
        printing(copyT,size);
    }
    printf("\n");

    for (int i=0;i<=230000;i+=10000)
    {
        size = i;
        filling_dec1 (t,size);
        copying(t,copyT,size);
        printing(copyT,size);
        S_time = clock();
        heap_sort(copyT,size);
        S_time = clock()-S_time;
        Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
        printf("%f\n",Stime_taken);
        fprintf(f,"%f\n" , Stime_taken);
        printing(copyT,size);
    }
    printf("\n");
    //fprintf(f,"\n" );
    for (int i=0;i<=230000;i+=10000)
    {
        size = i;
        filling_dec1 (t,size);
        copying(t,copyT,size);
        S_time = clock();
        quickSort(copyT,0,size-1);
        S_time = clock()-S_time;
        Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
        printf("%f\n",Stime_taken);
        fprintf(f,"%f\n" , Stime_taken);
        printing(copyT,size);
    }
     //fprintf(f,"\n" );
     printf("\n");




        fclose(f);
*/
/*
    copying(t,copyT,size);
    S_time = clock();
    insertion_sort(copyT,size);
    S_time = clock()-S_time;
    Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf("IS: %f\n",Stime_taken);
    //printing (copyT,size);


    copying(t,copyT,size);
    S_time = clock();
    selection_sort(copyT,size);
    S_time = clock()-S_time;
    Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf("SS: %f\n",Stime_taken);
    //printing (copyT,size);


    copying(t,copyT,size);
    S_time = clock();
    bubble_sort(copyT,size);
    S_time = clock()-S_time;
    Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf("BS: %f\n",Stime_taken);
    //printing (copyT,size);

*/
    copying(t,copyT,size);
    S_time = clock();
    quickSort(copyT,0,size-1);
    S_time = clock()-S_time;
    Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf("QS: %f\n",Stime_taken);
    //printing (copyT,size);



    copying(t,copyT,size);
    S_time = clock();
    heap_sort(copyT,size);
    S_time = clock()-S_time;
    Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf("HS: %f\n",Stime_taken);
    //printing (copyT,size);


    copying(t,copyT,size);
    S_time = clock();
    counting_sort(copyT,size);
    S_time = clock()-S_time;
    Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf("CS: %f\n",Stime_taken);
    //printing (copyT,size);

    copying(t,copyT,size);
    S_time = clock();
    shell_sort(copyT,size);
    S_time = clock()-S_time;
    Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf("ShS: %f\n",Stime_taken);
    //printing (copyT,size);

    copying(t,copyT,size);
    S_time = clock();
    merge_sort(copyT,0,size-1);
    S_time = clock()-S_time;
    Stime_taken = ((double)S_time)/CLOCKS_PER_SEC;
    printf("MS: %f\n",Stime_taken);
    //printing (copyT,size);

/*

    //second part of exercises 1 (comparing quicksort for different types of data - random, v-shape and A-shape )
    int size[3] = {99999,100000,100001} ;
    int range = 200;
    clock_t time;
    double time_taken;
    int t[100001];
    printf("time of executing quicksort according to database shape and number of elements in array\n\n\n");
    printf("shape/size      %d        %d        %d\n" ,size[0],size[1],size[2]);


    printf("V-shape");
    for (int i=0;i<3;i++)
    {
        filling_v_shape(t,size[i]);
        time = clock();
        quickSort(t,0,size[i]-1);
        time = clock()-time;
        time_taken = ((double)time)/CLOCKS_PER_SEC;
        printf("       %f",time_taken);
    }
    printf("\n");

    printf("A-shape");
    for (int i=0;i<3;i++)
    {
        filling_a_shape(t,size[i]);
        time = clock();
        quickSort(t,0,size[i]-1);
        time = clock()-time;
        time_taken = ((double)time)/CLOCKS_PER_SEC;
        printf("     %f",time_taken);
    }
    printf("\n");


    printf("random ");
    for (int i=0;i<3;i++)
    {
        fillingArray(t,size[i],range);
        time = clock();
        quickSort(t,0,size[i]-1);
        time = clock()-time;
        time_taken = ((double)time)/CLOCKS_PER_SEC;
        printf("      %f",time_taken);
    }
    printf("\n");
*/

  return 0;
}
