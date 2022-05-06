#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct keys_values {
    char *key;
    char *value;
    struct keys_values * next;
} kvll;
typedef struct section {
    char *section_name;
    struct keys_values *head;
    struct section *next;
} sectionll;

int split(char *string, char *a, char *b, char *oddzielnik){
    char *p = strtok(string, oddzielnik);
    //printf("%s\n", p);
    strcpy(a, p);
    p = strtok(NULL, oddzielnik);
    //printf("%s\n", p);
    strcpy(b, p);
    p = strtok(NULL, oddzielnik);

    if (p != NULL)
        return 0;
    return 1;
    
}
void split_3parts(char *string, char *a, char *b, char *c, char *oddzielnik){  
    char *p = strtok(string, oddzielnik);
    //printf("%s\n", p);
    strcpy(a, p);
    p = strtok(NULL, oddzielnik);
    //printf("%s\n", p);
    *b = p[0];
    p = strtok(NULL, oddzielnik);
    strcpy(c, p);
}
int iscorrect(char *string){
    for (size_t i=0;i<strlen(string);i++)
        if (string[i] != '-' && string[i] < 48)
            return 0;
        else if (string[i] >57 && string[i] < 65)
            return 0;
        else if (string[i] > 90 && string[i] < 97 )
            return 0;
        else if (string[i] >122)
            return 0;
    return 1;

}

int stringtoint (char s[]){
    int num=0;

    for (size_t i=0;i< strlen(s);i++) {
        num*=10;
        num  += (s[i] - 48);
    }

    return num;
}

int isnumber (char *string){
    for (size_t i=0;i<strlen(string);i++){
        //printf("%c", string[i]);
        if (!isdigit(string[i]))
            return 0;
    }
    //printf("\n");
    return 1;
}

void rm_spaces (char* string){
    if (string[strlen(string)-1] == '\n')
    string[strlen(string)-1] = '\0';
}


void push_section(sectionll ** head_ref, char *name){
    sectionll * new_node = malloc(sizeof(sectionll));
    char *temp =  (char *)malloc((strlen(name)+1) * sizeof(char));
    strcpy(temp, name);
    new_node->section_name = temp;
    kvll *head = NULL;
    new_node->head = head;
    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}

int push_kivi (kvll **head_ref, char *line){
    kvll *new_node = malloc(sizeof(kvll));
    char *key_tmp = (char *)malloc((strlen(line)+1) * sizeof(char));
    char *value_tmp = (char *)malloc((strlen(line)+1) * sizeof(char));
    int sprawdzak;
    sprawdzak = split(line, key_tmp, value_tmp, " = ");
    new_node->key = key_tmp;
    new_node ->value = value_tmp;
    new_node->next = (*head_ref);
    (*head_ref) = new_node;
    return sprawdzak;
}

void printlist_section (sectionll *head){

     sectionll *temporary = head;

     while (temporary != NULL){
         printf("%s\n",temporary-> section_name );
         temporary = temporary-> next;
     }

 }

void printlist_kivi (kvll *head){
    kvll *temp = head;
    while (temp != NULL){
        printf("%s%s\n", temp->key, temp->value);
        temp = temp->next;
    }
}

