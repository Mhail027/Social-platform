#include "users.h"

u_int get_users_number(char *users_db_path)
{
	// Open the file.
	FILE *db_file = fopen(users_db_path, "r");
	DIE(!db_file, "File didn't open\n");

	// Get the numbers of persons.
	u_int  users_number;
	fscanf(db_file, "%u", &users_number);

	// Close the file.
	fclose(db_file);

	// Return the number of users.
	return users_number;
}

char **users_db_create(char *users_db_path)
{
	// Open the file.
	FILE *db_file = fopen(users_db_path, "r");
	DIE(!db_file, "File didn't open\n");

	// Find the numbers of persons.
	u_int  users_number;
	fscanf(db_file, "%u", &users_number);

	// Create the users database.
	char **users_db = (char **)malloc(sizeof(char *) * users_number);
	DIE(!users_db, "malloc() failed\n");
	char tmp[32];
	for (u_int i = 0; i < users_number; i++) {
		// Read a name.
		fscanf(db_file, "%s", tmp);
		// Add the name in the databse.
		users_db[i] = (char *)malloc(sizeof(char) * users_number);
		DIE(!users_db[i], "malloc() failed\n");
		strcpy(users_db[i], tmp);
	}

	// Close the file.
	fclose(db_file);

	// Return the users's databse.
	return users_db;
}

u_int get_id(u_int users_number, char **users_db, char *name)
{
	// Search the name in the database.
	for (u_int i = 0; i < users_number; ++i)
		if (!strcmp(users_db[i], name))
			return i;

	// The name isn't in the database.
	DIE(1, "get_id() - The given name isn't in users's database.\n");
}

char *get_name(u_int users_number, char **users_db, u_int id)
{
	// Verify if the ID is in the databse.
	DIE(id >= users_number, "get_name() - ID isn't valid.\n");

	// Return the name corresponding to the ID.
	return users_db[id];
}

