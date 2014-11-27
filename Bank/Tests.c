#include "Tests.h"
#include "Database.h"

int DeleteClientTest(){	
	return SUCCESS;
}

int AddCardTest(){
	AddCardToDB(2, "Mr. Jack Michaelson", "01.12.2020", 537);
	if (CardExists(2)!=0)
	{
		return SUCCESS;
	}
	else
	{
		return FAILURE;
	}
}