void delete_kv (kvll ** head_ref){
    kvll* current = *head_ref;
    kvll* next;
    while (current != NULL){
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    *head_ref = NULL;
}
void deleting_section(sectionll** head_ref)
{
    sectionll *current = *head_ref;
    sectionll *next;

    while (current != NULL){
        delete_kv (&(current->head));
        free(current->section_name);
        next = current->next;
        free(current);
        current = next;
    }
    *head_ref = NULL;
    
}

sectionll *searching_section (sectionll *head, char* section_name){
    sectionll *temp;
    temp = head;
    while (temp != NULL){
        if (strcmp(temp->section_name, section_name) == 0) return temp;  
        temp = temp->next;
    }
    printf("missing section %s\n", section_name);
    return NULL;
}

char* searching_kivi(kvll *head, char* key_name, char *section_name){
    kvll *temp;
    temp = head;
    while (temp != NULL){
        if (strcmp(temp->key, key_name) == 0) return temp->value;  
        temp = temp->next;
    }
    printf("missing key %s in section %s\n", key_name, section_name);
    return NULL;
}

int main (int argc, char *argv[]){
 sectionll *head = NULL;
 int correct = 1;
 int size_of_line = 30;
 char * line = (char *)malloc(size_of_line * sizeof(char));
 FILE *dane = fopen(argv[1], "r");
 if (dane == NULL){
     printf("error opening file\n");
     return 1; }
//part that read file line by line
while(fgets(line, size_of_line, dane) != NULL ) {
    while( *(line+strlen(line)-1) != '\n' && *(line+strlen(line)-1) != '\0' ){
            line = realloc(line, 2 * size_of_line * sizeof(char));
            char * tmp = malloc(size_of_line * sizeof(char)); 
            if (tmp) {
                fgets(tmp, size_of_line, dane);
                strcat(line, tmp);
                size_of_line *= 2;
            }
            free(tmp);
        }
        
    rm_spaces(line);

    //printf("%lu %s",strlen(line), line);
    if (*line == '['){

        if (line[0] != '[' || line[strlen(line)-1] != ']') //part which is resposible for deleting [] from the beginning and the end of the line
            printf("Wrong input in function strip\n");
        line[strlen(line)-1] = '\0';

        push_section(&head, line+1);
        if (!iscorrect(line+1)){
            printf("wrong INI file\n");
            correct = 0;
            break;
        }
    
    }
    else if (strlen(line) > 1 && *line != ';'){
        if (push_kivi(&(head->head), line) == 0){
            printf("wrong INI file\n");
            correct = 0;
            break;
        }
        if (!iscorrect(head->head->key) || !iscorrect(head->head->value)){
            printf("wrong INI file\n");
            correct = 0;
            break;
        }
        
    }
    
}
//printf("%d\n", correct);
free(line);
fclose(dane);

if (correct){

    if (argc == 3){
    char *key_input = malloc(sizeof(char) * strlen(argv[2]));
    char *section_input = malloc(sizeof(char) * strlen(argv[2]));
    split(argv[2], section_input, key_input, ".");
    sectionll* temp_section = searching_section(head, section_input);
    //printf("%s %s\n", section_input, key_input);

    if (temp_section != NULL){
        char *value = searching_kivi(temp_section->head, key_input, temp_section->section_name);
        if (value != NULL)
        printf("value of key %s in section %s is: %s\n", key_input, section_input, value);
    }
    
    free(key_input);
    free(section_input);
}

//zadanie na 5
else if (argc == 4 && strcmp ("expression", argv[2] )== 0) {
    //printf("%d\n", argc);
    char *section_key1 = malloc(sizeof(char) * strlen(argv[3]));
    char *section_key2 = malloc(sizeof(char) * strlen(argv[3]));
    char *key1_input = malloc(sizeof(char) * strlen(argv[3]));
    char *section1_input = malloc(sizeof(char) * strlen(argv[3]));
    char *key2_input = malloc(sizeof(char) * strlen(argv[3]));
    char *section2_input = malloc(sizeof(char) * strlen(argv[3]));
    char operant;
    char *value1, *value2;

    split_3parts(argv[3], section_key1, &operant, section_key2, " ");
    split(section_key1, section1_input, key1_input, ".");
    split(section_key2, section2_input, key2_input, ".");
    
    sectionll* temp_section = searching_section(head, section1_input);
    if (temp_section != NULL)
        value1 = searching_kivi(temp_section->head, key1_input, temp_section->section_name);
    temp_section = searching_section(head, section2_input);
    if (temp_section != NULL)
        value2 = searching_kivi(temp_section->head, key2_input, temp_section->section_name);

//printf("%s, %s\n", value1, value2);
    if (operant != '+' && operant != '-' && operant != '*' && operant != '/')
        printf("invalid operant\n");
    else if (value1 != NULL && value2 != NULL){
        if (isnumber(value1) == 1 && isnumber(value2) == 1){
            int v1 = stringtoint(value1);
            int v2 = stringtoint(value2);

            if (operant == '+')
                printf("result of addition: %d\n", v1+v2);
            else if (operant == '-')
                printf("result of substraction: %d\n", v1-v2);
            else if (operant == '*')
                printf("result of multiplication: %d\n", v1*v2);
            else if (v2 != 0)
             printf("result of division: %f\n",(double) v1/v2);

            else printf("pamietaj cholero nie dziel przez 0\n");

        }        
        else  if (isnumber(value1) == 0 && isnumber(value2) == 0  ){
            if (operant != '+')
                printf("invalid operant for two strings\n");
            else {
                printf("result: %s\n", strcat(value1, value2));
            }
        }
        else printf("expression is invalid: string cannot be added to number\n");

    }
    
    
    free(section_key1);
    free(section_key2);
    free(key1_input);
    free(section1_input);
    free(key2_input);
    free(section2_input);

}
else printf("wrong input format\n");

}


//printlist_section(head);
//printlist_kivi(head->head);

deleting_section(&head);
    return 0;
}
