/* COMAN ROBERT - 313CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef __STRUCTURES__
#define __STRUCTURES__

#define MAX_COLUMN_NAME_LEN 30
#define MAX_TABLE_NAME_LEN  30
#define MAX_DB_NAME_LEN     30
#define MAX_CMD_LEN         300
#define LOWER 0
#define EQUAL 1
#define GREATER 2
#define NR_ZECIMALE 7

// int
typedef struct intCell {
	int    value;
	struct intCell* next;
} t_intCell;

typedef struct intLine {
	t_intCell*      cells;
	struct intLine* next;
} t_intLine;

// float
typedef struct floatCell {
	float  value;
	struct floatCell* next;
} t_floatCell;

typedef struct floatLine {
	t_floatCell*      cells;
	struct floatLine* next;
} t_floatLine;

// string
typedef struct stringCell {
	char*  value;
	struct stringCell* next;
} t_stringCell;

typedef struct stringLine {
	t_stringCell*       cells;
	struct stringLine* next;
} t_stringLine;

//generic
typedef struct GCell {
	void*  value;
	struct GCell* next;
} t_GCell;

typedef struct GLine {
	t_GCell*       cells;
	struct GLine* next;
} t_GLine;

// cell type
typedef enum {
	INT,
	FLOAT,
	STRING
} t_cellType;

// column
typedef struct column {
	char name[MAX_COLUMN_NAME_LEN];
	struct column* next;
} t_column;

// table
typedef struct table {
	char name[MAX_TABLE_NAME_LEN];
	t_cellType    type;
	t_column*     columns;
	t_GLine*         lines;
	struct table* next;
} t_table;

// database
typedef struct db {
	char name[MAX_DB_NAME_LEN];
	t_table* tables;
} t_db;

typedef void (*TFA[3])(void*); // functie afisare element

typedef int (*TFCmp[3])(void*, void*); // functie comparare element

t_db* INIT_DB(char name[MAX_DB_NAME_LEN]); // initializeaza baza de date

void free_columns(t_column* C); // elibereaza campul "coloana" dintr-un tabel

void free_GCell(t_GCell* c);

t_column* alloc_column(char name[MAX_COLUMN_NAME_LEN]);

t_GCell* alloc_GCell(void *ae, size_t d);

t_table* alloc_table(char name[MAX_TABLE_NAME_LEN], t_cellType type);

void free_table(t_table *t);

void DELETE_DB(t_db* database);

t_table* create_table(char* name, t_cellType type, t_column c);

t_table* table_in_db(t_db *db, char *table_name);

void afisare_db(t_db *database);

int unknown_type(char *type);

void afisare_tabel(t_table* t);

int add_line(t_table* t, void* values, int nr_values);

int search(t_table* t, char *column_name, char *relation,
	void *value);

int delete_table(t_db* database, char table_name[MAX_TABLE_NAME_LEN]);

int delete_lines(t_db* database, char *table_name, char *column_name, char *relation,
	void *value);

int clear_table(t_db* database, char name[MAX_TABLE_NAME_LEN]);

#endif
