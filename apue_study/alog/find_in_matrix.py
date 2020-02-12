#!/usr/bin/python2

def Find(target, array):
    
    row = len(array)
    col = len(array[0])
    row_idx = 0
    col_idx = col - 1

    while row_idx < row and col_idx >= 0:
        if target < array[row_idx][col_idx]:
            col_idx = col_idx - 1
        elif target > array[row_idx][col_idx]:
            row_idx = row_idx + 1
        else:
            return True

    return False


array = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
target = 5
print Find(target, array)
