#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME 70

typedef struct LL{
	char name[MAX_NAME];
	struct LL *next;
} Node;


void error(char *str)
{
	fprintf(stderr,"%s\n",str);
	exit(1);
}

Node *talloc(char *str)
{
	Node *newnode = (Node *) malloc(sizeof(Node));
	if(newnode)
	{
		strcpy(newnode->name, str);
		newnode->next = NULL;
	}
	else
		error("Can't allocate memory");
	return newnode;
}

void print_list(Node *head)
{
	while(head!=NULL)
	{
		printf("%s\n",head->name);
		head = head->next;
	}
}

Node *search(Node *head, char *str)
{
	while(head!=NULL)
	{
		if(strncmp(head->name,str,MAX_NAME)==0)
			return head;
		head = head->next;
	}
	return NULL;
}

Node* prepend(Node *list, char *str)
{
	int i = 0;
	while(str[i]!='\0')
	{
		i++;	
	}

	while(str[--i] == ' ')
	       	str[i] = '\0';

	Node *p = search(list, str);  //search if str is already present in the list
	if(p != NULL)
		return list;

	Node *newnode = talloc(str);
	newnode->next = list;
	return newnode;
}

Node* no_search_prepend(Node *list, char *str)
{
	int i = 0;
	while(str[i]!='\0')
	{
		i++;	
	}

	while(str[--i] == ' ')
	       	str[i] = '\0';

	Node *newnode = talloc(str);
	newnode->next = list;
	return newnode;
}




Node *delete(Node *head, char *str)
{
	Node *temp = head;
	if(strcmp(head->name, str)==0)
	{
		Node *t = head;
		head = head->next;
		free(t);
		return head;
	}

	while(temp->next->next!=NULL)
	{
		if(strcmp(temp->next->name, str)==0)
		{
			Node *t = temp->next;
			temp->next = temp->next->next;
			free(t);
			return head;
		}
		temp = temp->next;
	}

	if(strcmp(temp->next->name, str)==0)
	{
			free(temp->next);
			temp->next = NULL;
	}
	return head;
}
