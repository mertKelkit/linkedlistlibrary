#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_MODE 0
#define NAME_MODE 1
#define AUTHOR_MODE 2
#define UPDATE_BOOK_NAME 1
#define UPDATE_AUTHOR 2
#define MAIN_MENU 0
#define UPDATE_MENU 1
#define FAILURE -1

//default selected sorting mode is ID mode
int selectedMode = ID_MODE;

//Book struct which has an ID, book name and author name
typedef struct
{
	int id;
	char bookName[20];
	char author[30];
} Book;

//Node struct which has a Book and a next node pointer
typedef struct n
{
	Book book;
	struct n* next;
} Node;

//Global root node which is NULL at first
Node* root = NULL;

/*FUNCTION PROTOTYPES*/
//for main menu
void menu(void);
//for displaying current linked list
void print(void);
//for getting info to update a node
void updateInfo(void);
//for getting info to create a new node
void createInfo(void);
//for getting info to delete a node
void deleteInfo(void);
//for writing linked list to the text file
void writeToTextFile(void);
//to check a node's updateability
int updateCheck(int);
//power function for integer params and integer return value
int myPow(int, int);
//char control for ID for deleting and updating
int charControlID(char*);
//char control for ID for adding a new book mode 
int charControlAddID(char*);
//char control for menu choices
int charControlChoice(char*, int);
//char control for author name
char* charControlAuthor(char*);
/*THERE IS NO CHAR CONTROL FOR BOOK NAME BECAUSE IT CAN BE ANYTHING*/
//sorts list according to ID
Node* sortForID(void);
//sorts list according to book name
Node* sortForBookName(void);
//sorts list according to author name
Node* sortForAuthor(void);
//reads books from a text file if it exists at first
Node* readFromTextFile(void);
//delete a specific id from the list
Node* deleteFromList(int);
//update a specific id's author or book name from the list
Node* update(int, int);
//add a specific Book to the linked list with selected sort mode
Node* addToList(Book, int);

int main()
{
	//try to read from the text file. If there exists a text file, fill linked list with it's content
	root = readFromTextFile();
    printf("Welcome to the Library Program !!!\n----------------------------------");
	//call main menu
	menu();
	return 0;
}

void menu()
{
	//infinite loop
	while(1)
    {
        int choice;
        char choiceC[20];
        //display menu
        printf("\nLIBRARY PROGRAM\n----------------\n1 - List all books\n2 - Update a book\n3 - Create a new book\n4 - Delete a book\n");
        printf("5 - Sort all books according to id\n6 - Sort all books according to book name\n");
        printf("7 - Sort all books according to author name\n8 - Exit\nEnter your choice: ");
        //get an input for choice and control it's characters
        do
        {
        	scanf("%s", choiceC);
        } while(charControlChoice(choiceC, MAIN_MENU) == FAILURE);
        choice = charControlChoice(choiceC, MAIN_MENU);
        switch (choice) {
            case 1:
            	//if 1 is selected, print linked list
            	print();
                break;
            case 2:
            	//if 2 is selected, get info for the book which is wanted to update
            	updateInfo();
                break;
            case 3:
            	//if 3 is selected, get info for the book which is wanted to add
            	createInfo();
                break;
            case 4:
            	//if 4 is selected, get info for the book which is wanted to delete
           		deleteInfo();
                break;
            case 5:
            	//if 5 is selected, selected sorting mode will be ID mode
            	selectedMode = ID_MODE;
            	//if linked list is empty print a message
            	if(root == NULL)
            		printf("\nThere is no book but if you want to add a book, book will be added ID-sorted.\n");
              	//if it's not empty
                else
                {
                	//sort existing linked list according to ID
                	root = sortForID();
                	//and print new form of the list
                	print();
                }
                break;
            case 6:
            	//if 6 is selected, selected sorting mode will be name mode
            	selectedMode = NAME_MODE;
            	//if linked list is empty
            	if(root == NULL)
            		printf("\nThere is no book but if you want to add a book, book will be added book name-sorted.\n");
                //if it's not
                else
                {
                	//sort existing list according to book names
                	root = sortForBookName();
                	//and display it's new form
                	print();
                }
                break;
            case 7:
            	//if 7 is selected, selected sorting mode will be author name mode
            	selectedMode = AUTHOR_MODE;
            	//if list is empty print a message
            	if(root == NULL)
            		printf("\nThere is no book but if you want to add a book, book will be added author name-sorted.\n");
                //if it's not
                else
                {
                	//sort existing list according to author names
                	root = sortForAuthor();
                	//and print it's new form
                	print();
                }
                break;
            case 8:
            	//if 8 is selected
            	//if root isn't empty
            	if(root != NULL)
            		//sort all nodes according to ID
            		root = sortForID();
            	//write nodes to text file
        		writeToTextFile();
        		//and print a message
            	printf("\nRead more books :) Bye !\n");
                return;
        }
    }
}

