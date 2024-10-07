// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#include "posts.h"

void post_free(post_t *post)
{
	free(post->title);
	ht_free(&post->likes);
	free(post);
}

void key_post_free_function(void *data)
{
	// Make the needed cast.
	info_t *pair = (info_t *)data;

	// Free the memory allocated for the key.
	free(pair->key);
	// Free the memory allocated for the post.
	post_free(*(post_t **)pair->value);
	free(pair->value);
}

hashtable_t *create_posts_db(void)
{
	hashtable_t *posts_db = ht_create(1009, hash_uint, compare_function_uints,
									 key_post_free_function);
	return posts_db;
}

void create_post(sp_db_t *sp_db, char *user_name, char *post_title)
{
	// Verify if we reached the maximum id of a post.
	// If we reached the maximum id, we double the
	// number of post that we can store.
	if (sp_db->max_post_id == sp_db->posts_graph->nodes)
		graph_double_nodes(sp_db->posts_graph);

	// Allocate memory for the structure of the post.
	post_t *new_post = (post_t *)malloc(sizeof(post_t));
	DIE(!new_post, "malloc() failed\n");

	// Initialize the fields of the structure.

	// ID
	new_post->id = (++sp_db->max_post_id);
	// Parent ID
	new_post->parent_id = 0;
	// Title
	if (post_title)
		new_post->title = strdup(post_title);
	else
		new_post->title = NULL;
	// User ID
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;
	new_post->user_id = get_id(users_number, users_db, user_name);
	// Likes.
	new_post->likes = ht_create(137, hash_string, compare_function_strings,
								key_val_free_function);

	// Add the post in the database with posts.
	hashtable_t *pst_db = sp_db->posts_db;
	ht_put(pst_db, &new_post->id, sizeof(u_int), &new_post, sizeof(post_t *));
}

void create_repost(sp_db_t *sp_db, char *user_name, u_int parent_id)
{
	// Create the reposte and it in the database.
	create_post(sp_db, user_name, NULL);

	// Create a linking from the parent post to the repost.
	directed_graph_add_edge(sp_db->posts_graph, parent_id, sp_db->max_post_id);
	post_t *repost = *(post_t **)ht_get(sp_db->posts_db, &sp_db->max_post_id);
	repost->parent_id = parent_id;
}

void get_reposts(sp_db_t *sp_db, u_int post_id)
{
	// Go through the list of direct reposts.
	ll_node_t *curr_node = sp_db->posts_graph->neighbors[post_id]->head;
	while (curr_node) {
		// Find the ID of current repost.
		u_int curr_post_id = *(u_int *)curr_node->data;

		// Find the name of the user which created the repost
		hashtable_t *posts_db = sp_db->posts_db;
		post_t *curr_post =  *(post_t **)ht_get(posts_db, &curr_post_id);
		u_int users_number = sp_db->users_number;
		char **users_db = sp_db->users_db;
		char *user_name = get_name(users_number, users_db, curr_post->user_id);

		// Print on the screen the post's ID and the users's name.
		printf("Repost #%u by %s\n", curr_post->id, user_name);

		// Get the resposts from the current respost.
		get_reposts(sp_db, curr_post_id);

		// Go at the next repost.
		curr_node = curr_node->next;
	}
}

void
find_common_repost(sp_db_t *sp_db, u_int post, u_int repost_1, u_int repost_2)
{
	graph_t *posts_graph = sp_db->posts_graph;
	u_int comon_repost = tree_lca(posts_graph, post, repost_1, repost_2);
	printf("The first common repost of %u and %u is %u\n", repost_1, repost_2,
		   comon_repost);
}

void like_post(sp_db_t *sp_db, char *username, u_int post_id, int repost_id)
{
	// Find the title of the original post.
	post_t *post = *(post_t **)ht_get(sp_db->posts_db, &post_id);
	char *title = post->title;

	// Find the if of the post that we like.
	u_int id = post_id;
	if (repost_id != -1)
		id = repost_id;

	// Take the post which we like.
	post = *(post_t **)ht_get(sp_db->posts_db, &id);

	// (Un)like the post.
	if (!ht_has_key(post->likes, username)) {
		ht_put(post->likes, username, strlen(username) + 1, username,
			   strlen(username) + 1);
		if (id == post_id)
			printf("User %s liked post \"%s\"\n", username, title);
		else
			printf("User %s liked repost \"%s\"\n", username, title);
	} else {
		ht_remove_entry(post->likes, username);
		if (id == post_id)
			printf("User %s unliked post \"%s\"\n", username, title);
		else
			printf("User %s unliked repost \"%s\"\n", username, title);
	}
}

void get_likes(sp_db_t *sp_db, u_int post_id, int repost_id)
{
	if (repost_id == -1) {
		post_t *post = *(post_t **)ht_get(sp_db->posts_db, &post_id);
		printf("Post \"%s\" has %d likes\n", post->title, post->likes->size);
	} else {
		post_t *post = *(post_t **)ht_get(sp_db->posts_db, &repost_id);
		printf("Repost #%u has %d likes\n", post->id, post->likes->size);
	}
}

