#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_MODE 0
#define NAME_MODE 1
#define AUTHOR_MODE 2
#define UPDATE_BOOK_NAME 1
#define UPDATE_AUTHOR 2

int selectedMode = ID_MODE;

typedef struct
{
	int id;
	char bookName[20];
	char author[30];
} Book;

typedef struct n
{
	Book book;
	struct n* next;
} Node;

Node* root = NULL;

void menu(void);
Node* readFromTextFile(void);
void print(void);
void createInfo(void);
Node* addToList(Book, int);
void deleteInfo(void);
Node* deleteFromList(int);
void updateInfo(void);
int updateCheck(int);
Node* update(int, int);
Node* sortForID(void);
Node* sortForBookName(void);
Node* sortForAuthor(void);
void writeToTextFile(void);

int main()
{
	root = readFromTextFile();
    printf("Welcome to the Library Program !!!\n----------------------------------");
	menu();
	return 0;
}

void menu()
{
	while(1)
    {
        int choice;
        printf("\nLIBRARY PROGRAM\n----------------\n1 - List all books\n2 - Update a book\n3 - Create a new book\n4 - Delete a book\n");
        printf("5 - Sort all books according to id\n6 - Sort all books according to book name\n");
        printf("7 - Sort all books according to author name\n8 - Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
            	print();
                break;
            case 2:
            	updateInfo();
                break;
            case 3:
            	createInfo();
                break;
            case 4:
           		deleteInfo();
                break;
            case 5:
            	selectedMode = ID_MODE;
            	if(root == NULL)
            		printf("\nThere is no book but if you want to add a book, book will be added ID-sorted.\n");
                else
                	root = sortForID();
                break;
            case 6:
            	selectedMode = NAME_MODE;
            	if(root == NULL)
            		printf("\nThere is no book but if you want to add a book, book will be added book name-sorted.\n");
                else
                	sortForBookName();
                break;
            case 7:
            	selectedMode = AUTHOR_MODE;
            	if(root == NULL)
            		printf("\nThere is no book but if you want to add a book, book will be added author name-sorted.\n");
                else
                	sortForAuthor();
                break;
            case 8:
            	if(root != NULL)
            		sortForID();
        		writeToTextFile();
            	printf("\nRead more books :) Bye !\n");
                return;
        }
    }
}

Node* readFromTextFile()
{
	FILE* stream;
	Node* iter;
	Node* prev = NULL;
	int ctr = 0;
	if(stream = fopen("library", "r"))
	{
		fseek(stream, 0, SEEK_END);
		int len = ftell(stream);
		if(len != 0)
		{
			rewind(stream);
			while(!feof(stream))
			{
				if(ctr == 0)
				{
					root = (Node*)malloc(sizeof(Node));
					fscanf(stream, "%d %s %s", &root -> book.id, root -> book.bookName, root -> book.author);
					root -> next = NULL;
					iter = root;
				}
				else
				{
					if(ctr == 1)
						prev = root;
					else
						prev = iter;
					iter -> next = (Node*)malloc(sizeof(Node));
					iter = iter -> next; 
					fscanf(stream, "%d %s %s", &iter -> book.id, iter -> book.bookName, iter -> book.author);
					iter -> next = NULL; 
				}
				ctr++;
			}
			iter -> next = NULL;
			//deleting last node because there is a problem with reading the text file...
			prev -> next = iter -> next;
			free(iter);
		}
		fclose(stream);
	}
	return root;
}

void print()
{
	if(root == NULL)
	{
		printf("\nThere is no book :(\n");
		return;
	}
	if(selectedMode == ID_MODE)
		printf("\nSorted For ID\n--------------");
	else if(selectedMode == NAME_MODE)
		printf("\nSorted for book names\n---------------------");
	else if(selectedMode == AUTHOR_MODE)
		printf("\nSorted for author names\n-----------------------");
	Node* iter = root;
	while(iter != NULL)
	{
		if(iter == root)
			printf("\n%d %s %s\n", iter -> book.id, iter -> book.bookName, iter -> book.author);
		else
			printf("%d %s %s\n", iter -> book.id, iter -> book.bookName, iter -> book.author);
		iter = iter -> next;
	}
}

void createInfo()
{
	Book book;
    printf("\nPlease enter the book ID: ");
    scanf("%d", &book.id);
    printf("Please enter the book name: ");
    scanf("%s", book.bookName);
    printf("Please enter the author's name: ");
    scanf("%s", book.author);
    root = addToList(book, selectedMode);
}