//this function works at first and reads from text file if it exists
Node* readFromTextFile()
{
	FILE* stream;
	//there are iterator and previous node pointers
	Node* iter;
	Node* prev = NULL;
	int ctr = 0;
	//if file can be opened in read mode
	if(stream = fopen("library.txt", "r"))
	{
		//calculate file's length
		fseek(stream, 0, SEEK_END);
		int len = ftell(stream);
		//if file is not empty
		if(len != 0)
		{
			rewind(stream);
			//scan file till the last line
			while(!feof(stream))
			{
				//if root is NULL
				if(ctr == 0)
				{
					//open root
					root = (Node*)malloc(sizeof(Node));
					//fill it's properties with scanned values
					fscanf(stream, "%d %s %s", &root -> book.id, root -> book.bookName, root -> book.author);
					//make it's next node NULL
					root -> next = NULL;
					//iter comes root
					iter = root;
				}
				else
				{
					//if linked list only has root, prev will be root 
					if(ctr == 1)
						prev = root;
					//else prev will be iter
					else
						prev = iter;
					//open a new node for iterator's next
					iter -> next = (Node*)malloc(sizeof(Node));
					//move iterator to it
					iter = iter -> next; 
					//and fill it
					fscanf(stream, "%d %s %s", &iter -> book.id, iter -> book.bookName, iter -> book.author);
					//make NULL it's next
					iter -> next = NULL; 
				}
				ctr++;
			}
			//at last, make NULL last node's next pointer again.
			iter -> next = NULL;
			//deleting last node because there is a problem with reading the text file. adds one more empty node to the end
			prev -> next = iter -> next;
		}
		//close file
		fclose(stream);
	}
	//and return new list
	return root;
}

//display function
void print()
{
	//if list is empty
	if(root == NULL)
	{
		printf("\nThere is no book :(\n");
		return;
	}
	//print sort mode first
	if(selectedMode == ID_MODE)
		printf("\nSorted For ID\n--------------");
	else if(selectedMode == NAME_MODE)
		printf("\nSorted for book names\n---------------------");
	else if(selectedMode == AUTHOR_MODE)
		printf("\nSorted for author names\n-----------------------");
	//there is a iter node for displaying all nodes
	Node* iter = root;
	//from first to the last
	while(iter != NULL)
	{
		//print node's book's id, book name and author name
		if(iter == root)
			printf("\nID -> %d\tBook Name -> %s\tAuthor Name -> %s\n", iter -> book.id, iter -> book.bookName, iter -> book.author);
		else
			printf("ID -> %d\tBook Name -> %s\tAuthor Name -> %s\n", iter -> book.id, iter -> book.bookName, iter -> book.author);
		//move iter to it's next
		iter = iter -> next;
	}
}

//gets info for creating a new node
void createInfo()
{
	Book book;
	char tempID[10], tempAuthor[30];
    //getting ID for char and existence control
    printf("\nPlease enter the book ID: ");
    do
    {
    	scanf("%s", tempID);
    } while(charControlAddID(tempID) == FAILURE);
    book.id = charControlAddID(tempID);
    //getting book name without any control
    printf("Please enter the book name: ");
    scanf("%s", book.bookName);
    //getting author name with char control
    printf("Please enter the author's name: ");
    do
    {
    	scanf("%s", book.author);
    } while(charControlAuthor(book.author) == NULL);
    //call add function and print a success message
    root = addToList(book, selectedMode);
    printf("\nAdded successfuly !\nID -> %d\tBook Name -> %s\tAuthor Name -> %s\n", book.id, book.bookName, book.author);
}

