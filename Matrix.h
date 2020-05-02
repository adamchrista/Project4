//
// Created by Adam Christa on 4/24/20.
//

#ifndef HUNGARIANALGORITHM_MATRIX_H
#define HUNGARIANALGORITHM_MATRIX_H

#include "RowOrColumn.h"
#include <algorithm>

template<class T>
class Matrix
{
    template<class U> friend class RowOrColumn;
    template<class V> friend class Value;



private:

    vector<RowOrColumn<T>> allRows;
    vector<RowOrColumn<T>> allColumns;

    void resetStars(int);




public:

     Matrix();
     void addRow(RowOrColumn<T>&);
     void addCol(RowOrColumn<T>&);
     void doStepOne();
     void doStepTwo();
     void doStepThree(vector<string>, vector<string>, vector<int>, int&);
     void doStepFour(vector<string>, vector<string>, vector<int>,int&);
     void doStepFive(int, vector<string>, vector<string>, vector<int>, int&);
     void doStepSix(int, vector<string>, vector<string>, vector<int>, int&);
     void doFinalStep(vector<string>, vector<string>, vector<int>, int&);

     void colReduction();
     void createZeroMatrix(Matrix<T>&);
     vector<int> getToBeCovered();
     vector<int> getToBeCovered2();
     void coverRowsAndCols(vector<int>);
     void reset(vector<int>);
     void findOptimalAssignment(vector<string>, vector<string>, vector<int>);
     bool findIfUsefulResult();
     int hasAnyAssignments(int);
     void addAssignments();
     void markRowsNoAssignments(vector<int>&);
     void markColumns(vector<int>&, vector<int>);
     void markRowsWithAssignments(vector<int>&, vector<int>);

     int getSize();
     void print();
     void recursiveOptimal(vector<int>&);
     void recursiveInner(int, vector<int>&);
     void primeMinVal(int);
     void setAllPrimeToFalse();

};


template<class T>
void Matrix<T>::addRow(RowOrColumn<T>& currRow)
{
    allRows.push_back(currRow);
}

template<class T>
void Matrix<T>::addCol(RowOrColumn<T>& currCol)
{
    allColumns.push_back(currCol);
}

template<class T>
void Matrix<T>::doStepOne()
{
    for (int i = 0; i < allRows.size(); i++)
    {
        T minVal = 1000000;
        for (int j = 0; j < allRows[i].allValues.size(); j++)
        {
            if (allRows[i].allValues[j].data < minVal)
            {
                minVal = allRows[i].allValues[j].data;
            }
        }

        for (int j = 0; j < allRows[i].allValues.size(); j++)
        {
            allRows[i].allValues[j].data = allRows[i].allValues[j].data - minVal;
        }

    }

    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < allRows.size(); j++)
        {
            allColumns[i].allValues[j].data = allRows[j].allValues[i].data;
        }
    }

}

template<class T>
void Matrix<T>::doStepTwo()
{
    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < allRows[i].allValues.size(); j++)
        {
            if (allRows[i].allValues[j].data == 0)
            {
                if (allRows[i].starredZero == -1 && allColumns[j].starredZero == -1)
                {
                    allRows[i].starredZero = j;
                    allColumns[j].starredZero = i;
                    allRows[i].allValues[j].isStarred = true;
                    allColumns[j].allValues[i].isStarred = true;
                }
            }
        }
    }
}

template<class T>
void Matrix<T>::doStepThree(vector<string> allRowNames, vector<string> allColNames, vector<int> allWeights, int& done)
{
    int numRowsCovered = 0;
    for (int i = 0; i < allColumns.size(); i++)
    {
        if (allColumns[i].starredZero > -1)
        {
            allColumns[i].isCovered = true;
            numRowsCovered++;
        }
    }

    if (numRowsCovered == allRows.size())
    {
        doFinalStep(allRowNames, allColNames, allWeights, done);
    }
    else
    {
        doStepFour(allRowNames, allColNames, allWeights, done);
    }
}



