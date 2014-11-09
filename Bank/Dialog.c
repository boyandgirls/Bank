#include "Dialog.h"
#include "Database.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int running;
UserInfo currentUser;
MenuItem logoff, exitoption, addaccount, deleteaccount, addcard, deletecard;

int GetCode()
{
	int ch = getch();
	if (ch == 0 || ch == 224)
		ch = 256 + getch();
	return ch;
}

void Menu(MenuItem *menuItems, int n, char *header){
	short current = 0, i, old = 1, esc = 0;
	while (!esc) {
		if (old != current){
			old = current;
			system("cls");
			if (header) printf("%s\n", header);
			for (i = 0; i < n; i++){
				if (current == i) printf("-");
				printf("%s\n", menuItems[i].displayName);
			}
		}

		switch (GetCode()){
		case ARROW_UP:
			if (current > 0) current--;
			break;
		case ARROW_DOWN:
			if (current < n - 1) current++;
			break;
		case ENTER:
			system("cls");
			menuItems[current].action();
			esc = 1;
			break;
		}

	}
}

void LoginDialog(){
	char login[100], password[100];
	printf("Login: ");
	scanf("%s", login);
	printf("Password: ");
	scanf("%s", password);
	currentUser = GetUser(login, password);
	if (currentUser.Role == INVALID){
		printf("User or password are incorrect\n");
		system("pause");
	}
}

void LogOff(){
	currentUser.Role = INVALID;
}

void Exit(){
	running = 0;
}

void AddAccount(){
	char currency[10];
	int clientId;
	printf("Input currency.\n");
	scanf("%s", currency);
	printf("Input client Id.\n");
	scanf("%d", &clientId);
	if (AddAccountToDB(currency, clientId) == 0)
	{
		printf("Account created.\n");
		system("pause");
	}
}

void DeleteAccount(){
	int accountId;
	printf("Input account Id.\n");
	scanf("%d", &accountId);
	if (DeleteAccountFromDB(accountId) == 0)
	{
		printf("Account deleted.\n");
		system("pause");
	}
}

void AddCard(){
	srand(time(0));
	int accountId, cvv = rand() % 1000;
	char ownerName[100], expiredDate[10];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("Input account Id.\n");
	scanf("%d", &accountId);
	printf("Input owner name.\n");
	scanf(" %99[^\n]", ownerName);
	sprintf(expiredDate, "%d.%d.%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1910);
	if (AddCardToDB(accountId, ownerName, expiredDate, cvv) == 0)
	{
		printf("Card created. CVV is %d. Expiration Date is %s\n", cvv, expiredDate);
		system("pause");
	}
}

void DeleteCard() {
	int cardId;
	printf("Input card Id.\n");
	scanf("%d", &cardId);
	if (DeleteCardFromDB(cardId) == 0)
	{
		printf("Card deleted.\n");
		system("pause");
	}
}

void OperatorMenu(){
	int n = 2;
	char buffer[250];
	sprintf(buffer, "%s %s, %s", currentUser.FirstName, currentUser.LastName, "Operator");
	MenuItem *menuItems = malloc(n * sizeof(MenuItem));
	menuItems[n - 2] = logoff;
	menuItems[n - 1] = exitoption;
	Menu(menuItems, n, buffer);
}

void AdministratorMenu(){
	int n = 6;
	char buffer[250];
	sprintf(buffer, "%s %s, %s", currentUser.FirstName, currentUser.LastName, "Administrator");
	MenuItem *menuItems = malloc(n * sizeof(MenuItem));
	menuItems[n - 6] = addcard;
	menuItems[n - 5] = deletecard;
	menuItems[n - 4] = addaccount;
	menuItems[n - 3] = deleteaccount;
	menuItems[n - 2] = logoff;
	menuItems[n - 1] = exitoption;
	Menu(menuItems, n, buffer);
}

void InitializeDialog(){
	running = 1;
	currentUser.Role = INVALID;
	logoff.displayName = "Log Off";
	logoff.action = &LogOff;
	exitoption.displayName = "Exit";
	exitoption.action = &Exit;
	addaccount.displayName = "Add Account";
	addaccount.action = &AddAccount;
	deleteaccount.displayName = "Delete Account";
	deleteaccount.action = &DeleteAccount;
	addcard.displayName = "Add Card";
	addcard.action = &AddCard;
	deletecard.displayName = "Delete Card";
	deletecard.action = &DeleteCard;
}

void Dialog(){
	InitializeDialog();
	while (running){
		system("cls");
		switch (currentUser.Role){
		case ADMINISTRATOR:
			AdministratorMenu();
			break;
		case OPERATOR:
			OperatorMenu();
			break;
		default:
			LoginDialog();
			break;
		}
	}
}