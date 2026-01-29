#include<stdio.h>
#include<stdlib.h>
#include "library.h"

int main() {
    int choice;

    while (1) {
        printf("--- Library Management System ---\n\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Exit\n\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_book(); break;
            case 2: view_books(); break;
            case 3: search_book(); break;
            case 4: issue_book(); break;
            case 5: return_book(); break;
            case 6: delete_book(); break;
            case 7: exit(0);
            default: printf("\nInvalid choice!\n\n");
        }
    }
}