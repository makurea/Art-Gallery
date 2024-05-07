#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "func.h"
/** Главный модуль программы, в которой располагается функция WinMain. В этом модуле подключаются все остальные для организации работы программы **************************************************************/

/** Prototypes **************************************************************/

static INT_PTR CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
void Print (SPISOK *spisok);
static LRESULT WINAPI AddDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT WINAPI OpenDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT WINAPI SaveDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT WINAPI SaveTxtDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT WINAPI EditDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT WINAPI PoiskDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT WINAPI SortDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT WINAPI FiltrDlgProc(HWND, UINT, WPARAM, LPARAM);
void Filtr(SPISOK *,char *, int );
void Info(SPISOK *,char *);
/** Global variables ********************************************************/

char szAppName[] = "Галерея";
static HANDLE ghInstance;/** Дескриптор приложения  **/
static HWND hListBox, hComboBox;/** Дескриптор listbox главного окна и дескриптор combobox окна редактирования  **/
SPISOK baza;/** Список  **/

 int i;
 LPCSTR masGenre[6] = {"Портрет","Пейзаж","Живопись",
                        "Натюрморт","Автопортрет","Марина"};
/** WinMain *****************************************************************/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc;
    WNDCLASSEX wc;

    ghInstance = hInstance;

    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icc);

    wc.cbSize = sizeof(wc);
    if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wc))
        return 0;

    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wc.lpszClassName = szAppName;
    if (!RegisterClassEx(&wc))
        return 0;

    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
}
int AskConfirmation(HWND hwndDlg)
{
  return MessageBox(hwndDlg, "Вы действительно хотите закрыть программу?",
                    szAppName, MB_YESNO | MB_ICONQUESTION);
}

/** MainDlgProc *************************************************************/

static INT_PTR CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
          hListBox=GetDlgItem(hwndDlg,ID_LBOX);
	      static LOGFONT lf;	
	        lf.lfHeight=14;
	        lstrcpy((LPSTR)&lf.lfFaceName,"Courier");
	        HFONT hFont1=CreateFontIndirect(&lf);
	        SendMessage(hListBox,WM_SETFONT,(WPARAM)hFont1,TRUE);
	      char name[30]; 
	      int n;
            return TRUE;

        case WM_SIZE:
            return TRUE;

        case WM_COMMAND:
   
            switch (GET_WM_COMMAND_ID(wParam, lParam))
            {
              case IDD_NEW:
                  {
                   DeleteSp(&baza);
                   Print(&baza);
                   break;}
         
              case IDD_ADD:
                  {
                   if(DialogBox(ghInstance, MAKEINTRESOURCE(O_ADD),
                   hwndDlg, (DLGPROC)AddDlgProc))
                   Print(&baza);
                   break;}

              case IDD_OPEN:
                  {
                   DialogBox(ghInstance, MAKEINTRESOURCE(O_OPEN),
                   hwndDlg, (DLGPROC)OpenDlgProc);
                   Print(&baza);
                   break;}

              case IDD_SAVE:
                  {
                   DialogBox(ghInstance, MAKEINTRESOURCE(O_OPEN),
                   hwndDlg, (DLGPROC)SaveDlgProc);
                   break;}

              case IDD_SAVETXT:
                  {
                   DialogBox(ghInstance, MAKEINTRESOURCE(O_OPEN),
                   hwndDlg, (DLGPROC)SaveTxtDlgProc);
                   break;}
         
              case IDD_DEL:
                  {
                   n=SendMessage(hListBox,LB_GETCURSEL,0,0);
              		if(n<0) 
                     {
                      MessageBox(hwndDlg,"Выберете элемент в списке",NULL,MB_OK);
                      break;}
                  else
                     {
                      Delete(&baza,n);
                      Print(&baza);
                      break;}
                  }

              case IDD_EDIT:
                  {
                   n=SendMessage(hListBox,LB_GETCURSEL,0,0);
              		 if(n<0)
                     {
                      MessageBox(hwndDlg,"Выберете элемент в списке",NULL,MB_OK);
                      break;} 
                  else
                     {
                      baza.curr=baza.head;
              		for(int i=0;i<n;i++)
                    baza.curr=baza.curr->next;
              		if (DialogBox(ghInstance, MAKEINTRESOURCE(O_ADD),
                      hwndDlg, (DLGPROC)EditDlgProc)) 
                      Delete(&baza,n);
                      Print(&baza);
                      break;
                     }
                  }

              case IDD_POISK:
                  {
                   DialogBox(ghInstance, MAKEINTRESOURCE(O_OPEN),
                   hwndDlg, (DLGPROC)PoiskDlgProc);
                   break;
                  }

              case IDD_SORT:
                  {
                   DialogBox(ghInstance, MAKEINTRESOURCE(O_SORT),
                   hwndDlg, (DLGPROC)SortDlgProc);
                   Print(&baza);
                   break;
                  }

              case IDD_FILTR:
                  {
                   DialogBox(ghInstance, MAKEINTRESOURCE(O_FILTR),
                   hwndDlg, (DLGPROC)FiltrDlgProc);
                   break;
                  }

              case IDD_INFO:
                  {
                   GetDlgItemText(hwndDlg,ID_NAME,name,30);
                   Info(&baza,name);
                   break;
                  }

              case IDD_NOOT:
                  {
                   Print(&baza);
                   break;
                  }

              case IDCANCEL:
                   if(IDYES == AskConfirmation(hwndDlg))
                   EndDialog(hwndDlg, TRUE);
                   return TRUE;
            }
        break;

        case WM_CLOSE:
            if(IDYES == AskConfirmation(hwndDlg))
            EndDialog(hwndDlg, 0);
            return TRUE;

    }

    return FALSE;
}
/** Функция печати всего списка   **/
void Print (SPISOK *spisok)
 {
	   char s[150];
	   SendMessage(hListBox,LB_RESETCONTENT,0,0);
	   spisok->curr=spisok->head;
	   while (spisok->curr)
      {	
	     sprintf(s,"%31s %21s %11s %20s %10u %9u",
               spisok->curr->picture.Name,spisok->curr->picture.Avt,
		spisok->curr->picture.Date,spisok->curr->picture.Genre,
        spisok->curr->picture.Price,spisok->curr->picture.Zal);
	     SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)s);
	     spisok->curr=spisok->curr->next;
      }	
 }

