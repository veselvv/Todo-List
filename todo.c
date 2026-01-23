#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasks.h"
#include "task_api.h"
#include "print_functions.h"
#define MAX(a,b) a>b?a:b






void main_prog_cycle(){
    int running = 1;
    int choice;
    char contin;
    TaskList *task_list=init_TaskList();
    if(!task_list){
        perror("Err: ");
        return;
    };
    int(*functions[])(TaskList *)={wrapper_print_task_list,append_task,delete_task_from_task_list,change_status,change_priority,
    change_description};
    while(running){
        system("clear");
        print_banner();
        printf("╔════════════════════════════════════════════════╗\n");
        printf(
            "║%-48s║\n"
            "║%-48s║\n"
            "║%-48s║\n"
            "║%-48s║\n"
            "║%-48s║\n"
            "║%-48s║\n"
            "║%-48s║\n"
            "║%-48s║\n",
            "TASK MANAGER",
                " 0 - Print todo list",
                " 1 - Add task",
                " 2 - Deleate task by id",
                " 3 - Change task status",
                " 4 - Change task piority",
                " 5 - Change description by id",
                " 6 - Exit");
        printf("╚════════════════════════════════════════════════╝\n");
        
                scanf("%d", &choice);
                while (getchar() != '\n');  
        while (!(choice>=0&&choice<=6)){
            system("clear");
            print_banner();
            printf("Ошибка выбора!!!\n");
            printf("Выберите номер из перечня:\n"
                "║0 - Вывести список дел\n"
                "║1 - Добавить задачу\n"
                "║2 - удалить задачу по id\n"
                "║3 - изменить статус задачи\n"
                "║4 - изменить приоритет задачи\n"
                "║5 - выйти\n");
                scanf("%d", &choice);
                while (getchar() != '\n');
        }
        system("clear");
        switch (choice){
        case 6:
            running=0;
            break;
        default:
            print_banner();
            functions[choice](task_list);
            printf("Для выхода в меню менеджера задач нажмите <Enter>\n");
            while (getchar() != '\n'); 
        }
    }
    free_TaskList(task_list);
}




int main(void){
    main_prog_cycle();
    return 0;
}