// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DIE(assertion, call_description)                                      \
	do {                                                                      \
		if (assertion) {                                                      \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                \
			perror(call_description);                                         \
			exit(errno);                                                      \
		}                                                                     \
	} while (0)                                                               \

/******************************
 * ll_node = node from a (singly) linked list
*******************************/
typedef struct ll_node_t {
	/* The address of the next node. */
	struct ll_node_t *next;
	/* The information saved in the node. */
	void *data;
} ll_node_t;

/******************************
 * ll = (singly) linked list
*******************************/
typedef struct ll_t {
	/* The first node from list. */
	struct ll_node_t *head;
	/* The last node from list. */
	struct ll_node_t *tail;
	/* The number of nodes from list */
	u_int size;
	/* The dimension of information from every node, in bytes. */
	u_int data_size;
} ll_t;

/******************************
 * @brief Free the memory allocated for a node from a singly linked list,
 *		without to take in consideration that data can have in it other
 *		things which should be freed.
*******************************/
void free_ll_node(ll_node_t *node);

/******************************
 * ll_create() - Create and initialize a singly linked list.
 *
 * @param data_size: The length, in bytes, of the information
 *		from a node.
 *
 * @return - The created list.
*******************************/
ll_t *ll_create(u_int data_size);

/******************************
 * @brief Return the address of nth node from the given singly
 *		linked list. The count starts from 0, so n must be in the
 *		next interval: [0, size of the list - 1].
*******************************/
ll_node_t *ll_get_nth_node(ll_t *list, u_int n);

/******************************
 * @brief Remove the nth node from a singly linked list and return
 *		the address of that node. It is the responsability of user
 *		to free its memory. The position is indexed from 0. If the
 *		position is bigger or equal with the size of list, we will
 *		remove the last node.
*******************************/
ll_node_t *ll_remove_nth_node(ll_t *list, u_int n);

/******************************
 * @brief Create, initialize and put a node on nth postion in the given
 *		singly linked list. The position is indexed from 0. If the position
 *		n is bigger than the size of the list, the node is added at end.
*******************************/
void ll_add_nth_node(ll_t *list, u_int n, void *data);

/******************************
 * cdll_free() - Free the memory allocated a singly linked list.
 *
 * @param l: Pointer to the address of the list.
*******************************/
void ll_free(ll_t **l);

#endif