static LRESULT CALLBACK AddDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
   PICTURE tmp;
   char s[30];
   switch (uMsg)
    {
     case WM_INITDIALOG:
         hComboBox=GetDlgItem(hDlg,ID_GENRE);
         for (i = 0; i < 6; i++)
         SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)masGenre[i]);
         SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
   
     case WM_COMMAND:
       switch (GET_WM_COMMAND_ID(wParam, lParam))
        {
            case IDOK:
             {
              GetDlgItemText(hDlg,ID_NAME,tmp.Name,32);
              if(strlen(tmp.Name)>30)
                {
                 MessageBox(hDlg,"Длинное название",NULL,MB_OK);
                 return FALSE;}
              if(strlen(tmp.Name)<1)
                {
                 MessageBox(hDlg,"Не введено ни одного символа",NULL,MB_OK);
                 return FALSE;}
             GetDlgItemText(hDlg,ID_AVT,tmp.Avt,22);
              if(strlen(tmp.Avt)>20)
                {
                 MessageBox(hDlg,"Длинное имя",NULL,MB_OK);
                 return FALSE;
                }
             GetDlgItemText(hDlg,ID_DATE,tmp.Date,12);
              if(strlen(tmp.Date)!=10)
                {
                 MessageBox(hDlg,"Дата должна быть в формате дд.мм.гггг",NULL,MB_OK);
                 return FALSE;
                }
		          GetDlgItemText(hDlg,ID_GENRE,tmp.Genre,20);
			        if(HIWORD(wParam)==LBN_SELCHANGE)
			          {
                 SendMessage(hComboBox, CB_GETLBTEXT,
                 SendMessage(hComboBox, CB_SETCURSEL, 0, 0),
                 (LPARAM)s);
			           strcpy(tmp.Genre,s);
			          }
              GetDlgItemText(hDlg,ID_PRICE,s,6);
              if(strlen(s)==0)
                {
                 MessageBox(hDlg,"Введите стоимость картины",NULL,MB_OK);
                 return FALSE;
                }
              tmp.Price=atoi(s);

              GetDlgItemText(hDlg,ID_ZAL,s,5);
              if(strlen(s)>3)
                {
                 MessageBox(hDlg,"Номер зала не корректен",NULL,MB_OK);
                 return FALSE;
                }
              tmp.Zal=atoi(s);


  
              if (Add(&baza,tmp))
                {
                 MessageBox(hDlg,"Не удалось добавить элемент",NULL,MB_OK);
                 return FALSE;
                }
              EndDialog(hDlg, 1);
              return TRUE;
             }

         case IDCANCEL:
              EndDialog(hDlg, 1);
              return TRUE;
        }
     break;
     case WM_CLOSE:
          EndDialog(hDlg, 1);
          return TRUE;
    }
  return FALSE;
 }

