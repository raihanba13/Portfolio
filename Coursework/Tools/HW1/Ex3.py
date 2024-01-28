import sys
import random
from copy import deepcopy

try:
    x_len = int(sys.argv[1])
    y_len = int(sys.argv[2])
except:
    print('Cmd arg not provided. Default 25 X 25 is selected.')
    x_len = 25
    y_len = 25

# 20% live cell implementation
random_cell_status = [int(bool(x)) if not x%5 else x*0 for x in range(1, x_len*y_len+1) ]

random.seed(random.randint(0,20))
random.shuffle(random_cell_status)

life_matrix = []

for x in range(0, x_len):
    life_matrix.append(random_cell_status[:y_len])
    del random_cell_status[:y_len]
    

print("Next Gen Code Begins ==========\n\n")

def nexgen(life_matrix):
    new_life_matrix = deepcopy(life_matrix)
    
    for i in range(0, x_len):
        f_r_idx = i+1
        if f_r_idx == x_len:
            f_r_idx = 0
        _ = life_matrix[i]
        _b_r = life_matrix[i-1]
        _f_r = life_matrix[f_r_idx]
                
        for j in range(0, y_len):
            f_c_idx = j+1
            if f_c_idx == x_len:
                f_c_idx = 0
            life_count = _[j-1] + _[f_c_idx] + _b_r[j] + _b_r[j-1] + _b_r[f_c_idx] + _f_r[j] + _f_r[j-1] + _f_r[f_c_idx]
            
            # live cell logic
            if life_matrix[i][j] == 1:
                if life_count < 2 or life_count > 3:
                    new_life_matrix[i][j] = 0
            # dead cell logic
            else:
                if life_count == 3: 
                    new_life_matrix[i][j] = 1
                    
    return new_life_matrix
    
for i in range(0, 50):
    life_matrix = nexgen(life_matrix)
    
    for i in range(0, x_len):
        print(' '.join(str(x) for x in life_matrix[i]).replace('1', 'X'))
    print("===============================================")
    