template<class T>
void Matrix<T>::doStepFour(vector<string> allRowNames, vector<string> allColNames, vector<int> allWeights, int& done)
{
     int minVal = 1000000;
     for (int i = 0; i < allRows.size(); i++)
     {
         for (int j = 0; j < allColumns.size(); j++)
         {
             if (done == 0)
             {
                 if (allRows[i].allValues[j].data == 0 && allColumns[j].isCovered == false)
                 {
                     allRows[i].allValues[j].isPrimed = true;
                     allColumns[j].allValues[i].isPrimed = true;

                     if (allRows[i].starredZero == -1)
                     {
                         doStepFive(i, allRowNames, allColNames, allWeights, done);
                     }

                     if (done == 0)
                     {
                         allRows[i].isCovered = true;
                         allColumns[allRows[i].starredZero].isCovered = false;
                     }

                     break;

                 }
             }


             if (done == 0)
             {
                 if (allRows[i].allValues[j].data < minVal && allRows[i].isCovered == false &&
                     allColumns[j].isCovered == false)
                 {
                     minVal = allRows[i].allValues[j].data;
                 }
             }

         }
     }

     if (done == 0)
     {
         doStepSix(minVal, allRowNames, allColNames, allWeights, done);
     }

}


template<class T>
void Matrix<T>::doStepFive(int index, vector<string> allRowNames, vector<string> allColNames, vector<int> allWeights, int& done)
{
     bool starredZero = true;
     while (starredZero)
     {
         int newIndex = -1;
         int correctI = -1;
         for (int i = 0; i < allRows[index].allValues.size(); i++)
         {
             if (allRows[index].allValues[i].isPrimed == true)
             {
                 correctI = i;
                 if (allColumns[i].starredZero > -1)
                 {
                     newIndex = allColumns[i].starredZero;
                 }
                 else
                 {
                     starredZero = false;
                 }
             }
         }


         allRows[index].allValues[correctI].isPrimed = false;
         allColumns[correctI].allValues[index].isPrimed = false;

         allRows[index].allValues[correctI].isStarred = true;
         allColumns[correctI].allValues[index].isStarred = true;

         allRows[index].starredZero = correctI;
         allColumns[correctI].starredZero = index;

         if (newIndex != -1)
         {
             allRows[newIndex].allValues[correctI].isStarred = false;
             allColumns[correctI].allValues[newIndex].isStarred = false;
         }

         index = newIndex;

     }


     for (int i = 0; i < allRows.size(); i++)
     {
         if (allRows[i].isCovered == true)
         {
             allRows[i].isCovered = false;
         }

         for (int j = 0; j < allColumns.size(); j++)
         {
             if (allColumns[j].isCovered == true)
             {
                 allColumns[j].isCovered == false;
             }

             if (allRows[i].allValues[j].isPrimed == true)
             {
                 allRows[i].allValues[j].isPrimed = false;
                 allColumns[j].allValues[i].isPrimed = false;
             }

         }
     }

     doStepThree(allRowNames, allColNames, allWeights, done);


}

template<class T>
void Matrix<T>::doStepSix(int val, vector<string> allRowNames, vector<string> allColNames, vector<int> allWeights, int& done)
{
    for (int i = 0; i < allRows.size(); i++)
    {
        if (allRows[i].isCovered == true)
        {
            for (int j = 0; j < allRows[i].allValues.size(); j++)
            {
                if (allRows[i].allValues[j].isStarred == false && allRows[i].allValues[j].isPrimed == false)
                {
                    allRows[i].allValues[j].data = allRows[i].allValues[j].data + val;
                    allColumns[j].allValues[i].data = allColumns[j].allValues[i].data + val;
                }
            }
        }
    }

    for (int i = 0; i < allColumns.size(); i++)
    {
        if (allColumns[i].isCovered == false)
        {
            for (int j = 0; j < allRows[i].allValues.size(); j++)
            {
                if (allColumns[i].allValues[j].isStarred == false && allColumns[i].allValues[j].isPrimed == false)
                {
                    allColumns[i].allValues[j].data = allColumns[i].allValues[j].data - val;
                    allRows[j].allValues[i].data = allRows[j].allValues[i].data - val;
                }
            }
        }
    }

    doStepFour(allRowNames, allColNames, allWeights, done);

}

