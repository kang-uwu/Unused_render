#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link_list.h"
#include <time.h>
#define VERSION 9
#define MAX_LEN 1000

int readline(FILE *fp, char *line);
int slice_left(char *line);
void openfiles(FILE *fp[]);
Node *render_ll(Node *, FILE *fp);
Node *replace_img_tag( Node *list);
Node *chat_render( Node *list);

int main()
{
//	clock_t start = clock();
	if(VERSION/100)
		error("You Wish!");

	FILE *fp[VERSION];
	openfiles(fp);
	char line[MAX_LEN];

	Node *list =NULL;
	FILE *fp_render = fopen("renders.txt","r");

	Node *render_list = render_ll(list, fp_render); //adds renders to render_list

	for(int j=0; j<VERSION; j++)
	{
		FILE *temp_fp = fp[j];
		while(readline(temp_fp,line))
		{
			if(strncmp("show",line,4)==0 || strncmp("scene",line,5)==0)
			{
				slice_left(line);
				if(strncmp("screen ",line,7))
				{
					char *ptr1 = strstr(line," at ");
					char *ptr2 = strstr(line," with ");
					int i=0;
					while(line[i]!='\0')
					{
						if( ptr1==line+i || ptr2==line+i)
						{
							line[i]='\0';
							break;
						}
						i++;
					}
					if(ptr1==NULL && ptr2==NULL)
						line[--i]='\0';
				list = prepend(list, line);
				//it might be a video declared with image tag (not handled)
				}
				else //screen case handled seperately(not handled)
				{
				slice_left(line);
				char *ptr1 = strstr(line," at ");
				char *ptr2 = strstr(line," with ");
				int i=0;
				while(line[i]!='\0')
				{
					if( ptr1==line+i || ptr2==line+i)
					{
						line[i]='\0';
						break;
					}
					i++;
				}
				if(ptr1==NULL && ptr2==NULL)
					line[--i]='\0';
				list = prepend(list, line);
				}
			}
		}
	}

 	list = replace_img_tag( list);
	list = chat_render( list);

	Node *tmp = list;
	while(tmp!=NULL)
	{
		delete(render_list,tmp->name);
		tmp = tmp->next;
	}

 	list = replace_img_tag( list);
	print_list(render_list);

	/*
	clock_t  end= clock();
	double runtime = (double) (end - start)/CLOCKS_PER_SEC;
	printf("%lf\n",runtime);
	*/

}


int readline(FILE *fp, char *line) // read a line and return num of char read
{
	int i=0;
	char c='\0';
	while((c=getc(fp))!=EOF && c==' ') 
		;
	ungetc(c,fp);
	while((c = getc(fp))!=EOF && c!='\r' && c!='\n' && i<MAX_LEN-1)
	{
		line[i++]=c;
	}
	if(c=='\n' || c=='\r')
		line[i++]='\n';
	line[i]='\0';
	return i;
}

int slice_left(char *line) // remove the word and the blank space from the left
{
	int i=0;
	while(line[i]!='\0')
	{
		if(line[i]==' ')
		{
			int j = 0;
			while(line[i]!='\0')
				line[j++]=line[++i];
		}
		else
			i++;
	}
	return i;
}


void openfiles(FILE *fp[])
{
	fp[0] = fopen("script.rpy","r");
	for(int i=2; i<=VERSION; i++)
	{
		char str[15] = {'\0'};	
		sprintf(str, "script%d.rpy", i);
		fp[i-1] = fopen(str, "r");
	}
}

Node *render_ll(Node *list, FILE *fp)
{
	char str[MAX_LEN]={'\0'};
	while(readline(fp, str))
	{
		char s[MAX_LEN] = {'\0'};
		int i=0;
		while( str[i]!= '.')
		{
			s[i]=str[i];
			i++;
		}
		list = no_search_prepend(list, s);
	}
	return list;

}

Node *replace_img_tag( Node *list) //replace itag with the images in them Also adds the images in other folder like menus
				  //Doesn't remove '/' from image names (easy fix but isn't worth )
{
	FILE *fp = fopen("images.rpy","r");
	char line[MAX_LEN]; 
	char tag[MAX_NAME] = {'\0'};
	while( readline(fp, line) )
	{
		char *ptr = NULL;
		if( !strncmp("image ", line, 6) && (ptr = strchr( line, ':')) != NULL)
		{
			int i=0;
			for( i=0; line[6+i]!=*ptr; i++)
				tag[i] = line[6+i];
			tag[i] = '\0';
			list = delete(list, tag);

		}
		else if( line[0]=='\"')
		{
			char str[MAX_NAME]= {'\0'};
			for(int i=1; line[i]!='\"'; i++)
				str[i-1] = line[i];
			if( !strchr( str, '='))
			{
				list = prepend(list, str);
			}
		}
	}
	return list;
}

Node *chat_render( Node *list) // load render from chat.rpy
{
	const char str[] = "picture=\"";
	FILE *fp = fopen("chat.rpy","r");
	char line[MAX_LEN];
	while( readline(fp, line))
	{
		char *p = NULL;
		if( line[0] != '#' && (p = strstr( line, str))) // if it isn't a comment and contains picture="
		{
			char name[MAX_NAME]="";
			for(int i=0; p[i+strlen(str)]!='\"'; i++)
				name[i] = p[i+strlen(str)]; // extract name enclosed in ""
			list = prepend(list, name); 
		}
	}
	return list;
}


