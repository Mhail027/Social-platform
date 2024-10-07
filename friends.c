// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#include "friends.h"

void add_friendship(sp_db_t *sp_db, char *name_1, char *name_2)
{
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;

	// Get the IDs of the users.
	u_int id_1 = get_id(users_number, users_db, name_1);
	u_int id_2 = get_id(users_number, users_db, name_2);

	// Add an edge betwen them in the graph with friendships.
	undirected_graph_add_edge(sp_db->friends_graph, id_1, id_2);
	printf("Added connection %s - %s\n", name_1, name_2);
}

void remove_friendship(sp_db_t *sp_db, char *name_1, char *name_2)
{
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;

	// Get the IDs of the users.
	u_int id_1 = get_id(users_number, users_db, name_1);
	u_int id_2 = get_id(users_number, users_db, name_2);

	// Remove the edge betwen them from the graph with friendships.
	undirected_graph_remove_edge(sp_db->friends_graph, id_1, id_2);
	printf("Removed connection %s - %s\n", name_1, name_2);
}

u_int are_friends(sp_db_t *sp_db, char *name_1, char *name_2)
{
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;

	// Get the IDs of the users.
	u_int id_1 = get_id(users_number, users_db, name_1);
	u_int id_2 = get_id(users_number, users_db, name_2);

	// Verify if exists an edge from user 1 to user 2, in the
	// the graph of friends.
	return undirected_graph_has_edge(sp_db->friends_graph, id_1, id_2);
}

void find_distance(sp_db_t *sp_db, char *name_1, char *name_2)
{
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;

	// Get the IDs of the users.
	u_int id_1 = get_id(users_number, users_db, name_1);
	u_int id_2 = get_id(users_number, users_db, name_2);

	// Find the distance between the given persons.
	int distance = find_length_min_path(sp_db->friends_graph, id_1, id_2);
	if (distance != -1)
		printf("The distance between %s - %s is %d\n", name_1, name_2,
			   distance);
	else
		printf("There is no way to get from %s to %s\n", name_1, name_2);
}

void find_sugestions(sp_db_t *sp_db, char *name)
{
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;
	graph_t *friends_graph = sp_db->friends_graph;

	// Find the id of the user.
	u_int id = get_id(users_number, users_db, name);

	// Create a vector of type ok.
	// ok[i] == 1 ==> i is a friend of a friend
	// ok[i] == 0 ==> i is not a friend of a friend
	int *ok = (int *)calloc(users_number, sizeof(int));
	DIE(!ok, "calloc() failed\n");

	// Go through the friends's list of every friend and mark
	// them in the array ok.
	ll_node_t *curr_friend = friends_graph->neighbors[id]->head;
	while (curr_friend) {
		u_int friend_id = *(u_int *)curr_friend->data;

		ll_node_t *curr_suggestion = friends_graph->neighbors[friend_id]->head;
		while (curr_suggestion) {
			u_int suggestion_id = *(u_int *)curr_suggestion->data;
			ok[suggestion_id] = 1;
			curr_suggestion = curr_suggestion->next;
		}

		curr_friend = curr_friend->next;
	}

	// Mark with 0 the friends of the given user and also the user himself.
	curr_friend = friends_graph->neighbors[id]->head;
	while (curr_friend) {
		u_int friend_id = *(u_int *)curr_friend->data;
		ok[friend_id] = 0;
		curr_friend = curr_friend->next;
	}
	ok[id] = 0;

	// Verify if exists at least a sugestion.
	u_int exists_suggestions = 0;
	for (u_int i = 0; i < users_number; ++i)
		if (ok[i] == 1) {
			exists_suggestions = 1;
			break;
		}

	// Print the suggestions.
	if (!exists_suggestions) {
		printf("There are no suggestions for %s\n", name);
	} else {
		printf("Suggestions for %s:\n", name);
		for (u_int i = 0; i < users_number; ++i)
			if (ok[i])
				printf("%s\n", get_name(users_number, users_db, i));
	}

	// Free the memory which was allocated in this function.
	free(ok);
}

