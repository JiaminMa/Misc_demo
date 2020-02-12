def bubble_sort(array):
    array_len = len(array)
    for i in range(0, array_len - 1):
        for j in range(0, array_len - i - 1):
            if array[j] > array[j + 1]:
                temp = array[j]
                array[j] = array[j + 1]
                array[j + 1] = temp


array1 = [5,12,3,6,1,7]
array2 = []
array3 = [1]
array4 = [4,5,1,3,4,4]

bubble_sort(array1)
bubble_sort(array2)
bubble_sort(array3)
bubble_sort(array4)

print array1
print array2
print array3
print array4

