/* COMAN ROBERT - 313CB*/

#include "tdb.h"

t_db* INIT_DB(char name[MAX_DB_NAME_LEN])
{
	t_db* database = malloc(sizeof(t_db));
	if(!database)
		return NULL;
	strcpy(database->name, name);
	database->tables = NULL;
	return database;
}

t_column* alloc_column(char name[MAX_COLUMN_NAME_LEN])
{
	t_column* c = malloc(sizeof(t_column));
	if(!c)
		return NULL;
	strcpy(c->name, name);
	c->next = NULL;
	return c;
}

void free_columns(t_column *C)
{
  t_column* aux = NULL;
  while(C)
  {
    aux = C;
    C = C->next;
    free(aux);
  }
}

t_GCell* alloc_GCell(void *ae, size_t d) // aloca celula generica
{
	t_GCell* aux = malloc(sizeof(t_GCell));
	if(!aux)
		return NULL;
	aux->value = malloc(d);
	if(!aux->value)
	{
		free(aux);
		return NULL;
	}
	memcpy(aux->value, ae, d);
	aux->next = NULL;
	return aux;
}

t_GLine* alloc_GLine() // aloca linie generica
{
	t_GLine* l = malloc(sizeof(t_GLine));
	if(!l)
		return NULL;
	l->cells = NULL;
	l->next = NULL;
	return l;
}

void free_Gline(t_GLine* l) // elibereaza o linie din tabel
{
	t_GCell* aux;
	t_GCell* c = l->cells;
	while(c)
	{
		aux = c;
		c = c->next;
		free(aux->value);
		free(aux);
	}
	l->cells = NULL;
	free(l);
}

void free_Glines(t_GLine* l) // elibereaza toate liniile din tabel
{
	t_GLine* aux;
	while(l)
	{
		aux = l;
		l = l->next;
		free_Gline(aux);
	}
}

t_table* table_in_db(t_db *db, char *table_name);

int clear_table(t_db* database, char name[MAX_TABLE_NAME_LEN])
{
	t_table* t = table_in_db(database, name);;
	if(!t) //tabelul nu a fost gasit
		return -1;
	free_Glines(t->lines);
	t->lines = NULL;
	return 1;
}

/*aloca memorie pentru un tabel*/
t_table* alloc_table(char name[MAX_TABLE_NAME_LEN], t_cellType type)
{
	t_table* t = malloc(sizeof(t_table));
	if(!t)
		return NULL;
	strcpy(t->name, name);
	t->type = type;
	t->columns = NULL;
	t->lines = NULL;
	t->next = NULL;
	return t;
}

void free_table(t_table *t) // elibereaza un tabel
{
	//t_columns *C = t->columns;
	free_columns(t->columns);
	t->columns = NULL;
	free_Glines(t->lines);
	t->lines = NULL;
	free(t);
}

void free_tables(t_db* database) // elibereaza toate tabelele
{
	t_table* t = database->tables;
	t_table* aux;
	while(t)
	{
		aux = t;
		t = aux->next;
		free_table(aux);
	}
	database->tables = NULL;
}

void DELETE_DB(t_db* database) // elibereaza baza de date si opreste programul
{
	free_tables(database);
	free(database);
	exit(0);
}

/*sterge un tabel*/
int delete_table(t_db* database, char table_name[MAX_TABLE_NAME_LEN])
{
	t_table* t = database->tables, *aux;
	int found = 0;
	t_table* ant = NULL;
	while (t)
	{
		if(!strcmp(t->name,table_name))
		{
			if(!ant)
			{
				aux = t;
				t = t->next;
				database->tables = t;
				free_table(aux);
			}
			else
			{
				aux = t;
				t = t->next;
				ant->next = t;
				free_table(aux);

			}
			found = 1;
		}
		else
		{
			ant = t;
			t = t->next;
		}
	}
	return found;
}

void urmatoarea_coloana(int nr_spatii) // trece la urm coloana la afisare
{
	for(int i = 0; i < nr_spatii; i++)
		printf(" ");
	printf(" "); // padding
}

// calculeaza cate cifre are un intreg
int nr_cifre(int x)
{
	int nr = 0;
	if(x == 0)
		return 1;
	while (x > 0)
	{
		x/=10;
		nr++;
	}
	return nr;
}

