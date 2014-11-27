#include "Dialog.h"
#include "Database.h"
#include "Tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int running;
UserInfo currentUser;
MenuItem runtests, logoff, exitoption, addaccount, deleteaccount, addcard, deletecard,
watchclientbyid, credit, debit, watchclientcardsbyid, watchaccountbyaccountid, watchaccountbycardid, 
watchcardbycardid,  addclient, deleteclient, watchclientbycard, updateclient, blockaccount, unblockaccount, changecurrency, changefeeandquotes;;
MenuItem deleteClientTest, addCardTest;

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
	char ownerName[100], expiredDate[11];
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

void WatchAccountInfoByAccountId(){
	int accountId;
	Account account;
	printf("Account Id: ");
	scanf("%d", &accountId);
	account = GetAccountInfoByAccountId(accountId);
	if (account.Id != INVALID)
		printf("Account ID: %d\nCurrency: %s\nBalance: %f\n", account.Id, account.Currency, account.Balance);
	else
		printf("Account not found\n");
	system("pause");
}

void WatchAccountInfoByCardId(){
	int cardId;
	Account account;
	printf("Card Id: ");
	scanf("%d", &cardId);
	account = GetAccountInfoByCardId(cardId);
	if (account.Id != INVALID)
		printf("Account ID: %d\nCurrency: %s\nBalance: %f\n", account.Id, account.Currency, account.Balance);
	else
		printf("Account not found\n");
	system("pause");
}

void WatchCardInfoByCardId(){
	int cardId;
	Card card;
	printf("Card Id: ");
	scanf("%d", &cardId);
	card = GetCardInfoByCardId(cardId);
	if (card.Id != INVALID)
		printf("CardID: %d\nCVV: %d\nCardOwnerID: %s\nExpirationDate: %s\nTotalTransactions: %d\n", card.Id, card.CVV, card.CardOwnerID, card.ExpirationDate, card.TotalTransactions);
	else
		printf("Card not found\n");
	system("pause");
}

void ShowCard(void* card){
	Card* c = (Card*)card;
	printf("CardID: %d\nCVV: %d\nCardOwnerID: %s\nExpirationDate: %s\nTotalTransactions: %d\n", c->Id, c->CVV, c->CardOwnerID, c->ExpirationDate, c->TotalTransactions);
	system("pause");
}

void WatchClientCardsByPassportNumber(){
	int i = 0, j, k = 0, cycle, passportId;
	MenuItem *cards;
	Client client;
	char header[300];
	int CardAmount = 0;
	printf("Passport Number: ");
	scanf("%d", &passportId);
	client = GetClientCards(passportId);
	for (i = 0; i < client.AccountAmount; i++)
		CardAmount += client.Accounts[i].CardAmount;
	cards = malloc(sizeof(MenuItem)*(CardAmount + 1));
	cards[CardAmount].action = &Back;
	cards[CardAmount].args = &cycle;
	cards[CardAmount].displayName = "Back";
	if (client.Id != INVALID){
		sprintf(header, "Client ID: %d\nFirst Name: %s\nLast Name: %s\n", client.Id, client.FirstName, client.LastName);
		cycle = 1;
		for (i = 0; i < client.AccountAmount; i++){
			for (j = 0; j<client.Accounts[i].CardAmount; j++){
				cards[k].displayName = malloc(sizeof(char)* 30);
				cards[k].action = &ShowCard;
				cards[k].args = (void*)(&client.Accounts[i].Cards[j]);
				sprintf(cards[k].displayName, "Card %d", client.Accounts[i].Cards[j].Id);
				k++;
			}
		}
		while (cycle) {
			Menu(cards, client.AccountAmount + 1, header);
		}
	}
	else {
		printf("Client is not found\n");
		system("pause");
	}
}

void AddClient()
{
	int clientId;
	char firstName[120], lastName[120];
	printf("Input client's first name.\n");
	scanf("%s", firstName);
	printf("Input client's last name.\n");
	scanf("%s", lastName);
	if (AddClientToDB(firstName, lastName) == 0)
	{
		printf("Client added to the database.\n");
		system("pause");
	}
}

void DeleteClient()
{
	int clientId;
	printf("Input client Id.\n");
	scanf("%d", &clientId);
	if (DeleteClientFromDB(clientId) == 0)
	{
		printf("Client deleted.\n");
		system("pause");
	}
}

void UpdateClient()
{
	int clientId;
	Client client;
	char firstName[20], lastName[20];
	printf("Input client Id.\n");
	scanf("%d", &clientId);
	printf("Input client's first name.\n");
	scanf("%s", firstName);
	printf("Input client's last name.\n");
	scanf("%s", lastName);
	UpdateClient(clientId, firstName, lastName);
}

void RunTests()
{
	int testsNumber = 2;
	MenuItem * tests = (MenuItem *)malloc(sizeof(MenuItem)*(testsNumber));
	tests[0] = deleteClientTest;
	tests[1] = addCardTest;
	for (int i = 0; i < testsNumber; i++)
	{		
		int result = (int)tests[i].action(NULL);
		printf("%s: %s\n", tests[i].displayName, result == SUCCESS ? "Successful" : "Failed");
	}	
	system("pause");
}

