#include <stdio.h>
#include "Dialog.h"
#include "Database.h"
#include "Tests.h"

FILE * f;
int ok = 1;
MenuItem deleteClientTest, addCardTest, getClientTest, creditTest;

void initializeTests(){
	deleteClientTest.displayName = "Delete Client Test";
	deleteClientTest.action = &DeleteClientTest;
	addCardTest.displayName = "Add Card Test";
	addCardTest.action = &AddCardTest;
	getClientTest.displayName = "Get Client Test";
	getClientTest.action = &GetClientTest;
	creditTest.action = &CreditTest;
	creditTest.displayName = "Credit Test";
}

void runTests()
{
	int testsNumber = 4;
	MenuItem * tests = (MenuItem *)malloc(sizeof(MenuItem)*(testsNumber));
	tests[0] = deleteClientTest;
	tests[1] = addCardTest;
	tests[2] = getClientTest;
	tests[3] = creditTest;	
	for (int i = 0; i < testsNumber; i++)
	{		
		int result = (int)tests[i].action(NULL);
		if (!result) ok = 0;
		fprintf(f, "<p>%s: %s</p>\n", tests[i].displayName, result == SUCCESS ? "Passed" : "Failed");
	}		
}

int main()
{
	InitializeDatabase();
	fopen_s(&f, "testResults.html", "wt");
	initializeTests();
	runTests();
	fclose(f);
	CloseDatabase();
	if (!ok) return -1;
	return 0;
}
