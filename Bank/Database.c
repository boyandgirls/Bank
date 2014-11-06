#include "Database.h"

sqlite3 *db;

void InitializeDatabase(){
	int rc;
	rc = sqlite3_open("Database\\BankDatabase.db", &db);
}

void CloseDatabase(){
	sqlite3_close(db);
}

sqlite3 *GetDB(){
	return db;
}

UserInfo GetUser(const char *login, const char *password) {
	char *dbpassword, *dblogin;
	UserInfo user;
	user.Role = -1;
	int s, role;
	sqlite3_stmt *statement;
	sqlite3_prepare(GetDB(), "SELECT Role, FirstName, LastName, Login, Password FROM User", -1, &statement, 0);
	while ((s = sqlite3_step(statement)) == SQLITE_ROW) {
		role = sqlite3_column_int(statement, 0);
		dbpassword = sqlite3_column_text(statement, 4);
		dblogin = sqlite3_column_text(statement, 3);
		if (role >= 0 && !strcmp(login, dblogin) && !strcmp(password, dbpassword)){
			user.Role = role;
			strcpy(user.FirstName, sqlite3_column_text(statement, 1));
			strcpy(user.LastName, sqlite3_column_text(statement, 2));
			break;
		}
	}
	sqlite3_finalize(statement);
	return user;
}