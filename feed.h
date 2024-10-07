// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#ifndef FEED_H
#define FEED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "users.h"
#include "friends.h"
#include "posts.h"
#include "social_platform.h"

/******************************
 * is_post_in_grou() - Verify if a post is in the feed of a user.
 *
 * @param sp_db: Social platform's database.
 * @param post_id: ID of post.
 * @param user_id: ID of user.
 *
 * @return: 1, the post is in feed
 *			0, in contrary case
*******************************/
int is_post_in_feed(sp_db_t *sp_db, u_int post_id, u_int user_id);

/******************************
 * show_feed() - Print the most frquent posts from the feed of a user.
 *
 * @param sp_db: Social platform's database.
 * @param username: Name of user.
 * @param feed_size: Number of posts which will be showed.
*******************************/
void show_feed(sp_db_t *sp_db, char *username, u_int feed_size);

/******************************
 * show_profile() - Print the posts and the reposts of a user.
 *
 * @param sp_db: Social platform's database.
 * @param username: Name of user.
*******************************/
void show_profile(sp_db_t *sp_db, char *username);

/******************************
 * show_friends_that_reposted() - Print the names of friends of a user which
 *		replied to a post.
 *
 * @param sp_db: Social platform's database.
 * @param username: Name of user.
 * @param post_id: ID of post.
*******************************/
void show_friends_that_reposted(sp_db_t *sp_db, char *username, u_int post_id);

/******************************
 * show_common_group() - Print the biggest common friends's goup of a user.
 *
 * @param sp_db: Social platform's database.
 * @param username: Name of user.
*******************************/
void show_common_group(sp_db_t *sp_db, char *username);

/******************************
 * @brief: Function that handles the calling of every command from task 3.
*******************************/
void handle_input_feed(sp_db_t *sp_db, char *input);

#endif // FEED_H
