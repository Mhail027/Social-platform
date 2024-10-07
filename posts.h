// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#ifndef POSTS_H
#define POSTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "queue.h"
#include "hash_map.h"
#include "graph.h"
#include "users.h"
#include "social_platform.h"

/******************************
 * Structure to save the information of a (re)post.
*******************************/
typedef struct post_t {
	/* The id of the (re)post. */
	u_int id;
	/* The parent (re)post of a repost. 0 if it's a post. */
	u_int parent_id;
	/* The title of the post. NULL if it's a repost. */
	char *title;
	/* The id of the user which created the (re)post. */
	u_int user_id;
	/* The usernames of the person which like the (re)post.
	   username -> key
	   username -> value */
	hashtable_t *likes;
} post_t;

/******************************
 * @brief: Deallocate the memory of a post.
*******************************/
void post_free(post_t *post);

/******************************
 * @brief: Free the memory allocated for the key and the value of a
 *		pair from a hashtable. The value is a pointer to a post
 *		(post_t *), while the key is a pointer to a primitive
 *		type of data.
*******************************/
void key_post_free_function(void *data);

/******************************
 * create posts_db() - Create a hashtable in which to be stored the posts.
 *		id of the post -> key
 *		post -> value
 *
 * @return: The created hashtable.
*******************************/
hashtable_t *create_posts_db(void);

/******************************
 * create post() - Create a post and put it in the social platform's
 *		database.
 *
 * @param sp_db: Social platform's database.
 * @param user_name: The person which created the post.
 * @param post_title: The title of the post.
*******************************/
void create_post(sp_db_t *sp_db, char *user_name, char *post_title);

/******************************
 * create repost() - Create a repost and put it in the social platform's
 *		database.
 *
 * @param sp_db: Social platform's database.
 * @param user_name: The person which created the repost.
 * @param parent_id: The ID of the re(post) at which is the reply.
*******************************/
void create_repost(sp_db_t *sp_db, char *user_name, u_int parent_id);

/******************************
 * get_reposts() - Print on screen all the reposts to a post.
 *		Are taking in consideration also the resposts of a repost
 *		from the post and so forth.
 *
 * @param sp_db: Social platform's database.
 * @param post_id: Id of the post.
*******************************/
void get_reposts(sp_db_t *sp_db, u_int post_id);

/******************************
 * find_common_reposts() - Find the first common repost of 2 reposts.
 *
 * @param sp_db: Social platform's database.
 * @param post_id: ID of original post.
 * @param repost_1: ID of first repost.
 * @param repost_2: ID of second repost.
*******************************/
void
find_common_repost(sp_db_t *sp_db, u_int post, u_int repost_1, u_int repost_2);

/******************************
 * like_post() - Like a (re)post.
 *
 * @param sp_db: Social platform's database.
 * @param username: Name of the user which liked the (re)post.
 * @param post_id: ID of the post
 * @param repost_id: ID of the repost, if it's a repost
 *					 -1, if it isn't
*******************************/
void like_post(sp_db_t *sp_db, char *username, u_int post_id, int repost_id);

/******************************
 * get_likes() - Print the likes's number of a (re)post.
 *
 * @param sp_db: Social platform's database.
 * @param post_id: ID of the post
 * @param repost_id: ID of the repost, if it's a repost
 *					 -1, if it isn't
*******************************/
void get_likes(sp_db_t *sp_db, u_int post_id, int repost_id);

/******************************
 * is_ratio() - Determinate if a post get ratio'd by a repost.
 *
 * @param sp_db: Social platform's database.
 * @param post_id: ID of post.
*******************************/
void is_ratio(sp_db_t *sp_db, u_int post_id);

/******************************
 * delete_post() - Delete a (re)post and all its reposts.
 *
 * @param sp_db: Social platform's database.
 * @param post_id: ID of post.
 * @param repost_id: ID of the repost, if it's a repost
 *					 -1, if it isn't
*******************************/
void delete_post(sp_db_t *sp_db, u_int post_id, int repost_id);

/******************************
 * @brief: Function that handles the calling of every command from task 2.
*******************************/
void handle_input_posts(sp_db_t *sp_db, char *input);

#endif
