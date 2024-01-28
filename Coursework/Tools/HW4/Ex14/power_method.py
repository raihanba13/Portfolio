import numpy as np 
from copy import deepcopy
from numpy.linalg import norm, eigh
from timeit import default_timer as timer

# try:
#     n = int(input("Enter n: "))
# except Exception as e:
#     print(e)
#     exit()
   
n = 5 
initial_data = [4, -1, -2]

matrix = np.zeros((n, n), dtype=float)

# row loop
positive_dir = 0
for i in range(n):
    for idx, j in enumerate(initial_data):
        if positive_dir+idx == n:
            break
        matrix[i, positive_dir+idx] = j
    positive_dir += 1
    

initial_data = initial_data[1:]
for i in range(1, n):
    col_start = i-1
    loop_count = 0
    for j in initial_data:
        matrix[i, col_start] = j
        loop_count += 1
        col_start -= 1
        if loop_count == i:
            break
        
        
    
a = timer()
# Power method
x = np.array([1.0 for x in range(n)])
x = np.transpose(x)
tol = 1
while tol > 10e-12:
    y = deepcopy(x)
    x = np.matmul(matrix, y)/norm(np.matmul(matrix, y))
    tol = norm(x-y)
    
res = norm(np.matmul(matrix, x))
b = timer()

print("Result:", res)
print("Required time (Manually):", b-a)

print("=============================================")

a = timer()
w, v = eigh(matrix)
b = timer()
print("Result:", sorted(w)[-1])
print("Required time (numpy):", b-a)