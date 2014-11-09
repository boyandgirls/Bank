#include "Database.h"

sqlite3 *db;

void InitializeDatabase(){
	sqlite3_open("Database\\BankDatabase.db", &db);
}

void CloseDatabase(){
	sqlite3_close(db);
}

UserInfo GetUser(const char *login, const char *password) {
	char *dbpassword, *dblogin;
	UserInfo user;
	user.Role = INVALID;
	char query[300];
	sqlite3_stmt *statement;
	sprintf(query, "SELECT Role, FirstName, LastName FROM User WHERE Login = '%s' AND Password = '%s'", login, password);
	sqlite3_prepare(db, query, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		user.Role = sqlite3_column_int(statement, 0);
		strcpy(user.FirstName, sqlite3_column_text(statement, 1));
		strcpy(user.LastName, sqlite3_column_text(statement, 2));
	}
	sqlite3_finalize(statement);
	return user;
}

int ClientExists(int clientId)
{
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT ClientId FROM Client WHERE ClientId = (?)", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, clientId);
	sqlite3_step(stmt);
	clientId = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);
	if (clientId == 0)
	{
		printf("Error: The client doesn't exist.\n");
		system("pause");
	}
	return clientId;
}

int AccountExists(int accountId)
{
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT AccountId FROM Account WHERE AccountId = (?)", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, accountId);
	sqlite3_step(stmt);
	accountId = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);
	if (accountId == 0)
	{
		printf("Error: The account doesn't exist.\n");
		system("pause");
	}
	return accountId;
}

int CardExists(int cardId) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT CardId FROM Card WHERE CardId = (?)", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, cardId);
	sqlite3_step(stmt);
	cardId = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);
	if (cardId == 0)
	{
		printf("Error: The card doesn't exist.\n");
		system("pause");
	}
	return cardId;
}

int AddAccountToDB(const char *currency, int clientId) {
	if (ClientExists(clientId) != 0)
	{
		sqlite3_stmt *stmt;
		sqlite3_prepare(db, "INSERT INTO Account (CURRENCY, CLIENT_CLIENTID, BALANCE) VALUES ((?), (?), 0)", -1, &stmt, 0);
		sqlite3_bind_text(stmt, 1, currency, -1, 0);
		sqlite3_bind_int(stmt, 2, clientId);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 0;
	}
	return 1;
}

int DeleteAccountFromDB(int accountId) {
	if (AccountExists(accountId) != 0)
	{
		sqlite3_stmt *stmt;
		sqlite3_prepare(db, "DELETE FROM Account WHERE AccountId = (?)", -1, &stmt, 0);
		sqlite3_bind_int(stmt, 1, accountId);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 0;
	}
	return 1;
}

int AddCardToDB(int accountId, char* ownerName, char* date, int cvv) {
	if (AccountExists(accountId) != 0)
	{
		sqlite3_stmt *stmt;
		sqlite3_prepare(db, "INSERT INTO Card (ACCOUNT_ACCOUNTID, CARDOWNERID, EXPIRATIONDATE, CVV, TOTALTRANSACTIONS) VALUES ((?), (?), (?), (?), 0)", -1, &stmt, 0);
		sqlite3_bind_int(stmt, 1, accountId);
		sqlite3_bind_text(stmt, 2, ownerName, -1, 0);
		sqlite3_bind_text(stmt, 3, date, -1, 0);
		sqlite3_bind_int(stmt, 4, cvv);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 0;
	}
	return 1;
}

int DeleteCardFromDB(int cardId)
{
	if (CardExists(cardId) != 0)
	{
		sqlite3_stmt *stmt;
		sqlite3_prepare(db, "DELETE FROM Card WHERE CardId = (?)", -1, &stmt, 0);
		sqlite3_bind_int(stmt, 1, cardId);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 0;
	}
	return 1;
}