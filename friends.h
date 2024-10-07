// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#ifndef FRIENDS_H
#define FRIENDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "graph.h"
#include "hash_map.h"
#include "social_platform.h"

#define MAX_COMMAND_LEN 1000
#define MAX_PEOPLE 550

/******************************
 * add_friendship() - Add a friendship link between 2
 * users in a social platform's database.
 *
 * @param sp_db: Social platform's database.
 * @param name_1: Name of first user.
 * @param name_2: Name of second user.
*******************************/
void add_friendship(sp_db_t *sp_db, char *name_1, char *name_2);

/******************************
 * remove_friendship() - Remove a friendship link between 2
 * users in a social platform's database.
 *
 * @param sp_db: Social platform's database.
 * @param name_1: Name of first user.
 * @param name_2: Name of second user.
*******************************/
void remove_friendship(sp_db_t *sp_db, char *name_1, char *name_2);

/******************************
 * are_friends() - Verify if 2 users are friends..
 *
 * @param sp_db: Social platform's database.
 * @param name_1: Name of first user.
 * @param name_2: Name of second user.
 *
 * @return: 1, if the users are friends
 *			0, in contrary case
*******************************/
u_int are_friends(sp_db_t *sp_db, char *name_1, char *name_2);

/******************************
 * find_distance() - Print the distance from a user at another.
 *		The distance between 2 users is the number of persons
 *		through a user must go to get to a person
 *
 * @param sp_db: Social platform's database.
 * @param name_1: Name of first user.
 * @param name_2: Name of second user.
*******************************/
void find_distance(sp_db_t *sp_db, char *name_1, char *name_2);

/******************************
 * find_comoon_friends() - Print the common friends of 2 users.
 *
 * @param sp_db: Social platform's database.
 * @param name_1: Name of first user.
 * @param name_2: Name of second user.
*******************************/
void find_common_friends(sp_db_t *sp_db, char *name_1, char *name_2);

/******************************
 * find_number_of_friends() - Print the friends's number of a user.
 *
 * @param sp_db: Social platform's database.
 * @param name: Name of user.
*******************************/
void find_number_of_friends(sp_db_t *sp_db, char *name);

/******************************
 * find_most_popular() - Find the most popular person from the group
 *		of friends of a user.
 *
 * @param sp_db: Social platform's database.
 * @param name: Name of user.
*******************************/
void find_most_popular(sp_db_t *sp_db, char *name);

/******************************
 * @brief: Function that handles the calling of every command from task 1.
*******************************/
void handle_input_friends(sp_db_t *sp_db, char *input);

#endif // FRIENDS_H
