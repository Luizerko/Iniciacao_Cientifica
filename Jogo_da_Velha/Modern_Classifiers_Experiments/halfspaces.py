import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import seaborn as sns
import time
import random
from copy import copy, deepcopy
from collections import OrderedDict
from sklearn.linear_model import Perceptron, LogisticRegression

def empty_tab(tab):
	for i in range(3):
		for j in range(3):
			tab[i][j] = -1

def terminal_state(tab):

	#Checa se alguém ganhou em linha
	for i in range(3):
	    if tab[i][0] != -1:
	        val = tab[i][0]
	        ganhou = val
	        for j in range(1,3):
	            if tab[i][j] != val:
	            	ganhou = -2
	            	break
	        if ganhou == val:
	        	return ganhou

	#Checa se alguém ganhou em coluna
	for j in range(3):
	    if tab[0][j] != -1:
	        val = tab[0][j]
	        ganhou = val
	        for i in range(1,3):
	            if tab[i][j] != val:
	            	ganhou = -2
	        if ganhou == val:
	        	return ganhou

	#Checa se alguém ganhou em diagonal
	if tab[0][0] != -1:
	    val = tab[0][0]
	    ganhou = val
	    for i in range(1,3):
	        if tab[i][i] != val:
	        	ganhou = -2
	    if ganhou == val:
	    	return ganhou

	if tab[0][2] != -1:
	    val = tab[0][2]
	    ganhou = val
	    for i in range(1,3):
	        if tab[i][2-i] != val:
	        	ganhou = -2
	    if ganhou == val:
	    	return ganhou

	#Checa se o jogo não tem vencedores e tem espaços vazios - estado não terminal
	for i in range(3):
	    for j in range(3):
	        if tab[i][j] == -1:
	        	return -2

	#Jogo empatou
	return -1

#Retorna 1 se o jogador 'X' ganhou na horizontal, 0 caso constrário
def horizontal(tab):
	
	for i in range(3):
	    if tab[i][0] == 1:
	        ganhou = 1
	        for j in range(1,3):
	            if tab[i][j] != 1:
	            	ganhou = 0
	        if ganhou == 1:
	        	return ganhou

	return 0

#Retorna 1 se o jogador 'X' ganhou na vertical, 0 caso constrário
def vertical(tab):
	
	for j in range(3):
	    if tab[0][j] == 1:
	        ganhou = 1
	        for i in range(1,3):
	            if tab[i][j] != 1:
	            	ganhou = 0
	        if ganhou == 1:
	        	return ganhou

	return 0

#Retorna 1 se o jogador 'X' ganhou na diagonal, 0 caso constrário
def diagonal(tab):
	
	if tab[0][0] == 1:
	    ganhou = 1
	    for i in range(1,3):
	        if tab[i][i] != 1:
	        	ganhou = 0
	    if ganhou == 1:
	    	return ganhou

	if tab[0][2] == 1:
	    ganhou = 1
	    for i in range(1,3):
	        if tab[i][2-i] != 1:
	        	ganhou = 0
	    if ganhou == 1:
	    	return ganhou

	return 0

#Retorna a lista com 9 valores indicando a presença ou a ausência do X em certa
#casa do tabuleiro. Ilustração de conversão de tabuleiro para lista:
#Tabuleiro: -------------------
#			|(0,0)|(0,1)|(0,2)|
#			-------------------
#			|(1,0)|(1,1)|(1,2)|
#			-------------------
#			|(2,0)|(2,1)|(2,2)|
#			-------------------
#lista: [(0,0), (0,1), (0,2), (1,0), (1,1), (1,2), (2,0), (2,1), (2,2)]
def tab_to_positions(tab):
	pos_00 = 0
	pos_01 = 0
	pos_02 = 0
	pos_10 = 0
	pos_11 = 0
	pos_12 = 0
	pos_20 = 0
	pos_21 = 0
	pos_22 = 0

	if tab[0][0] == 1:
		pos_00 = 1
	if tab[0][1] == 1:
		pos_01 = 1
	if tab[0][2] == 1:
		pos_02 = 1
	if tab[1][0] == 1:
		pos_10 = 1
	if tab[1][1] == 1:
		pos_11 = 1
	if tab[1][2] == 1:
		pos_12 = 1
	if tab[2][0] == 1:
		pos_20 = 1
	if tab[2][1] == 1:
		pos_21 = 1
	if tab[2][2] == 1:
		pos_22 = 1

	return [pos_00, pos_01, pos_02, pos_10, pos_11, pos_12, pos_20, pos_21, pos_22]


