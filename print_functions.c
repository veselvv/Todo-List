#include "print_functions.h"
#include "task_api.h"
#include <stdio.h>

void print_task_list(TaskList *task_list){
    char *priority_color;
    char *status_color;
    char *reset = "\033[0m";
    

    printf("┌─────┬");
    for(int i = 0;i<task_list->max_title_len+2; i++) printf("─");
    printf("┬");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┬");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┬");
    for(int i = 0;i<task_list->max_description_len+2; i++) printf("─");
    printf("┐\n");
    printf("│%5s│", "id");
    printf("%*s│", task_list->max_title_len+2, "title");
    printf("%17s│","Status");
    printf("%17s│", "Prioritet");
    printf("%*s│\n", task_list->max_description_len+2, "Description");
    printf("├─────┼");
    for(int i = 0;i<task_list->max_title_len+2; i++) printf("─");
    printf("┼");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┼");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┼");
    for(int i = 0;i<task_list->max_description_len+2; i++) printf("─");
    printf("┤\n");


    for(int i=0;i<task_list->count;i++){
        if(task_list->task[i]!=NULL){
            priority_color = choice_color_prioritet(task_list, i);
            status_color = choice_color_status(task_list, i);
            printf("│%5d│%*s│%s%17s%s│%s%17s%s│%*s│\n",task_list->task[i]->id,task_list->max_title_len+2,task_list->task[i]->title,status_color,task_list->task[i]->output_status,reset,priority_color,task_list->task[i]->output_priority,reset,task_list->max_description_len+2,task_list->task[i]->description);
            
        }
        if(i!=task_list->count-1){
            printf("├─────┼");
             for(int i = 0;i<task_list->max_title_len+2; i++) printf("─");
            printf("┼");
            for(int i=0; i<15+2; i++) printf("─");
            printf("┼");
            for(int i=0; i<15+2; i++) printf("─");
            printf("┼");
            for(int i = 0;i<task_list->max_description_len+2; i++) printf("─");
            printf("┤\n");

            
        }
    }
   printf("└─────┴");
    for(int i = 0;i<task_list->max_title_len+2; i++) printf("─");
    printf("┴");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┴");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┴");
    for(int i = 0;i<task_list->max_description_len+2; i++) printf("─");
    printf("┘\n");
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
