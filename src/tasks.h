#ifndef TASKS_H
#define TASKS_H
#include <time.h>
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
    time_t target_date;
    time_t created_date;
    int id;
    STATUS status;
    PRIORITY priority;
    char *description;
    char *title;
}Tasks;



typedef struct{
    int capacity;
    int count;
    Tasks **task;
    int max_description_len;
    int max_title_len;
    int id;
}TaskList;

TaskList *init_TaskList();
int check_capacity(TaskList *task_list);
int init_tasks(TaskList *task_list);
int free_TaskList(TaskList *task_list);
int init_task_item(TaskList *task_list);



#endif