#Printa o estado do tabuleiro
def print_tab(tab):

	print(u'\u0020\u005F\u0020\u005F\u0020\u005F')

	for i in range(3):
		string = ''
		for j in range(3):
			if tab[i][j] == -1:
				string = string + '| \u0332'
			elif tab[i][j] == 0:
				string = string + '|O\u0332'
			else:
				string = string + '|X\u0332'
		string = string + '|'
		print(string)

	print()

#Função que simula todos os possíveis jogos, alcançando qualquer estado do jogo e 
#contando o número de vitórias para cada um dos jogadores, bem como o número de empates.
def simulate_every_state(jogador, tab):
	for i in range(3):
		for j in range(3):
			if tab[i][j] == -1:
				tab[i][j] = jogador
				
				global vitorias_X
				global vitorias_O
				global empates
				global estados_terminais
				global dicionario_de_estados
				
				'''
				global vitorias_horizontal
				global vitorias_vertical
				global vitorias_horizontal_vertical
				global vitorias_diagonal
				global vitorias_horizontal_diagonal
				global vitorias_vertical_diagonal
				'''

				if terminal_state(tab) == jogador:
					#print_tab(tab)
					#time.sleep(1) 
					estados_terminais += 1
					dicionario_de_estados[estados_terminais] = deepcopy(tab)

					if jogador == 0:
						vitorias_O += 1
					else:
						vitorias_X += 1
						
						'''
						if horizontal(tab) == 1 and vertical(tab) == 0 and diagonal(tab) == 0:
							vitorias_horizontal += 1
						elif horizontal(tab) == 0 and vertical(tab) == 1 and diagonal(tab) == 0:
							vitorias_vertical += 1
						elif horizontal(tab) == 1 and vertical(tab) == 1 and diagonal(tab) == 0:
							vitorias_horizontal_vertical += 1
						elif horizontal(tab) == 0 and vertical(tab) == 0 and diagonal(tab) == 1:
							vitorias_diagonal += 1
						elif horizontal(tab) == 1 and vertical(tab) == 0 and diagonal(tab) == 1:
							vitorias_horizontal_diagonal += 1
						elif horizontal(tab) == 0 and vertical(tab) == 1 and diagonal(tab) == 1:
							vitorias_vertical_diagonal += 1
						'''
					
					tab[i][j] = -1

					continue

				elif terminal_state(tab) == -1:
					#print_tab(tab)
					#time.sleep(1)
					empates += 1
					estados_terminais += 1
					dicionario_de_estados[estados_terminais] = deepcopy(tab)
					tab[i][j] = -1
					return

				simulate_every_state(1-jogador, tab)
				tab[i][j] = -1

#Função que simula todos os possíveis jogos até o estado terminal de número 'iteração'
#passado como parâmetro para a função.
def pick_a_state(jogador, tab):
	for i in range(3):
		for j in range(3):
			if tab[i][j] == -1:
				tab[i][j] = jogador

				global iteracao

				if terminal_state(tab) == jogador:
					if iteracao-1 == 0:
						return tab
					iteracao -= 1
					tab[i][j] = -1
					continue

				elif terminal_state(tab) == -1:
					if iteracao-1 == 0:
						return tab
					iteracao -= 1
					tab[i][j] = -1
					return None

				retorno = pick_a_state(1-jogador, tab)
				if retorno is not None:
					return retorno
				tab[i][j] = -1
	return None

#Usa da técnica de perceptrons para calcular um separador linear para os dados fornecidos.
#Essa função também plota a superfície separadora e os pontos amostrais utilizados (R²).
#(Função precisa ser revista, uma vez que o problema passado em R² não é linearmente separável).
def perceptron_calculating_2d(x, y, label):
	df = pd.DataFrame(dict(x=x, y=y, label=label))
	colors = {1: 'green', -1: 'red'}

	clf = Perceptron(tol=1e-3)
	lista_coord = [[x, y] for x, y in zip(df['x'].to_list(), df['y'].to_list())]
	clf.fit(lista_coord, df['label'].to_list())

	x_min, x_max = min(x) - 1, max(x) + 1
	y_min, y_max = min(y) - 1, max(y) + 1
	xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.02), np.arange(y_min, y_max, 0.02))

	fig, ax = plt.subplots()
	Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])

	Z = Z.reshape(xx.shape)
	ax.contourf(xx, yy, Z, cmap=plt.cm.Paired)
	ax.axis('off')

	ax.scatter(x, y, c=df['label'].apply(lambda x: colors[x]))
	ax.set_title('Perceptron 2D')

	plt.show()

	return clf