template<class T>
void Matrix<T>::doFinalStep(vector<string> allRowNames, vector<string> allColNames, vector<int> allWeights, int& done)
{
    cout << "Optimal Assignment Found! O(n^3)" << endl;
    int totalWeight = 0;
    for (int i =0; i < allRows.size(); i++)
    {
        int columnOfStarredZero = allRows[i].starredZero;
        int indexWeight = (i*allRowNames.size()) + columnOfStarredZero;

        cout << allRowNames[i] << " -- " << allColNames[columnOfStarredZero] << "  (Weight: " << allWeights[indexWeight] << ")" << endl;
        totalWeight += allWeights[indexWeight];
    }

    cout << "Total Weight: " << totalWeight << endl;

    done = 1;

}

template<class T>
void Matrix<T>::colReduction()
{
    for (int i = 0; i < allColumns.size(); i++)
    {
        T minVal = 1000000;
        for (int j = 0; j < allColumns[i].allValues.size(); j++)
        {
            if (allColumns[i].allValues[j].data < minVal)
            {
                minVal = allColumns[i].allValues[j].data;
            }
        }

        for (int j = 0; j < allColumns[i].allValues.size(); j++)
        {
            allColumns[i].allValues[j].data = allColumns[i].allValues[j].data - minVal;
        }

    }

    for (int i = 0; i < allColumns.size(); i++)
    {
        for (int j = 0; j < allColumns.size(); j++)
        {
            allRows[i].allValues[j].data = allColumns[j].allValues[i].data;
        }
    }
}

template<class T>
void Matrix<T>::createZeroMatrix(Matrix<T>& zeroMatrix)
{
    for (int i = 0; i < allRows.size(); i++)
    {
        RowOrColumn<int> row;
        for (int j = 0; j < allColumns.size(); j++)
        {
            if (allRows[i].allValues[j].data != 0)
            {
                Value<int> val(0);
                row.addTo(val);
            }
            else
            {
                int numZerosInRow = 0;
                int numZerosInCol = 0;

                for (int k = 0; k < allRows.size(); k++)
                {
                    if (allRows[i].allValues[k].data == 0)
                    {
                        numZerosInRow++;
                    }
                }

                for (int l = 0; l < allRows.size(); l++)
                {
                    if (allColumns[j].allValues[l].data == 0)
                    {
                        numZerosInCol++;
                    }
                }

                int finalVal = 0;
                if (numZerosInRow > numZerosInCol)
                {
                    finalVal = numZerosInRow * -1;
                }
                else
                {
                    finalVal = numZerosInCol;
                }

                Value<int> val(finalVal);
                row.addTo(val);

        }



        }

        zeroMatrix.addRow(row);

    }

}

template<class T>
vector<int> Matrix<T>::getToBeCovered()
{
    vector<int> coveredRowsAndCols;

    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < allRows.size(); j++)
        {
            if (allRows[i].allValues[j].data != 0)
            {
                if (allRows[i].allValues[j].data > 0)
                {
                    int colToBeCovered = j + 1;
                    coveredRowsAndCols.push_back(colToBeCovered);
                }
                else
                {
                    int rowToBeCovered = (i + 1) * -1;
                    coveredRowsAndCols.push_back(rowToBeCovered);
                }
            }


        }
    }

    return coveredRowsAndCols;
}

template<class T>
void Matrix<T>::coverRowsAndCols(vector<int> toBeCovered) {

    for (int i = 0; i < toBeCovered.size(); i++)
    {
        if (toBeCovered[i] > 0)
        {
            allColumns[toBeCovered[i] - 1].isCovered = true;
        }
        else
        {
            int index = (toBeCovered[i] + 1) * -1;
            allRows[index].isCovered = true;
        }

    }

}