void find_common_friends(sp_db_t *sp_db, char *name_1, char *name_2)
{
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;
	graph_t *friends_graph = sp_db->friends_graph;

	// Find the id of the users.
	u_int id_1 = get_id(users_number, users_db, name_1);
	u_int id_2 = get_id(users_number, users_db, name_2);

	// Create a vector cnt with next meaning:
	// cnt[i] == 0 ==> i isn't friend with either of the two users
	// cnt[i] == 1 ==> i is friend with one of them
	// cn[i] == 2 ==> i is friend with both.=
	int *cnt = (int *)calloc(users_number, sizeof(int));
	DIE(!cnt, "calloc() failed\n");

	// Go through the list of friends of fisrt user and mark with +1
	// his friend in the vector cnt.
	ll_node_t *curr_friend = friends_graph->neighbors[id_1]->head;
	while (curr_friend) {
		u_int friend_id = *(u_int *)curr_friend->data;
		cnt[friend_id] += 1;
		curr_friend = curr_friend->next;
	}

	// Do the same thing for the second user.
	curr_friend = friends_graph->neighbors[id_2]->head;
	while (curr_friend) {
		u_int friend_id = *(u_int *)curr_friend->data;
		cnt[friend_id] += 1;
		curr_friend = curr_friend->next;
	}

	// Verify if exists at least a common friend.
	u_int exists_common_friends = 0;
	for (u_int i = 0; i < users_number; ++i)
		if (cnt[i] == 2) {
			exists_common_friends = 1;
			break;
		}

	// Print the coomon_friends.
	if (!exists_common_friends) {
		printf("No common friends for %s and %s\n", name_1, name_2);
	} else {
		printf("The common friends between %s and %s are:\n", name_1, name_2);
		for (u_int i = 0; i < users_number; ++i)
			if (cnt[i] == 2)
				printf("%s\n", get_name(users_number, users_db, i));
	}

	// Free the memory which was allocated in this function.
	free(cnt);
}

void find_number_of_friends(sp_db_t *sp_db, char *name)
{
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;
	graph_t *friends_graph = sp_db->friends_graph;

	// Find the id of the user.
	u_int id = get_id(users_number, users_db, name);

	// Print the number of friends.
	printf("%s has %d friends\n", name, friends_graph->neighbors[id]->size);
}

void find_most_popular(sp_db_t *sp_db, char *name)
{
	u_int users_number = sp_db->users_number;
	char **users_db = sp_db->users_db;
	graph_t *friends_graph = sp_db->friends_graph;

	// Find the id of the user.
	u_int id = get_id(users_number, users_db, name);

	// Find the person with the most friends from the user's group.
	u_int popular_id = id;
	// Go through the list of user's friends.
	ll_node_t *curr_friend = friends_graph->neighbors[id]->head;
	while (curr_friend) {
		u_int friend_id = *(u_int *)curr_friend->data;
		u_int friends_number = friends_graph->neighbors[friend_id]->size;
		u_int max_friends_number = friends_graph->neighbors[popular_id]->size;
		// Verify if we found a more popular person.
		if (max_friends_number < friends_number) {
			popular_id = friend_id;
		} else if (max_friends_number == friends_number) {
			if (popular_id != id && popular_id > friend_id)
				popular_id = friend_id;
		}
		// Go at the next friend.
		curr_friend = curr_friend->next;
	}

	// Print the most popular person from the group.
	if (id == popular_id) {
		printf("%s is the most popular\n", name);
	} else {
		char *popular_user = get_name(users_number, users_db, popular_id);
		printf("%s is the most popular friend of %s\n", popular_user, name);
	}
}

void handle_input_friends(sp_db_t *sp_db, char *input)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *name_1 = strtok(NULL, " ");
		char *name_2 = strtok(NULL, "\n");
		add_friendship(sp_db, name_1, name_2);
	} else if (!strcmp(cmd, "remove")) {
		char *name_1 = strtok(NULL, " ");
		char *name_2 = strtok(NULL, "\n");
		remove_friendship(sp_db, name_1, name_2);
	} else if (!strcmp(cmd, "distance")) {
		char *name_1 = strtok(NULL, " ");
		char *name_2 = strtok(NULL, "\n");
		find_distance(sp_db, name_1, name_2);
	} else if (!strcmp(cmd, "suggestions")) {
		char *name = strtok(NULL, "\n");
		find_sugestions(sp_db, name);
	} else if (!strcmp(cmd, "common")) {
		char *name_1 = strtok(NULL, " ");
		char *name_2 = strtok(NULL, "\n");
		find_common_friends(sp_db, name_1, name_2);
	} else if (!strcmp(cmd, "friends")) {
		char *name = strtok(NULL, "\n");
		find_number_of_friends(sp_db, name);
	} else if (!strcmp(cmd, "popular")) {
		char *name = strtok(NULL, "\n");
		find_most_popular(sp_db, name);
	}

	free(commands);
}
