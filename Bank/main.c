#include <stdio.h>
#include "Database.h"
#include "Dialog.h"

int main(){
	InitializeDatabase();
	Dialog();
	CloseDatabase();
	return 0;
}