void WatchClientByCard()
{
	int i = 0, cycle, CardId;
	MenuItem *accounts;
	Client client;
	char header[300];
	printf("Card ID: ");
	scanf("%d", &CardId);
	if(CardExists(CardId)!=0)
	{
		client = GetClientByCardID(CardId);
		accounts = (MenuItem *)malloc(sizeof(MenuItem)*(client.AccountAmount + 1));
		accounts[client.AccountAmount].action = &Back;
		accounts[client.AccountAmount].args = &cycle;
		accounts[client.AccountAmount].displayName = "Back";
		if (client.Id != INVALID){
			sprintf(header, "Client ID: %d\nFirst Name: %s\nLast Name: %s\n", client.Id, client.FirstName, client.LastName);
			cycle = 1;
			for (i = 0; i < client.AccountAmount; i++){
				accounts[i].displayName = (char *)malloc(sizeof(char) * 30);
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
	else
	{
		printf("Client is not found\n");
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
	int n = 20;
	char buffer[250];
	sprintf(buffer, "%s %s, %s", currentUser.FirstName, currentUser.LastName, "Administrator");
	MenuItem *menuItems = malloc(n * sizeof(MenuItem));
	menuItems[0] = addcard;
	menuItems[1] = deletecard;
	menuItems[2] = addaccount;
	menuItems[3] = deleteaccount;
	menuItems[4] = watchclientbyid;
	menuItems[5] = watchclientcardsbyid;
	menuItems[6] = watchaccountbyaccountid;
	menuItems[7] = watchaccountbycardid;
	menuItems[8] = watchcardbycardid;
	menuItems[9] = addclient;
	menuItems[10] = deleteclient;
	menuItems[11] = watchclientbycard;
	menuItems[12] = updateclient;
	menuItems[13] = runtests;
	menuItems[14] = blockaccount;
	menuItems[15] = unblockaccount;
	menuItems[16] = changecurrency;
	menuItems[17] = changefeeandquotes;
	menuItems[n - 2] = logoff;
	menuItems[n - 1] = exitoption;
	Menu(menuItems, n, buffer);
}

void InitializeTests(){
	deleteClientTest.displayName = "Delete Client Test";
	deleteClientTest.action = &DeleteClientTest;
	addCardTest.displayName = "Add Card Test";
	addCardTest.action = &AddCardTest;
}

void ChangeFeeAndQuotes(){
	int Fee, Quota, AccountId;
	printf("Input account ID\n");
	scanf("%d", AccountId);
	printf("Input fee value\n");
	scanf("%d", Fee);
	printf("Input quote value\n");
	scanf("%d", Quota);
	if (ChangeFeeAndQuotesInAccount(AccountId, Fee, Quota) == 0)
		printf("Successful\n");
	else
	{
		printf("Error\n");
	}
}

void ChangeCurrency(){
	char Currency[5];
	int ClientId;
	printf("Input value of currency\n");
	scanf("%s", Currency);
	printf("Input client ID\n");
	scanf("%d", ClientId);
	if (ChangeCurrencyInAccount(Currency, ClientId) == 0)
		printf("Currency changed succsesful\n");
	else printf("Error in changing currency\n");
}

void BlockA(){
	int ClientId;
	printf("Input client ID\n");
	scanf("%d", ClientId);
	if (BlockAccount == 0)
	{
		printf("Account is blocked\n");
	}
	else printf("Error in blocking account\n");
}

void UnblockA(){
	int ClientID;
	printf("Input client ID\n");
	scanf("%d", ClientID);
	if (UnblockAccount == 0)
	{
		printf("Account is blocked\n");
	}
	else printf("Error in blocking account\n");
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
	watchclientcardsbyid.action = &WatchClientCardsByPassportNumber;
	watchclientcardsbyid.displayName = "Watch client cards";
	watchaccountbyaccountid.action = &WatchAccountInfoByAccountId;
	watchaccountbyaccountid.displayName = "Watch account info by account id";
	watchaccountbycardid.action = &WatchAccountInfoByCardId;
	watchaccountbycardid.displayName = "Watch account info by card id";
	watchcardbycardid.action = &WatchCardInfoByCardId;
	watchcardbycardid.displayName = "Watch card info by card id";
	addclient.displayName = "Add Client";
	addclient.action = &AddClient;
	deleteclient.displayName="Delete Client";
	deleteclient.action = &DeleteClient;
	watchclientbycard.displayName="Watch client by card";
	watchclientbycard.action= &WatchClientByCard;
	updateclient.action= &UpdateClient;
	updateclient.displayName="Update client";
	runtests.displayName = "Run tests";
	runtests.action = &RunTests;
	blockaccount.action = &BlockA;
	blockaccount.displayName = "Block Account";
	unblockaccount.action = &UnblockA;
	unblockaccount.displayName = "Unblock Account";
	changecurrency.action = &ChangeCurrency;
	changecurrency.displayName = "Change currency in Account";
	changefeeandquotes.action = &ChangeFeeAndQuotes;
	changefeeandquotes.displayName = "Change fee and quotes in Account";
	InitializeTests();
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