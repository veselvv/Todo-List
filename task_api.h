#ifndef TASK_API
#define TASK_API
#include "tasks.h"


void set_output_priority(TaskList *task_list, int index);
void set_output_status(TaskList *task_list, int index);
char *choice_color_status(TaskList *task_list, int index);
char *choice_color_prioritet(TaskList *task_list, int index);
int change_status(TaskList *task_list);
int change_priority(TaskList *task_list);
int change_description(TaskList *task_list);
int append_task(TaskList *task_list);
int delete_task_from_task_list(TaskList *task_list);

#endif