void afisare_int(void *element)
{
	int* x = (int *) element;
	printf("%d", *x);
	int nr_spatii = MAX_COLUMN_NAME_LEN - nr_cifre(*x);
	urmatoarea_coloana(nr_spatii);
}
void afisare_float(void *element)
{
	float* x = (float *) element;
	printf("%.6f", *x);
	int nr_spatii = MAX_COLUMN_NAME_LEN - nr_cifre((int)*x) - NR_ZECIMALE;
	urmatoarea_coloana(nr_spatii);
}

void afisare_string(void *element)
{
	char* x = (char *) element;
	int nr_spatii = strlen(x);
	printf("%s", x);
	urmatoarea_coloana(MAX_COLUMN_NAME_LEN - nr_spatii);
}

void afisare_linie(t_GLine* l, TFA afi_cel, t_cellType type)
{
	t_GCell* cells = l->cells;
	while(cells)
	{
		afi_cel[type](cells->value);
		cells = cells->next;
	}
	printf("\n");
}

void afisare_coloane(t_column* c, int *nr_coloane)
{
	while(c)
	{
		printf("%s", c->name);
		int nr_spatii = 30 - strlen(c->name);
		urmatoarea_coloana(nr_spatii);
		c = c->next;
		(*nr_coloane)++;
	}
	printf("\n");
}

void afisare_spatiu_dupa_coloane(int nr_coloane)
{
	for(int i = 0; i < nr_coloane; i++)
	{
		for(int j = 0; j < MAX_COLUMN_NAME_LEN; j++)
			printf("-");
		printf(" "); // padding
	}
	printf("\n");
}

void afisare_tabel(t_table* t)
{
	printf("TABLE: %s\n", t->name);
	int nr_coloane = 0;
	afisare_coloane(t->columns, &nr_coloane);
	afisare_spatiu_dupa_coloane(nr_coloane);
	t_GLine* l = t->lines;
	TFA afi_cell = {afisare_int, afisare_float, afisare_string};
	while(l)
	{
		afisare_linie(l, afi_cell,t->type);
		l = l->next;
	}
	printf("\n");
}

void afisare_db(t_db *database)
{
	printf("DATABASE: %s", database->name);
	printf("\n\n");
	t_table* t = database->tables;
	while(t)
	{
		afisare_tabel(t);
		t = t->next;
	}
}

int compare_int(void *a, void *b)
{
	int *x = (int *) a;
	int *y = (int *) b;
	if(*x < *y)
		return LOWER;
	else
		if(*x == *y)
			return EQUAL;
		else
			return GREATER;
}

int compare_float(void *a, void *b)
{
	float *x = (float *) a;
	float *y = (float *) b;
	float epsilon = 0.000001;
	if(fabs(*x - *y) < epsilon)
		return EQUAL;
	else
		if(*x < *y)
			return LOWER;
		else
			return GREATER;
}

int compare_string(void *a, void *b)
{
	char *s1 = (char *) a;
	char *s2 = (char *) b;
	int rez = strcmp(s1, s2);
	if(rez < 0)
		return LOWER;
	else
		if(rez == 0)
			return EQUAL;
		else
			return GREATER;
}

int check_if_true(char *relation, int rez)
{
//verifica daca relatia este adevarata, comparand rezultatul cu codul intors
//de functia de comparare
	if(!strcmp(relation, "<") && (rez == LOWER))
		return 1;
	else
		if(!strcmp(relation, "<=") && ((rez == LOWER) || (rez == EQUAL)))
			return 1;
		else
			if(!strcmp(relation, "==") && rez == EQUAL)
				return 1;
			else
				if(!strcmp(relation, "!=") && rez != EQUAL)
					return 1;
				else
					if(!strcmp(relation, ">=") && ((rez == EQUAL) ||
						rez == GREATER))
						return 1;
					else
						if(!strcmp(relation, ">") && rez == GREATER)
							return 1;
	return 0;
}
// verifica daca un tabel se gaseste in baza de date si intoarce un pointer
// catre acesta in caz afirmativ
t_table* table_in_db(t_db *db, char *table_name)
{
	t_table* t = db->tables;
	while(t)
	{
		if(!strcmp(t->name, table_name))
			return t;
		t = t->next;
	}
	return NULL;;
}

