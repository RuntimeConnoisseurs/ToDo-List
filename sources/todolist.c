#include "todolist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_FILENAME_LENGTH 50
#define MAX_TASK_LENGTH 100
#define MAX_LISTS 10
#define MAX_TASKS 50

// Creates a new todo list with a given user inputted filename
static void createTodoList(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to create file.\n");
        exit(1);
    }
    fprintf(file, "# Todo List - %s\n\n", filename); // Write a header to the file indicating it as a todo list
    fclose(file);
}

static void addTaskToTodoList(const char *filename) { // Adds tasks to a specified todo list file.
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    char task[MAX_TASK_LENGTH];
    char choice;

    int task_count = 0;

    do {
        if (task_count >= MAX_TASKS) {
            printf("Maximum number of tasks reached for this todo list.\n");
            break;
        }

        printf("Enter task: ");
        fgets(task, MAX_TASK_LENGTH, stdin);
        fprintf(file, "- [ ] %s", task); // Markdown checkbox for an unchecked task

        printf("Do you want to add another task? (y/n): ");
        scanf(" %c", &choice); // The space before %c is to consume the newline character
        getchar(); // Consume newline character left by scanf

        task_count++;
    } while (choice == 'y' || choice == 'Y');

    fclose(file);
}

static void viewTodoList(const char *filename) { // view contents of a todo list
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    printf("Contents of %s:\n", filename);
    char buffer[MAX_TASK_LENGTH]; // stores each line of the file
    while (fgets(buffer, MAX_TASK_LENGTH, file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
}

void countMarkdownFilesAndStoreNames() { // count markdown files in the current directory and store the names in a separate file
    DIR *dir;
    struct dirent *ent; 
    int count = 0;
    char filename[MAX_FILENAME_LENGTH];

    if ((dir = opendir(".")) != NULL) {
        FILE *file = fopen("todo_lists.txt", "w");
        if (file == NULL) {
            printf("Error: Unable to create file.\n");
            exit(1);
        }

        printf("List of todo lists saved in 'todo_lists.txt'.\n");
        while ((ent = readdir(dir)) != NULL) {
            
            if (strstr(ent->d_name, ".md") != NULL) {
                fprintf(file, "%s\n", ent->d_name); // writing gile name to todo_lists.txt
                printf("%s\n", ent->d_name); // printing file names to console
                count++;
            }
        }

        closedir(dir); 
        fclose(file);

        printf("Number of todo lists: %d\n", count);
    } else {
        printf("Error: Unable to open directory.\n");
        exit(1);
    }
}

int countTodoLists() { // count the number of list in todo_lists.txt
    FILE *file = fopen("todo_lists.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open todo lists file.\n");
        exit(1);
    }

    int count = 0;
    char line[MAX_FILENAME_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        count++;
    }

    fclose(file);
    return count;
}

void printTodoLists() { // function to print names of todo lists
    FILE *file = fopen("todo_lists.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open todo lists file.\n");
        exit(1);
    }

    printf("Select a Todo List:\n");
    int count = 0;
    char line[MAX_FILENAME_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove the file extension ".md" from the todo list name
        char *pos = strstr(line, ".md");
        if (pos != NULL) {
            *pos = '\0'; // Replace the dot(extension) with a null terminator
        }
        printf("%d. %s\n", ++count, line);
    }

    fclose(file);
}


void createTodoListAndCount() { // creating a new todolist and accepting a user input
    char filename[MAX_FILENAME_LENGTH];
    printf("Enter the name for your todo list (without extension): ");
    fgets(filename, MAX_FILENAME_LENGTH, stdin);
    filename[strcspn(filename, "\n")] = '\0'; // Remove newline character
    strcat(filename, ".md"); // Append .md extension
    createTodoList(filename);
    countMarkdownFilesAndStoreNames(); // Update todo_lists.txt
}

void addTaskToSelectedList() { // add tasks to selected todolist 
    char input[MAX_FILENAME_LENGTH];
    printTodoLists();
    printf("Enter the name of the Todo List (without extension): ");
    scanf("%s", &input);
    getchar(); // Consume newline character left by scanf
    char filename[MAX_FILENAME_LENGTH];
    snprintf(filename, MAX_FILENAME_LENGTH, "%s.md", input); // Format the filename provided by the user with extension 

    addTaskToTodoList(filename);
}