//adding a new book b with sorting mode "mode"
Node* addToList(Book b, int mode)
{
	//iter shows root node
	Node* iter = root;
	//temp is NULL right now
	Node* temp = NULL;
	//if id mode is selected
	if(mode == ID_MODE)
	{
		//if list is empty
		if(root == NULL)
		{
			//open root node
			root = (Node*)malloc(sizeof(Node));
			//make it's book b
			root -> book = b;
			//make it's next NULL
			root -> next = NULL;
			//make iter show root again
			iter = root;
			//return new list
			return root;
		}
		//if root needs to be changed
		else if(b.id < root -> book.id)
		{
			//open temp node and make root pointer show temp node
			temp = (Node*)malloc(sizeof(Node));
			temp -> book = b;
			temp -> next = root;
			root = temp;
			//return new list
			return root;	
		}
		//while iterator has next and it's next's book id is smaller than parameter book's id
		while(iter -> next != NULL && iter -> next -> book.id < b.id)
			//move iter to the next
			iter = iter -> next;
		//at last open temp node and temp's next pointer must show iterator's next pointer. then iterator's next must be temp in order to put it in
		temp = (Node*)malloc(sizeof(Node));
		temp -> book = b;
		temp -> next = iter -> next;
		iter -> next = temp;
		//return new list
		return root;
	}
	//if name mode is selected
	else if(mode == NAME_MODE)
	{
		//if list is empty
		if(root == NULL)
		{
			//open root node
			root = (Node*)malloc(sizeof(Node));
			//make it's book b
			root -> book = b;
			//make it's next NULL
			root -> next = NULL;
			//make iter root again
			iter = root;
			//return new list
			return root;
		}
		//if root needs to be changed according to book name
		else if(strcmp(root -> book.bookName, b.bookName) > 0)
		{
			//open temp node and make root pointer show temp node
			temp = (Node*)malloc(sizeof(Node));
			temp -> book = b;
			temp -> next = root;
			root = temp;
			return root;	
		}
		//while iterator has next and it's next's book name comes before the parameter's b's book name
		while(iter -> next != NULL && strcmp(iter -> next -> book.bookName, b.bookName) < 0)
			//move iter to it's next
			iter = iter -> next;
		//at last open temp node and temp's next pointer must show iterator's next pointer. then iterator's next must be temp in order to put it in
		temp = (Node*)malloc(sizeof(Node));
		temp -> book = b;
		temp -> next = iter -> next;
		iter -> next = temp;
		//return new list
		return root;
	}
	else if(mode == AUTHOR_MODE)
	{
		//if list is empty
		if(root == NULL)
		{
			//open root node
			root = (Node*)malloc(sizeof(Node));
			//make it's book b
			root -> book = b;
			//make it's next NULL
			root -> next = NULL;
			//make iter root again
			iter = root;
			//return new list
			return root;
		}
		//if root needs to be changed
		else if(strcmp(root -> book.author, b.author) > 0)
		{
			//open temp node and make root pointer show temp node
			temp = (Node*)malloc(sizeof(Node));
			temp -> book = b;
			temp -> next = root;
			root = temp;
			//return new list
			return root;	
		}
		//while iterator has next and it's next's author name comes before the parameter's b's author name
		while(iter -> next != NULL && strcmp(iter -> next -> book.author, b.author) < 0)
			//iter comes to it's next
			iter = iter -> next;
		//at last open temp node and temp's next pointer must show iterator's next pointer. then iterator's next must be temp in order to put it in
		temp = (Node*)malloc(sizeof(Node));
		temp -> book = b;
		temp -> next = iter -> next;
		iter -> next = temp;
		//return new list
		return root;
	}
}

