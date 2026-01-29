#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "library.h"

void Current_time(char *c_time) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(c_time, "%02d-%02d-%04d",
        tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

int id_check(int id) {
    struct Book b;
    FILE *fp = fopen(DB_FILE, "rb");

    if (!fp) {
        printf("\nFile doesn't exist!\n\n");
        return 0;
    }

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (id == b.id) {
            printf("\nID already exists!\n\n");
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void sort_books() {
    struct Book books[100], temp;
    FILE *fp = fopen(DB_FILE, "rb");
    int count = 0, i, j;

    if (!fp) {
        printf("\nNo books to print!\n\n");
        return;
    }

    while (fread(&books[count], sizeof(struct Book), 1, fp))
        count++;
    fclose(fp);

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (strcmp(books[j].title, books[j + 1].title) > 0) {
                temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }

    for (i = 0; i < count; i++) {
        printf("%-6d %-20s %-20s %-8d\n", 
            books[i].id, books[i].title, books[i].author, books[i].quantity);
    }
}

void add_book() {
    FILE *fp = fopen(DB_FILE, "ab");
    struct Book b;
    int id;

    printf("\nEnter book ID: ");
    scanf("%d", &id);

    while (id_check(id) != 0) {
        printf("Enter book ID: ");
        scanf("%d", &id);
    }
    b.id = id;

    printf("Enter title: ");
    scanf(" %[^\n]", b.title);

    printf("Enter Author: ");
    scanf(" %[^\n]", b.author);

    printf("Enter quantity: ");
    scanf("%d", &b.quantity);

    fwrite(&b, sizeof(struct Book), 1, fp);
    fclose(fp);

    printf("\nBook added successfully!\n\n");
}

void view_books() {
    printf("\n%-6s %-20s %-20s %-8s\n",
           "ID", "Title", "Author", "Quantity");
    printf("-------------------------------------------------\n");

    sort_books();
    printf("\n");
}

void search_book() {
    FILE *fp = fopen(DB_FILE, "rb");
    struct Book b;
    int id, found = 0;

    printf("\nEnter Book ID to search: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (b.id == id) {
            printf("\nFound: %s by %s | Copies: %d\n\n",
                   b.title, b.author, b.quantity);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nBook not found!\n\n");

    fclose(fp);
}

void issue_book() {
    struct Book b;
    struct Issue_Return ir;
    FILE *fp = fopen(DB_FILE, "rb+");
    FILE *fpir = fopen(IR_FILE, "ab");
    int id, found = 0;

    if (!fp) {
        printf("\nFile doesn't exist!\n\n");
        return;
    }

    printf("\nEnter book ID to issue: ");
    scanf("%d", &id);

    printf("Enter your name: ");
    scanf(" %[^\n]", ir.name);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (id == b.id) {
            found = 1;

            if (b.quantity == 0) {
                printf("\nNo copies available!\n\n");
                fclose(fp);
                return;
            }

            ir.id = id;
            strcpy(ir.title, b.title);
            Current_time(ir.issue_date);

            b.quantity--;

            fseek(fp, -sizeof(struct Book), SEEK_CUR);
            fwrite(&b, sizeof(struct Book), 1, fp);

            fwrite(&ir, sizeof(struct Issue_Return), 1, fpir);

            printf("\nBook issued to %s\n\n", ir.name);
            break;
        }
    }

    if (!found)
        printf("\nBook not found!\n\n");
    
    fclose(fp);
    fclose(fpir);
}

void return_book() {
    struct Book b;
    struct Issue_Return ir;
    FILE *fp = fopen(DB_FILE, "rb+");
    FILE *fpir = fopen(IR_FILE, "rb+");
    int id, found = 0;
    char name[30];
    long pos;

    if (!fp) {
        printf("\nFile doesn't exist!\n\n");
        return;
    }

    printf("\nEnter book ID to return: ");
    scanf("%d", &id);

    printf("Enter your name: ");
    scanf(" %[^\n]", name);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (id == b.id) {
            while (fread(&ir, sizeof(struct Issue_Return), 1, fpir)) {
                if (b.id == ir.id && strcmp(name, ir.name) == 0) {
                    Current_time(ir.return_date);

                    pos = ftell(fpir) - sizeof(struct Issue_Return);
                    fseek(fpir, pos, SEEK_SET);
                    fwrite(&ir, sizeof(struct Issue_Return), 1, fpir);
                    
                    b.quantity++;

                    fseek(fp, -sizeof(struct Book), SEEK_CUR);
                    fwrite(&b, sizeof(struct Book), 1, fp);

                    printf("\nBook returned successfully!\n\n");
                    found = 1;
                    break;
                }
            }
            break;
        }
    }

    if (!found)
        printf("\nBook not found!\n\n");
    
    fclose(fp);
    fclose(fpir);
}

void delete_book() {
    FILE *fp = fopen(DB_FILE, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Book b;
    int id, found = 0;

    if (!fp) {
        printf("\nFile error!\n\n");
        return;
    }

    printf("\nEnter Book ID to delete: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (b.id != id)
            fwrite(&b, sizeof(struct Book), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(DB_FILE);
    rename("temp.dat", DB_FILE);

    if (found)
        printf("\nBook deleted!\n\n");
    else
        printf("\nBook not found!\n\n");
}