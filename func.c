#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/** Модуль, содержит типы данных, использующиеся для хранения базы данных. Реализует функции, отвечающие за работу с односвязным списком:
int Add (SPISOK *spisok, FILM film);
void Delete (SPISOK *spisok,int n);
void DeleteSp (SPISOK *spisok);
int Save (SPISOK *spisok,char *filename);
int SaveTxt (SPISOK *spisok,char *filename);
int Load (SPISOK *spisok,char *filename);
int Poisk (SPISOK *spisok,char *s,int n);
int CmpInc (char *a, char *b);
int CmpDec (char *a, char *b);
void Sort (SPISOK *spisok, int x1, int x2);
void Exc (SPISOK *spisok).
 **************************************************************/
/** Функция добавления элемента в конец списка**/
int Add(SPISOK *spisok, PICTURE picture)
 {
	ELEMENT *tmp=(ELEMENT *)malloc(sizeof(ELEMENT));
	if (!tmp)
    return 1;
	if (!spisok->head)
    spisok->head=tmp;
	else 
     {	
      if(!spisok->curr)
        spisok->curr=spisok->head;
			while (spisok->curr->next)
           spisok->curr=spisok->curr->next;
			     spisok->curr->next=tmp;
     }
	tmp->picture=picture;
	tmp->next=NULL;
	spisok->curr=tmp;
	return 0;
 }
/** Функция удаления элемента**/
void Delete (SPISOK *spisok,int n)
 {	
	spisok->curr=spisok->head;
	for(int i=0;i<n-1;i++)
  spisok->curr=spisok->curr->next;
	ELEMENT *tmp;
	if (n==0)
    {
     tmp=spisok->head; spisok->head=spisok->head->next;
    }
	else 
   if (spisok->curr->next->next)
     {
      tmp=spisok->curr->next;
      spisok->curr->next=spisok->curr->next->next;
     }
	else
    {
     tmp=spisok->curr->next;
     spisok->curr->next=NULL;
    }
  free(tmp);
 }
/** Функция удаления списка**/
void DeleteSp(SPISOK *spisok)
 {
	while (spisok->head)
    {
	   spisok->curr=spisok->head;
	   spisok->head=spisok->head->next;
	   free(spisok->curr);
    }
	spisok->head=spisok->curr=NULL;
 }
/** Функция записи списка в бинарный файл**/
int Save (SPISOK *spisok,char *filename)
 {
	FILE *f = fopen(filename,"wb");
	if (!f)
    return 1;
	spisok->curr=spisok->head;
	while (spisok->curr)
    {
		 if (fwrite(&spisok->curr->picture,sizeof(PICTURE),1,f)<1)
      {
       fclose(f);
       return 1;
      }
		spisok->curr=spisok->curr->next;
    }
	fclose(f);
	return 0;
 }
/** Функция записи списка в текстовый файл**/
int SaveTxt (SPISOK *spisok,char *filename)
 {
	FILE *f = fopen(filename,"w");
	if (!f)
   return 1;
	spisok->curr=spisok->head;
	while (spisok->curr)
   {
		if (fprintf(f,"%31s %21s %11s %20s %10u %9u",spisok->curr->picture.Name,spisok->curr->picture.Avt,
		spisok->curr->picture.Date,spisok->curr->picture.Genre,
        spisok->curr->picture.Price,spisok->curr->picture.Zal)<1)
      {
       fclose(f);
       return 1;
      }
		spisok->curr=spisok->curr->next;}
	fclose(f);
	return 0;
 }
/** Функция чтения списка из файла**/
int Load (SPISOK *spisok,char *filename)
 {
	DeleteSp(spisok);
	FILE *f = fopen(filename,"rb");
	if (!f)
   return 1;
	PICTURE tmp;
	while (fread(&tmp,sizeof(PICTURE),1,f)==1)
   {
    if(Add(spisok,tmp))
     {
      fclose(f);
      return 2;
     }
   }
	fclose(f);
	return 0;
 }
/** Функция поиска  **/
int Poisk (SPISOK *spisok,char *s,int n)
 {
	while (spisok->curr) 
   {
	  if ((spisok->curr->picture.Price==atoi(s))
				|| ((spisok->curr->picture.Zal==atoi(s))) 
				|| !strncmp(spisok->curr->picture.Name,s,strlen(s))
        || !strncmp(spisok->curr->picture.Genre,s,strlen(s)) 
			 || !strncmp(spisok->curr->picture.Date,s,strlen(s)) 
				|| !strncmp(spisok->curr->picture.Avt,s,strlen(s)))
	    {
       spisok->curr=spisok->curr->next;
       n++;
       return n;
      }
	  n++;
    spisok->curr=spisok->curr->next;
   } return -1;
 }
/** Функции сравнения двух элементов**/
int CmpInc(char *a, char *b)
 {
	if (strcmp(a,b)>0)
    return 1;
	return 0;
 }

int CmpDec(char *a, char *b)
 {
	if (strcmp(a,b)<0)
    return 1;
	return 0;
 }

int CmpIncU(unsigned a, unsigned b)
 {
	if (a>b)
    return 1;
	return 0;
 }

int CmpDecU(unsigned a,unsigned b)
 {
	if (a<b)
    return 1;
	return 0;
 }
/** Функция сортировки  **/
void Sort(SPISOK *spisok, int x1, int x2)
 {
	if(!spisok->head)
    return;
	int (*cmp)(char*,char*);
	int (*cmpU)(unsigned,unsigned);
	if (x2==0)
    {
     cmp=CmpInc;
     cmpU=CmpIncU;
    }
  else 
    {
     cmp=CmpDec;
     cmpU=CmpDecU;
    };
  int Flag1 = 1;
	int Flag2 = 0;  
 	while(Flag1)
     {   
    	Flag1 = 0;       
   	  spisok->curr = spisok->head;    
      while(spisok->curr->next)
        {
	       switch(x1)
           {
    			   case 1: {Flag2=cmp(spisok->curr->picture.Name,
                      spisok->curr->next->picture.Name);break;}
    			   case 2: {Flag2=cmp(spisok->curr->picture.Avt,
                      spisok->curr->next->picture.Avt);break;}
    			   case 3: {Flag2=cmp(spisok->curr->picture.Date,
                      spisok->curr->next->picture.Date);break;}
                   case 4: {Flag2=cmp(spisok->curr->picture.Genre,
                      spisok->curr->next->picture.Genre);break;}
    			   case 5: {Flag2=cmpU(spisok->curr->picture.Price,
                      spisok->curr->next->picture.Price);break;}	
    			   case 6: {Flag2=cmpU(spisok->curr->picture.Zal,
                      spisok->curr->next->picture.Zal);break;}
           }	
         if(Flag2)
           {
            Exc(spisok);
            Flag1 = 1;
           }
	       else
           spisok->curr = spisok->curr->next;
        }
     }
	spisok->curr = spisok->head;
 }
/** Функция перестановки двух элементов**/
void Exc(SPISOK *spisok)
 {
	PICTURE tmp;
	memcpy(&tmp,&spisok->curr->picture,sizeof(PICTURE));
	memcpy(&spisok->curr->picture, &spisok->curr->next->picture,sizeof(PICTURE));
	memcpy(&spisok->curr->next->picture, &tmp, sizeof(PICTURE));
 }
