#include "Dialog.h"
#include "Database.h"
#include <stdio.h>
#include <stdlib.h>

int running;
UserInfo currentUser;
MenuItem logoff, exitoption;

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
	int n = 2;
	char buffer[250];
	sprintf(buffer, "%s %s, %s", currentUser.FirstName, currentUser.LastName, "Administrator");
	MenuItem *menuItems = malloc(n * sizeof(MenuItem));
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