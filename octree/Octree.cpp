/////////////////////////////////////////////////////////////////////////////
// 
// OCTREE.CPP : Octree data structure
// Each node in an octree subdivides the space it represents into eight 
// octants. The subdivision point is implicitly the center of the space the 
// node represents. The root node of an octree must represent a finite 
// bounded space so that the implicit centers are well-defined.
//

#include "stdafx.h"
#include "Octree.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
Octree::Octree(const Vector& origin, const Vector& halfDims) : data(nullptr)
{
    this->origin = origin;
    this->halfDims = halfDims;
    memset(children, 0, sizeof(Octree*) * 8);
}

/////////////////////////////////////////////////////////////////////////////
Octree::~Octree()
{
    destroy();
}

/////////////////////////////////////////////////////////////////////////////
void Octree::destroy()
{
    if (data != nullptr) {
        delete data;
        data = nullptr;
    }

    for (int i = 0; i < 8; ++i)
        delete children[i];
}

/////////////////////////////////////////////////////////////////////////////
bool Octree::isLeaf() const
{
    return children[0] == nullptr;
}

/////////////////////////////////////////////////////////////////////////////
uint8_t Octree::octant(const Datum& datum) const
{
    return octant(datum.getPosition());
}

/////////////////////////////////////////////////////////////////////////////
uint8_t Octree::octant(const Vector& point) const
{
    uint8_t octant = 0;

    if (point.x >= origin.x) octant |= 4;
    if (point.y >= origin.y) octant |= 2;
    if (point.z >= origin.z) octant |= 1;

    return octant;
}

/////////////////////////////////////////////////////////////////////////////
void Octree::insert(const Datum& d)
{
    if (isLeaf()) {
        if (data == nullptr) {
            data = new Datum(d);
            return;
        }

        // retain original value
        Datum* save = data;
        data = nullptr;

        // split the tree
        for (int i = 0; i < 8; ++i) {
            Vector newOrigin = origin;
            newOrigin.x += halfDims.x * (i & 4 ? .5f : -.5f);
            newOrigin.y += halfDims.y * (i & 2 ? .5f : -.5f);
            newOrigin.z += halfDims.z * (i & 1 ? .5f : -.5f);
            children[i] = new Octree(newOrigin, halfDims * .5f);
        }

        // re-insert original value
        children[octant(*save)]->insert(*save);
    }
    children[octant(d)]->insert(d);
}

/////////////////////////////////////////////////////////////////////////////
Region Octree::getRegion() const
{
    // compute min/max corners of the octant
    return Region(origin - halfDims, origin + halfDims);
}

/////////////////////////////////////////////////////////////////////////////
void Octree::query(const Region& region, DatumVec& results) const
{
    if (isLeaf()) {
        if (data != nullptr) {
            const Vector& v = data->getPosition();
            if (region.contains(v)) results.push_back(*data);
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            // check if query intersects child's bounding region
            Region childRegion = children[i]->getRegion();
            if (region.intersects(childRegion))
                children[i]->query(region, results);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void Octree::query(const wstring& q, DatumVec& results) const
{
    query(q, 0, results);
}

/////////////////////////////////////////////////////////////////////////////
void Octree::query(const wstring& q, uint32_t index, DatumVec& results) const
{
    if (index >= q.length()) {
        if (isLeaf()) {
            if (data != nullptr)
                results.push_back(*data);
        } else {
            for (int i = 0; i < 8; ++i) {
                children[i]->query(q, index, results);
            }
        }
    } else if (!isLeaf()) {
        wchar_t c = q[index];
        if (c < L'0' || c > L'7')
            return; // bad query

        uint8_t octant = c - L'0';

        children[octant]->query(q, index + 1, results);
    }
}

/////////////////////////////////////////////////////////////////////////////
bool Octree::contains(const Vector& point) const
{
    if (isLeaf()) {
        if (data != nullptr) {
            return point == data->getPosition();
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            // check if point is contained in child's bounding region
            Region childRegion = children[i]->getRegion();
            if (childRegion.contains(point))
                if (children[i]->contains(point))
                    return true;
        }
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////
wchar_t Octree::encodeOctant(uint8_t octant) const
{
    return L'0' + (octant & 7);
}

/////////////////////////////////////////////////////////////////////////////
wstring Octree::encode(const Vector& point) const
{
    ByteStack stack;

    if (!encodeR(point, stack))
        return L"";

    wstring output;
    uint8_t octant;
    while (!stack.empty()) {
        octant = stack.top();
        output += encodeOctant(octant);
        stack.pop();
    }

    return output;
}

/////////////////////////////////////////////////////////////////////////////
bool Octree::encodeR(const Vector& point, ByteStack& stack) const
{
    if (isLeaf()) {
        if (data != nullptr) {
            if (point == data->getPosition()) {
                return true;
            }
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            // check if point is contained in child's bounding region
            Region childRegion = children[i]->getRegion();
            if (childRegion.contains(point)) {
                if (children[i]->encodeR(point, stack)) {
                    stack.push(octant(point));
                    return true;
                }
            }
        }
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////
void Octree::Walk(PWALKER pWalker)
{
    if (isLeaf()) {
        if (data != nullptr) {
            pWalker->Call(data);
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            children[i]->Walk(pWalker);
        }
    }
}