int search(t_table* t, char *column_name, char *relation,
	void *value)
{
	TFCmp cmp = {compare_int, compare_float, compare_string};
	TFA afisare = {afisare_int, afisare_float, afisare_string};
	int pozitie_coloana = 1;
	t_column* c = t->columns;
	int found = 0;
	while(c && !found) // identificare pozitie coloana
	{
		if(!strcmp(c->name, column_name))
		{
			found = 1;
			break;
		}
		c = c->next;
		pozitie_coloana++;
	}
	if(!found) // nu exista coloana
		return -1;
	printf("TABLE: %s\n", t->name);
	int nr_coloane = 0;
	afisare_coloane(t->columns, &nr_coloane);
	afisare_spatiu_dupa_coloane(nr_coloane);
	t_GLine* l = t->lines;
	while(l)
	{
		t_GCell *cells = l->cells;
		// parcurgere pana la elementul din dreptul coloanei date
		for(int i = 1; i < pozitie_coloana; i++, cells = cells->next);
		int rez = cmp[t->type](cells->value, value);
		if(check_if_true(relation, rez))
		{
			afisare_linie(l, afisare, t->type);
		}
		l = l->next;
	}
	printf("\n");
	return 1;
}

int add_line(t_table* t, void* values, int nr_values)
{
	size_t d; // setare dimensiune
	if(t->type == INT)
		d = sizeof(int);
	else
		if(t->type == FLOAT)
			d = sizeof(float);
	//ajung la ultima linie
	t_GLine* new_line = alloc_GLine();
	if(!t->lines)
		t->lines = new_line;
		else
		{
			t_GLine* l = t->lines;
			while(l->next)
				l = l->next;
			l->next = new_line;
		}
	//adaug elementele
		int *x = values;
		float *y = values;
	t_GCell* c = NULL, * ant = NULL;
	if(t->type != STRING) // valori nr intregi sau nr reale
		for(int i = 0; i < nr_values; i++)
		{
			if(!ant) // prima celula
			{
				if(d == sizeof(int)) // int
					c = alloc_GCell(&x[i], d);
				else //float
					c = alloc_GCell(&y[i], d);
				if(!c)
					return 0;
				new_line->cells = c;
				ant = c;
			}
			else
			{
				if(d == sizeof(int))
					c = alloc_GCell(&x[i], d);
				else //float
					c = alloc_GCell(&y[i], d);
				if(!c)
					return 0;
				ant->next = c;
				ant = c;
				ant->next = NULL;
			}
		}
		else //STRING
		{
			for(int i = 0; i < nr_values; i++)
			{	
				char *s = values + i * MAX_COLUMN_NAME_LEN;
				d = strlen(s);
				if(!ant)
				{
					c = alloc_GCell(s, d + 1);
				if(!c)
					return 0;
				new_line->cells = c;
				ant = c;
				}
				else
				{
					c = alloc_GCell(s, d + 1);
					if(!c)
						return 0;
					ant->next = c;
					ant = c;
				}
			}
		}
	return 1;
}

int delete_lines(t_db* database, char *table_name, char *column_name,
 				char *relation, void *value)
{
	t_table* t = database->tables;
	int found = 0;
	while(!found && t) // cauta tabel cu numele table_name
	{
		if(strcmp(table_name, t->name) == 0)
		{
			found = 1;
			break;
		}
		t = t->next;
	}
	if(!found) // nu exista tabelul
		return -1;
	int pozitie_coloana = 1;
	found = 0;
	TFCmp cmp = {compare_int, compare_float, compare_string};
	//TFA afisare= {afisare_int, afisare_float, afisare_string};
	t_column* c = t->columns;
	while(c && !found) // identificare pozitie coloana
	{
		if(!strcmp(c->name, column_name))
		{
			found = 1;
			break;
		}
		c = c->next;
		pozitie_coloana++;
	}
	if(!found) // nu exista coloana
		return -2;
	t_GLine* l = t->lines, *ant = NULL, *aux;
	while(l)
	{	
		aux = l;
		t_GCell *cells = l->cells;
		for(int i = 1; i < pozitie_coloana; i++, cells = cells->next);
		int rez = cmp[t->type](cells->value, value);
		if(check_if_true(relation, rez)){
			if(!ant)
			{
				t->lines = l->next;
				l = l->next;
				free_Gline(aux);
			}
			else
			{
				ant->next = l->next;
				l = l->next;
				free_Gline(aux);
			}
		}
		else
		{
			ant = l;
			l = l->next;
		}
	}	
	return 1;
}