//#include "../main/includes.h"
#include <Windows.h>
#include <tchar.h>

void debug(LPWSTR message){
	TCHAR temp[20]={0};
	wsprintf(temp, message);
	MessageBox(NULL,temp, TEXT("message"),MB_OK);
}