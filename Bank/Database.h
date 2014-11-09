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
UserInfo GetUser(const char *login, const char *password);
int AddAccountToDB(const char *currency, int clientId);
int DeleteAccountFromDB(int accountId);
int AddCardToDB(int accountId, char* ownerName, char* date, int cvv);
int DeleteCardFromDB(int cardId);
int AccountExists(int accountId);
int ClientExists(int clientId);
int CardExists(int cardId);

#endif
