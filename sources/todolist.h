#ifndef TODO_H
#define TODO_H

#define MAX_FILENAME_LENGTH 50
#define MAX_TASK_LENGTH 100
#define MAX_LISTS 10
#define MAX_TASKS 50

void createTodoListAndCount();
void addTaskToSelectedList();
void viewSelectedTodoList();
void completeTask();
void removeTodoList();
void printTodoLists();
int countTodoLists();
void countMarkdownFilesAndStoreNames();

#endif
