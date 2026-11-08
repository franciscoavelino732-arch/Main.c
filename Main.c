#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[50];
    int quantidade;
} Produto;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void listarEstoque() {
    FILE *file = fopen("inventory.txt", "r");
    
    printf("========================================\n");
    printf("             Estoque Atual              \n");
    printf("========================================\n");
    
    if (file == NULL) {
        printf("O estoque está vazio.\n");
        return;
    }
    
    char nome[50];
    int quantidade;
    int vazio = 1;
    
    while (fscanf(file, " %[^\n]", nome) == 1) {
        if (fscanf(file, "%d", &quantidade) == 1) {
            printf("Nome: %s\n", nome);
            printf("Quantidade: %d\n\n", quantidade);
            vazio = 0;
        }
    }
    
    fclose(file);
    
    if (vazio) {
        printf("O estoque está vazio.\n");
    }
}

void adicionarItem() {
    char nome[50];
    int quantidade;
    
    printf("Digite o nome do item: ");
    scanf(" %[^\n]", nome);
    printf("Digite a quantidade: ");
    scanf("%d", &quantidade);
    
    FILE *file = fopen("inventory.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    int encontrado = 0;
    char nomeLido[50];
    int qtdLida;
    
    if (file != NULL && temp != NULL) {
        while (fscanf(file, " %[^\n]", nomeLido) == 1) {
            fscanf(file, "%d", &qtdLida);
            if (strcmp(nomeLido, nome) == 0) {
                qtdLida += quantidade;
                encontrado = 1;
            }
            fprintf(temp, "%s\n%d\n", nomeLido, qtdLida);
        }
        fclose(file);
        fclose(temp);
        
        remove("inventory.txt");
        rename("temp.txt", "inventory.txt");
    }
    
    if (!encontrado) {
        file = fopen("inventory.txt", "a");
        if (file != NULL) {
            fprintf(file, "%s\n%d\n", nome, quantidade);
            fclose(file);
        }
    }
    
    printf("Item adicionado com sucesso!\n");
}

void removerItem() {
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("O estoque está vazio. Item não encontrado.\n");
        return;
    }
    
    char nomeBusca[50];
    int qtdRemover;
    
    printf("Digite o nome do item: ");
    scanf(" %[^\n]", nomeBusca);
    printf("Digite a quantidade a ser removida: ");
    scanf("%d", &qtdRemover);
    
    FILE *temp = fopen("temp.txt", "w");
    char nomeLido[50];
    int qtdLida;
    int encontrado = 0;
    
    while (fscanf(file, " %[^\n]", nomeLido) == 1) {
        fscanf(file, "%d", &qtdLida);
        
        if (strcmp(nomeLido, nomeBusca) == 0) {
            encontrado = 1;
            if (qtdRemover > qtdLida) {
                printf("Estoque insuficiente. Quantidade disponível: %d\n", qtdLida);
                fprintf(temp, "%s\n%d\n", nomeLido, qtdLida);
            } else if (qtdLida - qtdRemover == 0) {
                printf("Item removido do estoque!\n");
            } else {
                qtdLida -= qtdRemover;
                printf("Quantidade atualizada com sucesso!\n");
                fprintf(temp, "%s\n%d\n", nomeLido, qtdLida);
            }
        } else {
            fprintf(temp, "%s\n%d\n", nomeLido, qtdLida);
        }
    }
    
    fclose(file);
    fclose(temp);
    
    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");
    
    if (!encontrado) {
        printf("Item não encontrado.\n");
    }
}

int main() {
    int opcao;
    char continuar;
    
    do {
        printf("========================================\n");
        printf("           Controle de Estoque          \n");
        printf("========================================\n");
        printf("Selecione uma opção:\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Estoque\n");
        printf("4. Sair\n");
        printf("Opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Opção inválida! Tente novamente.\n");
            limparBuffer();
            continue;
        }
        
        switch (opcao) {
            case 1:
                adicionarItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarEstoque();
                break;
            case 4:
                printf("Obrigado por usar o Controle de Estoque.\n");
                return 0;
            default:
                printf("Opção inválida! Tente novamente.\n");
                continue;
        }
        
        printf("\nDeseja realizar outra operação? (s/n): ");
        scanf(" %c", &continuar);
        
        if (continuar != 's' && continuar != 'S') {
            printf("Obrigado por usar o Controle de Estoque.\n");
            break;
        }
        
    } while (1);
    
    return 0;
}
