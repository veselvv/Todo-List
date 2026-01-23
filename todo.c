#include <stdio.h>
#include <stdlib.h>
#include <string.h>




typedef enum{
    PRIORITY_LOW,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH,
    PRIORITY_CRITICAL
}PRIORITY;


typedef enum{
    TODO,
    IN_PROCESS,
    DONE
}STATUS;


typedef struct{
    STATUS status;
    PRIORITY priority;
    int id;
    char description[500];
    char title[100];
    char output_status[25];
    char output_priority[25];

}Tasks;



typedef struct{
    int capacity;
    int count;
    Tasks **task;
}TaskList;



TaskList *init_TaskList(){
    TaskList *task_list = malloc(sizeof(TaskList));
    if (!task_list){
        perror("Could not init memmory: ");
        return NULL;
    }
    task_list->capacity=10;
    task_list->count=0;
    task_list->task=NULL;
    return task_list;
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
int init_tasks(TaskList *task_list){
    if (!task_list){
        printf("task_list == NULL\n");
        return 0;
    }
    task_list->task = malloc(sizeof(Tasks*)*task_list->capacity);

    if(!task_list->task){
        perror("Could not init memmory for tsaks: ");
        return 0;
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



void set_output_priority(TaskList *task_list, int index){
    switch (task_list->task[index]->priority){
        case PRIORITY_LOW:
            strcpy(task_list->task[index]->output_priority, "LOW");
            break;
        case PRIORITY_MEDIUM:
            strcpy(task_list->task[index]->output_priority, "MEDIUM");
            break;
        case PRIORITY_HIGH:
            strcpy(task_list->task[index]->output_priority, "HIGH");
            break;
        case PRIORITY_CRITICAL:
            strcpy(task_list->task[index]->output_priority, "CRITICAL");
            break;
        default: 
            break;
    }
}



void set_output_status(TaskList *task_list, int index){
    switch (task_list->task[index]->status){
        case TODO:
            strcpy(task_list->task[index]->output_status, "TODO");
            break;
        case IN_PROCESS:
            strcpy(task_list->task[index]->output_status, "IN PROCESS");
            break;
        case DONE:
            strcpy(task_list->task[index]->output_status, "DONE");
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
        set_output_status(task_list, find_index);
        return 1;
    }else{
        printf("Не найдена задача с таким Id\n");
        return 0;
    }

}


int append_task(TaskList *task_list){
    static int id_var = 1;
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
    task_list->task[task_list->count]->id = id_var++;
    char buffer[500];
    printf("Выедите название задачи: ");
    scanf("%s", buffer);
    while (getchar() != '\n'); 
    
    strncpy(task_list->task[task_list->count]->title, buffer, 100);
    printf("Введите описание задачи: ");
    
    scanf("%s", buffer);
    while (getchar() != '\n'); 
    strncpy(task_list->task[task_list->count]->description, buffer, 500);

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
    set_output_priority(task_list, task_list->count);
    set_output_status(task_list, task_list->count);
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


void print_task_list(TaskList *task_list){
    char *priority_color;
    char *status_color;
    char *reset = "\033[0m";
    
    printf("┌─────┬──────────┬───────────────┬───────────────┬─────────────────────────────┐\n");
    printf("│%5s│%10s│%15s│%15s│%29s│\n","Id","Title","Status", "Prioritet","Description");
    printf("├─────┼──────────┼───────────────┼───────────────┼─────────────────────────────┤\n");

    for(int i=0;i<task_list->count;i++){
        if(task_list->task[i]!=NULL){
            priority_color = choice_color_prioritet(task_list, i);
            status_color = choice_color_status(task_list, i);
            printf("│%5d│%10s│%s%15s%s│%s%15s%s│%29s│\n",task_list->task[i]->id,task_list->task[i]->title,status_color,task_list->task[i]->output_status,reset,priority_color,task_list->task[i]->output_priority,reset,task_list->task[i]->description);
            
        }
        if(i!=task_list->count-1){
            printf("├─────┼──────────┼───────────────┼───────────────┼─────────────────────────────┤\n");
        }
    }
    printf("└─────┴──────────┴───────────────┴───────────────┴─────────────────────────────┘\n");
    int done=0;
    int in_process=0;
    int todo=0;
    for(int i=0;i<task_list->count;i++){
        switch (task_list->task[i]->status)
        {
        case DONE: 
            done+=1;
            break;
        case IN_PROCESS:
            in_process+=1;
            break;
        case TODO:
            todo+=1;
            break;
        default:
            break;
        }
    }
    printf("Всего задач: %d\tВыполнено:%d\tВ процессе:%d\tНадо сделать: %d\n",task_list->count,done, in_process,todo);
}
int wrapper_print_task_list(TaskList *task_list){
    print_task_list(task_list);
    return 1;
}

void print_banner(){
    printf("\033[0;32m");
    char *s = "\n"
              "████████╗ ██████╗ ██████╗  ██████╗ ██╗     ██╗███████╗████████╗\n"
              "╚══██╔══╝██╔═══██╗██╔══██╗██╔═══██╗██║     ██║██╔════╝╚══██╔══╝\n"
              "   ██║   ██║   ██║██║  ██║██║   ██║██║     ██║███████╗   ██║\n"   
              "   ██║   ██║   ██║██║  ██║██║   ██║██║     ██║╚════██║   ██║\n"   
              "   ██║   ╚██████╔╝██████╔╝╚██████╔╝███████╗██║███████║   ██║\n"   
              "   ╚═╝    ╚═════╝ ╚═════╝  ╚═════╝ ╚══════╝╚═╝╚══════╝   ╚═╝\n"
            "\n";
    printf("%s", s);
    printf("\033[0m");
}



void main_prog_cycle(){
    int running = 1;
    int choice;
    char contin;
    TaskList *task_list=init_TaskList();
    if(!task_list){
        perror("Err: ");
        return;
    };
    int(*functions[])(TaskList *)={wrapper_print_task_list,append_task,delete_task_from_task_list,change_status};
    while(running){
        system("clear");
        print_banner();
        printf("╔═══════════════════════════╗\n"
            "║Что вы хотите сделать?:\n"
                "║0 - Вывести список дел\n"
                "║1 - Добавить задачу\n"
                "║2 - удалить задачу по id\n"
                "║3 - изменить статус задачи\n"
                "║4 - выйти\n");
                scanf("%d", &choice);
                while (getchar() != '\n');  
        while (!(choice>=0&&choice<=4)){
            system("clear");
            print_banner();
            printf("Ошибка выбора!!!\n");
            printf("Выберите номер из перечня:\n"
                "1 - Вывести список дел\n"
                "2 - Добавить задачу\n"
                "3 - удалить задачу по id\n"
                "4 - изменить статус задачи\n"
                "0 - выйти\n");
                scanf("%d", &choice);
                while (getchar() != '\n');
        }
        system("clear");
        switch (choice){
        case 4:
            running=0;
            break;
        default:
            print_banner();
            functions[choice](task_list);
            printf("Для выхода в меню менеджера задач нажмите <Enter>\n");
            while (getchar() != '\n'); 
        }
    }
}




int main(void){
    main_prog_cycle();
    return 0;
}