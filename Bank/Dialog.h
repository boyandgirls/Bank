#ifndef DIALOG_H
#define DIALOG_H

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