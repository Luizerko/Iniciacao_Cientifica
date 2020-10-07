#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Jogo da velha implementado em C.

Tabuleiro numa Matriz   -> (-1) vazio
                        -> 0 jogador "O"
                        -> 1 jogador "X"
*/

/*Devolve 1 estamos num estado terminal e o jogador "X" ganhou, 0 se estamos num estado terminal e o jogador "O" ganhou,
devolve -1 se o jogo empatou e -2 se não é um estado terminal.*/
int terminal_state(int** tab) {
    
    int ganhou;

    //Checa se alguém ganhou em linha
    for(int i = 0; i < 3; i++) {
        if(tab[i][0] != -1) {
            int val = tab[i][0];
            ganhou = val;
            for(int j = 1; j < 3; j++) {
                if(tab[i][j] != val) ganhou = -2;
            }
            if(ganhou == val) return ganhou;
        }
    }

    //Checa se alguém ganhou em coluna
    for(int j = 0; j < 3; j++) {
        if(tab[0][j] != -1) {
            int val = tab[0][j];
            ganhou = val;
            for(int i = 1; i < 3; i++) {
                if(tab[i][j] != val) ganhou = -2;
            }
            if(ganhou == val) return ganhou;
        }
    }

    //Checa se alguém ganhou em diagonal
    if(tab[0][0] != -1) {
        int val = tab[0][0];
        ganhou = val;
        for(int i = 1; i < 3; i++){
            if(tab[i][i] != val) ganhou = -2;
        }
        if(ganhou == val) return ganhou;
    }
    if(tab[0][2] != -1) {
        int val = tab[0][2];
        ganhou = val;
        for(int i = 1; i < 3; i++){
            if(tab[i][2-i] != val) ganhou = -2;
        }
        if(ganhou == val) return ganhou;
    }

    //Checa se o jogo não tem vencedores e tem espaços vazios - estado não terminal
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(tab[i][j] == -1) return (-2);
        }
    }

    //Jogo empatou
    return -1;
}

//Retorna o jogador que ganhou na horizontal ou zero se nenhum ganhou na vertical
int horizontal(int** tab) {
	int ganhou;

	//Checa se alguém ganhou em linha
	for(int i = 0; i < 3; i++) {
	    if(tab[i][0] != -1) {
	        int val = tab[i][0];
	        ganhou = val;
	        for(int j = 1; j < 3; j++) {
	            if(tab[i][j] != val) ganhou = -2;
	        }
	        if(ganhou == val) return ganhou;
	    }
	}

	return 0;
}

//Retorna o jogador que ganhou na vertical ou zero se nenhum ganhou na vertical
int vertical(int** tab) {
	int ganhou;
	
	//Checa se alguém ganhou em coluna
    for(int j = 0; j < 3; j++) {
        if(tab[0][j] != -1) {
            int val = tab[0][j];
            ganhou = val;
            for(int i = 1; i < 3; i++) {
                if(tab[i][j] != val) ganhou = -2;
            }
            if(ganhou == val) return ganhou;
        }
    }

    return 0;
}

//Retorna o jogador que ganhou na diagonal ou zero se nenhum ganhou na vertical
int diagonal(int** tab) {
	int ganhou;

	//Checa se alguém ganhou em diagonal
	if(tab[0][0] != -1) {
	    int val = tab[0][0];
	    ganhou = val;
	    for(int i = 1; i < 3; i++){
	        if(tab[i][i] != val) ganhou = -2;
	    }
	    if(ganhou == val) return ganhou;
	}
	if(tab[0][2] != -1) {
	    int val = tab[0][2];
	    ganhou = val;
	    for(int i = 1; i < 3; i++){
	        if(tab[i][2-i] != val) ganhou = -2;
	    }
	    if(ganhou == val) return ganhou;
	}

	return 0;
}

//Retorna 1 se o jogador ocupava o meio do tabuleiro. Retorna 0 caso contrário.
int meio_ocupado(int jogador, int** tab) {
    if(tab[1][1] == jogador) return 1;
    return 0;
}

