Team Homework 3 SDA: <br>
 mihail.necula<br>
 cosmin.buciu1904<br>

## Readme Homework 3

### The description of the program ###

We have 6 main files: "social_platform.h", "users.c", "friends.c", <br>
"posts.c", "feed.c", "social_media.c". In the other files: "list.c", <br>
"queue.c", "hash_map.c", "graph.c", we have implemented some data <br>
structures. <br>

***A. SOCIAL_PLATFORM.H***

The file with .c is empty. If we remove it, the makefile gives an error. <br>
Because doesn't ocupy much space and didn't find other solution, we let <br>
file with .c to exist. <br> 

What truly matters is in the file with .h. There is definied a structure <br>
in which to save the infromation about our social platform. The fields of <br>
it are the following:
- users_number - the number of users which have accounts on our social <br>
platform
- users_db - a matrix with the names of users (a name can't have space <br>
in it; a name is unique) <br>
- friends_graph - an undirected graph in which we are remember the <br>
relations of friendships between users; every user has an ID (which <br>
is a unique number); to be easier, we represent every user through a <br>
number in the graph; we'll speak more about the user - ID system in <br>
the next chapter <br>
- posts_db - an hashtable (french joke) in which we save every post and <br>
repost; ID of the post -> key, the post -> value ; the system post - ID <br>
and the structure of a post are explained in chapter D <br>
- max_post_id - the maximum id of a post (the id of a post is a number)
- posts_graph - a directed graph in which a (re)post has links to all its <br>
reposts; again, we use a system post-ID as in the friends graph; more <br>
about this in chapter D (:D)

***B. USERS.C***

Welcome in the chapter B, a magic place, where the story truly starts. <br>
Firstly, we'll explaim the system user - ID. To every user, which is <br>
identified through a name, will assing also a number, to be easier to work <br>
with them. The IDs start from 0 and are natural numbers. <br>

In "users.c" we have 4 functions:
- get_users_number() - we give it the file with the all the users <br>
and returns the number of them
- user_db_create() - we give it the same file as at previuos functiuon; <br>
the difference is that this function returns a matrix with all the <br>
usernames; the id of an user will be the line's index from the matrix <br>
were his/her name is
- get_id() - return the name of a user knowing the id
- get_name() - return the id of a user, knowing the name

***C. FRIENDS.C***

Now, in the created online world, the users are starting to interact <br>
between them. Friendships, enmities and love stories begins to take <br>
place. In this file, we'll occupy with the realations of friendhips. <br>
To do this, we hqave the following functions:
- add friendship() - add an edge betwwen 2 users in friends graph
- remove_friendship() - remove the edge between 2 users in friends graph
- are_friends() - verify if 2 users are friends
- find_distance() - find the minimum number of person with which a user <br>
must interact to get to other user
- find_suggestions() - find the friends of the friends of a user, with <br>
with which the user is not already friend
- find_common_friends() - find the common friends of 2 users (the 2 users <br>
are not taken in consideration)
- find_number_of_friends() - find the number of friends of a user
- find_most_popular() - find the persons with the most friends and which <br>
has a ggod relation with a user (a user has a ggod relation with himself)
- handle_input_friends() - handle the commands which are related with <br>
the friendship

***D. POSTS.C***

Any user can post a message and anyone can make a repost to it. The <br>
information of a (re)post are saved in a structure with the next fields: <br>
- id - id of the (re)post (any post is associated with a positive and <br>
natural number) <br>
- parent_id - the id of the (re)post to whcih is the repost; 0, if <br>
we have a post
- title - if we have a post
- user_id - the id of the user which created the (re)post
- likes - hashtable in which we save the names of the users which liked <br>
the (re)post

To manage the posts, we built the next functions:
- post_free() - deallocate the memory of a post
- key_post_free_function() - free the memory of a pair from a hashtable, <br>
in which the value is a post 
- create_posts_db() - create a hashtable in which: key - id of post, <br>
value - the post
- create_post() - create a post and add it in the posts_db
- create_repost() - create a repost and add it in posts_db
- get_reposts() - print all the reposts of a (re)post
- find common repost() - find the first common (re)posts of 2 reposts 
- like_post() - a user like a post; if the user already liked the post, <br>
th operaion is transformed in "unlike the post" 
- get_likes() - get the number of likes of a (re)post
- is_ratio() - verify if exists repost which has more likes than the <br>
original post
- delete_post() - delete a (re)post with all its reposts
- handple_input_posts() - handle the commands which are related with <br>
the posts

***E. FEED.C***

Through the following functions we manage the feeds of the persons: <br>
- is_post_in_feed() - verify if a post is in the feed of a user
- show_feed() - show the first posts from the feed of a user
- show_profile() - print the post and the reposts of a user
- show_friends_that_reposted() - print all the friends of a user which <br>
replied to a post
- show_common_group() - determinate the biggest group of friends in <br>
which is a user
- handle_input_friend() - handle the commands which are related with <br>
the feed

***F. SOCIAL_MEDIA.C***

Unify all the files which was mentioned before.