static LRESULT CALLBACK OpenDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
	char s[99];

    	switch (uMsg)
        {
        case WM_COMMAND:
            switch (wParam)
               {
                case IDOK:
                    {
                     GetDlgItemText(hDlg,ID_OPEN,s,99); 
				             if (Load(&baza,strcat(s,".bin")))
                     MessageBox(hDlg,"Не удалось открыть файл",NULL,MB_OK);
                    }
                case IDCANCEL:
                     EndDialog(hDlg, TRUE);
                     return TRUE;
               }
             break;
        }  
  return FALSE;
 }

static LRESULT CALLBACK SaveDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
  char s[99];

  switch (uMsg)
    {
     case WM_COMMAND:
         switch (wParam)
            {
             case IDOK:
                 {
                  GetDlgItemText(hDlg,ID_OPEN,s,99); 
		              if (Save(&baza,strcat(s,".bin")))
                  MessageBox(hDlg,"Не удалось сохранить файл",NULL,MB_OK);
                 }
            case IDCANCEL:                 
                EndDialog(hDlg, TRUE);
                return TRUE;
            }
         break;
    } 
  return FALSE;
 }

static LRESULT CALLBACK SaveTxtDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
  char s[99];

  switch (uMsg)
    {
     case WM_COMMAND:
         switch (wParam)
            {
             case IDOK:
                 {
                  GetDlgItemText(hDlg,ID_OPEN,s,99); 
		              if (SaveTxt(&baza,strcat(s,".txt")))
                  MessageBox(hDlg,"Не удалось сохранить файл",NULL,MB_OK);
                 }
            case IDCANCEL:                 
                EndDialog(hDlg, TRUE);
                return TRUE;
            }
         break;
    } 
  return FALSE;
 }

static LRESULT CALLBACK EditDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
  ELEMENT *tmpe=baza.curr;
  PICTURE tmp;
	char s[30];

  switch (uMsg)
     {
	    case WM_INITDIALOG:
          {
      			SetDlgItemText(hDlg,ID_NAME,tmpe->picture.Name);
      			SetDlgItemText(hDlg,ID_AVT,tmpe->picture.Avt);
      			SetDlgItemText(hDlg,ID_DATE,tmpe->picture.Date);
          hComboBox=GetDlgItem(hDlg,ID_GENRE);
            for (i = 0; i < 6; i++)
            SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)masGenre[i]);
            SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
      			SetDlgItemText(hDlg,ID_GENRE,tmpe->picture.Genre);
            sprintf(s,"%u",tmpe->picture.Price);
      			SetDlgItemText(hDlg,ID_PRICE,s);

            sprintf(s,"%u",tmpe->picture.Zal);
      			SetDlgItemText(hDlg,ID_ZAL,s);

  

             return TRUE;
          }
	    case WM_COMMAND:
          switch (wParam)
            {
   case IDOK:
             {
              GetDlgItemText(hDlg,ID_NAME,tmp.Name,32);
              if(strlen(tmp.Name)>30)
                {
                 MessageBox(hDlg,"Длинное название",NULL,MB_OK);
                 return FALSE;}
              if(strlen(tmp.Name)<1)
                {
                 MessageBox(hDlg,"Не введено ни одного символа",NULL,MB_OK);
                 return FALSE;}
             GetDlgItemText(hDlg,ID_AVT,tmp.Avt,22);
              if(strlen(tmp.Avt)>20)
                {
                 MessageBox(hDlg,"Длинное имя",NULL,MB_OK);
                 return FALSE;
                }
             GetDlgItemText(hDlg,ID_DATE,tmp.Date,12);
              if(strlen(tmp.Date)!=10)
                {
                 MessageBox(hDlg,"Дата должна быть в формате дд.мм.гггг",NULL,MB_OK);
                 return FALSE;
                }
		          GetDlgItemText(hDlg,ID_GENRE,tmp.Genre,20);
			        if(HIWORD(wParam)==LBN_SELCHANGE)
			          {
                 SendMessage(hComboBox, CB_GETLBTEXT,
                 SendMessage(hComboBox, CB_SETCURSEL, 0, 0),
                 (LPARAM)s);
			           strcpy(tmp.Genre,s);
			          }
              GetDlgItemText(hDlg,ID_PRICE,s,6);
              if(strlen(s)==0)
                {
                 MessageBox(hDlg,"Введите стоимость картины",NULL,MB_OK);
                 return FALSE;
                }
              tmp.Price=atoi(s);

              GetDlgItemText(hDlg,ID_ZAL,s,5);
              if(strlen(s)>3)
                {
                 MessageBox(hDlg,"Номер зала не корректен",NULL,MB_OK);
                 return FALSE;
                }
              tmp.Zal=atoi(s);


  
              if (Add(&baza,tmp))
                {
                 MessageBox(hDlg,"Не удалось добавить элемент",NULL,MB_OK);
                     return FALSE;
      			        }
                  EndDialog(hDlg, 1);
                  return TRUE;
                 }

             case IDCANCEL:
                  EndDialog(hDlg, 0);
                  return TRUE;
	          }
          break;

          case WM_CLOSE:
            EndDialog(hDlg, 0);
            return TRUE;
     }
   return FALSE;
 }

