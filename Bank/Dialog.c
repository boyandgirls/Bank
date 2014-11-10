#include "Dialog.h"
#include "Database.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int running;
UserInfo currentUser;
MenuItem logoff, exitoption, addaccount, deleteaccount, addcard, deletecard, watchclientbyid, credit, debit;

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
			menuItems[current].action(menuItems[current].args);
			esc = 1;
			break;
		}

	}
}

void Back(int *cycle){
	*cycle = 0;
}

void Credit(void *m){
	int cardId;
	int mul = (int)m;
	float amount;
	printf("Card ID: ");
	scanf("%d", &cardId);
	printf("Amount: ");
	scanf("%f", &amount);
	if (CreditMoney(cardId, mul*amount)){
		printf("Transaction succeed\n");
	}
	else {
		printf("Transaction failed\n");
	}
	system("pause");
}

void ShowAccount(void* account){
	Account* acc = (Account*)account;
	printf("Account ID: %d\nCurrency: %s\nBalance: %f\n", acc->Id, acc->Currency, acc->Balance);
	system("pause");
}

void WatchClientByPassportNumber(){
	int i = 0, cycle, passportId;
	MenuItem *accounts;
	Client client;
	char header[300];
	printf("Passport Number: ");
	scanf("%d", &passportId);
	client = GetClient(passportId);
	accounts = malloc(sizeof(MenuItem)*(client.AccountAmount + 1));
	accounts[client.AccountAmount].action = &Back;
	accounts[client.AccountAmount].args = &cycle;
	accounts[client.AccountAmount].displayName = "Back";
	if (client.Id != INVALID){
		sprintf(header, "Client ID: %d\nFirst Name: %s\nLast Name: %s\n", client.Id, client.FirstName, client.LastName);
		cycle = 1;
		for (i = 0; i < client.AccountAmount; i++){
			accounts[i].displayName = malloc(sizeof(char) * 30);
			accounts[i].action = &ShowAccount;
			accounts[i].args = (void*)(&client.Accounts[i]);
			sprintf(accounts[i].displayName, "Account %d", client.Accounts[i].Id);
		}

		while (cycle) {
			Menu(accounts, client.AccountAmount + 1, header);
		}
	}
	else {
		printf("Client is not found\n");
		system("pause");
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
	int n = 4;
	char buffer[250];
	sprintf(buffer, "%s %s, %s", currentUser.FirstName, currentUser.LastName, "Operator");
	MenuItem *menuItems = malloc(n * sizeof(MenuItem));
	menuItems[0] = credit;
	menuItems[1] = debit;
	menuItems[n - 2] = logoff;
	menuItems[n - 1] = exitoption;
	Menu(menuItems, n, buffer);
}

void AdministratorMenu(){
	int n = 7;
	char buffer[250];
	sprintf(buffer, "%s %s, %s", currentUser.FirstName, currentUser.LastName, "Administrator");
	MenuItem *menuItems = malloc(n * sizeof(MenuItem));
	menuItems[0] = addcard;
	menuItems[1] = deletecard;
	menuItems[2] = addaccount;
	menuItems[3] = deleteaccount;
	menuItems[4] = watchclientbyid;
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
	watchclientbyid.action = &WatchClientByPassportNumber;
	watchclientbyid.displayName = "Watch client";
	credit.action = &Credit;
	credit.args = 1;
	credit.displayName = "Credit";
	debit.action = &Credit;
	debit.args = -1;
	debit.displayName = "Debet";
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