Node* addToList(Book b, int mode)
{
	Node* iter = root;
	Node* temp = NULL;
	if(mode == ID_MODE)
	{
		if(root == NULL)
		{
			root = (Node*)malloc(sizeof(Node));
			root -> book = b;
			root -> next = NULL;
			iter = root;
			return root;
		}
		else if(b.id < root -> book.id)
		{
			temp = (Node*)malloc(sizeof(Node));
			temp -> book = b;
			temp -> next = root;
			root = temp;
			return root;	
		}
		while(iter -> next != NULL && iter -> next -> book.id < b.id)
			iter = iter -> next;
		temp = (Node*)malloc(sizeof(Node));
		temp -> book = b;
		temp -> next = iter -> next;
		iter -> next = temp;
		return root;
	}
	else if(mode == NAME_MODE)
	{
		if(root == NULL)
		{
			root = (Node*)malloc(sizeof(Node));
			root -> book = b;
			root -> next = NULL;
			iter = root;
			return root;
		}
		else if(strcmp(root -> book.bookName, b.bookName) > 0)
		{
			temp = (Node*)malloc(sizeof(Node));
			temp -> book = b;
			temp -> next = root;
			root = temp;
			return root;	
		}
		while(iter -> next != NULL && strcmp(iter -> next -> book.bookName, b.bookName) < 0)
			iter = iter -> next;
		temp = (Node*)malloc(sizeof(Node));
		temp -> book = b;
		temp -> next = iter -> next;
		iter -> next = temp;
		return root;
	}
	else if(mode == AUTHOR_MODE)
	{
		if(root == NULL)
		{
			root = (Node*)malloc(sizeof(Node));
			root -> book = b;
			root -> next = NULL;
			iter = root;
			return root;
		}
		else if(strcmp(root -> book.author, b.author) > 0)
		{
			temp = (Node*)malloc(sizeof(Node));
			temp -> book = b;
			temp -> next = root;
			root = temp;
			return root;	
		}
		while(iter -> next != NULL && strcmp(iter -> next -> book.author, b.author) < 0)
			iter = iter -> next;
		temp = (Node*)malloc(sizeof(Node));
		temp -> book = b;
		temp -> next = iter -> next;
		iter -> next = temp;
		return root;
	}
}

void deleteInfo()
{
	int id;
	if(root != NULL)
	{
		printf("\nEnter an id for the book which you want to delete: ");
		scanf("%d", &id);
		root = deleteFromList(id);
	}
	else
		printf("\nThere is no book :(\n");
}

Node* deleteFromList(int id)
{
    Node* iter = root;
    Node* prev = NULL;
    int i=0;
    while(iter != NULL)
    {
        if(iter -> book.id == id)
        {
            if(prev == NULL)
            {
                root = iter -> next;
                iter -> next = NULL;
                free(iter);
                return root;
            }
            else
            {
                prev -> next = iter -> next;
                iter -> next = NULL;
                free(iter);
                return root;
            }
        }
        iter = iter -> next;
        if(i==0)
            prev = root;
        else
            prev = prev -> next;
        i++;
    }
    printf("\nCannot found ID %d in the library!\n", id);
    return root;
}

void updateInfo()
{
	int id;
    printf("\nEnter an ID for the book which you want to update: ");
    scanf("%d", &id);
    if(updateCheck(id))
    {   
        int choice;
        printf("\n1 - Update book name\n2 - Update author name\nEnter you choice: ");
        scanf("%d", &choice);
        root = update(id, choice);
    }
}

int updateCheck(int id)
{
	Node* iter = root;
    while(iter != NULL)
    {
        if(iter -> book.id == id)
        {
        	free(iter);
            return 1;
        }
        iter = iter -> next;
    }
    printf("\nCannot found ID %d in the library!\n", id);
    free(iter);
    return 0;
}

Node* update(int id, int choice)
{
	Node* iter = root;
	if(choice == UPDATE_BOOK_NAME)
	{
		char newBookName[20];
		while(iter != NULL)
		{
			if(iter -> book.id == id)
				break;
			iter = iter -> next;
		}
		printf("\nEnter new book name: ");
		scanf("%s", newBookName);
		strcpy(iter -> book.bookName, newBookName);
	}
	else if(choice == UPDATE_AUTHOR)
	{
		char newAuthor[30];
		while(iter != NULL)
		{
			if(iter -> book.id == id)
				break;
			iter = iter -> next;
		}
		printf("\nEnter new author name: ");
		scanf("%s", newAuthor);
		strcpy(iter -> book.author, newAuthor);
	}
	free(iter);
	return root;
}

Node* sortForID()
{
	Node* temp = root;
	Node* iter;
	while(temp -> next != NULL)
	{
		iter = temp -> next;
		while(iter != NULL)
		{
			if(iter -> book.id < temp -> book.id)
			{
				Book tempBook = iter -> book;
				iter -> book = temp -> book;
				temp -> book = tempBook;
			}
			iter = iter -> next;
		}
		temp = temp -> next;
	}
	return root;
}

Node* sortForBookName()
{
	Node* temp = root;
	Node* iter;
	while(temp -> next != NULL)
	{
		iter = temp -> next;
		while(iter != NULL)
		{
			if(strcmp(iter -> book.bookName, temp -> book.bookName) < 0)
			{
				Book tempBook = iter -> book;
				iter -> book = temp -> book;
				temp -> book = tempBook;
			}
			iter = iter -> next;
		}
		temp = temp -> next;
	}
	return root;
}

Node* sortForAuthor()
{
	Node* temp = root;
	Node* iter;
	while(temp -> next != NULL)
	{
		iter = temp -> next;
		while(iter != NULL)
		{
			if(strcmp(iter -> book.author, temp -> book.author) < 0)
			{
				Book tempBook = iter -> book;
				iter -> book = temp -> book;
				temp -> book = tempBook;
			}
			iter = iter -> next;
		}
		temp = temp -> next;
	}
	return root;
}

void writeToTextFile()
{
	FILE* stream = fopen("library", "w");
	if(root == NULL)
		fprintf(stream, "%s", "");
	else
		while(root != NULL)
		{
			fprintf(stream, "%d %s %s\n", root -> book.id, root -> book.bookName, root -> book.author);
			root = root -> next;
		}
	fclose(stream);
}
