// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#ifndef SOCIAL_PLATFORM_H
#define SOCIAL_PLATFORM_H

#include <stdio.h>
#include "hash_map.h"
#include "graph.h"

/******************************
 * sp_db = social platform's data base
*******************************/
typedef struct sp_db_t {
	/* Number of users. */
	u_int users_number;
	/* Users's database. */
	char **users_db;
	/* Undirected graph of frenships. */
	graph_t *friends_graph;
	/* Posts's database.
	   id of post -> key
	   post -> value */
	hashtable_t *posts_db;
	/* The id of the last post. */
	u_int max_post_id;
	/* Oriented graph which contains the links
	   between posts. A repost has an edge from
	   the original post. So, the connected components
	   from graph are trees. */
	graph_t *posts_graph;
} sp_db_t;

#endif

