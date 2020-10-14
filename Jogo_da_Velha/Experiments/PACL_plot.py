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

f = open("Erro_PACL.txt", "r")

contador = {}
for i in f:
    i = i.split("\n")
    i = i[0].split(" ")
    if i[1] in contador.keys():
        contador[i[1]] += 1
    else:
        contador[i[1]] = 1

soma = 0;
prob = 0;
for i in contador.keys():
    if i != '28' and i != '12' and i != '24' and i != '20' and i != '8' and i != '16' and i != '4':
        prob += contador[i]
    soma += contador[i]

print(contador)
print(prob/soma)

f, a = plt.subplots(2)

a[0].set_xlabel('Chosen hypothesis (h_s)')
a[0].set_ylabel('Probability')
a[0].set_title('Histogram of chosen hypothesis with m=7')
a[0].hist(data, len(lista_auxiliar) - 1, density=True, facecolor="blue", alpha=0.5)

x = dicionario.keys()
y = dicionario.values()
a[1].set_xlabel('Chosen hypothesis (h_s)')
a[1].set_ylabel('Sigma error')
a[1].set_title('Scatter plot of chosen hypothesis and its error with m=7')
a[1].scatter(x, y, color="red")

plt.tight_layout()
plt.show()