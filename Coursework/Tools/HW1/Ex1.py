import sys 
from copy import deepcopy

try:
    input_val = int(sys.argv[1])
except:
    input_val = 50

global res
res = [input_val]

def collatz(n):

    if int(n) == 1:
        return None

    if n % 2 == 0:
        res.append(n/2)
        collatz(n/2)
    else:
        res.append(n*3+1)
        collatz(n*3+1)

collatz(input_val)
print(res)