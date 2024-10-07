// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#include "feed.h"

int is_post_in_feed(sp_db_t *sp_db, u_int post_id, u_int user_id)
{
	// Get the post to know the user which created it.
	post_t *post = *(post_t **)ht_get(sp_db->posts_db, &post_id);

	// Verify if the post wa created by the given user.
	if (post->user_id == user_id)
		return 1;

	// Verify of the post was create by a friend of the user.
	ll_node_t *friend = sp_db->friends_graph->neighbors[user_id]->head;
	while (friend) {
		u_int friend_id = *(int *)friend->data;

		if (friend_id == post->user_id)
			return 1;

		friend = friend->next;
	}

	// The post isn't in feed.
	return 0;
}

void show_feed(sp_db_t *sp_db, char *username, u_int feed_size)
{
	// Get the ID of the user.
	u_int user_id = get_id(sp_db->users_number, sp_db->users_db, username);

	// We go from the most frequent post to the oldest one, in
	// the database, until we build the feed.
	u_int post_id = sp_db->max_post_id;
	while (feed_size && post_id) {
		// Verify if the post with the current id exist.
		if (!ht_has_key(sp_db->posts_db, &post_id)) {
			post_id--;
			continue;
		}

		// Verify if the current post it's not a repost.
		post_t *post = *(post_t **)ht_get(sp_db->posts_db, &post_id);
		if (post->parent_id) {
			post_id--;
			continue;
		}

		// Verify if the post is in the group of friends of the user.
		int	show_on_feed = is_post_in_feed(sp_db, post_id, user_id);
		if (show_on_feed) {
			char *name = get_name(sp_db->users_number, sp_db->users_db,
								  post->user_id);
			printf("%s: \"%s\"\n", name, post->title);
			feed_size--;
		}

		// Go at the next post.
		post_id--;
	}
}

void show_profile(sp_db_t *sp_db, char *username)
{
	// Get the ID of the user.
	u_int user_id = get_id(sp_db->users_number, sp_db->users_db, username);

	// We go from the oldest (re)post to the most frequent one, in
	// the database, until we build the profile.
	u_int post_id = 1;
	while (post_id <= sp_db->max_post_id) {
		// Verify if the (re)post with the current id exist.
		if (!ht_has_key(sp_db->posts_db, &post_id)) {
			post_id++;
			continue;
		}

		// Get the current (re)post.
		post_t *post = *(post_t **)ht_get(sp_db->posts_db, &post_id);
		// Verify if the current (re)post was done by the user in which
		// we're interested.
		if (post->user_id == user_id) {
			if (post->parent_id) {
				printf("Reposted: ");
				while (post->parent_id) {
					u_int post_id = post->parent_id;
					post = *(post_t **)ht_get(sp_db->posts_db, &post_id);
				}
			} else {
				printf("Posted: ");
			}
			printf("\"%s\"\n", post->title);
		}

		// Go at the next post.
		post_id++;
	}
}

void show_friends_that_reposted(sp_db_t *sp_db, char *username, u_int post_id)
{
	// reposted[i] == 1 ==> the user with id i reposted to the post and is
	//						friend with the user
	//			   == 0 ==> the user with id i didn't repost to the post or
	//						isn'friends with the user.
	u_int *reposted = (u_int *)calloc(sp_db->users_number, sizeof(u_int));
	DIE(!reposted, "calloc() failed\n");
	// Array with the IDs of all reposts.
	u_int *reposts = bfs_graph(sp_db->posts_graph, post_id);

	// Verify every repost.
	for (u_int i = 1; i <= reposts[0]; ++i) {
		// Get the repost.
		post_t *repost = *(post_t **)ht_get(sp_db->posts_db, &reposts[i]);

		// Verify if the creator of the post id friend with te user.
		u_int users_number = sp_db->users_number;
		char **users_db = sp_db->users_db;
		char *creator = get_name(users_number, users_db, repost->user_id);
		if (are_friends(sp_db, username, creator))
			reposted[repost->user_id] = 1;
	}

	// Print the name of every friend which replied to the post.
	for (u_int i = 0; i < sp_db->users_number; ++i) {
		if (reposted[i]) {
			char *name = get_name(sp_db->users_number, sp_db->users_db, i);
			printf("%s\n", name);
		}
	}

	// Free the memory allocated in this function.
	free(reposted);
	free(reposts);
}

void show_common_group(sp_db_t *sp_db, char *username)
{
	// Get the ID of the user.
	u_int user_id = get_id(sp_db->users_number, sp_db->users_db, username);

	// Find the biggest common group in which is the user.
	u_int *clique = get_clique(sp_db->friends_graph, user_id, 1);
	while (1) {
		u_int new_size = clique[0] + 1;
		u_int *new_clique = get_clique(sp_db->friends_graph, user_id, new_size);
		if (new_clique[0]) {
			free(clique);
			clique = new_clique;
		} else {
			free(new_clique);
			break;
		}
	}

	// Sort the clique using Selection Sort.
	for (u_int i = 1; i < clique[0]; ++i) {
		for (u_int j = i + 1; j <= clique[0]; ++j) {
			if (clique[i] > clique[j]) {
				u_int tmp = clique[i];
				clique[i] = clique[j];
				clique[j] = tmp;
			}
		}
	}

	// Print every user from clique.
	printf("The closest friend group of %s is:\n", username);
	for (u_int i = 1; i <= clique[0]; ++i) {
		char *name = get_name(sp_db->users_number, sp_db->users_db, clique[i]);
		printf("%s\n", name);
	}

	// Free the memory.
	free(clique);
}

void handle_input_feed(sp_db_t *sp_db, char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *username = strtok(NULL, " ");
		u_int feed_size = atoi(strtok(NULL, "\n"));
		show_feed(sp_db, username, feed_size);
	} else if (!strcmp(cmd, "view-profile")) {
		char *username = strtok(NULL, "\n");
		show_profile(sp_db, username);
	} else if (!strcmp(cmd, "friends-repost")) {
		char *username = strtok(NULL, " ");
		u_int post_id = atoi(strtok(NULL, "\n"));
		show_friends_that_reposted(sp_db, username, post_id);
	} else if (!strcmp(cmd, "common-group")) {
		char *username = strtok(NULL, "\n");
		show_common_group(sp_db, username);
	}

	free(commands);
}
