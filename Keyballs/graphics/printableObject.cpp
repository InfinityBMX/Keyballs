#include "printableObject.h"

PrintableObject::PrintableObject(){
	this->text = new wchar_t[25];
	this->display = false;
}

PrintableObject::PrintableObject(int size){
	this->text = new wchar_t[size];
	this->display = false;
}

PrintableObject::PrintableObject(wchar_t *string){
	this->text = new wchar_t[(wcslen(string))];
	this->display = false;
}

PrintableObject::~PrintableObject(){
	delete this->text;
}

void PrintableObject::SetText(wchar_t *string){
	this->text = new wchar_t[wcslen(string)];
	this->text = string;
}

wchar_t *PrintableObject::GetText(){
	return text;
}

void PrintableObject::SetDisplay(bool display){
	this->display = display;
}

bool PrintableObject::GetDisplay(){
	return this->display;
}