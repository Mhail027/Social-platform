#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_map.h"

#define DIE(assertion, call_description)                                      \
	do {                                                                      \
		if (assertion) {                                                      \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                \
			perror(call_description);                                         \
			exit(errno);                                                      \
		}                                                                     \
	} while (0)                                                               \

/******************************
 * get_users_number - Find the number of persons from a database.
 *
 * @param user_db_path: The path to the file which conatins the
 *      databse. (The number of users is on first line.)
 *
 * @return: The number of persons.
*******************************/
u_int get_users_number(char *users_db_path);

/******************************
 * users_db_create() - Create a matrix with the names of users.
 *
 * @param users_db_path: The path to the file which contains the
 *		database.
 *
 * @return: The created matrix.
*******************************/
char **users_db_create(char *users_db_path);

/******************************
 * get_id: Find the id of a user after name.
 *
 * @param users_db: Matrix with all names.
 * @param users_number: Number of users / names.
 * @param name: The searched name.
 *
 * @return: ID of the user.
*******************************/
u_int get_id(u_int users_number, char **users_db, char *name);

/******************************
 * get_name: Find the name of a user after id.
 *
 * @param users_db: Matrix with all names.
 * @param users_number: Number of users / names.
 * @param name: The searched ID.
 *
 * @return: Name of the user.
*******************************/
char *get_name(u_int users_number, char **users_db, u_int id);