void viewSelectedTodoList() {
    char input[MAX_FILENAME_LENGTH];
    printTodoLists();
    printf("Enter the name of the Todo List (without extension): ");
    scanf("%s", &input);
    getchar(); // Consume newline character left by scanf
    char filename[MAX_FILENAME_LENGTH];
    snprintf(filename, MAX_FILENAME_LENGTH, "%s.md", input); // Format the filename provided by the user with extension 
    viewTodoList(filename);
}


void completeTask() { // Mark tasks as complete in the selected todo list.
    char input[MAX_FILENAME_LENGTH];
    printTodoLists();
    printf("Enter the name of the Todo List (without extension): ");
    scanf("%s", &input); 
    getchar(); // Consume newline character left by scanf
    char filename[MAX_FILENAME_LENGTH];
    snprintf(filename, MAX_FILENAME_LENGTH, "%s.md", input);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }

    // Create a temporary file to store the updated content
    FILE *temp_file = fopen("templist.txt", "w");
    if (temp_file == NULL) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    printf("Contents of %s:\n", filename); 
    char buffer[MAX_TASK_LENGTH];
    int count = 0;

    // Copy only the lines containing tasks to the temporary file
    while (fgets(buffer, MAX_TASK_LENGTH, file) != NULL) {
        if (strstr(buffer, "- [X]") != NULL) { // Checks if task is already completed
            // Prints a message indicating that the task is already completed
            printf("%d. %s task is completed\n", ++count, buffer + 6); // Skip the "- [X] " prefix
            fprintf(temp_file, "%s", buffer); // Write the line to the temporary file line remains unchanged
            continue; // Skip further processing of this line
        } else if (strstr(buffer, "- [ ]") != NULL) { // Check if the line contains a task
            printf("%d. %s", ++count, buffer); // Print the task

            // Asks user to mark a task as complete/incomplete
            printf("Do you want to mark this task as complete? (y/n): ");
            char choice;
            scanf(" %c", &choice);
            getchar(); // Consume newline character left by scanf

            // If user chooses to mark task as complete, modify the line
            if (choice == 'y' || choice == 'Y') {
                buffer[3] = 'X'; // Mark the checkbox as [X]
            }
        }

        // Write the line to the temporary file
        fprintf(temp_file, "%s", buffer);
    }

    fclose(file);
    fclose(temp_file);

    // Copy content from temporary file back to original file
    file = fopen(filename, "w");
    temp_file = fopen("templist.txt", "r");
    if (file == NULL || temp_file == NULL) {
        printf("Error: Unable to open files for writing.\n");
        return;
    }

    while (fgets(buffer, MAX_TASK_LENGTH, temp_file) != NULL) {
        fprintf(file, "%s", buffer);
    }

    fclose(file);
    fclose(temp_file);

    // Delete the temporary file
    remove("templist.txt");
    printf("The list now is: ");
    viewTodoList(filename);// prints content of the list
    printf("Task marked as complete and saved to %s.\n", filename);
}

void removeTodoList() // function to remove a todo list
{   char input[MAX_FILENAME_LENGTH];
    printTodoLists();
    printf("Enter the name of the Todo List (without extension): ");
    scanf("%s", &input);
    getchar(); // Consume newline character left by scanf
    char filename[MAX_FILENAME_LENGTH];
    snprintf(filename, MAX_FILENAME_LENGTH, "%s.md", input); // appending file extension to user input

    if (remove(filename) == 0) { // deletes todo list file
        printf("Todo list '%s' successfully deleted.\n", filename);
        FILE *todo_list_file = fopen("todo_lists.txt", "r"); // Update todo_lists.txt
        if (todo_list_file == NULL) {
            printf("Error: Unable to open todo lists file.\n");
            return;
        }

        // Copy contents to a temporary list, excluding the deleted todo list filename
        char temp_lists[MAX_TASK_LENGTH] = "";
        char line[MAX_TASK_LENGTH];
        while (fgets(line, sizeof(line), todo_list_file) != NULL) {
            //  Read each line from 'todo_lists.txt' and append to temporary buffer (if it does not match the deleted todo list).
            if (strstr(line, input) == NULL) {
                strcat(temp_lists, line);
            }
        }

        fclose(todo_list_file);

        // open todo_lists.txt in write mode
        todo_list_file = fopen("todo_lists.txt", "w");
        if (todo_list_file == NULL) {
            printf("Error: Unable to open todo lists file for writing.\n");
            return;
        }

        fputs(temp_lists, todo_list_file); // Write the updated list to todo_lists.txt
        fclose(todo_list_file);

        printf("Todo list '%s' removed from 'todo_lists.txt'.\n", filename);

    } else {
        printf("Error: Unable to delete todo list '%s'.\n", filename);
    }
}

// :)