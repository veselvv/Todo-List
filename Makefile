all:
	gcc ./src/dynamic_string.c ./src/print_functions.c ./src/task_api.c ./src/tasks.c ./src/todo.c -o ./build/todo
