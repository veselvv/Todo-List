#include "dynamic_string.h"
#include <stdlib.h>
#include <stdio.h>




int check_capacity_string(char **string, int *capacity, int count){
    if(count>=*capacity){
        char *temp = realloc(*string, sizeof(char)* *capacity*2);
        if(!temp){
            perror("Could not init memmory: ");
            return 0;
        }
        *capacity*=2;
        *string = temp;
        return 1;
    }
    return 1;
}

char *mgetline(){
    int capacity =10;
    int count =0;
    char *string = malloc(sizeof(char)*capacity);
    if(!string){
        perror("Could not init memmory: ");
        return NULL;
    }
    int c;
    printf(">");
    while ((c = getc(stdin))!='\n'){
        if(check_capacity_string(&string, &capacity, count+1)){
            string[count++] = (char)c;
        }else{
            printf("Не удалось перевыделить память\n");
            free(string);
            string = NULL;
            return NULL;
        }
    }
    string[count] = '\0';
    return string;
}