static LRESULT CALLBACK PoiskDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
  char s[42];
  static int nom;

  switch (uMsg)
     {   
      case WM_INITDIALOG:
           {
            SetDlgItemText(hDlg,ID_OPEN,"поиск");
           }

      case WM_COMMAND:
           switch (wParam)
              { 
		           case MAKELONG(ID_OPEN, EN_CHANGE):
                   {
                    baza.curr=baza.head;nom=0;
                    break;
                   }
				 
		           case IDOK:
                   {
                    GetDlgItemText(hDlg,ID_OPEN,s,32);
				            nom=Poisk(&baza,s,nom);
				            if(nom>0)
                      SendMessage(hListBox,LB_SETCURSEL,nom-1,0);
				            else 
                      MessageBox(hDlg,"Объект не найден",NULL,MB_OK);
                      break;
                   }

        	     case IDCANCEL:
                   while (baza.curr)baza.curr=baza.curr->next;
                   EndDialog(hDlg, TRUE);
              }
           break;
     }
  return FALSE;
 }

static LRESULT CALLBACK SortDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
  static int x1,x2;

    switch (uMsg)
     {
      case WM_INITDIALOG:
        {
         CheckRadioButton(hDlg,ID_NAME,ID_AVT,ID_NAME);
			   CheckRadioButton(hDlg,ID_V,ID_Y,ID_V);
	       x1=1;x2=0;return TRUE;
        }
      case WM_COMMAND:
         switch (wParam)
          {	
      		 case ID_NAME: x1=1; return TRUE;
			case ID_AVT: x1=2; return TRUE;
			case ID_DATE: x1=3; return TRUE;
      		 case ID_GENRE: x1=4; return TRUE;
           case ID_PRICE: x1=5; return TRUE;
      		 case ID_ZAL: x1=6; return TRUE;


      		 case ID_V: x2=0; return TRUE;
      		 case ID_Y: x2=1; return TRUE;

           case IDOK:
             Sort(&baza,x1,x2);

           case IDCANCEL:
             EndDialog(hDlg, TRUE);
             return TRUE;
          }
      break;
     }
  return FALSE;
 }
