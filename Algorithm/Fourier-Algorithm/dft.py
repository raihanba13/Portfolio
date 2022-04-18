from pprint import pprint
import pandas as pd
from numpy import pi as PI
from numpy import sin, cos, exp
import numpy as np
from copy import deepcopy

# input a column df
def dft(f, N):
    k = pd.DataFrame({
            1: [float(x)*((2*PI)/N) for x in range(0, int(N))]
        })
    N_series = pd.DataFrame({
            1: [float(x) for x in range(0, int(N))]
        })
    ft = pd.DataFrame(0+1j, index=np.arange(0, N), columns=[1])

    for i in k[1].tolist():
        _ = exp(-1j*i*N_series)
        _f = deepcopy(f)
        _f = _f.multiply(_)
        _f = _f.sum(axis=1)
        # print(type(_f))
        # pprint(_f)
        print("=============")
        print(ft.at[int(i), 1])
        ft.at[int(i), 1] = _f[1]
        print(ft.at[int(i), 1])
        print("=============")

    return ft
