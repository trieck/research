#ifndef __VECTOR_H__
#define	__VECTOR_H__

/////////////////////////////////////////////////////////////////////////////
class Vector
{
public:
    Vector();
    Vector(float x, float y, float z);
    Vector(const Vector& orig);
    virtual ~Vector();

    Vector& operator =(const Vector& rhs);
    Vector operator +(const Vector& r) const;
    Vector operator -(const Vector& r) const;
    Vector operator *(float r) const;
    Vector operator /(float r) const;

    float x, y, z;
    friend bool operator ==(const Vector& lhs, const Vector& rhs);
};

#endif	// __VECTOR_H__
