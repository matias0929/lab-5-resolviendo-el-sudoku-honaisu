#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
   int sudo[9][9];
} Node;

Node* createNode(){
  Node* n = (Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new = (Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file(char* file_name){
  Node* n = createNode();
  FILE* file = fopen(file_name, "r");
  int i, j;
  for(i = 0; i < 9; i++){
       for(j = 0; j < 9; j++){
          if(!fscanf(file, "%d", &n->sudo[i][j]))
            printf("Error al leer el archivo!\n");
       }
  }
  fclose(file);
  return n;
}

void print_node(Node* n){
    int i, j;
    for(i = 0; i < 9; i++){
       for(j = 0; j < 9; j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    // Verificación de filas y columnas
    for(int i = 0; i < 9; i++){
        int fila[10] = {0};
        int columna[10] = {0};
        for(int j = 0; j < 9; j++){
            int f = n->sudo[i][j];
            int c = n->sudo[j][i];
            if(f && fila[f]) return 0;
            if(c && columna[c]) return 0;
            fila[f] = c ? 1 : fila[f];
            columna[c] = f ? 1 : columna[c];
        }
    }

    // Verificación de submatrices 3x3
    for(int block = 0; block < 9; block++){
        int listaSub[10] = {0};
        int row_offset = (block / 3) * 3;
        int col_offset = (block % 3) * 3;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                int num = n->sudo[row_offset + i][col_offset + j];
                if(num && listaSub[num]) return 0;
                listaSub[num] = num ? 1 : listaSub[num];
            }
        }
    }
    return 1;
}

List* get_adj_nodes(Node* n){
    List* list = createList();
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(n->sudo[i][j] == 0){
                for(int k = 1; k <= 9; k++){
                    Node* copy_node = copy(n);
                    copy_node->sudo[i][j] = k;
                    if(is_valid(copy_node)) pushBack(list, copy_node);
                    else free(copy_node);
                }
                return list;
            }
        }
    }
    return list;
}

int is_final(Node* n){
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(n->sudo[i][j] == 0) return 0;
    return 1;
}

Node* DFS(Node* initial, int* cont){
    Stack* stack = createStack();
    push(stack, initial);

    while(!is_empty(stack)){
        Node* current = top(stack);
        pop(stack);
        (*cont)++;

        if(is_final(current)) return current;

        List* adj = get_adj_nodes(current);
        Node* adj_node = first(adj);
        while(adj_node){
            push(stack, adj_node);
            adj_node = next(adj);
        }
        free(current);
    }

    return NULL;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Uso: %s <archivo_sudoku.txt>\n", argv[0]);
        return 1;
    }

    Node* initial = read_file(argv[1]);
    int cont = 0;
    Node* final = DFS(initial, &cont);

    if(final){
        printf("Solución encontrada en %d iteraciones:\n", cont);
        print_node(final);
        free(final);
    } else {
        printf("No se encontró solución.\n");
    }

    free(initial);
    return 0;
}
