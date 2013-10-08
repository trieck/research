/////////////////////////////////////////////////////////////////////////////
//
// ATTRIBUTE.H : Step file attribute.
//
// An attribute is defined recursively as a value or a list of other 
// attributes:
//
// attribute := value | list
// value := '{string}'
// list  := ( attribute, attribute, ... )
// 

#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

/////////////////////////////////////////////////////////////////////////////
class Attribute
{
	// Construction / Destruction
public:
	Attribute();
	Attribute(const wchar_t* val);
	~Attribute();

	// Interface
	void clear();

	enum AttrType { AT_EMPTY, AT_VALUE, AT_LIST };
	AttrType getType() const { return type; }
	
	void add(const wstring& value);
	void add(Attribute *pAttr);

	// Implementation
private:
	void alloclist();
	void clearR();
	union {
		wchar_t *value;
		Attribute **attribs;
	};

	AttrType type;
	uint32_t numAttrs;	// for list
	uint32_t allocSize;	// allocated size of list
};

#endif // __ATTRIBUTE_H__
