#ifndef DATABASE_H
#define DATABASE_H
#include "tasks.h"
#include <libpq-fe.h>
#define MAX(a,b) a>b?a:b


int save_tasks_to_database(TaskList *task_list, PGconn *conn);
PGconn *connection_to_database();
int load_tasks_from_database(TaskList *task_list, PGconn *conn);




#endif