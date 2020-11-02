from random import random
from random import seed
from datetime import datetime
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import operator

seed(datetime.now())

dicionario = {}
for i in range(3264):
    dicionario[i] = 0

for j in range(1):
    for i in range(1154):
        r = random()*3264
        r = int(r)
        r = r%3264
        dicionario[r] = dicionario[r] + 1

contador = {}
for i in range(3264):
    if dicionario[i] in contador.keys():
        contador[dicionario[i]] += 1
    else:
        contador[dicionario[i]] = 1

print(contador)

plt.bar(dicionario.keys(), dicionario.values(), color='blue')

plt.tight_layout()
plt.show()