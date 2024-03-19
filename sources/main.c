#include "todolist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int num_lists = countTodoLists();

    if (num_lists >= MAX_LISTS) {
        printf("Maximum number of todo lists reached. Cannot create more.\n");
        return 1;
    }

    int choice;
    do {
        printf("\nChoose an option:\n");
        printf("1. Create a New Todo List\n");
        printf("2. Add Task\n");
        printf("3. View Todo List\n");
        printf("4. Check how many ToDo Lists are pending \n");
        printf("5. Edit Task\n");
        printf("6. Delete ToDo List\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character left by scanf

        switch (choice) {
            case 1:
                createTodoListAndCount();
                break;
            case 2:
                addTaskToSelectedList();
                break;
            case 3:
                viewSelectedTodoList();
                break;
            case 4:
                countMarkdownFilesAndStoreNames();
                break;
            case 5:
                completeTask();    
                break;
            case 6:
                removeTodoList();
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}

// gcc main.c todolist.c -o main
