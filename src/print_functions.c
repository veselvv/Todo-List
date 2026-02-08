#include "print_functions.h"
#include "task_api.h"
#include <stdio.h>
#include <string.h>
#include<time.h>
void print_task_list(TaskList *task_list){
    char *priority_color;
    char *status_color;
    char *buffer_status;
    char *buffer_priority;
    char *reset = "\033[0m";
    

    printf("┌─────┬");
    for(int i = 0;i<task_list->max_title_len+2; i++) printf("─");
    printf("┬");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┬");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┬");
    for(int i=0; i<25+2; i++) printf("─");
    printf("┬");
    for(int i = 0;i<task_list->max_description_len+2; i++) printf("─");
    printf("┐\n");
    printf("│%5s│", "id");
    printf("%*s│", task_list->max_title_len+2, "title");
    printf("%17s│","Status");
    printf("%17s│", "Prioritet");
    printf("%27s|", "Deadline");
    printf("%*s│\n", task_list->max_description_len+2, "Description");
    printf("├─────┼");
    for(int i = 0;i<task_list->max_title_len+2; i++) printf("─");
    printf("┼");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┼");
    for(int i=0; i<15+2; i++) printf("─");
    printf("┼");
    for(int i=0; i<25+2; i++) printf("─");
    printf("┼");
    for(int i = 0;i<task_list->max_description_len+2; i++) printf("─");
    printf("┤\n");
   for(int i=0;i<task_list->count;i++){
        int count=0;
        if(task_list->task[i]!=NULL){
            priority_color = choice_color_prioritet(task_list, i);
            status_color = choice_color_status(task_list, i);
            buffer_priority = set_output_priority(task_list,i);
            buffer_status = set_output_status(task_list, i);
            struct tm *expiry_time = localtime(&task_list->task[i]->target_date);
            char buffer_deadline[27];
            sprintf(buffer_deadline, "%d-%d-%d %d:%d:%d",expiry_time->tm_year+1900,expiry_time->tm_mon+1,expiry_time->tm_mday, expiry_time->tm_hour, expiry_time->tm_min,expiry_time->tm_sec);
            printf("│%5d│%*s│%s%17s%s│%s%17s%s│%27s|",task_list->task[i]->id,task_list->max_title_len+2,task_list->task[i]->title,status_color,buffer_status,reset,priority_color,buffer_priority,reset,buffer_deadline);
            if(strlen(task_list->task[i]->description)<40){
                printf("%*s│\n",task_list->max_description_len+2,task_list->task[i]->description);
                if(i!=task_list->count-1){
                    printf("├─────┼");
                    for(int i = 0;i<task_list->max_title_len+2; i++) printf("─");
                    printf("┼");
                    for(int i=0; i<15+2; i++) printf("─");
                    printf("┼");
                    for(int i=0; i<15+2; i++) printf("─");
                    printf("┼");
                     for(int i=0; i<25+2; i++) printf("─");
                    printf("┼");
                    for(int i = 0;i<task_list->max_description_len+2; i++) printf("─");
                    printf("┤\n");
                }
                continue;

            }
            char *ptr = task_list->task[i]->description;
            int temp = strlen(ptr);
            for(int j = 0; j<=strlen(task_list->task[i]->description)/42 ;j++){
                if(j==0){
                    for(int k = 0;k<42;k++){
                        if(*ptr!='\n'&&*ptr!='\0'){
                            printf("%c", *ptr++);

                        }
                    
                    }
                    printf("│\n");
                    count=0;
                }
                else{
                    printf("│%5s│%*s│%17s│%17s│%27s|","",task_list->max_title_len+2,"","","","");
                    for(int k = 0;k<42;k++){
                        if(*ptr!='\n'&&*ptr!='\0'){
                            printf("%c", *ptr++);
                            count++;
                        }
                    }
                    if(j<strlen(task_list->task[i]->description)/42){
                        printf("│\n");
                        count =0;
                    }
                }
            }

            printf("%*s│\n", 42-count, "");
        }
        if(i!=task_list->count-1){
            printf("├─────┼");
             for(int i = 0;i<task_list->max_title_len+2; i++) printf("─");
            printf("┼");
            for(int i=0; i<15+2; i++) printf("─");
            printf("┼");
            for(int i=0; i<15+2; i++) printf("─");
            printf("┼");
            for(int i=0; i<25+2; i++) printf("─");
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
    for(int i=0; i<25+2; i++) printf("─");
    printf("┴");
    for(int i = 0;i<task_list->max_description_len+2; i++) printf("─");
    printf("┘\n");
    
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
