#include <corecrt_malloc.h>  
#include <string.h>  
#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  

#define MAX_KEY_LEN  120  
#define MAX_VALUE_LEN  512  
#define MAX_TABLE_SIZE 512  

enum {  
RES_OK = 0,  
RES_FAILURE = -1,  
};  

typedef struct kvpair_s  
{  
char key[MAX_KEY_LEN];  
char value[MAX_VALUE_LEN];  

}kvpair_t;  

typedef struct kvstore_s  
{  
kvpair_t* table;  
int num_pairs;  
int max_pairs;  

pthread_mutex_t lock;  

}kvstore_t;  
  
int init_kvstore(kvstore_t* kvs)  
{  
if (!kvs) return RES_FAILURE;  
kvs->table = (kvpair_t*)malloc(sizeof(kvpair_t) * MAX_TABLE_SIZE);  
kvs->num_pairs = 0;  
kvs->max_pairs = MAX_TABLE_SIZE;  

pthread_mutex_init(&kvs->lock, NULL);  

return RES_OK;  
}  

void dest_kvstore(kvstore_t* kvs)  
{  
if (!kvs) return;  
free(kvs->table);  
}  

int put_kvpair(kvstore_t* kvs, const char* key, const char* value)  
{  
if (!kvs) return RES_FAILURE;  
if (kvs->num_pairs >= kvs->max_pairs)  
{  
	return RES_FAILURE;  
}  
pthread_mutex_lock(&kvs->lock);  
int idx = kvs->num_pairs++;  
pthread_mutex_unlock(&kvs->lock);  

strncpy_s(kvs->table[idx].key, MAX_KEY_LEN, key, MAX_KEY_LEN - 1);  
strncpy_s(kvs->table[idx].value, MAX_VALUE_LEN, value, MAX_VALUE_LEN - 1);  

return RES_OK;  
}  

const char* get_kvpair(kvstore_t* kvs, const char* key)  
{  
if (!kvs) return NULL;  
for (int i = 0; i < kvs->num_pairs; i++)  
{  
	if (strcmp(kvs->table[i].key, key) == 0)  
	{  
		return kvs->table[i].value;  
	}  
}  
return NULL;  
}  

int main()  
{  
kvstore_t store;  

init_kvstore(&store);  

put_kvpair(&store, "position", "fixed");  
put_kvpair(&store, "left", "0");  
put_kvpair(&store, "top", "0");  
put_kvpair(&store, "width", "100%");  
put_kvpair(&store, "height", "100%");
put_kvpair(&store, "background_color", "rgba(0, 0, 0, 0.3)");
put_kvpair(&store, "display", "none");
put_kvpair(&store, "justify_content", "center");
put_kvpair(&store, "align_items", "center");
put_kvpair(&store, "z_index", "999999");

const char* position = get_kvpair(&store, "position");
printf("position: %s\n", position);

const char* left = get_kvpair(&store, "left");
printf("left: %s\n", left);

const char* top = get_kvpair(&store, "top");
printf("top: %s\n", top);

const char* width = get_kvpair(&store, "width");
printf("width: %s\n", width);

const char* height = get_kvpair(&store, "height");
printf("height: %s\n", height);

const char* background_color = get_kvpair(&store, "background_color");
printf("background_color: %s\n", background_color);

const char* display = get_kvpair(&store, "display");
printf("display: %s\n", display);

const char* justify_content = get_kvpair(&store, "justify_content");
printf("justify_content: %s\n", justify_content);

const char* align_items = get_kvpair(&store, "align_items");
printf("align_items: %s\n", align_items);

const char* z_index = get_kvpair(&store, "z_index");
printf("z_index: %s\n", z_index);

dest_kvstore(&store);  
}