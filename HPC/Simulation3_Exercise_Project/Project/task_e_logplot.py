import pandas as pd 
import matplotlib.pyplot as plt
import seaborn as sns

c_csv = pd.read_csv("C_data.csv", header=None)

del c_csv[c_csv.columns[-1]]

a = [x for x in range(c_csv.shape[1])]
fig = plt.figure()
ax = fig.add_subplot(2, 1, 1)

line, = ax.plot(a, c_csv.iloc[0, :].tolist(), color='blue', lw=2)

ax.set_yscale('log')

plt.show()