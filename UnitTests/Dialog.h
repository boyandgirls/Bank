#ifndef DIALOG_H
#define DIALOG_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define ARROW_UP  328
#define ARROW_DOWN  336
#define ENTER 13

typedef struct MenuItem {
	char *displayName;
	void*(*action) (void*);
	void* args;
} MenuItem;

void Dialog();

#endif