#Usa da técnica de perceptrons para calcular um separador linear para os dados fornecidos.
#Essa função também plota a superfície separadora e os pontos amostrais utilizados (R³).
def perceptron_calculating_3d(x, y, z, label):
	df = pd.DataFrame(dict(x=x, y=y, z=z, label=label))
	colors = {1: 'green', -1: 'red'}

	clf = Perceptron(tol=1e-3)
	lista_coord = [[x, y, z] for x, y, z in zip(df['x'].to_list(), df['y'].to_list(), df['z'].to_list())]
	clf.fit(lista_coord, df['label'].to_list())

	global grafico

	if(random.randint(1, 1001) > 999 and grafico == 0):
		grafico = 1

		x_min, x_max = min(x) - 1, max(x) + 1
		y_min, y_max = min(y) - 1, max(y) + 1
		xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.02), np.arange(y_min, y_max, 0.02))
		if clf.coef_[0][2] > 0:
			Z = (-clf.coef_[0][0]*xx - clf.coef_[0][1]*yy)/clf.coef_[0][2]
		else:
			Z = (-clf.coef_[0][0]*xx - clf.coef_[0][1]*yy)/1e-15

		plt3d = plt.figure().gca(projection='3d')
		plt3d.plot_surface(xx, yy, Z, alpha=0.4)
		ax = plt.gca()

		for estado in ['vitoria', 'nao_vitoria']:
			if estado == 'vitoria':
				ax.scatter(df[df['label'] == 1]['x'].to_list(), df[df['label'] == 1]['y'].to_list(),
					df[df['label'] == 1]['z'].to_list(), c=df[df['label'] == 1]['label'].apply(lambda x: colors[x]),
					label='Vitória')
			else:
				ax.scatter(df[df['label'] == -1]['x'].to_list(), df[df['label'] == -1]['y'].to_list(),
					df[df['label'] == -1]['z'].to_list(), c=df[df['label'] == -1]['label'].apply(lambda x: colors[x]),
					label='Não vitória')
				
		ax.set_title('Perceptron 3D')
		ax.legend()

		plt.show()
	

	return clf

#Função que contabiliza erro associado o classificador passado como parâmetro. 
def counting_error_classifiers_perceptron_3d(clf):
	erro = 0

	#Não vitória
	if clf.predict([[0, 0, 0]]) != -1:
		erro += (77904+46080)/255168
	
	#Horizontal
	if clf.predict([[1, 0, 0]]) != 1:
		erro += 38772/255168
	#Vertical
	if clf.predict([[0, 1, 0]]) != 1:
		erro += 38772/255168
	#Horizontal e vertical
	if clf.predict([[1, 1, 0]]) != 1:
		erro += 5184/255168
	#Diagonal
	if clf.predict([[0, 0, 1]]) != 1:
		erro += 41544/255168
	#Horizontal e diagonal
	if clf.predict([[1, 0, 1]]) != 1:
		erro += 3456/255168
	#Vertical e diagonal
	if clf.predict([[0, 1, 1]]) != 1:
		erro += 3456/255168

	return erro

#Usa a técnica de regressão logística para calcular a probabilidade de cada estado de jogo fornecido ser
#vitória do jogador 'X'. Essa função também plota a superfície separadora encontrada para atribuir
#probabilidades e os ponstos amostrais utilizados (R³).
def logistic_regression_calculating(x, y, z, label):
	df = pd.DataFrame(dict(x=x, y=y, z=z, label=label))

	lista_coord = [[x, y, z] for x, y, z in zip(df['x'].to_list(), df['y'].to_list(), df['z'].to_list())]
	clf = LogisticRegression()
	clf.fit(lista_coord, df['label'].to_list())

	x_min, x_max = min(x) - 1, max(x) + 1
	y_min, y_max = min(y) - 1, max(y) + 1
	xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.02), np.arange(y_min, y_max, 0.02))
	Z = (-clf.coef_[0][0]*xx - clf.coef_[0][1]*yy)/clf.coef_[0][2]

	plt3d = plt.figure().gca(projection='3d')
	plt3d.plot_surface(xx, yy, Z, alpha=0.4)

	lista_prob = clf.predict_proba(np.array(lista_coord))

	ax = plt.gca()
	PCM = ax.scatter(x, y, z, c=lista_prob[:, 1], cmap=cm.viridis)
	ax.set_title('Logistic Regression 3D')
	plt.colorbar(PCM, ax=ax)

	plt.show()

	return clf

#Usa técnica de regressão logística para calcular o hiperplano separador num espaço de nove
#dimensões, cada uma delas uma casa do tabuleiro de jogo da velha.
def logistic_regression_9d(X, label):
	
	clf = LogisticRegression()
	clf.fit(X, label)

	print(clf.predict_proba(np.array([[1, 1, 1, 1, 1, 1, 1, 1, 1]])))

	if clf.predict_proba(np.array([[1, 1, 1, 1, 1, 1, 1, 1, 1]]))[0][0] > 0.5:
		return clf, 0
	
	else:
		return clf, 1