/** Функция фильтра   **/
void Filtr(SPISOK *spisok,char *s, int n)
 {
  SendMessage(hListBox,LB_RESETCONTENT,0,0);
  spisok->curr=spisok->head;
  char s1[150];

  switch (n)
    {

    	case 1:{ 
               while (spisok->curr)
                  {
    				       if(!strcmp(spisok->curr->picture.Name,s)) 
                     {
                      sprintf(s1,"%31s %21s %11s %20s %10u %9u",
               spisok->curr->picture.Name,spisok->curr->picture.Avt,
		spisok->curr->picture.Date,spisok->curr->picture.Genre,
        spisok->curr->picture.Price,spisok->curr->picture.Zal);
    	                        SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)s1);
                     }
    				       spisok->curr=spisok->curr->next;
                  }
                break;
             }
	case 2:{
            	while (spisok->curr)
                {
    				     if(!strcmp(spisok->curr->picture.Avt,s)) 
                   {
                    sprintf(s1,"%31s %21s %11s %20s %10u %9u",
               spisok->curr->picture.Name,spisok->curr->picture.Avt,
		spisok->curr->picture.Date,spisok->curr->picture.Genre,
        spisok->curr->picture.Price,spisok->curr->picture.Zal);
    	              SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)s1);
                   }
    				     spisok->curr=spisok->curr->next;
                }
              break;
             }
	case 3:{
            	while (spisok->curr)
                {
    				     if(!strcmp(spisok->curr->picture.Date,s)) 
                   {
                    sprintf(s1,"%31s %21s %11s %20s %10u %9u",
               spisok->curr->picture.Name,spisok->curr->picture.Avt,
		spisok->curr->picture.Date,spisok->curr->picture.Genre,
        spisok->curr->picture.Price,spisok->curr->picture.Zal);
    	              SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)s1);
                   }
    				     spisok->curr=spisok->curr->next;
                }
              break;
             }
    	case 4:{
            	while (spisok->curr) 
                {
    				     if(!strcmp(spisok->curr->picture.Genre,s)) 
                   {
		sprintf(s1,"%31s %21s %11s %20s %10u %9u",
               spisok->curr->picture.Name,spisok->curr->picture.Avt,
		spisok->curr->picture.Date,spisok->curr->picture.Genre,
        spisok->curr->picture.Price,spisok->curr->picture.Zal);
    	              SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)s1);
                   }
    				     spisok->curr=spisok->curr->next;
                }
              break;
             }
    	case 5:{
              while (spisok->curr)
               {
    				    if(spisok->curr->picture.Price==atoi(s)) 
                  {
		sprintf(s1,"%31s %21s %11s %20s %10u %9u",
               spisok->curr->picture.Name,spisok->curr->picture.Avt,
		spisok->curr->picture.Date,spisok->curr->picture.Genre,
        spisok->curr->picture.Price,spisok->curr->picture.Zal);
          	       SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)s1);
                  }
    				    spisok->curr=spisok->curr->next;
               }
              break;
             }

    	case 6:{
              while (spisok->curr)
                {
    				     if(spisok->curr->picture.Zal==atoi(s)) 
                   {
		sprintf(s1,"%31s %21s %11s %20s %10u %9u",
               spisok->curr->picture.Name,spisok->curr->picture.Avt,
		spisok->curr->picture.Date,spisok->curr->picture.Genre,
        spisok->curr->picture.Price,spisok->curr->picture.Zal);
            	      SendMessage(hListBox,LB_ADDSTRING,0,(LPARAM)s1);
                   }
    				     spisok->curr=spisok->curr->next;
                }
              break;
             }

    }
 }

static LRESULT CALLBACK FiltrDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
  static int x1;
	char s1[50];

    switch (uMsg)
       {
        case WM_INITDIALOG:
            {
             CheckRadioButton(hDlg,ID_NAME,ID_ZAL,ID_NAME);
             x1=1;
             return TRUE;
            }
        case WM_COMMAND:
            switch (wParam)
               {
      		 case ID_NAME: x1=1; return TRUE;
			case ID_AVT: x1=2; return TRUE;
			case ID_DATE: x1=3; return TRUE;
      		 case ID_GENRE: x1=4; return TRUE;
           case ID_PRICE: x1=5; return TRUE;
      		 case ID_ZAL: x1=6; return TRUE;
            		case IDOK:
                    {
                     GetDlgItemText(hDlg,ID_OPEN,s1,30);
                     Filtr(&baza,s1,x1);
                     break;
                    }		
                case IDCANCEL:
                    Print(&baza);
                    EndDialog(hDlg, TRUE);
                    return TRUE;
               }
            break;
       } 
   return FALSE;
 }
/** Функция дополнительной информации     **/
void Info(SPISOK *spisok,char *s)
 {
  SendMessage(hListBox,LB_RESETCONTENT,0,0);
  char name[30],s1[1000];
  spisok->curr=spisok->head;

	while (spisok->curr)
     {
      sscanf(baza.curr->picture.Name,"%s",name);
			if(!strcmp(spisok->curr->picture.Name,s)) 
        {
         sprintf(s1,"%s %u %u %s ","Зал:",spisok->curr->picture.Zal,spisok->curr->picture.Price, "руб.");
         Print(&baza);
         MessageBox(hListBox,s1,"Стоимость картины в зале:",MB_OK);
         return 0;
        }
		 spisok->curr=spisok->curr->next;
     }
      sscanf(s,"%s",name);
      sprintf(s,"%s",name);
      if(spisok->curr==NULL)
        {
         Print(&baza);
         MessageBox(hListBox,"Нет такого зала",NULL,MB_OK|MB_ICONINFORMATION);
         return 0;
			  }
 }