template<class T>
void Matrix<T>::reset(vector<int> toBeCovered)
{
    int minVal = 1000000;
    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < allRows[i].allValues.size(); j++)
        {
            if ((allRows[i].allValues[j].data < minVal) && allRows[i].allValues[j].data != 0)
            {
                if (allRows[i].isCovered == false && allColumns[j].isCovered == false)
                {
                    minVal = allRows[i].allValues[j].data;
                }
            }
        }
    }

    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < allRows[i].allValues.size(); j++)
        {
            if (allRows[i].isCovered == false && allColumns[j].isCovered == false)
            {
                allRows[i].allValues[j].data = allRows[i].allValues[j].data - minVal;
            }

            if (allRows[i].isCovered == true && allColumns[j].isCovered == true)
            {
                allRows[i].allValues[j].data = allRows[i].allValues[j].data + minVal;
            }

        }
    }

    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < allRows.size(); j++)
        {
            allColumns[i].allValues[j].data = allRows[j].allValues[i].data;
        }
    }

    for (int i = 0; i < toBeCovered.size(); i++)
    {
        if (toBeCovered[i] > 0)
        {
            allColumns[toBeCovered[i] - 1].isCovered = false;
        }
        else
        {
            int index = (toBeCovered[i] + 1) * -1;
            allRows[index].isCovered = false;
        }

    }

    for (int i = 0; i < allRows.size(); i++)
    {
        allRows[i].starredZero = -1;
        allColumns[i].starredZero = -1;
        for (int j = 0; j < allColumns.size(); j++)
        {
            allRows[i].allValues[j].isPrimed = false;
            allColumns[j].allValues[i].isPrimed = false;
        }
    }

}

template<class T>
void Matrix<T>::primeMinVal(int row) {

    int minVal = 1000000;
    for (int i = 0; i < allColumns.size(); i++)
    {
        if (allRows[row].allValues[i].data < minVal && allRows[row].allValues[i].isPrimed == false)
        {
            minVal = allRows[row].allValues[i].data;
        }
    }

    for (int j = 0; j < allColumns.size(); j++)
    {
        if (allRows[row].allValues[j].data == minVal)
        {
            allRows[row].allValues[j].isPrimed = true;
        }
    }



}

template<class T>
void Matrix<T>::findOptimalAssignment(vector<string> rowNames, vector<string> colNames, vector<int> allWeights)
{



    setAllPrimeToFalse();
    vector<int> curr;
    vector<int>::iterator it;
    int currRow = 0;
    bool reset = false;
    int highestRowReachedBeforePanic = 0;
    int maximumAllowable = 0;
    int onRebound = 0;
    int oldHighestReached = 0;

    while(curr.size() < allRows.size())
    {
        if (reset)
        {
            if (currRow == -1)
            {
                resetStars(currRow);
                if (highestRowReachedBeforePanic != oldHighestReached)
                {
                    highestRowReachedBeforePanic++;
                }
                primeMinVal(highestRowReachedBeforePanic);
                currRow = 0;
                onRebound = 1;
                oldHighestReached = highestRowReachedBeforePanic;
            }
            else
            {
                int resetCol = curr[curr.size() - 1];
                curr.pop_back();
                allRows[currRow].allValues[resetCol].isStarred = true;
                resetStars(currRow);
            }
        }
        int found = 0;
        for (int j = 0; j < allColumns.size(); j++)
        {

            int minVal = 1000000;
            for (int a = 0; a < allRows.size(); a++)
            {
                if (allRows[currRow].allValues[a].data < minVal &&  allRows[currRow].allValues[a].isPrimed == false)
                {
                    onRebound = 0;
                    //it = find(curr.begin(), curr.end(), a);
                    minVal = allRows[currRow].allValues[a].data;

                }
            }
            maximumAllowable = minVal;

            if (allRows[currRow].allValues[j].data == maximumAllowable && allRows[currRow].allValues[j].isStarred == false)
            {
                it = find(curr.begin(), curr.end(), j);
                if (it == curr.end())
                {
                    curr.push_back(j);
                    found = 1;
                    reset = false;
                    break;
                }
            }
        }

        if (found == 1)
        {
            if (onRebound == 0 && currRow > highestRowReachedBeforePanic)
            {
                highestRowReachedBeforePanic = currRow;
            }

            currRow++;
            maximumAllowable = 0;

        }
        else
        {
             currRow--;
             reset = true;
        }
    }

    cout << "Optimal Assignment Found! O(n^4)" << endl;
    int totalWeight = 0;
    for (int i = 0; i < rowNames.size(); i++)
    {
        cout << rowNames[i] << " -- " << colNames[curr[i]] << " (Weight: " << allWeights[(i*allRows.size()) + curr[i]] << ")" << endl;
        totalWeight += allWeights[(i*allRows.size()) + curr[i]];
    }

    cout << "Total Weight: " << totalWeight << endl;

}


template<class T>
int Matrix<T>::hasAnyAssignments(int row) {

    for (int j = 0; j < allColumns.size(); j++)
    {
        if (allRows[row].allValues[j].isPrimed == true)
        {
            return j;
        }
    }

    return -1;

}

