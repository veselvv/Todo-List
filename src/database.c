#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasks.h"
#include "database.h"



PGconn *connection_to_database(){
    const char *conninfo="host=localhost password=123 user=postgres port=5433 dbname=tasks";
    PGconn *conn = PQconnectdb(conninfo);
    if(PQstatus(conn)!=CONNECTION_OK){
        printf("не удалось подключиться к базе данных\n");
        PQfinish(conn);
        return NULL;
    }
    printf("Подключение успешно!\n");
    return conn;
}




int load_tasks_from_database(TaskList *task_list, PGconn *conn){
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
    const char *query = "SELECT task_id, task_status, task_priority, task_description, task_title FROM tasks ORDER BY task_id";
    PGresult *res = PQexec(conn, query);
    if(PQresultStatus(res)!=PGRES_TUPLES_OK){
        fprintf(stderr, "Query err: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return 0;
    }
    int rows = PQntuples(res);
    int cols = PQnfields(res);

    for(int i = 0; i<rows; i++){
        if(!init_task_item(task_list)){  
            perror("Failed to init task: ");
            free_TaskList(task_list);
            PQclear(res);
            return 0;
        }
        char *task_id = PQgetvalue(res,i,0);
        char *task_status = PQgetvalue(res,i,1);
        char *task_priority = PQgetvalue(res,i,2);
        char *task_description = PQgetvalue(res,i,3);
        char *task_title = PQgetvalue(res,i,4);
        task_list->task[task_list->count]->id = atoi(task_id);
        task_list->task[task_list->count]->status = (STATUS)atoi(task_status);
        task_list->task[task_list->count]->priority = (PRIORITY)atoi(task_priority);
        task_list->task[task_list->count]->description = malloc(sizeof(char)*strlen(task_description)+1);
        if(!task_list->task[task_list->count]->description){
            perror("Could not init memmory: ");
            PQclear(res);
            free_TaskList(task_list);
            return 0;
        }
        task_list->task[task_list->count]->title = malloc(sizeof(char)*strlen(task_title)+1);
        if(!task_list->task[task_list->count]->title){
            perror("Could not init memmory: ");
            PQclear(res);
            free(task_list->task[task_list->count]->description);
            free_TaskList(task_list);
            return 0;
        }
        strncpy(task_list->task[task_list->count]->description, task_description, strlen(task_description)+1);
        strncpy(task_list->task[task_list->count]->title,  task_title, strlen(task_title)+1);
        task_list->max_title_len = MAX(task_list->max_title_len, strlen(task_list->task[task_list->count]->title));
        if(MAX(task_list->max_description_len, strlen(task_list->task[task_list->count]->description))>50){
            task_list->max_description_len = 50;
        }else{
            task_list->max_description_len = MAX(task_list->max_description_len, strlen(task_list->task[task_list->count]->description));
        }
        task_list->count++;
        task_list->id++;
    }
    PQclear(res);
    return task_list->count;
}



int save_tasks_to_database(TaskList *task_list, PGconn *conn) {
    if (!task_list || !conn) {
        fprintf(stderr, "Ошибка: неверные аргументы\n");
        return 0;
    }
    
    printf("Сохранение %d задач в БД...\n", task_list->count);
    
    // 1. Начинаем транзакцию
    PGresult *res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Ошибка начала транзакции: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return 0;
    }
    PQclear(res);
    
    // 2. Очищаем таблицу (полная перезапись)
    res = PQexec(conn, "TRUNCATE TABLE tasks RESTART IDENTITY");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Ошибка очистки таблицы: %s\n", PQerrorMessage(conn));
        PQexec(conn, "ROLLBACK");
        PQclear(res);
        return 0;
    }
    PQclear(res);
    
    // 3. Вставляем все задачи
    int saved_count = 0;
    for (int i = 0; i < task_list->count; i++) {
        Tasks *task = task_list->task[i];
        if (!task) continue;
        
        // Экранируем строки для безопасности
        char *escaped_title = NULL;
        char *escaped_desc = NULL;
        
        // Экранирование заголовка
        if (task->title) {
            escaped_title = malloc(strlen(task->title) * 2 + 1);
            if (!escaped_title) {
                fprintf(stderr, "Ошибка выделения памяти для заголовка\n");
                goto rollback;
            }
            PQescapeStringConn(conn, escaped_title, task->title, 
                              strlen(task->title), NULL);
        } else {
            // Если заголовок NULL - ошибка
            fprintf(stderr, "Ошибка: задача %d без заголовка\n", i);
            goto rollback;
        }
        
        // Экранирование описания (может быть NULL)
        if (task->description && strlen(task->description) > 0) {
            escaped_desc = malloc(strlen(task->description) * 2 + 1);
            if (!escaped_desc) {
                fprintf(stderr, "Ошибка выделения памяти для описания\n");
                free(escaped_title);
                goto rollback;
            }
            PQescapeStringConn(conn, escaped_desc, task->description,
                              strlen(task->description), NULL);
        }
        
        // Формируем SQL запрос
        char query[4096];
        if (escaped_desc) {
            snprintf(query, sizeof(query),
                "INSERT INTO tasks (task_status, task_priority, task_description, task_title) "
                "VALUES (%d, %d, '%s', '%s')",
                task->status,
                task->priority,
                escaped_desc,
                escaped_title);
        } else {
            snprintf(query, sizeof(query),
                "INSERT INTO tasks (task_status, task_priority, task_title) "
                "VALUES (%d, %d, '%s')",
                task->status,
                task->priority,
                escaped_title);
        }
        
        // Выполняем запрос
        res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Ошибка вставки задачи %d: %s\n", i, PQerrorMessage(conn));
            fprintf(stderr, "Запрос: %s\n", query);
            free(escaped_title);
            if (escaped_desc) free(escaped_desc);
            PQclear(res);
            goto rollback;
        }
        PQclear(res);
        
        // Освобождаем временные буферы
        free(escaped_title);
        if (escaped_desc) free(escaped_desc);
        
        saved_count++;
    }
    
    // 4. Завершаем транзакцию
    res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Ошибка завершения транзакции: %s\n", PQerrorMessage(conn));
        PQexec(conn, "ROLLBACK");
        PQclear(res);
        return 0;
    }
    PQclear(res);
    
    printf("Успешно сохранено %d/%d задач\n", saved_count, task_list->count);
    return 1;

rollback:
    PQexec(conn, "ROLLBACK");
    return 0;
}