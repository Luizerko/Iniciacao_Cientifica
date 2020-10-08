import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab

f = open("Erro_PACL.txt", "r")

data = []
for i in f:
    i = i.split(" ")
    i = i[1].split("\n")
    data.append(i[0])

lista_auxiliar = []
for i in data:
    if i not in lista_auxiliar:
        lista_auxiliar.append(i)

f = open("Erro_PACL.txt", "r")

dicionario = {}
for i in f:
    i = i.split("\n")
    i = i[0].split(" ")
    dicionario[i[1]] = i[0]

f, a = plt.subplots(2)

x = dicionario.keys()
y = dicionario.values()
a[1].scatter(x, y, color="red")


a[0].set_xlabel('Chosen hypothesis (h_s)')
a[0].set_ylabel('Probability')
a[0].set_title('Histogram of chosen hypothesis on the hypothesis class with m=26')
a[0].hist(data, len(lista_auxiliar) - 1, density=True, facecolor="blue", alpha=0.5)
plt.show()