#include "task_api.h"
#include "dynamic_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX(a,b) a>b?a:b









char  *set_output_priority(TaskList *task_list, int index){
    switch (task_list->task[index]->priority){
        case PRIORITY_LOW:
            return "LOW";
            break;
        case PRIORITY_MEDIUM:
            return  "MEDIUM";
            break;
        case PRIORITY_HIGH:
            return  "HIGH";
            break;
        case PRIORITY_CRITICAL:
            return "CRITICAL";
            break;
        default: 
            break;
    }
}



char *set_output_status(TaskList *task_list, int index){
    switch (task_list->task[index]->status){
        case TODO:
            return "TODO";
            break;
        case IN_PROCESS:
            return "IN PROCESS";
            break;
        case DONE:
            return "DONE";
            break;
        default: 
            break;
    }
}

char *choice_color_status(TaskList *task_list, int index){
    switch (task_list->task[index]->status){
        case TODO:
            return "\033[31m";
        
        case IN_PROCESS:
            return "\033[33m";
            break;
        case DONE:
            return "\033[32m";
        default: 
            break;
    }
}

char *choice_color_prioritet(TaskList *task_list, int index){
    switch (task_list->task[index]->priority){
        case PRIORITY_LOW: return  "\033[32m";
        case PRIORITY_MEDIUM: return "\033[33m";
        case PRIORITY_HIGH:return "\033[31m";
        case PRIORITY_CRITICAL:return "\033[91m";
        default: return  "\033[32m";
    }
}




int change_status(TaskList *task_list){
    if (!task_list){
        printf("invalid data\n");
        return 0;
    }
    if(!task_list->task){
        printf("invalid data\n");
        return 0;
    }
    int choice_id;  
    int choice_status;
    int flag=0;
    int find_index;
    printf("Введите id задачи, статус который хотите поменять\n");
    scanf("%d", &choice_id);
    while (getchar() != '\n'); 
    printf("Введите новый статус выполнения задачи\n"
            "0 - TODO\n"
            "1 - IN_PROCESS\n"
            "2 - DONE\n");
    scanf("%d", &choice_status);
    while (getchar() != '\n');
    for(int i =0; i<task_list->count; i++){
        if(task_list->task[i]!=NULL){
            if(task_list->task[i]->id==choice_id){
                flag=1;
                find_index=i;
                break;
            }
        }
    }
    if(flag){
        task_list->task[find_index]->status=(STATUS)choice_status;
        return 1;
    }else{
        printf("Не найдена задача с таким Id\n");
        return 0;
    }

}





int change_priority(TaskList *task_list){
    if (!task_list){
        printf("invalid data\n");
        return 0;
    }
    if(!task_list->task){
        printf("invalid data\n");
        return 0;
    }
    int choice_id;  
    int choice_priority;
    int flag=0;
    int find_index;
    printf("Введите id задачи, статус который хотите поменять\n");
    scanf("%d", &choice_id);
    while (getchar() != '\n'); 
     printf("Введите новую степень важности дела\n"
            "0 - PRIORITY_LOW\n"
            "1 - PRIORITY_MEDIUM\n"
            "2 - PRIORITY_HIGH\n"
            "3 - PRIORITY_CRITICAL\n");
    scanf("%d", &choice_priority);
    while (getchar() != '\n');
    for(int i =0; i<task_list->count; i++){
        if(task_list->task[i]!=NULL){
            if(task_list->task[i]->id==choice_id){
                flag=1;
                find_index=i;
                break;
            }
        }
    }
    if(flag){
        task_list->task[find_index]->priority=(PRIORITY)choice_priority;
        return 1;
    }else{
        printf("Не найдена задача с таким Id\n");
        return 0;
    }

}