template<class T>
void Matrix<T>::markRowsNoAssignments(vector<int>& markedRows) {

    for (int i = 0; i < allRows.size(); i++)
    {
        int doesRowHaveAnyAssignments = hasAnyAssignments(i);
        if (doesRowHaveAnyAssignments == -1)
        {
            allRows[i].starredZero = 1;
            markedRows.push_back(i);
        }
    }

}

template<class T>
void Matrix<T>::markColumns(vector<int>& markedColumns, vector<int> markedRows)
{
    for (int i = 0; i < allColumns.size(); i++)
    {
        for (int j = 0; j < markedRows.size(); j++)
        {
            if (allColumns[i].allValues[markedRows[j]].data == 0)
            {
                if (allColumns[i].starredZero != 1)
                {
                    markedColumns.push_back(i);
                    allColumns[i].starredZero = 1;
                }
            }
        }
    }
}

template<class T>
void Matrix<T>::markRowsWithAssignments(vector<int>& markedRows, vector<int> markedColumns)
{
   /* for (int i = 0; i < markedColumns.size(); i++)
    {
        for (int j = 0; j < allColumns.size(); j++)
        {
            if (allColumns[markedColumns[i]].allValues[j].isPrimed == true)
            {
                if (allRows[j].starredZero != 1)
                {
                    markedRows.push_back(j);
                    allRows[j].starredZero = 1;
                }
            }
        }
    }*/

    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < markedColumns.size(); j++)
        {
            if (allRows[i].allValues[markedColumns[j]].isPrimed == true)
            {
                if (allRows[i].starredZero != 1)
                {
                    markedRows.push_back(i);
                    allRows[i].starredZero = 1;
                }
            }
        }
    }

}

template<class T>
void Matrix<T>::addAssignments() {

    vector<int> unavailableCols;
    vector<int> unavailableRows;

    for (int i = 0; i < allRows.size(); i++) {
        for (int j = 0; j < allColumns.size(); j++) {

            if (allRows[i].allValues[j].data == 0) {
                vector<int>::iterator it;
                vector<int>::iterator it2;
                it = find(unavailableCols.begin(), unavailableCols.end(), j);
                it2 = find(unavailableRows.begin(), unavailableRows.end(), i);
                if (it == unavailableCols.end() && it2 == unavailableRows.end())
                {
                    allRows[i].allValues[j].isPrimed = true;
                    allColumns[j].allValues[i].isPrimed = true;
                    unavailableRows.push_back(i);
                    unavailableCols.push_back(j);
                }
            }

        }
    }

}

