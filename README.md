# Project4
My Project 4 for CS3353

#Trivial (Munkres) Solution Steps (O(n^4) to O(n!)):

Step 1 (Row Reduction):

Find the lowest element value in the first row. Subtract this value from every element in the first row. Repeat for all rows. Go to Step 2.

Step 2 (Column Reduction):

Find the lowest element value in the first column. Subtract this value from every element in the first column. Repeat for all columns. Go to Step 3.

Step 3 (Cover All Zeros With Minimum Lines):

a. Look through first row. If there exists an element with zero value in a column that does not already have a starred zero, star the element. Only star one element in the row. Repeat for all rows. 

b. Mark all rows with no starred zeros

c. Mark all columns having any zeros in newly marked rows

d. Mark all rows having starred zeros in newly marked columns

Repeat steps 3c-3d until no new rows are marked

e. Cover marked columns and unmarked rows. If number of covered rows + covered cols = N, GO TO Step 5. Else, GO TO Step 4

Step 4 (Adjusting the values):

Find the minimum uncovered value in the matrix. Subtract this value from every uncovered value. Add this value to values covered twice. Uncover all rows, columns. Unmark all rows and columns. Unstar all starred values. GO TO Step 3. 

Step 5 (Getting Final Answer):

Find the optimal assignment

#Non-Trivial (Jonker-Volgenant) Solution  Steps (O(n^3)):

Step 1:

For each row of the matrix, find the smallest element and subtract it from every element in its row.  Go to Step 2.

Step 2:

Find a zero in the resulting matrix.  If there is no starred zero in its row or column, star Z. Repeat for each element in the matrix. Go to Step 3.

Step 3:

Cover each column containing a starred zero.  If N columns are covered, the starred zeros describe a complete set of unique assignments.  In this case, Go to Final Step, otherwise, Go to Step 4.

Step 4: 

Find a noncovered zero and prime it.  If there is no starred zero in the row containing this primed zero, Go to Step 5.  Otherwise, cover this row and uncover the column containing the starred zero. Continue in this manner until there are no uncovered zeros left. Save the smallest uncovered value and Go to Step 6.

Step 5:

Construct a series of alternating primed and starred zeros as follows.  Let Z0 represent the uncovered primed zero found in Step 4.  Let Z1 denote the starred zero in the column of Z0 (if any). Let Z2 denote the primed zero in the row of Z1 (there will always be one).  Continue until the series terminates at a primed zero that has no starred zero in its column.  Unstar each starred zero of the series, star each primed zero of the series, erase all primes and uncover every line in the matrix.  Return to Step 3.

Step 6:

Add the value found in Step 4 to every element of each covered row, and subtract it from every element of each uncovered column.  Return to Step 4 without altering any stars, primes, or covered lines.

Final Step:

Assignment pairs are indicated by the positions of the starred zeros in the cost matrix.  If C(i,j) is a starred zero, then the element associated with row i is assigned to the element associated with column j.