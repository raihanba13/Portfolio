import pandas as pd 
import matplotlib.pyplot as plt
import seaborn as sns

d_p0_csv = pd.read_csv("D_data_0_.txt", header=None)
d_p1_csv = pd.read_csv("D_data_1_.txt", header=None)
d_p2_csv = pd.read_csv("D_data_2_.txt", header=None)
d_p3_csv = pd.read_csv("D_data_3_.txt", header=None)

# drop the last column
# this is happening as we have an ending , at every line
del d_p0_csv[list(d_p0_csv.columns)[-1]]
del d_p1_csv[list(d_p1_csv.columns)[-1]]
del d_p2_csv[list(d_p2_csv.columns)[-1]]
del d_p3_csv[list(d_p3_csv.columns)[-1]]

# concat p0 and p1; p2 and p3 vertically 
# concat the results horizontally
d_p0_p1_csv = pd.concat([d_p0_csv, d_p1_csv], axis=1)
d_p2_p3_csv = pd.concat([d_p2_csv, d_p3_csv], axis=1)
d_csv = pd.concat([d_p0_p1_csv, d_p2_p3_csv], axis=0)

d_csv.index = [x for x in range(d_csv.shape[0])]
d_csv.columns = [x for x in range(d_csv.shape[1])]

print(d_csv)

d_csv.to_csv("test_d_merged.csv")

sns.heatmap(d_csv, annot=True)

# sns.heatmap(d_csv, annot=False)
plt.show()