import pandas as pd 
import matplotlib.pyplot as plt
import seaborn as sns

d_csv = pd.read_csv("D_data.csv", header=None)
# d_csv = pd.read_csv("D_data_v3.csv", header=None)

# sns.heatmap(d_csv, annot=True)
sns.heatmap(d_csv, annot=False)
plt.show()