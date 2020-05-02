//
// Created by Adam Christa on 4/24/20.
//

#ifndef HUNGARIANALGORITHM_ROWORCOLUMN_H
#define HUNGARIANALGORITHM_ROWORCOLUMN_H


#include <vector>
#include "Value.h"
#include "Matrix.h"

//#include "Matrix.h"

template<class T>
class RowOrColumn
{
    template<class U> friend class Matrix;
    template<class V> friend class Value;

private:

    vector<Value<T>> allValues;
    bool isRow;
    bool isColumn;
    bool isCovered;
    int starredZero;




public:

    RowOrColumn();
    void addTo(Value<T>&);
    void setRow();
    void setCol();

};


template<class T>
RowOrColumn<T>::RowOrColumn(): allValues(), isRow(false), isColumn(false), isCovered(false), starredZero(-1){}

template<class T>
void RowOrColumn<T>::addTo(Value<T>& currVal)
{
    allValues.push_back(currVal);
}

template<class T>
void RowOrColumn<T>::setRow() {
    isRow = true;

}

template<class T>
void RowOrColumn<T>::setCol() {
    isColumn = true;

}


#endif //HUNGARIANALGORITHM_ROWORCOLUMN_H
