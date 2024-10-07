// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "social_platform.h"

sp_db_t *sp_db_create(char *users_db_path)
{
	// Allocate memory for the structure of a social
	// platform's database.
	sp_db_t *sp_db = (sp_db_t *)malloc(sizeof(sp_db_t));
	DIE(!sp_db, "malloc() failed\n");

	// Initialize every field from the structure.
	sp_db->users_number = get_users_number(users_db_path);
	sp_db->users_db = users_db_create(users_db_path);
	sp_db->friends_graph = graph_create(sp_db->users_number);
	sp_db->posts_db = create_posts_db();
	sp_db->posts_graph = graph_create(1000);
	sp_db->max_post_id = 0;

	// Return the created databse.
	return sp_db;
}

void sp_db_free(sp_db_t *sp_db)
{
	// Verify the parameter.
	if (!sp_db)
		return;

	// Free the allocated memory.
	for (u_int i = 0; i < sp_db->users_number; ++i)
		free(sp_db->users_db[i]);
	free(sp_db->users_db);
	graph_free(sp_db->friends_graph);
	ht_free(&sp_db->posts_db);
	graph_free(sp_db->posts_graph);
	free(sp_db);
}

int main(void)
{
	sp_db_t *sp_db = sp_db_create("users.db");

	char *input = (char *)malloc(MAX_COMMAND_LEN);

	while (fgets(input, MAX_COMMAND_LEN, stdin)) {
		handle_input_friends(sp_db, input);

		handle_input_posts(sp_db, input);

		handle_input_feed(sp_db, input);
	}

	free(input);
	sp_db_free(sp_db);
	return 0;
}
