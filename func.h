#ifndef FUNC
#define FUNC
/** —ложный тип данных PICTURE дл€ создани€ структуры данных**/
typedef struct {
char Name[30], Avt[20], Genre[18], Date[10]; 
unsigned Price:14, Zal:10;
} PICTURE;
/** —ложный тип данных ELEMENT дл€ создани€ структуры списка**/
typedef struct _ELEMENT{
PICTURE picture;
struct _ELEMENT *next;
} ELEMENT;
/** —ложный тип данных SPISOK дл€ создани€ списка**/
typedef struct {
ELEMENT *head,*curr;
} SPISOK;

int Add(SPISOK *spisok, PICTURE picture);
void Delete (SPISOK *spisok,int n);
void DeleteSp(SPISOK *spisok);
int Save (SPISOK *spisok,char *filename);
int SaveTxt (SPISOK *spisok,char *filename);
int Load (SPISOK *spisok,char *filename);
int Poisk (SPISOK *spisok,char *s,int n);
int CmpInc(char *a, char *b);
int CmpDec(char *a, char *b);
void Sort(SPISOK *spisok, int x1, int x2);
void Exc(SPISOK *spisok);

#endif
