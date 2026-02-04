all:
	gcc -I/usr/include/postgresql ./src/database.c ./src/dynamic_string.c ./src/print_functions.c ./src/task_api.c ./src/tasks.c ./src/todo.c -lpq -o ./build/todo
