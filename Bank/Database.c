#include "Database.h"

sqlite3 *db;

void InitializeDatabase(){
	sqlite3_open("Database\\BankDatabase.db", &db);
}

void CloseDatabase(){
	sqlite3_close(db);
}

void IncreaseTransactions(int cardId) {
	char query[300];
	sqlite3_stmt *statement;
	sprintf(query, "UPDATE Card SET TotalTransactions = TotalTransactions + 1 WHERE CardId = %d", cardId);
	sqlite3_prepare(db, query, -1, &statement, 0);
	sqlite3_step(statement);
	sqlite3_finalize(statement);
}

int CreditMoney(int cardId, float amount) {
	char query1[300], query2[300];
	sqlite3_stmt *statement;
	float balance;
	int accountId;
	sprintf(query1, "SELECT Balance, AccountId FROM Card, Account WHERE Card.Account_AccountId = Account.AccountId AND CardId = %d", cardId);
	sqlite3_prepare(db, query1, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		balance = sqlite3_column_double(statement, 0);
		balance += amount;
		if (balance < 0) return 0;
		accountId = sqlite3_column_int(statement, 1);
	}
	else return 0;
	sqlite3_finalize(statement);
	sprintf(query2, "UPDATE Account SET Balance = %f WHERE AccountId = %d", balance, accountId);
	sqlite3_prepare(db, query2, -1, &statement, 0);
	sqlite3_step(statement);
	sqlite3_finalize(statement);
	IncreaseTransactions(cardId);
	return 1;
}

Client GetClient(int clientId){
	Client client;
	Account account;
	int i = 0;
	client.Id = INVALID;
	char query[300], query2[300], query3[300];
	sqlite3_stmt *statement;
	sprintf(query, "SELECT FirstName, LastName FROM Client WHERE ClientId = %d", clientId);
	sqlite3_prepare(db, query, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		client.Id = clientId;
		strcpy(client.FirstName, sqlite3_column_text(statement, 0));
		strcpy(client.LastName, sqlite3_column_text(statement, 1));
	}
	sqlite3_finalize(statement);

	sprintf(query2, "SELECT COUNT(*) FROM Account WHERE Client_ClientId = %d", clientId);
	sqlite3_prepare(db, query2, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) client.AccountAmount = sqlite3_column_int(statement, 0);
	sqlite3_finalize(statement);

	if (client.AccountAmount) client.Accounts = malloc(sizeof(account)*client.AccountAmount);

	sprintf(query3, "SELECT AccountId, Currency, Balance FROM Account WHERE Client_ClientId = %d", clientId);
	sqlite3_prepare(db, query3, -1, &statement, 0);
	while (sqlite3_step(statement) == SQLITE_ROW) {
		account.Id = sqlite3_column_int(statement, 0);
		strcpy(account.Currency, sqlite3_column_text(statement, 1));
		account.Balance = sqlite3_column_double(statement, 2);
		client.Accounts[i] = account;
	    i++;
	}
	sqlite3_finalize(statement);
	return client;
}