#Função que contabiliza erro associado o classificador passado como parâmetro.
def counting_error_logistic_regression_9d(clf, param):
	global dicionario_de_estados
	erro = 0
	for i in dicionario_de_estados.values():
		if clf.predict_proba(np.array([tab_to_positions(i)]))[0][param] > 0.5 and terminal_state(i) != 1:
			erro += 1
		elif clf.predict_proba(np.array([tab_to_positions(i)]))[0][param] < 0.5 and terminal_state(i) == 1:
			erro += 1

	return erro/255168

#Inicializa tabuleiro vazio
tab = np.array([[-1, -1, -1], [-1, -1, -1], [-1, -1, -1]])
#print_tab(tab)


#Inicializa os contadores de estados.
vitorias_X = 0
vitorias_O = 0
empates = 0
estados_terminais = 0
dicionario_de_estados = {}

'''
#Simula todos os estados do jogo para testar funcionalidade do programa, bem como
#conta todos os tipos de estados desejados.
simulate_every_state(1, tab)


#131184
print(vitorias_X)
#77904
print(vitorias_O)
#46080
print(empates)
#255168
print(estados_terminais)

print_tab(tab)
'''

'''
#Inicializa contadores de estados especiais para averiguar resultados de certos
#experimentos.
#(0, 0, 1)
vitorias_horizontal = 0
#(0, 1, 0)
vitorias_vertical = 0
#(0, 1, 1)
vitorias_horizontal_vertical = 0
#(1, 0, 0)
vitorias_diagonal = 0
#(1, 0, 1)
vitorias_horizontal_diagonal = 0
#(1, 1, 0)
vitorias_vertical_diagonal = 0

#Simula todos os estados do jogo para testar funcionalidade do programa, bem como
#conta todos os tipos de estados desejados.
simulate_every_state(1, tab)

#
print(nao_vitorias)

#38772
print(vitorias_horizontal)
#38772
print(vitorias_vertical)
#5184
print(vitorias_horizontal_vertical)
#41544
print(vitorias_diagonal)
#3456
print(vitorias_horizontal_diagonal)
#3456
print(vitorias_vertical_diagonal)
'''

simulate_every_state(1, tab)

#Inicializa o número de jogos que o usuário quer alcançar e gera aleatoriamente
#esse número de estados finais.
print('Insira o número de estados que você quer considerar na sua amostra: ')
m = input()
dicionario_conta_erro = {}
grafico = 0

'''
for i in range(1000):
	x = []
	y = []
	z = []
	label = []
	for i in range(int(m)):
		iteracao = random.randint(1, 255169)
		#iteracao = random.randint(1, 2000)
		
		#tab_aux = deepcopy(pick_a_state(1, tab))
		#empty_tab(tab)
		tab_aux = dicionario_de_estados[iteracao]

		#print_tab(tab_aux)
		
		#print((horizontal(tab_aux), vertical(tab_aux), diagonal(tab_aux)))
		x.append(horizontal(tab_aux))
		y.append(vertical(tab_aux))
		z.append(diagonal(tab_aux))
		if terminal_state(tab_aux) == 1:
			label.append(1)
		else:
			label.append(-1)

	#Chama a função de perceptron para duas dimensões.
	#clf = perceptron_calculating_2d(x, y, label)

	#Chama a função de perceptron para três dimensões.
	clf = perceptron_calculating_3d(x, y, z, label)
	erro = counting_error_classifiers_perceptron_3d(clf)
	if "%.3f" % erro in dicionario_conta_erro.keys():
		dicionario_conta_erro["%.3f" % erro] += 1
	else:
		dicionario_conta_erro["%.3f" % erro] = 1
	#Chama a função de regressão logística para três dimensões.
	#clf = logistic_regression_calculating(x, y, z, label)

for i in dicionario_conta_erro.keys():
	dicionario_conta_erro[i] = dicionario_conta_erro[i]/1000
dicionario_conta_erro = OrderedDict(sorted(dicionario_conta_erro.items()))
print(dicionario_conta_erro)
plt.bar(dicionario_conta_erro.keys(), dicionario_conta_erro.values())
plt.title('Distribution of errors m=26')
plt.xlabel('Error')
plt.ylabel('Probability')
plt.show()
'''

for i in range(20):
	X = []
	label = []
	for i in range(int(m)):
		iteracao = random.randint(1, 255169)
		tab_aux = dicionario_de_estados[iteracao]

		X.append(tab_to_positions(tab_aux))
		label.append(0 if terminal_state(tab_aux) != 1 else 1)

	clf, param = logistic_regression_9d(X, label)
	print(counting_error_logistic_regression_9d(clf, param))