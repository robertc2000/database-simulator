/* COMAN ROBERT - 313CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdb.h"

// intoarce numarul de cuvinte dintr-un sir de caractere
int word_count(char string[MAX_CMD_LEN])
{
	int count = 1;
	int l = strlen(string);
	for(int i = 0; i < l; i++)
		if(string[i] == ' ')
			count++;
	return count;
}

//intoarce codul comenzii
int get_function_code(char *string, char functions [10][10])
{
	char *function = strtok(string, " ");
	int code = -1;
	for(int i = 0; i < 9; i++)
		if(!strcmp(function, functions[i]))
		{
			code = i;
			break;
		}
	return code;

}
// intoarce codul corespunzator tipului de date
int convert_to_type(char *ctype)
{
	if(!strcmp(ctype, "INT"))
		return INT;
	else
		if(!strcmp(ctype, "FLOAT"))
			return FLOAT;
		else
			return STRING;
}

//verifica daca tipul de date este int, float sau string
int unknown_type(char *type)
{
	if(!strcmp(type, "INT") || !strcmp(type, "FLOAT") || 
		!strcmp(type, "STRING"))
		return 0;
	return 1;
}

// converteste elementele de tip char in int si le introduce in vector
void read_vector_int(int* v)
{
	int i = 0;
	char *s = strtok(NULL, " ");
	while(s)
	{
		v[i] = atoi(s);
		i++;
		s = strtok(NULL, " ");
	}
}
// converteste elementele de tip char in float si le introduce in vector
void read_vector_float(float* v)
{
	int i = 0;
	char *s = strtok(NULL, " ");
	while(s)
	{
		v[i] = strtof(s, NULL);
		i++;
		s = strtok(NULL, " ");
	}
}


int main()
{
	char *command = malloc(MAX_CMD_LEN*sizeof(char));
	char functions[10][10] = {"INIT_DB", "DELETE_DB", "CREATE", "DELETE",
	 "PRINT_DB", "PRINT", "SEARCH", "ADD", "CLEAR"};
	 t_db* database;
	 int first_command = 1;
	while(1)
	{
		//citire comanda
		fgets(command, MAX_CMD_LEN, stdin);
		command[strlen(command) - 1] = '\0';
		int max = word_count(command);
		//prelucrare comanda
		int code = get_function_code(command, functions);
		if(first_command && (code != 0)) // prima comanda nu este "INIT_DB"
			code = -2;
		if(code == -1)
			printf("Unknown command: \"%s\".\n", command);
		switch(code)
		{
			case 0: // INIT_DB
			{
				database = INIT_DB(strtok(NULL, " "));
				first_command = 0;
				break;
			}
			case 1: //DELETE_DB
			{
				free(command);
				DELETE_DB(database);
				break;
			}
			case 2: //CREATE
			{
				char *table_name = strtok(NULL, " ");
				if(table_in_db(database, table_name))
				{
					printf("Table \"%s\" already exists.\n", table_name);
					break;
				}
				char *ctype = strtok(NULL, " ");
				if(unknown_type(ctype))
				{
					printf("Unknown data type: \"%s\"\n", ctype);
					break;
				}
				t_cellType Type = convert_to_type(ctype);
				t_column* c = alloc_column(strtok(NULL, " "));
				char *aux = strtok(NULL, " ");
				t_column *urm = NULL, *ant = NULL;
				while(aux)
				{
					if(!ant)
					{
						urm = alloc_column(aux);
						c->next = urm;
						ant = urm;
					}
					else
					{
						urm = alloc_column(aux);
						ant->next = urm;
						ant = urm;
					}
				aux = strtok(NULL, " ");
			    }
				t_table* new_table = alloc_table(table_name, Type);
				if(!new_table)
					exit(1);
				new_table->columns = c;
				t_table *t = database->tables;
				if(!t)
					database->tables = new_table;
					else
					{	
						for(;t->next != NULL; t = t->next);
						t->next = new_table;
					}
				break;
			}
			case 3: //DELETE
			{
				if(max == 2) // DELETE TABLE
				{
					char *table_name = strtok(NULL, " ");
					int found = delete_table(database, table_name);
					if(!found)
						printf("Table \"%s\" not found in database.\n",
					 table_name);
				}
				else //DELETE LINE
				{
				char *table_name = strtok(NULL, " ");
				char *column_name = strtok(NULL, " ");
				char * relation = strtok(NULL, " ");
				char *value = strtok(NULL, " ");
				t_table* t = table_in_db(database, table_name);
				if(!t)
				{
					printf("Table \"%s\" not found in database.\n",
					 table_name);
					break;
				}
				void *val;
				int val_int;
				if(t->type == INT)
				{
					val_int = atoi(value);
					val = (void*) &val_int;
				}
				else
					if(t->type == FLOAT)
					{
						float val_float = strtof(value, NULL);
						val = (void*) &val_float;
					}
					else
						val = (void*) value;
				int rez = delete_lines(database, table_name, column_name,
					relation, val);
				if(rez == -1)
					printf("Table \"%s\" not found in database.\n",
					 table_name);
				else
					if(rez == -2)
						printf("Table \"%s\" does not contain column \"%s\".\n"
							, table_name, column_name);

				}
				break;
			}
			case 4: //PRINT_DB
			{
				afisare_db(database);
				break;
			}
			case 5: //PRINT
			{
				char *table_name = strtok(NULL, " ");
				t_table *t = table_in_db(database, table_name);
				if(!t)
				{
					printf("Table \"%s\" not found in database.\n",
					 table_name);
					break;
				}
				afisare_tabel(t);
				break;
			}
			case 6: //SEARCH
			{
				char *table_name = strtok(NULL, " ");
				char *column_name = strtok(NULL, " ");
				char * relation = strtok(NULL, " ");
				char *value = strtok(NULL, " ");
				t_table* t = table_in_db(database, table_name);
				if(!t)
				{
					printf("Table \"%s\" not found in database.\n",
					 table_name);
					break;
				}
				void *val;
				int val_int;
				if(t->type == INT)
				{
					val_int = atoi(value);
					val = (void*) &val_int;
				}
				else
					if(t->type == FLOAT)
					{
						float val_float = strtof(value, NULL);
						val = (void*) &val_float;
					}
					else
						val = (void*) value;
				int rez = search(t, column_name, relation, val);
				if(rez == 1)
					continue;
				else
					if(rez == -1)
							printf("Table \"%s\" does not contain column \"%s\".\n", table_name,
							 column_name);
				break;
			}
			case 7: //ADD
			{
				char *table_name = strtok(NULL, " ");
				t_table *t = table_in_db(database, table_name);
				if(!t)
				{
					printf("Table \"%s\" not found in database.\n",
					 table_name);
					break;
				}
				int nr_elem = max - 2;
				if(t->type == INT){
					int* v = malloc(nr_elem*sizeof(int));
					if(!v)
						exit(1);
					read_vector_int(v);
					add_line(t, (void*) v, nr_elem);
					free(v);
				}
				else
					if(t->type == FLOAT)
					{
						float *v = malloc(nr_elem * sizeof(float));
						if(!v)
							exit(1);
						read_vector_float(v);
						add_line(t, (void *) v, nr_elem);
						free(v);
					}
					else // string
					{	
						char v[20][MAX_COLUMN_NAME_LEN];
						char *s = strtok(NULL, " ");
						for(int i = 0; i < nr_elem; i++)
						{
							strcpy(v[i], s);
							s = strtok(NULL, " "); 
						}
						add_line(t, (void*) v, nr_elem);
					}
				break;
			}
			case 8: //CLEAR
			{
				char *table_name = strtok(NULL, " ");
				int rez = clear_table(database, table_name);
				if(rez == 1)
					continue;
				else
					printf("Table \"%s\" not found in database.\n",
					 table_name);
				break;
			}

		}

	}
	
}