UserInfo GetUser(const char *login, const char *password) {
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

Client GetClientCards(int clientId){
	Client client = GetClient(clientId);
	Card card;
	int i, j;
	char query[300], query2[300];
	sqlite3_stmt *statement;
	for (i = 0; i<client.AccountAmount; i++){
		j = 0;
		sprintf(query, "SELECT COUNT(*) FROM Card WHERE Account_AccountId = %d", client.Accounts[i].Id);
		sqlite3_prepare(db, query, -1, &statement, 0);
		if (sqlite3_step(statement) == SQLITE_ROW) client.Accounts[i].CardAmount = sqlite3_column_int(statement, 0);
		sqlite3_finalize(statement);

		if (client.Accounts[i].CardAmount)
			client.Accounts[i].Cards = malloc(sizeof(card)*client.Accounts[i].CardAmount);

		sprintf(query2, "SELECT CardID, CVV, CardOwnerID, ExpirationDate, TotalTransactions FROM Card WHERE Account_AccountId = %d", client.Accounts[i].Id);
		sqlite3_prepare(db, query2, -1, &statement, 0);
		while (sqlite3_step(statement) == SQLITE_ROW) {
			card.Id = sqlite3_column_int(statement, 0);
			card.CVV = sqlite3_column_int(statement, 1);
			strcpy(card.CardOwnerID, sqlite3_column_text(statement, 2));
			strcpy(card.ExpirationDate, sqlite3_column_text(statement, 3));
			card.TotalTransactions = sqlite3_column_int(statement, 4);
			client.Accounts[i].Cards[i] = card;
			j++;
		}
		sqlite3_finalize(statement);
	}
	return client;
}

Account GetAccountInfoByAccountId(int accountId){
	Account account;
	int i = 0;
	account.Id = INVALID;
	char query[300];
	sqlite3_stmt *statement;
	sprintf(query, "SELECT Currency,Balance FROM Account WHERE AccountId = %d", accountId);
	sqlite3_prepare(db, query, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		account.Id = accountId;
		strcpy(account.Currency, sqlite3_column_text(statement, 0));
		account.Balance = sqlite3_column_int(statement, 1);
	}
	sqlite3_finalize(statement);
	return account;
}

Card GetCardInfoByCardId(int cardId){
	Card card;
	int i = 0;
	card.Id = INVALID;
	char query[300];
	sqlite3_stmt *statement;
	sprintf(query, "SELECT CardID, CVV, CardOwnerID, ExpirationDate, TotalTransactions FROM Card WHERE CardID = %d", cardId);
	sqlite3_prepare(db, query, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		card.Id = sqlite3_column_int(statement, 0);
		card.CVV = sqlite3_column_int(statement, 1);
		strcpy(card.CardOwnerID, sqlite3_column_text(statement, 2));
		strcpy(card.ExpirationDate, sqlite3_column_text(statement, 3));
		card.TotalTransactions = sqlite3_column_int(statement, 4);
	}
	sqlite3_finalize(statement);
	return card;
}

Account GetAccountInfoByCardId(int cardId){
	Account account;
	int i = 0;
	account.Id = INVALID;
	char query[300], query2[300];
	sqlite3_stmt *statement;
	sprintf(query, "SELECT Account_AccountId FROM Card WHERE CardID = %d", cardId);
	sqlite3_prepare(db, query, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		account.Id = sqlite3_column_int(statement, 0);
	}
	sqlite3_finalize(statement);
	sprintf(query2, "SELECT Currency,Balance FROM Account WHERE AccountId = %d", account.Id);
	sqlite3_prepare(db, query2, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		strcpy(account.Currency, sqlite3_column_text(statement, 0));
		account.Balance = sqlite3_column_int(statement, 1);
	}
	sqlite3_finalize(statement);
	return account;
}

int AddClientToDB(int clientId, char* firstName, char* lastName){
	if (ClientExists(clientId) != 0)
	{
		sqlite3_stmt *stmt;
		sqlite3_prepare(db, "INSERT INTO Client (CLIENTID, FIRSTNAME, LASTNAME) VALUES ((?), (?), (?))", -1, &stmt, 0);
		sqlite3_bind_int(stmt, 1, clientId);
		sqlite3_bind_text(stmt, 2, firstName, -1, 0);
		sqlite3_bind_text(stmt, 3, lastName, -1, 0);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 0;
	}
	return 1;
}

int DeleteClientFromDB(int clientId)
{
	if (ClientExists(clientId) != 0)
	{
		sqlite3_stmt *stmt;
		sqlite3_prepare(db, "DELETE FROM Client WHERE CardId = (?)", -1, &stmt, 0);
		sqlite3_bind_int(stmt, 1, clientId);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 0;
	}
	return 1;
}

Client GetClientByCardID(int cardId)
{
	sqlite3_stmt *statement;
	int ID=0;
	Client client;
	Account account;
	int i = 0;
	client.Id = INVALID;
	char query[300], query2[300], query3[300];
	sprintf(query, "SELECT Client.ClientId, Client.FirstName, Client.LastName FROM Client INNER JOIN Account ON Client.ClientId=Account.Client_ClientId INNER JOIN Card ON Card.Account_AccountId=Account.AccountId WHERE Card.CardId = %d", cardId);
	sqlite3_prepare(db, query, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) {
		client.Id = atoi((char *)sqlite3_column_text(statement,0));
		strcpy(client.FirstName, (char *)sqlite3_column_text(statement,1));
		strcpy(client.LastName, (char *)sqlite3_column_text(statement, 2));
		ID=client.Id;
	}
	sqlite3_finalize(statement);

	sprintf(query2, "SELECT COUNT(*) FROM Account WHERE Client_ClientId = %d", ID);
	sqlite3_prepare(db, query2, -1, &statement, 0);
	if (sqlite3_step(statement) == SQLITE_ROW) client.AccountAmount = sqlite3_column_int(statement, 0);
	sqlite3_finalize(statement);

	if (client.AccountAmount) client.Accounts = (Account*)malloc(sizeof(account)*client.AccountAmount);

	sprintf(query3, "SELECT AccountId, Currency, Balance FROM Account WHERE Client_ClientId = %d", ID);
	sqlite3_prepare(db, query3, -1, &statement, 0);
	while (sqlite3_step(statement) == SQLITE_ROW) {
		account.Id = sqlite3_column_int(statement, 0);
		strcpy(account.Currency, (char *)sqlite3_column_text(statement, 1));
		account.Balance = sqlite3_column_double(statement, 2);
		client.Accounts[i] = account;
	    i++;
	}
	sqlite3_finalize(statement);
	return client;
}

int UpdateClientInDB(int clientId, char* firstName, char* lastName)
{
	if (ClientExists(clientId) != 0)
	{
		sqlite3_stmt *stmt;
		char *query="UPDATE CLIENT SET FIRSTNAME = (?), LASTNAME = (?) WHERE CLIENTID=", *number;
		itoa(clientId, number, 10);
		sqlite3_prepare(db, strcat(query, number), -1, &stmt, 0);
		sqlite3_bind_text(stmt, 1, firstName, -1, 0);
		sqlite3_bind_text(stmt, 2, lastName, -1, 0);
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
		return 0;
	}
	return 1;
}