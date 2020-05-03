//
// Created by Adam Christa on 4/24/20.
//

#ifndef HUNGARIANALGORITHM_VALUE_H
#define HUNGARIANALGORITHM_VALUE_H


#include "Matrix.h"

/*
 * This is the Value class. It is templated and represents an element in a Matrix. The data can be starred or primed
 * using the isStarred boolean or isPrimed boolean.
 */

using namespace std;

template<class T>
class Value
{

private:

    T data;
    bool isStarred;
    bool isPrimed;


public:

    template<class U> friend class RowOrColumn;
    template<class V> friend class Matrix;

    explicit Value(T);


};



template<class T>
Value<T>::Value(T val): data(val), isStarred(false), isPrimed(false){}





#endif //HUNGARIANALGORITHM_VALUE_H

