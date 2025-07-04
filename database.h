/* database.h */

// #define NDEBUG

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#pragma GCC diagnostic ignored "-Wstringop-truncation"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic push

#define MAX_PATH_LEN 256
#define MAX_KEY_LEN 128
#define HASH_TABLE_SIZE 1024

#define find_last_leaf(x)		find_last_leaf_linear(x)
#define find_last_child_node(x)		find_last_child_node_linear(x)
#define find_leaf(x)			find_leaf_hash(x)
#define find_node(x, y)			find_node_linear(x, y)

#define CONCAT_PATH(dest, parent_path, child_path, max_len)			\
	do {									\
		if (strcmp((parent_path), "/") == 0)				\
			snprintf((dest), (max_len), "/%s", (child_path));	\
		else								\
			snprintf((dest), (max_len), "%s/%s", (parent_path), (child_path));	\
	} while (0)

#define CHECK_NULL(ptr, err_message)						\
	do {									\
		if (!(ptr)) {							\
			fprintf(stderr, "%s\n", err_message);			\
			return NULL;						\
		}								\
	} while (0)

#define HASH_KEY(key, size)							\
	({									\
	 	uint32_t hash = 2166136261u;					\
		const char *k = (key);						\
		while (*k) {							\
			hash ^= (uint32_t)*k++;					\
			hash *= 16777619u;					\
		}								\
		hash % (size);							\
	})

typedef struct s_node Node;
typedef struct s_leaf Leaf;
typedef struct s_hash_entry HashEntry;

typedef unsigned int int32;
typedef unsigned short int int16;
typedef unsigned char int8;

typedef enum {
	VALUE_STRING,
	VALUE_INT,
	VALUE_DOUBLE,
	VALUE_BINARY
} ValueType;

typedef union {
	char *string;
	int32_t integer;
	double floating;
	struct {
		void *data;
		size_t size;
	} binary;
} LeafValue;


struct s_node {
	struct s_node *parent;
	struct s_node *child;
	struct s_node *sibling;
	Leaf *leaf;
	char path[MAX_PATH_LEN];
};

struct s_leaf {
	struct s_node *parent;
	struct s_leaf *sibling;
	char key[MAX_KEY_LEN];
	LeafValue value;
	ValueType type;
};

struct s_hash_entry {
	char key[MAX_KEY_LEN];
	Leaf *leaf;
	struct s_hash_entry *next;
};

void zero(char *, size_t);
// char *indent(int8);
// void print_original_node(Node *, int8, int);
// void print_leaves_of_node(Node *, int8, int);
// void print_node_and_leaves(Node *, int8, int );
void print_tree(int, Node *);
Node *create_root_node();
// Node *find_first_child_node(Node *);
// Node *find_last_child_node_linear(Node *);
Node *create_new_node(Node *, char *);
// Leaf *find_first_leaf(Node *);
// Leaf *find_last_leaf_linear(Node *);
// void add_leaf_to_table(Leaf *);
Leaf *find_leaf_by_hash(char *);
// Leaf *create_new_leaf_prototype(Node *, char *);
Leaf *create_new_leaf_string(Node *, char *, char *, size_t);
Leaf *create_new_leaf_int(Node *, char *, int32_t);
Leaf *create_new_leaf_double(Node *, char *, double);
Leaf *create_new_leaf_binary(Node *, char *, void *, size_t);
Node *find_node_linear(Node *, char *);
Leaf *find_leaf_linear(Node *, char *);
void print_node(Node *);
void print_leaf(Leaf *);
void free_leaf(Leaf *);
void free_node(Node *);
void hash_table_init();
void hash_table_free();
