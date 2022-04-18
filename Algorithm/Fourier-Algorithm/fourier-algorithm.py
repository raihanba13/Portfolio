from pprint import pprint
import pandas as pd
from numpy import pi as PI
from numpy import sin, cos

# custom defined
import dft

# discrete point
N = 50.0
# interval, max limit
L = 1.0
# step size
a = L/N;

x = pd.DataFrame({
        1: [float(x)*a for x in range(0, int(N))]
    }
)

k = pd.DataFrame({
        1: [float(x) for x in range(0, int(N))]
    }
)

f = 7*sin(2.0*PI*x/L) + 3*sin(4*PI*x/L) + cos(8*PI*x/L)

ft = dft.dft(f, N)
ft.to_csv('test.csv')

pprint(ft)
pprint(ft.shape)

# pprint(x)
# pprint(x.shape)
#
# pprint(k)
# pprint(k.shape)
#
# pprint(f)
# pprint(f.shape)
# print(type(f))
