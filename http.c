//
//  Counts http keywords from a file "http.txt" and prints a summary afterwards. 
//  http keywords are essentailly "keyword :" at the leading edge of each line. 
//
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <search.h>
#include <glib.h>

//  Built on Fedora 24 using glib hash tables. 
//  #  sudo dnf install glib2*
// gcc  -I/usr/include/glib-2.0/ -I//usr/lib64/glib-2.0/include/ -lglib-2.0   -g -o http http.c

#define SSIZE 1024

typedef struct http_type {
	char *name; 
	unsigned int count; 
} http_type_t;


static void print_entries (gpointer key, gpointer value, gpointer user_data); 

static void print_entries (gpointer key, gpointer value, gpointer user_data)
{
	char *kname = (char *)key;
	http_type_t *keydata = (http_type_t *)value;
	printf ("key : %s, name : %s, count %d\n",kname,keydata->name,keydata->count); 
}

int
main (int argc,  char **argv) 
{
	char *sptr = malloc(SSIZE); 
	char *stok = malloc(SSIZE); 
	FILE *fp = fopen("http.txt","r"); 
	int i,count=0;
	GHashTable *table = g_hash_table_new (g_str_hash, g_str_equal);

	if (!table) { 
		perror ("table create failed\n"); 
		exit (1);
	}

	if (!fp) {
		perror ("open failed\n"); 
		exit (1);
	}

	while ((sptr = fgets(sptr,SSIZE,fp)) != NULL) { 
		char *sdup = strdup(sptr);  //  because strtok destroys sptr
		http_type_t *http_keyp,*http_keyout; 
		stok = strtok(sdup,":"); 
		if (stok && !(strcmp(stok,"\n") == 0) ) {
			char *stokdup=strdup(stok);
			char *keyout;
			if (!g_hash_table_lookup_extended (table, stokdup,(void *)&keyout, (void *)&http_keyout)) {
				http_keyp=(http_type_t *)malloc(sizeof (http_type_t));
				http_keyp->name = stokdup;
				http_keyp->count = 1; 
				g_hash_table_insert (table, stokdup, (void *)http_keyp);
				count++;
			} else { 
				http_keyout->count ++;
			}
		}
		if (sdup) free (sdup);
	}

	g_hash_table_foreach (table, print_entries, NULL);
	printf("Records processed: %d\n",count); 
} 


