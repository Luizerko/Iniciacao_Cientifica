import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import operator

f = open("Erro_PACL.txt", "r")
dicionario = {}
for i in f:
    i = i.split("\n")
    i = i[0].split(" ")
    dicionario[i[1]] = i[0]
f.close()

f = open("Erro_PACL.txt", "r")
contador = {}
for i in f:
    i = i.split("\n")
    i = i[0].split(" ")
    if i[1] in contador.keys():
        contador[i[1]] += 1
    else:
        contador[i[1]] = 1
f.close()

for i in dicionario:
    dicionario[i] = float(dicionario[i])

dicionario_ordenado = sorted(dicionario.items(), key=operator.itemgetter(1))

dicionario = {}

for i in dicionario_ordenado:
    dicionario[i[0]] = i[1]

contador_final = {}
for i in dicionario:
    contador_final[i] = contador[i]
contador = contador_final

soma = 0
prob = 0
for i in contador.keys():
    if i != '28':
        prob += contador[i]
    soma += contador[i]

for i in contador.keys():
    contador[i] = contador[i]/soma

print("Probability of L_{d,f} of h_s bigger than epsilon: " + str(prob/soma))

f, a = plt.subplots(2)

a[0].set_xlabel('Chosen hypothesis (h_s)')
a[0].set_ylabel('Probability')
a[0].set_title('Histogram of chosen hypothesis with m=20')
a[0].bar(contador.keys(), contador.values(), color='blue')

x = dicionario.keys()
y = dicionario.values()
a[1].set_xlabel('Chosen hypothesis (h_s)')
a[1].set_ylabel('Sigma error')
a[1].set_title('Scatter plot of chosen hypothesis and its error with m=20')
a[1].scatter(x, y, color="red")

plt.tight_layout()
plt.show()