//gets info for the book which needs to be deleted
void deleteInfo()
{
	int id;
	char idC[10];
	//if list is not empty
	if(root != NULL)
	{
		//get id with char control for the book which will be deleted 
		printf("\nEnter an id for the book which you want to delete: ");
		do
		{
			scanf("%s", idC);
		} while(charControlID(idC) == FAILURE);
		id = charControlID(idC);
		//call delete function
		root = deleteFromList(id);
	}
	//if list is empty, print a message
	else
		printf("\nThere is no book :(\n");
}

//deletes a node from the list
Node* deleteFromList(int id)
{
	//iter shows root at first
    Node* iter = root;
    //prev is NULL at first
    Node* prev = NULL;
    int i=0;
    //from first node to the last node
    while(iter != NULL)
    {
    	//if input id is found
        if(iter -> book.id == id)
        {
        	//print it's properties
        	printf("\nID -> %d\tBook Name -> %s\tAuthor Name -> %s\n", iter -> book.id, iter -> book.bookName, iter -> book.author);
            //if prev is NULL, it means that we're on the root node
            if(prev == NULL)
            {
            	//and make new root old root's next
                root = iter -> next;
           		//and delete iter node
                iter -> next = NULL;
                free(iter);
                //print success message and return new list
                printf("\nDeleted successfuly !\n");
                return root;
            }
         	//if it's not NULL...
            else
            {
            	//prev comes before iter
            	//prev's next must point iter's next at first
                prev -> next = iter -> next;
                //and delete iter
                iter -> next = NULL;
                free(iter);
                //print a success message and return new list
                printf("\nDeleted successfuly !\n");
                return root;
            }
        }
        //iter goes it's next
        iter = iter -> next;
        //make prev root at the last of the first iteration
        if(i==0)
            prev = root;
        //after first iteration, prev will be it's next
        else
            prev = prev -> next;
        i++;
    }
    //if there is no book with ID "id", print an error message and return linked list
    printf("\nCannot found ID %d in the library!\n", id);
    return root;
}

//gets information for the book which is wanted to be updated
void updateInfo()
{
	//if list is not empty
	if(root != NULL)
	{
		int id;
		char idC[10];
		//get id with char control
	    printf("\nEnter an ID for the book which you want to update: ");
	    do
	    {
	    	scanf("%s", idC);
	    } while(charControlID(idC) == FAILURE);
	    id = charControlID(idC);
	    //if this id exists...
	    if(updateCheck(id))
	    {   
	        int choice;
	        char choiceC[10];
	        //get choice with char control
	        printf("\n1 - Update book name\n2 - Update author name\nEnter you choice: ");
	        do
	        {
	        	scanf("%s", choiceC);
	        } while(charControlChoice(choiceC, UPDATE_MENU) == FAILURE);
	        choice = charControlChoice(choiceC, UPDATE_MENU);
	        //and update a specific node
	        root = update(id, choice);
		}    
	}
	//if list is emtpy, print a message
	else
		printf("\nThere is no book :(\n");
}

//this function checks an id's existence in the linked list
int updateCheck(int id)
{
	//iter starts from root node
	Node* iter = root;
	//while it's not NULL
    while(iter != NULL)
    {
    	//if ID is found in the list, print it's properties and return 1 as success
        if(iter -> book.id == id)
        {
        	printf("\nID -> %d\tBook Name -> %s\tAuthor Name -> %s\n", iter -> book.id, iter -> book.bookName, iter -> book.author);
            return 1;
        }
        //iter goes it's next
        iter = iter -> next;
    }
    //if ID cannot found in the list, print an error message and return 0 as failure
    printf("\nCannot found ID %d in the library!\n", id);
    return 0;
}

