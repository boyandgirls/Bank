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

typedef struct Card {
	int Id;
	int CVV;
	char CardOwnerID[100];
	char ExpirationDate[20];
	int TotalTransactions;
} Card;

typedef struct Account {
	int Id;
	int IsBlocked;
	char Currency[5];
	float Balance;
	int CardAmount;
	Card *Cards;
} Account;

typedef struct Client {
	int Id;
	char FirstName[100];
	char LastName[100];
	int AccountAmount;
	Account *Accounts;
} Client;

Client GetClient(int clientId);
void InitializeDatabase();
void CloseDatabase();
int CreditMoney(int cardId, float amount);
UserInfo GetUser(const char *login, const char *password);
int AddAccountToDB(const char *currency, int clientId);
int DeleteAccountFromDB(int accountId);
int AddCardToDB(int accountId, char* ownerName, char* date, int cvv);
int DeleteCardFromDB(int cardId);
int AccountExists(int accountId);
int ClientExists(int clientId);
int CardExists(int cardId);
Client GetClientCards(int clientId);
Account GetAccountInfoByAccountId(int accountId);
Card GetCardInfoByCardId(int cardId);
Account GetAccountInfoByCardId(int cardId);
int AddClientToDB(char* firstName, char* lastName);
int DeleteClientFromDB(int clientId);
Client GetClientByCardID(int cardId);
int UpdateClientInDB(int clientId, char* firstName, char* lastName);
int ChangeCurrencyInAccount(const char *newcurrency, int clientID);
int ChangeFeeAndQuotesInAccount(int AccountID, int FeeValue, int QuotesValue);
int BlockAccount(int AccountID);
int UnblockAccount(int AccountID);
#endif
