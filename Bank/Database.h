#ifndef DATABASE_H
#define DATABASE_H
#include "sqlite3.h"

#define INVALID -1
#define ADMINISTRATOR 0
#define OPERATOR 1

typedef struct UserInfo {
	char FirstName[100];
	char LastName[100];
	int Role;
} UserInfo;

void InitializeDatabase();
void CloseDatabase();
sqlite3 *GetDB();
UserInfo GetUser(const char *login, const char *password);

#endif
