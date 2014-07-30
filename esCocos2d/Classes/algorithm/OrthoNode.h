//  Created by ZK on 14-5-29.

#pragma  once

class OrthoEdge;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)   do { if(p) { delete (p); (p) = 0; } } while(0)
#endif

class OrthoNode
{
public:
    int			_data;
    OrthoNode*	_parent;
    int			_distance_from_start;
    bool		_choosen;
    bool		_validate;

    inline bool  operator== (const OrthoNode& n) const
    {
        return  this->_data == n._data;
    }

    inline bool  operator!= (const OrthoNode& n) const
    {
        return ! (*this == n);
    }
    OrthoNode (int data_,
               OrthoEdge* nextOut_=0,
               OrthoEdge* nextIn_=0) :
        _data (data_),
        _nextIn (nextIn_),
        _nextOut (nextOut_),
        _validate (true)
    {
        init();
    }
    virtual ~OrthoNode();
    void init();

    void AddNextOut (OrthoEdge* next);
    void AddNextIn (OrthoEdge* next);
    OrthoEdge*	get_nextOut()const
    {
        return _nextOut;
    }
    OrthoEdge*	get_nextIn()const
    {
        return _nextIn;
    }
private:
    OrthoEdge*	_nextOut;
    OrthoEdge*	_nextIn;
    void AddNextOut (OrthoEdge** e, OrthoEdge* next);
    void AddNextIn (OrthoEdge** e, OrthoEdge* next);

};