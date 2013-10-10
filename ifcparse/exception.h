/////////////////////////////////////////////////////////////////////////////
//
//	EXCEPTION.H : simple exception class
//

#ifndef __EXCEPTION_INCLUDE_H__
#define __EXCEPTION_INCLUDE_H__

/////////////////////////////////////////////////////////////////////////////
class Exception
{
public:
	Exception(const wstring &msg) { description = msg; }
	Exception(const Exception & E) { *this = E; }
	Exception(const wchar_t *fmt, ...);
	Exception & operator = (const Exception & E) {
		if (this != & E)
			description = E.description;
		return *this;
	}

	wstring getDescription() const { return description; }
private:
	wstring description;
};
/////////////////////////////////////////////////////////////////////////////

#endif // __EXCEPTION_INCLUDE_H__