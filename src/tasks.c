#include "tasks.h"
#include <stdlib.h>
#include <stdio.h>



TaskList *init_TaskList(){
    TaskList *task_list = malloc(sizeof(TaskList));
    if (!task_list){
        perror("Could not init memmory: ");
        return NULL;
    }
    task_list->capacity=10;
    task_list->count=0;
    task_list->task=NULL;
    task_list->max_description_len=25;
    task_list->max_title_len=10;
    return task_list;
}


int init_tasks(TaskList *task_list){
    if (!task_list){
        printf("task_list == NULL\n");
        return 0;
    }
    task_list->task = malloc(sizeof(Tasks*)*task_list->capacity);
    task_list->id=1;
    if(!task_list->task){
        perror("Could not init memmory for tsaks: ");
        return 0;
    }
    return 1;
}


int check_capacity(TaskList *task_list){
    if (!task_list){
        perror("Could not get access for memmory: ");
        return 0;
    }
    if (task_list->capacity<=task_list->count){
        Tasks **temp = realloc(task_list->task,sizeof(Tasks*)*task_list->capacity*2);
        if(!temp){
            perror("Could not init new memmory: ");
            return 0;
        }
        task_list->capacity*=2;
        task_list->task=temp;
        return 1;
    }
    return 1;
}






int free_TaskList(TaskList *task_list){
    if (!task_list){
        return 1;
    }
    if (!task_list->task){
        free(task_list);
        task_list=NULL;
        return 1;
    }
    for(int i =0; i <task_list->count;i++){
        free(task_list->task[i]->description);
        free(task_list->task[i]->title);
        free(task_list->task[i]);
        task_list->task[i]=NULL;
    }
    free(task_list->task);
    free(task_list);
    task_list=NULL;
    return 1;
}



int init_task_item(TaskList *task_list){
    if (!task_list){
        printf("task_list == NULL\n");
        return 0;
    }
    check_capacity(task_list);
    task_list->task[task_list->count] = malloc(sizeof(Tasks));
    if(!task_list->task[task_list->count]){
        perror("Could not init memmory for task: ");
        return 0;
    }
    return 1;
}

