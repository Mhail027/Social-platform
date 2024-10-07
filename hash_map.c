// Copyright Necula Mihail 313CAa 2023-2024
// Copyright Buciu Cosmin 313CAa 2023-2024
#include "hash_map.h"

u_int hash_uint(void *key)
{
	u_int uint_key = *((u_int *)key);

	uint_key = ((uint_key >> 16u) ^ uint_key) * 0x45d9f3b;
	uint_key = ((uint_key >> 16u) ^ uint_key) * 0x45d9f3b;
	uint_key = (uint_key >> 16u) ^ uint_key;

	return uint_key;
}

u_int hash_string(void *key)
{
	u_char *key_string = (u_char *)key;
	u_int hash = 5381;
	int c;

	while ((c = *key_string++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

int compare_function_uints(void *a, void *b)
{
	// Make the needed casts.
	u_int uint_a = *((u_int *)a);
	u_int uint_b = *((u_int *)b);

	// Compare the numbers.
	if (uint_a == uint_b)
		return 0;
	else if (uint_a < uint_b)
		return -1;
	else
		return 1;
}

int compare_function_strings(void *a, void *b)
{
	// Make the needed casts.
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	// Compare the strings.
	return strcmp(str_a, str_b);
}

void key_val_free_function(void *data)
{
	// Make the needed cast.
	info_t *pair = (info_t *)data;

	// Free the memory allocated for the key.
	free(pair->key);
	// Free the memory allocated for the value.
	free(pair->value);
}

hashtable_t *ht_create(u_int hmax, u_int (*hash_function)(void *),
					   int (*compare_function)(void*, void*),
					   void (*key_val_free_function)(void *))
{
	// Create the hashtable.
	hashtable_t *ht = (hashtable_t *)malloc(sizeof(hashtable_t));
	DIE(!ht, "malloc() failed");

	// Initialize the fields of the hashtable.
	ht->size = 0;
	ht->hmax = hmax;
	ht->hash_function = hash_function;
	ht->compare_function = compare_function;
	ht->key_val_free_function = key_val_free_function;

	// Create every list from hashtable.
	ht->buckets = (ll_t **)malloc(hmax * sizeof(ll_t *));
	for (u_int i = 0; i < hmax; ++i)
		ht->buckets[i] = ll_create(sizeof(info_t));

	// Return the hashmap.
	return ht;
}

int ht_has_key(hashtable_t *ht, void *key)
{
	// Find the index of the list where to search.
	u_int index = ht->hash_function(key) % ht->hmax;

	// Take every node from the list and verify the key.
	ll_node_t *curr_node = ht->buckets[index]->head;
	while (curr_node) {
		info_t *pair = (info_t *)curr_node->data;
		if (!ht->compare_function(pair->key, key))
			return 1;
		curr_node = curr_node->next;
	}

	return 0;
}

void *ht_get(hashtable_t *ht, void *key)
{
	// Find the index of the list where to search.
	u_int index = ht->hash_function(key) % ht->hmax;

	// Take every node from the list and verify the key.
	ll_node_t *curr_node = ht->buckets[index]->head;
	while (curr_node) {
		info_t *pair = (info_t *)curr_node->data;
		if (!ht->compare_function(pair->key, key))
			return pair->value;
		curr_node = curr_node->next;
	}

	return NULL;
}

void ht_put(hashtable_t *ht, void *key, u_int key_size,
			void *value, u_int value_size)
{
	// If the key is asociated already with a value, we must
	// remove that entry before to create the new one.
	if (ht_has_key(ht, key))
		ht_remove_entry(ht, key);

	// Create the new pair key-value which will be put in the hasthable.
	info_t *new_pair = (info_t *)malloc(sizeof(info_t));
	DIE(!new_pair, "malloc() failed\n");
	new_pair->key = malloc(key_size);
	DIE(!new_pair->key, "malloc() failed\n");
	memcpy(new_pair->key, key, key_size);
	new_pair->value = malloc(value_size);
	DIE(!new_pair->value, "malloc() failed\n");
	memcpy(new_pair->value, value, value_size);

	// Find the index of the list where will put the pair.
	u_int index = ht->hash_function(key) % ht->hmax;

	// Put the pair in that list.
	ll_t *curr_list = ht->buckets[index];
	ll_add_nth_node(curr_list, curr_list->size, new_pair);

	// Update the parameters of the hasthable.
	ht->size++;

	// Free the unnecesary mermory.
	free(new_pair);
}

void ht_remove_entry(hashtable_t *ht, void *key)
{
	// Find the index of the list where to search.
	u_int index = ht->hash_function(key) % ht->hmax;

	// Take every node from the list and verify the key.
	ll_t *curr_list = ht->buckets[index];
	ll_node_t *curr_node = curr_list->head;
	u_int pos = 0;
	while (curr_node) {
		info_t *d = (info_t *)curr_node->data;
		if (!ht->compare_function(d->key, key)) {
			// Take out the node from the list.
			curr_node = ll_remove_nth_node(curr_list, pos);
			// Free the memory of the pair.
			ht->key_val_free_function(curr_node->data);
			// Free the memory of the node.
			free_ll_node(curr_node);
			// Update the parameters of the hahstable.
			ht->size--;
			// We finished our work.
			return;
		}
		curr_node = curr_node->next;
		pos++;
	}
}

void ht_free(hashtable_t **hashtable)
{
	// Get to the ht.
	hashtable_t *ht = *hashtable;

	// Verify the parameter.
	if (!ht || !ht->buckets) {
		fprintf(stderr, "ht_free() - hashtable isn't valid\n");
		return;
	}

	// Free the memory of every list from array.
	for (u_int i = 0; i < ht->hmax; ++i) {
		ll_node_t *curr_node = ht->buckets[i]->head;
		while (curr_node) {
			ht->key_val_free_function(curr_node->data);
			curr_node = curr_node->next;
		}
		ll_free(&ht->buckets[i]);
	}

	// Free the array of lists.
	free(ht->buckets);

	// Free the hashthable structure's memory.
	free(ht);

	// Lose the address of the hashtable, which
	// doesn't exist anymore.
	*hashtable = NULL;
}

u_int ht_get_size(hashtable_t *ht)
{
	if (!ht)
		return 0;
	return ht->size;
}

u_int ht_get_hmax(hashtable_t *ht)
{
	if (!ht)
		return 0;
	return ht->hmax;
}

