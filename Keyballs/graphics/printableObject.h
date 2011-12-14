#ifndef PRINTABLEOBJECT_H
#define PRINTABLEOBJECT_H

#include <string>

//Font Class
class PrintableObject
{
public:
	//Methods
	PrintableObject();
	PrintableObject(int size);
	PrintableObject(wchar_t *string);
	~PrintableObject();
	void SetText(wchar_t *string);
	wchar_t *GetText();
	void SetDisplay(bool display);
	bool GetDisplay();

private:
	//Data Members
	bool display;
	wchar_t *text;
};

#endif