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