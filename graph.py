import sys
import pandas as pd
from matplotlib import pyplot as plt

data = pd.read_csv(sys.argv[1], delimiter='|')

df = pd.DataFrame(data)

X = list(df.iloc[:, 0])
Y = list(df.iloc[:, 1])

plt.bar(X, Y, color=None)
title = "Porcentagem de NULL de %s"%(sys.argv[1])
plt.title(title)
plt.xlabel("Colunas")
plt.ylabel("Porcentagem")
plt.xticks([])

plt.savefig(sys.argv[1])
