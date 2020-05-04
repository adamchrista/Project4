//
// Created by Adam Christa on 4/24/20.
//

#ifndef HUNGARIANALGORITHM_MATRIX_H
#define HUNGARIANALGORITHM_MATRIX_H

#include "RowOrColumn.h"
#include <algorithm>


/*
 * This is the Matrix class. It represents a matrix data structure. It has two vectors to represent the matrix.
 * One vector is meant to represent the rows and one is meant to represent the columns. The reason for the separation
 * between the rows is columns is to allow easier access to covering the rows or columns in the Hungarian Algorithm
 */
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

     //Functions between the dashed lines below belong to the Non-Trivial (Jonker-Volgenant) Implementation
     //-------------------------------------------------------
     void addRow(RowOrColumn<T>&);
     void addCol(RowOrColumn<T>&);
     void doStepOne();
     void doStepTwo();
     void doStepThree(vector<string>&, vector<string>&, vector<int>&, int&);
     void doStepFour(vector<string>&, vector<string>&, vector<int>&,int&);
     void doStepFive(int, vector<string>&, vector<string>&, vector<int>&, int&);
     void doStepSix(int, vector<string>&, vector<string>&, vector<int>&, int&);
     void doFinalStep(vector<string>&, vector<string>&, vector<int>&, int&);
     //---------------------------------------------------------

     //Functions between dashed lines below belong to the the Trivial (Munkres) Implementation
     //---------------------------------------------------------
     void colReduction();
     vector<int> getToBeCovered2();
     void coverRowsAndCols(vector<int>);
     void reset(vector<int>);
     void findOptimalAssignment(vector<string>&, vector<string>&, vector<int>&);
     int hasAnyAssignments(int);
     void addAssignments();
     void markRowsNoAssignments(vector<int>&);
     void markColumns(vector<int>&, vector<int>);
     void markRowsWithAssignments(vector<int>&, vector<int>);
     //---------------------------------------------------------
     int getSize();
     void print();
     void primeMinVal(int);
     void setAllPrimeToFalse();

};

/*
 * Pushes a newly created row into the matrix
 */
template<class T>
void Matrix<T>::addRow(RowOrColumn<T>& currRow)
{
    allRows.push_back(currRow);
}

/*
 * Pushes a newly created column into the matrix
 */
template<class T>
void Matrix<T>::addCol(RowOrColumn<T>& currCol)
{
    allColumns.push_back(currCol);
}

/*
 * This is the stepOne for the more efficient algorithm of the Hungarian Algorithm
 * For each row of the matrix, find the smallest element and subtract it from every element in its row.
 * This function is only performed once.
 */
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

/*
 * This is the stepTwo function for the more efficient implementation of the Hungarian Algorithm
 * Find a zero (Z) in the resulting matrix.  If there is no starred zero in its row or column, star Z.
 * Repeat for each element in the matrix.
 * This function is only performed once.
 */
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

/*
 * This is the stepThree for the more efficient implementation of the Hungarian Algorithm.
 * Cover each column containing a starred zero.  If K columns are covered, the starred zeros
 * describe a complete set of unique assignments.  In this case, Go to FinalStep, otherwise, Go to Step 4.
 */
template<class T>
void Matrix<T>::doStepThree(vector<string>& allRowNames, vector<string>& allColNames, vector<int>& allWeights, int& done)
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


/*
 * This is the stepFour for the more efficient implementation of the Hungarian Algorithm.
 * Find a noncovered zero and prime it.  If there is no starred zero in the row containing
 * this primed zero, Go to Step 5.  Otherwise, cover this row and uncover the column containing
 * the starred zero. Continue in this manner until there are no uncovered zeros left. Save the
 * smallest uncovered value and Go to Step 6.
 */
template<class T>
void Matrix<T>::doStepFour(vector<string>& allRowNames, vector<string>& allColNames, vector<int>& allWeights, int& done)
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

