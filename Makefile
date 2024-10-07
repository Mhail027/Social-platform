CC=gcc
CFLAGS=-Wall -Wextra -Werror -g

.PHONY: build clean

all: build

build: friends posts feed

LIST = list
HASH_MAP = hash_map
QUEUE = queue
GRAPH = graph
USERS = users
SP = social_platform

$(LIST).o: $(LIST).c $(LIST).h
	$(CC) $(CFLAGS) $^ -c

$(QUEUE).o: $(QUEUE).c $(QUEUE).h
	$(CC) $(CFLAGS) $^ -c

$(HASH_MAP).o: $(HASH_MAP).c $(HASH_MAP).h
	$(CC) $(CFLAGS) $^ -c

$(GRAPH).o: $(GRAPH).c $(GRAPH).h
	$(CC) $(CFLAGS) $^ -c

$(USERS).o: $(USERS).c $(USERS).h
	$(CC) $(CFLAGS) $^ -c

$(SP).o: $(SP).c $(SP).h
	$(CC) $(CFLAGS) $^ -c

# Any other object file that is needed for the other files
# add it here, separated by space
UTILS = $(LIST).o $(QUEUE).o $(HASH_MAP).o $(GRAPH).o $(USERS).o $(SP).o 

friends: $(UTILS) friends.o posts.o feed.o social_media_friends.o
	$(CC) $(CFLAGS) -o $@ $^

posts: $(UTILS) friends.o posts.o feed.o social_media_posts.o
	$(CC) $(CFLAGS) -o $@ $^
	
feed: $(UTILS) posts.o friends.o feed.o social_media_feed.o
	$(CC) $(CFLAGS) -o $@ $^

social_media_friends.o:
	$(CC) $(CFLAGS) -c -D TASK_1 -o $@ social_media.c

social_media_posts.o:
	$(CC) $(CFLAGS) -c -D TASK_2 -o $@ social_media.c

social_media_feed.o:
	$(CC) $(CFLAGS) -c -D TASK_1 -D TASK_2 -D TASK_3 -o $@ social_media.c

clean:
	rm -rf *.o friends posts feed
	rm -f *.h.gch