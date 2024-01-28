from math import sqrt, pow, sin, cos
import numpy as np

def lenght(x1, y1, x2, y2):
    return sqrt( pow((x1-y1), 2) + pow((x2-y2), 2) )

def ellipse(a, b, t):
    return (a*cos(t), b*sin(t), t)

def circumference(a, b, n):
    ran = np.arange(0, 361, 360/n)

    ellipse_point_list = [ellipse(a, b, x*(3.1416/180.0)) for x in ran]
    circumf = 0.0 

    for i in range(0, len(ellipse_point_list)-1):
        p1 = ellipse_point_list[i]
        p2 = ellipse_point_list[i+1]
        circumf += lenght(p1[0], p2[0], p1[1], p2[1])

    return circumf

a = 13.0
b = 7.0

n_ = 2
circumf2 = circumference(a, b, n_)

n_max = pow(2, 24)
circumf_result = [ (circumf2, n_, 'N/A')]


while 1:
    n_ *= 2
    circumf_n = circumference(a, b, n_)

    err = abs((circumf_n - circumf2)/circumf_n)
    try:
        if err < 1e-12 or n_ >= n_max:
            circumf_result.append((circumf_n, n_, err))
            break
        else:
            circumf_result.append((circumf_n, n_, err))
            circumf2 = circumf_n
    except Exception as e:
        print(e)
        break
    
print('circumference , n , error')    
print(circumf_result)