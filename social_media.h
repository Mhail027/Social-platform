// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#ifndef SOCIAL_MEDIA_H
#define SOCIAL_MEDIA_H

/******************************
 * sp_db_create() - Create and initialize a social
 *		platform's database.
 *
 * @param users_db_path: Path to the file which
 *		contains the name of the users.
 *
 * @return - The created data base.
*******************************/
sp_db_t *sp_db_create(char *users_db_path);

/******************************
 * @brief: Free the memory which was allocated
 *		for a social platform's database.
*******************************/
void sp_db_free(sp_db_t *sp_db);

#endif