//Retorna 1 se o jogador tem uma sequência de dois no tabuleiro. Retorna 0 caso contrário
int sequencia_de_dois(int jogador, int** tab) {
    
    //Testa se as linhas têm alguma sequência de dois
    for(int i = 0; i < 3; i++) {
        if(tab[i][0] == jogador) {
            if(tab[i][1] == jogador)
                return 1;
        }
        else if(tab[i][1] == jogador){
            if(tab[i][2] == jogador)
                return 1;
        }
    }

    //Testa se as colunas têm alguma sequência de dois
    for(int j = 0; j < 3; j++) {
        if(tab[0][j] == jogador) {
            if(tab[1][j] == jogador)
                return 1;
        }
        else if(tab[1][j] == jogador){
            if(tab[2][j] == jogador)
                return 1;
        }
    }

    //Testa se as diagonais têm alguma sequência de dois
    for(int i = 0; i < 2; i++) {
        if(tab[i][i] == jogador)
            if(tab[i+1][i+1] == jogador)
                return 1;
        if(tab[i][2-i] == jogador)
            if(tab[i+1][2-(i+1)] == jogador)
                return 1;
    }

    return 0;
}

//Funçõ para imprimir o tabuleiro na tela
void printa_tabuleiro(int** tab) {
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			printf("%d ", tab[i][j]);
		}
		printf("\n");
	}
}

//Função de comparação para uso em qsort() 
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main() {

	srand(time(NULL));

	int jogador = 1;

	int** tab = malloc(3*sizeof(int*));
	for(int i = 0; i < 3; i++) {
	    tab[i] = malloc(3*sizeof(int));
	}

	int H_tam = 32;
	int* classe_hipoteses = malloc(H_tam*sizeof(int));
	for(int i = 0; i < H_tam; i++) {
		classe_hipoteses[i] = 0;
	}

	int m = 10;

	for(int w = 0; w < m; w++) {

		for(int i = 0; i < 3; i++) {
	    	for(int j = 0; j < 3; j++) {
	        	tab[i][j] = -1;
	    	}
		}

		int terminal = terminal_state(tab);
		while(terminal == -2) {
			int aux_i = rand()%3;
			int aux_j = rand()%3;
			while(tab[aux_i][aux_j] != -1) {
				aux_i = rand()%3;
				aux_j = rand()%3;
			}
			tab[aux_i][aux_j] = jogador;
			jogador = 1 - jogador;
			terminal = terminal_state(tab);
			int r = rand()%100;
			if(r == 0)
				break;
		}

		
		printf("\n");
		printf("%d\n", terminal);
		printa_tabuleiro(tab);
		

		int vetor_bits[5] = {0, 0, 0, 0, 0};
		int p_0 = 0, p_1 = 0, p_2 = 0, p_3 = 0, p_4 = 0;
		int indice = 0;

		for(int i = 0; i < H_tam; i++) {
			p_0 = 0; p_1 = 0; p_2 = 0; p_3 = 0; p_4 = 0;
			if(vetor_bits[0])
				p_0 = horizontal(tab);
			if(vetor_bits[1])
				p_1 = vertical(tab);
			if(vetor_bits[2])
				p_2 = diagonal(tab);
			if(vetor_bits[3])
				p_3 = meio_ocupado(1, tab);
			if(vetor_bits[4])
				p_4 = sequencia_de_dois(1, tab);

			if(p_0 || p_1 || p_2 || p_3 || p_4) {
				if(terminal != 1)
					classe_hipoteses[indice]++;
			}
			else {
				if(terminal == 1)
					classe_hipoteses[indice]++;
			}
			indice++;

			for(int j = 4; j >= 0; j--) {
				if(vetor_bits[j])
					vetor_bits[j] = 0;
				else if(!vetor_bits[j]) {
					vetor_bits[j] = 1;
					break;
				}
			}
		}
	}

	//qsort(classe_hipoteses, H_tam, sizeof(int), cmpfunc);
	
	int min = 10000;
	for(int i = 0; i < H_tam; i++) {
		if(classe_hipoteses[i] < min)
			min = classe_hipoteses[i];
	}

	int aux_indice = 0;
	int* ERM = malloc(H_tam*sizeof(int));
	for(int i = 0; i < H_tam; i++) {
		ERM[i] = -1;
	}
	for(int i = 0; i < H_tam; i++) {
		if(classe_hipoteses[i] == min) {
			ERM[aux_indice] = i;
			aux_indice++;
		}
	}

	int h = ERM[rand()%aux_indice];
	
	printf("Erro L_s(h) sobre a amostra: %.4f\n", (float)classe_hipoteses[h]/m);

	FILE* ptr = fopen("Erro_PACL.txt", "a+");
	fprintf(ptr, "%.4f %d\n", (float)classe_hipoteses[h]/m, h);
	fclose(ptr);

	return 0;
}