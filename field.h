/////////////////////////////////////////////////////////////////////////////
//
// FIELD.H : Step file field.
//
// A field is defined recursively as a string or a list of other fields.
//

#ifndef __FIELD_H__
#define __FIELD_H__

/////////////////////////////////////////////////////////////////////////////
class Field
{
	// Construction / Destruction
public:
	Field();
	Field(const wchar_t* val);
	~Field();

	// Interface
	enum FieldType { FT_EMPTY, FT_VALUE, FT_LIST };

	FieldType getType() const { return type; }

	// Implementation
private:
	void destroy();
	void destroyV();

	union {
		wchar_t *value;
		Field **fields;
	};

	FieldType type;
	uint32_t numFields;	// for list
};

typedef Field* LPFIELD;
typedef vector<LPFIELD> fieldvec;

#endif // __FIELD_H__