template<class T>
vector<int> Matrix<T>::getToBeCovered2() {


    addAssignments();
    vector<int> newRowMarks;
    vector<int> newColumns;
    markRowsNoAssignments(newRowMarks);
    markColumns(newColumns, newRowMarks);
    int exit = 0;
    while (exit == 0)
    {
        vector<int> rowWithAssignments;
        markRowsWithAssignments(rowWithAssignments, newColumns);
        newColumns.clear();
        markColumns(newColumns, rowWithAssignments);

        if (rowWithAssignments.size() == 0)
        {
            exit = 1;
        }
    }

    vector<int> unmarkedRows;
    vector<int> markedCols;

    vector<int> finalToBeCovered;

    for (int i = 0; i < allRows.size(); i++)
    {
        if (allRows[i].starredZero == -1)
        {
            int temp = (i + 1) * -1;
            finalToBeCovered.push_back(temp);
        }

        if (allColumns[i].starredZero == 1)
        {
            finalToBeCovered.push_back(i + 1);
        }
    }




    return finalToBeCovered;
/*    vector<int> unassignedRows;
    vector<int> unavailableCols;

    vector<int> numbersZeroToN;

    vector<int> finalToBeCovered;

    for (int i = 0; i < allRows.size(); i++)
    {
        bool addToUnassignedRows = true;
        int cameAgain = 0;
        for (int j = 0; j < allColumns.size(); j++)
        {

            if (allRows[i].allValues[j].data == 0)
            {
                vector<int>::iterator it;
                it = find(unavailableCols.begin(), unavailableCols.end(), j);
                if (it == unavailableCols.end())
                {
                    if (cameAgain == 1)
                    {
                        allRows[i].starredZero = 1;
                    }
                    if (cameAgain == 0)
                    {
                        addToUnassignedRows = false;
                        unavailableCols.push_back(j);
                        cameAgain = 1;
                    }

                }
            }

        }

        if (addToUnassignedRows)
        {
            unassignedRows.push_back(i);
        }

        numbersZeroToN.push_back(i);


    }

    vector<int> markedCols;

    for (int i = 0; i < unassignedRows.size(); i++)
    {
        for (int j = 0; j < allRows.size(); j++)
        {
            if (allRows[unassignedRows[i]].allValues[j].data == 0)
            {
                markedCols.push_back(j);
            }
        }
    }

    vector<int> markedRows;

    for (int i = 0; i < markedCols.size(); i++)
    {
        for (int j = 0; j < allColumns.size(); j++)
        {
            if (allColumns[markedCols[i]].allValues[j].data == 0)
            {
                if (allRows[j].starredZero != 1)
                {
                    markedRows.push_back(j);
                }
            }
        }
    }

    vector<int> unmarkedRows;

    for (int i = 0; i < numbersZeroToN.size(); i++)
    {
        vector<int>::iterator it;
        it = find(markedRows.begin(), markedRows.end(), i);
        if (it == markedRows.end())
        {
            unmarkedRows.push_back(i);
        }
    }


    for (int i = 0; i < markedCols.size(); i++)
    {
        finalToBeCovered.push_back(markedCols[i] + 1);
    }

    for (int i = 0; i < unmarkedRows.size(); i++)
    {
        int temp = (unmarkedRows[i] + 1) * -1;
        finalToBeCovered.push_back(temp);
    }

    return finalToBeCovered;*/



}





template<class T>
void Matrix<T>::resetStars(int row) {

    for (int i = row +1; i < allRows.size(); i++)
    {
        for (int j = 0; j < allColumns.size(); j++)
        {
            allRows[i].allValues[j].isStarred = false;
        }
    }

}


template<class T>
int Matrix<T>::getSize() {
    return allRows.size();
}

template<class T>
Matrix<T>::Matrix() {

}

template<class T>
bool Matrix<T>::findIfUsefulResult() {

    vector<int> curr;
    vector<int>::iterator it;
    int currRow = 0;
    bool reset = false;
    int badResult = 0;

    while(curr.size() < allRows.size())
    {
        if (reset)
        {
            if (currRow == -1)
            {
                badResult = 1;
                break;
            }
            int resetCol = curr[curr.size() - 1];
            curr.pop_back();
            allRows[currRow].allValues[resetCol].isStarred = true;
            resetStars(currRow);
        }
        int found = 0;
        for (int j = 0; j < allColumns.size(); j++)
        {
            if (allRows[currRow].allValues[j].data == 0 && allRows[currRow].allValues[j].isStarred == false)
            {
                it = find(curr.begin(), curr.end(), j);
                if (it == curr.end())
                {
                    curr.push_back(j);
                    found = 1;
                    reset = false;
                    break;
                }
            }
        }

        if (found == 1)
        {
            currRow++;
        }
        else
        {
            currRow--;
            reset = true;
        }
    }

    if (badResult == 1)
    {
        return false;
    }

    if (badResult == 0)
    {
        return true;
    }

}

template<class T>
void Matrix<T>::print() {

    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < allColumns.size(); j++)
        {
            cout << allRows[i].allValues[j].data << " ";
        }

        cout << endl;
    }

}

template<class T>
void Matrix<T>::recursiveOptimal(vector<int>& rows) {

    recursiveInner(0, rows);

}

template<class T>
void Matrix<T>::recursiveInner(int row, vector<int>& rows) {

    if (rows.size() == allRows.size())
    {
        return;
    }



}

template<class T>
void Matrix<T>::setAllPrimeToFalse() {

    for (int i = 0; i < allRows.size(); i++)
    {
        for (int j = 0; j < allColumns.size(); j++)
        {
            allRows[i].allValues[j].isPrimed = false;
        }
    }

}





//template<class T>
//Matrix<T>::Matrix(): allRows(), allColumns() {}










#endif //HUNGARIANALGORITHM_MATRIX_H
