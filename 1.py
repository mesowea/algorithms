import numpy as np
length = 120

arr = np.random.randint(1, length, length)
arr
min_ = arr[0]
max_ = min
for i in arr:
  min_ = np.min([min_, i])
  max_ = np.max([mix_, i])

print(f'Minimun: {mn}\nMaximum: {mx}')
array_asc = np.sort(arr)
array_descending = np.flip(array_ascending)


print(f'Sorted array asc:{array_ascending}\n')
print(f'Sorted array desc:{array_descending}')
