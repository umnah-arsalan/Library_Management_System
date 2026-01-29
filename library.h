#ifndef LIBRARY_H
#define LIBRARY_H

#define DB_FILE "library.dat"
#define IR_FILE "issue_return.dat"

struct Book {
    int id;
    char title[30];
    char author[30];
    int quantity;
};

struct Issue_Return {
    int id;
    char title[30];
    char name[30];
    char issue_date[20];
    char return_date[20];
};

void Current_time(char *c_time);
int id_check(int id);
void sort_books(void);
void add_book(void);
void view_books(void);
void search_book(void);
void issue_book(void);
void return_book(void);
void delete_book(void);

#endif