/*
 * This is the stepFive for the more efficient implementation of the Hungarian Algorithm.
 * Construct a series of alternating primed and starred zeros as follows.  Let Z0 represent
 * the uncovered primed zero found in Step 4.  Let Z1 denote the starred zero in the column
 * of Z0 (if any). Let Z2 denote the primed zero in the row of Z1 (there will always be one).
 * Continue until the series terminates at a primed zero that has no starred zero in its column.
 * Unstar each starred zero of the series, star each primed zero of the series, erase all primes
 * and uncover every line in the matrix.  Return to Step 3.
 */
template<class T>
void Matrix<T>::doStepFive(int index, vector<string>& allRowNames, vector<string>& allColNames, vector<int>& allWeights, int& done)
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
                 allColumns[j].isCovered = false;
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

/*
 * This is the stepSix for more efficient implementation of the Hungarian Algorithm
 * Add the value found in Step 4 to every element of each covered row, and subtract
 * it from every element of each uncovered column.  Return to Step 4 without altering
 * any stars, primes, or covered lines.
 */
template<class T>
void Matrix<T>::doStepSix(int val, vector<string>& allRowNames, vector<string>& allColNames, vector<int>& allWeights, int& done)
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

/*
 * This is the finalStep of the more efficient implementation of the Hungarian Algorithm
 * Each starred zero is the row and column number of the assignment in the matrix that will result
 * in the most optimal final product.
 */
template<class T>
void Matrix<T>::doFinalStep(vector<string>& allRowNames, vector<string>& allColNames, vector<int>& allWeights, int& done)
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

/*
 * This is a function that finds and subtracts the lowest value from each column of the matrix. This operation
 * is performed once in the less efficient implementation of the Hungarian Algorithm
 */
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

/*
 * This is the coverRowsAndCols function. It has a parameter of toBeCovered that represents the rows and cols that
 * need to be covered. The function iterates through the vector and covers all the necessary rows and cols.
 */
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

/*
 * This is the reset function. The function is called in order to eliminate all starred zeros, primed zeroes, and
 * row covers.
 */
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

/*
 * This is the primeMinVal function. This function finds the minimum value in the row given and primes all of the
 * elements in that row that has the minimum value.
 */
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

/*
 * This is the findOptimalAssignment function. This function is called when the number of columns and rows that
 * are covered is equal to N. This function is what prohibits the algorithm from being optimal. The function attempts
 * to find the minimal possible assignment in an efficient manner. However, in order to perfectly find the assignment
 * every time, the algorithm would approach O(n!). This function allows the algorithm to approach O(n^4). However,
 * the assignments found might be slightly wrong. The function does a good job at getting very close to the optimal
 * without being inefficient.
 */
template<class T>
void Matrix<T>::findOptimalAssignment(vector<string>& rowNames, vector<string>& colNames, vector<int>& allWeights)
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

/*
 * Determines if any rows contain primed zeroes.
 */
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

/*
 * This function stars the rows that do not contain any assignments (primed zeros)
 */
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

/*
 * This is the markColumns function that adds columns to the rows that have zeros in the newly marked  rows
 */
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

/*
 * This is the markRowsWithAssignments function. It will mark all of the rows that have starred zeros in the
 * newly marked columns.
 */
template<class T>
void Matrix<T>::markRowsWithAssignments(vector<int>& markedRows, vector<int> markedColumns)
{
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

/*
 * This is the addAssignments function. It attempts to star one zero in each row. However, if only one zero exists
 * and there already exists a zero that is storred in the same column, the row will not receive a starred zero.
 */
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

/*
 * This is the getToBeCovered function. This function determines the minimal number of rows and/or columns necessary
 * to cover all resulting zeros in the matrix. The function returns a vector of integers that represents the rows
 * and columns to be covered.
 */
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

        if (rowWithAssignments.empty())
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

}




/*
 * This function resets all stars on all elements to false
 *
 */
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

/*
 * returns N sizee of matrix
 */
template<class T>
int Matrix<T>::getSize() {
    return allRows.size();
}

template<class T>
Matrix<T>::Matrix() {

}


/*
 * prints all elements of matrix
 */
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

/*
 * Sets all prime values to false
 */
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


#endif //HUNGARIANALGORITHM_MATRIX_H