//this function updates a specific node
Node* update(int id, int choice)
{
	//iterator starts from root node
	Node* iter = root;
	//if book name wanted to be updated
	if(choice == UPDATE_BOOK_NAME)
	{
		char newBookName[20];
		//check nodes until ID is found in the list
		while(iter != NULL)
		{
			if(iter -> book.id == id)
				break;
			iter = iter -> next;
		}
		//after id is found in the list, get a book name without any char controls
		printf("\nEnter new book name: ");
		scanf("%s", newBookName);
		//create a new book
		Book b;
		b.id = iter -> book.id;
		strcpy(b.bookName, newBookName);
		strcpy(b.author, iter -> book.author);
		//delete current node from the list
		root = deleteFromList(iter -> book.id);
		//and add it to the list with the new book name
		root = addToList(b, selectedMode);

	}
	//if author name wanted to be updated
	else if(choice == UPDATE_AUTHOR)
	{
		char newAuthor[30];
		//check nodes until ID is found in the linked list
		while(iter != NULL)
		{
			//if it's found, exit from the loop
			if(iter -> book.id == id)
				break;
			iter = iter -> next;
		}
		//get new author name with char control
		printf("\nEnter new author name: ");
		do
		{
			scanf("%s", newAuthor);
		} while(charControlAuthor(newAuthor) == NULL);
		//create a new book
		Book b;
		b.id = iter -> book.id;
		strcpy(b.bookName, iter -> book.bookName);
		strcpy(b.author, newAuthor);
		//delete current node from the list
		root = deleteFromList(iter -> book.id);
		//and add it to the list with the new author name
		root = addToList(b, selectedMode);
	}
	//print success message
	printf("\nUpdated successfuly !\n");
	//and display it's properties
	printf("\nID -> %d\tBook Name -> %s\tAuthor Name -> %s\n", iter -> book.id, iter -> book.bookName, iter -> book.author);
	//return new linked list
	return root;
}

//this function sorts linked list according to their ID's, uses selection sort
Node* sortForID()
{
	/*These nodes are for using selection sort*/
	//temp node starts from root
	Node* temp = root;
	//iter node will start from temp node everytime and go to the next
	Node* iter;
	//outer loop
	while(temp -> next != NULL)
	{
		//iter starts from temp's next node
		iter = temp -> next;
		//inner loop
		while(iter != NULL)
		{
			//if forward node's ID must come before the previous node's ID for sorted for ID mode...
			if(iter -> book.id < temp -> book.id)
			{
				//exchange Book's of nodes
				Book tempBook = iter -> book;
				iter -> book = temp -> book;
				temp -> book = tempBook;
			}
			//iter goes it's next in inner loop
			iter = iter -> next;
		}
		//temp goes it's next in outer loop
		temp = temp -> next;
	}
	//return new linked list
	return root;
}

//this function sorts linked list according to their book names, uses selection sort
Node* sortForBookName()
{
	/*These nodes are for using selection sort*/
	//temp node starts from root
	Node* temp = root;
	//iter node will start from temp node everytime and go to the next
	Node* iter;
	//outer loop
	while(temp -> next != NULL)
	{
		//iter starts from temp's next node
		iter = temp -> next;
		//inner loop
		while(iter != NULL)
		{
			//if forward node's book name must come before the previous node's book name for sorted for book name mode...
			if(strcmp(iter -> book.bookName, temp -> book.bookName) < 0)
			{
				//exchange book variables of nodes
				Book tempBook = iter -> book;
				iter -> book = temp -> book;
				temp -> book = tempBook;
			}
			//iter goes it's next in inner loop
			iter = iter -> next;
		}
		//temp goes it's new in outer loop
		temp = temp -> next;
	}
	//return new linked list
	return root;
}

//this function sorts linked list according to their author names, uses selection sort
Node* sortForAuthor()
{
	/*These nodes are for using selection sort*/
	//temp node starts from root
	Node* temp = root;
	//iter node will start from temp node everytime and go to the next
	Node* iter;
	//outer loop
	while(temp -> next != NULL)
	{
		//iter starts from temp's next node
		iter = temp -> next;
		//inner loop
		while(iter != NULL)
		{
			//if forward node's author name must come before the previous node's author name for sorted for author mode...
			if(strcmp(iter -> book.author, temp -> book.author) < 0)
			{
				//exchange Book variables of nodes
				Book tempBook = iter -> book;
				iter -> book = temp -> book;
				temp -> book = tempBook;
			}
			//move iter to the next in inner loop
			iter = iter -> next;
		}
		//move temp to the next in outer loop
		temp = temp -> next;
	}
	//return new linked list
	return root;
}

