#include <iostream>
#include "Matrix.h"
#include "RowOrColumn.h"
#include "Value.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <iterator>

/*
 * This is the loadInputIntoMatrix function. This function takes the input file and loads all of the necessary
 * data into the matrix that will then be operated on to find the minimum assignment. This function is called right
 * for each time a new algorithm needs to be performed on the input data.
 */
void loadInputIntoMatrix(Matrix<int>& matrix, fstream& inputFile, fstream& inputFile2, vector<string>& rowNames,
        vector<string>& columnNames, vector<int>& allWeights)
{
    char temp1[100];
    inputFile.getline(temp1, 100);
    string numberOfRowsAndColumn = temp1;
    int numRowsAndColumns = stoi(numberOfRowsAndColumn);

    char temp[500];

    vector<RowOrColumn<int>> tempColumns;

    for (int i = 0; i < numRowsAndColumns; i++)
    {
        RowOrColumn<int> col;
        tempColumns.push_back(col);
    }

    for (int i = 0; i < numRowsAndColumns; i++)
    {
        RowOrColumn<int> row;
        for (int j = 0; j < numRowsAndColumns; j++)
        {
            inputFile.getline(temp, 500);
            vector<string> results;
            istringstream iss(temp);
            copy(istream_iterator<string>(iss),
                 istream_iterator<string>(),
                 back_inserter(results));

            string number = results[2];
            string rowName = results[0];
            string columnName = results[1];
            int weight = stoi(number);

            allWeights.push_back(weight);

            if (j == 0)
            {
                rowNames.push_back(rowName);
            }
            if (i == 0)
            {
                columnNames.push_back(columnName);
            }

            Value<int> val(weight);
            row.addTo(val);
        }
        matrix.addRow(row);
    }

    inputFile2.getline(temp, 300);

    for (int i = 0; i < numRowsAndColumns; i++)
    {
        for (int j = 0; j < numRowsAndColumns; j++)
        {
            inputFile2.getline(temp, 500);
            vector<string> results;
            istringstream iss(temp);
            copy(istream_iterator<string>(iss),
                 istream_iterator<string>(),
                 back_inserter(results));

            string number = results[2];
            string rowName = results[0];
            string columnName = results[1];
            int weight = stoi(number);

            Value<int> val(weight);

            tempColumns[j].addTo(val);

        }
    }

    for (int i = 0; i < numRowsAndColumns; i++)
    {
        matrix.addCol(tempColumns[i]);
    }
}

/*
 * Removes the duplicates from vectors
 */
void remove(vector<int> &v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }

    v.erase(end, v.end());
}

/*
 * This is the logic that is used in order to perform the O(n^3) Hungarian Algorithm on the data for the
 * Assignment Problem. As you can see, stepOne and stepTwo are only going to be performed once. However, stepThree
 * includes a cyclic step that leads to steps Four, Five, and Six depending on their outcomes
 */
void doHungarianAlgorithm(Matrix<int>& matrix, vector<string>& rowNames,
        vector<string>& columnNames, vector<int>& allWeights)
{
    int done = 0;
    matrix.doStepOne();
    matrix.doStepTwo();
    matrix.doStepThree(rowNames, columnNames, allWeights, done);
}

/*
 * This is the function to perform the less efficient version of the HUngarian Algorithm for the Assignment Problem.
 * The complexity can range from O(n^4) to O(n!) depending on the input data. While the logic of the algorithm is
 * easier to understand than the more efficient algorithm, it is much slower.
 */
void doLessEfficientHungarianAlgorithm(Matrix<int>& matrix2, vector<string>& rowNames,
                                       vector<string>& columnNames, vector<int>& allWeights)
{
    matrix2.doStepOne();
    matrix2.colReduction();
    int exit = 0;

    while (exit == 0)
    {
        vector<int> toBeCovered = matrix2.getToBeCovered2();
        remove(toBeCovered);


        if (toBeCovered.size() == matrix2.getSize())
        {
            exit = 1;
        }
        else
        {
            matrix2.coverRowsAndCols(toBeCovered);
            matrix2.reset(toBeCovered);
        }

    }

    matrix2.findOptimalAssignment(rowNames, columnNames, allWeights);
}

/*
 * Main function. I input the data into Matrix objects and perform both the more and less efficient implementations
 * of the Hungarian Algorithm for the Assignment Problem.
 */
int main() {

    random_device randNum;
    std::mt19937 mt(randNum());
    std::uniform_int_distribution<int> dist(25, 100);

    ofstream outputFile;
    outputFile.open("munkresinput.txt");

    int N = 5;
    outputFile << N << endl;
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            int randVal = dist(mt);
            outputFile << "Worker" << i << " Job" << j << " " << randVal << endl;
        }
    }

    Matrix<int> matrix;
    Matrix<int> matrix2;

    fstream inputFile;
    fstream inputFile2;
    fstream inputFile3;
    fstream inputFile4;
    inputFile.open("munkresinput.txt");
    inputFile2.open("munkresinput.txt");
    inputFile3.open("munkresinput.txt");
    inputFile4.open("munkresinput.txt");

    vector<string> rowNames; vector<string> rowNames2;
    vector<string> columnNames; vector<string> columnNames2;
    vector<int> allWeights; vector<int> allWeights2;

    loadInputIntoMatrix(matrix2, inputFile3, inputFile4, rowNames2, columnNames2, allWeights2);

    auto start2 = chrono::steady_clock::now();
    doLessEfficientHungarianAlgorithm(matrix2, rowNames2, columnNames2, allWeights2);
    auto end2 = chrono::steady_clock::now();
    int tempTime2 = chrono::duration_cast<chrono::microseconds>(end2-start2).count();
    cout << "Time (us): " << tempTime2 << endl << endl;

    loadInputIntoMatrix(matrix, inputFile, inputFile2, rowNames, columnNames, allWeights);

    auto start = chrono::steady_clock::now();
    doHungarianAlgorithm(matrix, rowNames, columnNames, allWeights);
    auto end = chrono::steady_clock::now();
    int tempTime = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout << "Time (us): " << tempTime << endl;




    return 0;
}