void is_ratio(sp_db_t *sp_db, u_int post_id)
{
	// Take the original post.
	post_t *post = *(post_t **)ht_get(sp_db->posts_db, &post_id);

	// Go through all the reposts and find the top post.(the post
	// with the most likes).
	post_t *top_post = post;
	u_int *bfs = bfs_graph(sp_db->posts_graph, post_id);
	for (u_int i = 2; i <= bfs[0]; ++i) {
		// Get a repost.
		post_t *repost = *(post_t **)ht_get(sp_db->posts_db, &bfs[i]);

		// Comprae the repost with the curent top post.
		if (top_post->likes->size < repost->likes->size) {
			top_post = repost;
		} else if (top_post->likes->size == repost->likes->size) {
			if (top_post->id > repost->id)
				top_post = repost;
		}
	}

	// Determinate if the post got ratio's by a repost.
	if (post == top_post)
		printf("The original post is the highest rated\n");
	else
		printf("Post %u got ratio'd by repost %u\n", post_id, top_post->id);

	// Eliminate the memory which was allocated in this function.
	free(bfs);
}

void delete_post(sp_db_t *sp_db, u_int post_id, int repost_id)
{
	// Print the corresponding message.
	post_t *post = *(post_t **)ht_get(sp_db->posts_db, &post_id);
	if (repost_id == -1)
		printf("Deleted \"%s\"\n", post->title);
	else
		printf("Deleted repost #%u of post \"%s\"\n", repost_id, post->title);

	// Determinate the id of the post which will be deleted.
	u_int id = post_id;
	if (repost_id != -1)
		id = repost_id;

	// To remove all the reposts from the deleted post and so forth,
	// we'll do a BFS.

	// Create the queue with the nodes which must be deleted.
	queue_t *q = q_create(sizeof(u_int), sp_db->posts_db->size, free);

	// Start BFS.
	q_enqueue(q, &id);
	while (!q_is_empty(q)) {
		u_int curr_id = *(u_int *)q_front(q);
		q_dequeue(q);

		// Put every neighbour in queue.
		ll_node_t *neighbor = sp_db->posts_graph->neighbors[curr_id]->head;
		while (neighbor) {
			u_int nbor_id = *(u_int *)neighbor->data;
			q_enqueue(q, &nbor_id);
			neighbor = neighbor->next;
		}

		// Deallocate the memory for the list of neighbours,
		ll_free(&sp_db->posts_graph->neighbors[curr_id]);
		// Create an empty list of neighbour.
		sp_db->posts_graph->neighbors[curr_id] = ll_create(sizeof(u_int));

		// Remove the current post from database.
		ht_remove_entry(sp_db->posts_db, &curr_id);
	}

	// If the function was called for a repost, we must break
	// the link between it and its dad.
	if (repost_id != -1) {
		u_int parent = tree_find_parent(sp_db->posts_graph, post_id,
										repost_id);
		directed_graph_remove_edge(sp_db->posts_graph, parent, repost_id);
	}

	// Free the memory which was allocated in this function.
	q_free(q);
}

void handle_input_posts(sp_db_t *sp_db, char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;

	if (!strcmp(cmd, "create")) {
		char *user_name = strtok(NULL, " ");
		char *post_title = strtok(NULL, "\"");

		create_post(sp_db, user_name, post_title);
		printf("Created \"%s\" for %s\n", post_title, user_name);
	} else if (!strcmp(cmd, "repost")) {
		char *user_name = strtok(NULL, " ");
		char *post_id = strtok(NULL, " ");
		char *repost_id = strtok(NULL, " ");
		u_int id = (repost_id) ? atoi(repost_id) : atoi(post_id);

		create_repost(sp_db, user_name, id);
		printf("Created repost #%d for %s\n", sp_db->max_post_id, user_name);
	} else if (!strcmp(cmd, "get-reposts")) {
		char *post_id = strtok(NULL, " ");
		char *repost_id = strtok(NULL, " ");
		u_int id = (repost_id) ? atoi(repost_id) : atoi(post_id);

		post_t *post = *(post_t **)ht_get(sp_db->posts_db, &id);
		char *user_name = get_name(sp_db->users_number, sp_db->users_db,
								   post->user_id);
		if (!repost_id)
			printf("\"%s\" - Post by %s\n", post->title, user_name);
		else
			printf("Repost #%u by %s\n", post->id, user_name);
		get_reposts(sp_db, post->id);
	} else if (!strcmp(cmd, "common-repost")) {
		u_int post = atoi(strtok(NULL, " "));
		u_int repost_1 = atoi(strtok(NULL, " "));
		u_int repost_2 = atoi(strtok(NULL, " "));

		find_common_repost(sp_db, post, repost_1, repost_2);
	} else if (!strcmp(cmd, "like")) {
		char *user_name = strtok(NULL, " ");
		char *post_id = strtok(NULL, " ");
		char *repost_id = strtok(NULL, " ");

		if (repost_id)
			like_post(sp_db, user_name, atoi(post_id), atoi(repost_id));
		else
			like_post(sp_db, user_name, atoi(post_id), -1);
	} else if (!strcmp(cmd, "get-likes")) {
		char *post_id = strtok(NULL, " ");
		char *repost_id = strtok(NULL, " ");

		if (repost_id)
			get_likes(sp_db, atoi(post_id), atoi(repost_id));
		else
			get_likes(sp_db, atoi(post_id), -1);
	} else if (!strcmp(cmd, "ratio")) {
		char *post_id = strtok(NULL, " ");

		is_ratio(sp_db, atoi(post_id));
	} else if (!strcmp(cmd, "delete")) {
		char *post_id = strtok(NULL, " ");
		char *repost_id = strtok(NULL, " ");

		if (repost_id)
			delete_post(sp_db, atoi(post_id), atoi(repost_id));
		else
			delete_post(sp_db, atoi(post_id), -1);
	}

	free(commands);
}