//this function write every node to the text file at the end of the program
void writeToTextFile()
{
	FILE* stream = fopen("library.txt", "w");
	//if list is empty, create an emtpy text file
	if(root == NULL)
		fprintf(stream, "%s", "");
	else
		//go to the every node
		while(root != NULL)
		{
			//and print their properties to the text file
			fprintf(stream, "%d %s %s\n", root -> book.id, root -> book.bookName, root -> book.author);
			//go to the next node
			root = root -> next;
		}
	//close text file
	fclose(stream);
}

//this function controls chars of an ID for updating and deleting a node
int charControlID(char* input)
{
	int i, val=0;
	int ctr = 0;
	for(i=strlen(input)-1; i>=0; i--)
	{
		//ID must be an integer. if there is a char which is a non integer character
		if(input[i] > 57 || input[i] < 49)
		{
			//print an error message and return 
			printf("\nID must be a positive integer. Please enter again: ");
			return FAILURE;
		}
		//convert input to integer
		val += (input[i]-48)*myPow(10, ctr);
		ctr++;
	}
	//return integer value if there isn't any problem
	return val;
}

//this function controls an input of choice from the update menu or main menu
int charControlChoice(char* input, int mode)
{
	//if controlling update menu, update choices must be between 1 and 2
	if(mode == UPDATE_MENU)
	{
		int i, val = 0, ctr = 0;
		for(i = strlen(input)-1; i>=0; i--)
		{
			//if there is a char which is a non integer value or there is a char which is not between 1 and 2
			if(input[i] < 49 || input[i] > 50)
			{
				//print an error message
				printf("\nChoice must be an integer between 1 and 2. Please enter again: ");
				return FAILURE;
			}
			//convert input to integer and return it
			val += (input[i]-48)*myPow(10, ctr);
			return val;
		}
	}
	//if controlling main menu, choices must be between 1 and 8
	else if(mode == MAIN_MENU)
	{
		int i, val = 0, ctr = 0;
		for(i = strlen(input)-1; i>=0; i--)
		{
			//if there is a char which is a non integer value or there is a char which is not between 1 and 8
			if(input[i] < 49 || input[i] > 56)
			{
				printf("\nChoice must be an integer between 1 and 8. Please enter again: ");
				return FAILURE;
			}
			//convert input to integer and return it
			val += (input[i]-48)*myPow(10, ctr);
			return val;
		}
	}
}

//this function controls author name, author name mustn't contain an integer
char* charControlAuthor(char* input)
{
	int i;
	//checking every char ...
	for(i=strlen(input)-1; i>=0; i--)
	{
		//if there is an integer character..
		if(input[i] <= 57 && input[i] >= 48)
		{
			//print an error message
			printf("\nAuthor name cannot contain an integer. Please enter again: ");
			return NULL;
		}
	}
	//returns input if it's in correct form
	return input;
}

//this function controls is ID an integer or does it exist
int charControlAddID(char* input)
{
	int i, ctr = 0, val = 0;
	//iterator node starts from root node
	Node* iter = root;
	for(i = strlen(input)-1; i>=0; i--)
	{
		//checks if there is any non integer character
		if(input[i] > 57 || input[i] < 48)
		{
			//if it's found print an error message
			printf("\nID must be a positive integer. Please enter again: ");
			return FAILURE;
		}
		//convert char array to integer
		val += (input[i]-48)*myPow(10, ctr);
		ctr++;
	}
	//iter starts from the root to the NULL
	while(iter != NULL)
	{
		//if ID is found in the linked list
		if(iter -> book.id == val)
		{
			//print an error message
			printf("\nThis ID exists. Please enter again: ");
			return FAILURE;
		}
		//go to the next node
		iter = iter -> next;
	}
	//return converted ID integer if there is no problem
	return val;
}

//power function
int myPow(int base, int e)
{
	int i;
	int ret = 1;
	//multiplies base e times
	for(i=0; i<e; i++)
		ret *= base;
	return ret;
}