int change_description(TaskList *task_list){
    if(!task_list){
        printf("invalid data\n");
        return 0;
    }
    if(!task_list->task){
        printf("invalid daat\n");
        return 0;
    }
    int flag=0;
    int find_index;
    int choice_id;
    printf("Введите Id задачи, описание которой вы хотите поменять\n");
    scanf("%d", &choice_id);
    while (getchar() != '\n');
    for(int i =0; i<task_list->count; i++){
        if(task_list->task[i]!=NULL){
            if(task_list->task[i]->id==choice_id){
                flag=1;
                find_index=i;
                break;
            }
        }
    }
    if(flag){
        printf("Текущее описание:\n");
        printf("%s\n", task_list->task[find_index]->description);
        free(task_list->task[find_index]->description);
        printf("Введите новое описание:\n");
        task_list->task[find_index]->description = mgetline();
        if(!task_list->task[find_index]->description){
            printf("Не удалось установить новое описание\n");
            return 0;
        }
        if(MAX(task_list->max_description_len, strlen(task_list->task[find_index]->description))>50){
            task_list->max_description_len = 50;
        }else{
            task_list->max_description_len = MAX(task_list->max_description_len, strlen(task_list->task[find_index]->description));
        }

        return 1;
    }else{
        printf("Не найдена задача с таким Id\n");
        return 0;
    }
}










int append_task(TaskList *task_list){
    if(!task_list){
        perror("Err: ");
        return 0;
    }
    if(!task_list->task){
        if(!init_tasks(task_list)){
            perror("Err: ");
            return 0;
        }
    }
    if(!init_task_item(task_list)){
        perror("Err: ");
        return 0;
    }
    task_list->task[task_list->count]->id = task_list->id++;
    printf("Введите название задачи: \n");
    task_list->task[task_list->count]->title = mgetline();
    if(!task_list->task[task_list->count]->title){
        printf("Ошибка при создании имени\n");
        return 0;
    }
    printf("Введите описание задачи: \n");
    task_list->task[task_list->count]->description = mgetline();
    printf("Введите степень важности дела\n"
            "0 - PRIORITY_LOW\n"
            "1 - PRIORITY_MEDIUM\n"
            "2 - PRIORITY_HIGH\n"
            "3 - PRIORITY_CRITICAL\n");
    int temp;
    scanf("%d", &temp);
    while (getchar() != '\n'); 
    task_list->task[task_list->count]->priority = (PRIORITY)temp;
    printf("Введите статус выполнения задачи\n"
            "0 - TODO\n"
            "1 - IN_PROCESS\n"
            "2 - DONE\n");
    scanf("%d", &temp);
    while (getchar() != '\n');
    task_list->task[task_list->count]->status = (STATUS)temp;
    task_list->max_title_len = MAX(task_list->max_title_len, strlen(task_list->task[task_list->count]->title));
    if(MAX(task_list->max_description_len, strlen(task_list->task[task_list->count]->description))>50){
            task_list->max_description_len = 50;
        }else{
            task_list->max_description_len = MAX(task_list->max_description_len, strlen(task_list->task[task_list->count]->description));
        }
    task_list->count++;
    printf("Задача успешно добавлена!\n");
    return 1;
}






int delete_task_from_task_list(TaskList *task_list){
    if (!task_list){
        printf("invalid task list\n");
        return 0;
    }
    if (!task_list->task){
        printf("Invalid task list\n");
        return 0;
    }
    printf("Введите Id для удаления задачи\n");
    int choice_id;
    int flag=0;
    int find_index;
    scanf("%d", &choice_id);
    while (getchar() != '\n');
    for(int i =0; i<task_list->count; i++){
        if(task_list->task[i]!=NULL){
            if(task_list->task[i]->id==choice_id){
                flag=1;
                find_index=i;
                free(task_list->task[i]->title);
                free(task_list->task[i]->description);
                free(task_list->task[i]);
                task_list->task[i]=NULL;
                break;
            }
        }
    }
    if(flag){
        for(int i = find_index; i<task_list->count-1;i++){
            task_list->task[i] = task_list->task[i+1];
            
        }
        task_list->task[task_list->count-1] = NULL;
        task_list->count-=1;
        return 1;

    }else{
        printf("Задача с таким id не найдена\n");
        return 0;
    }
}


