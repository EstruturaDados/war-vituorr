#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Territorio
{
    char nome[30];
    char cor[10];
    int tropas;
};

struct Territorio *territorios = NULL;
int MaxTer = 0;
int iAtacante, iDefensor;

// Missões
char *missaoJogador1 = NULL;
char *missaoJogador2 = NULL;

// Protótipos
void NumTerritorios();
void CadastroTerritorios();
void ExibicaoMapa();
void Atacar(struct Territorio *Atacante, struct Territorio *Defensor);
void LiberacaoMem();
void atribuirMissao(char **missao, char *missoes[], int totalMissoes);
int verificarMissao(char *missao, struct Territorio *mapa, int tamanho);
void exibirMissao(char *missao);

int main()
{
    srand(time(NULL));

    // Territórios
    NumTerritorios();
    CadastroTerritorios();
    ExibicaoMapa();

    // Definir missões
    char *missoes[5] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor Vermelho",
        "Ter 5 territorios com pelo menos 3 tropas",
        "Controlar todos os territorios de cor Azul",
        "Possuir mais tropas que o adversario em 2 territorios"
    };

    // Alocar e sortear missões para dois jogadores
    atribuirMissao(&missaoJogador1, missoes, 5);
    atribuirMissao(&missaoJogador2, missoes, 5);

    printf("\nMissao do Jogador 1: ");
    exibirMissao(missaoJogador1);

    printf("\nMissao do Jogador 2: ");
    exibirMissao(missaoJogador2);

    printf("\nNumeros podem ser escolhidos de 1 a %d (0 para sair)...\n", MaxTer);

    do
    {
        printf("\nEscolha um atacante: ");
        scanf("%d", &iAtacante);
        if (iAtacante == 0) break;

        printf("Escolha um alvo: ");
        scanf("%d", &iDefensor);
        if (iDefensor == 0) break;

        Atacar(&territorios[iAtacante - 1], &territorios[iDefensor - 1]);

        // Verificar missões
        if (verificarMissao(missaoJogador1, territorios, MaxTer))
        {
            printf("\nJogador 1 cumpriu sua missao e venceu o jogo!");
            break;
        }
        if (verificarMissao(missaoJogador2, territorios, MaxTer))
        {
            printf("\nJogador 2 cumpriu sua missao e venceu o jogo!");
            break;
        }

    } while (1);

    LiberacaoMem();
    return 0;
}

// Implementações

void NumTerritorios()
{
    MaxTer = 5;

    territorios = calloc(MaxTer, sizeof(struct Territorio));
    if (!territorios)
    {
        printf("Erro de alocacao!\n");
        exit(1);
    }
}

void CadastroTerritorios()
{
    printf("\n=======================\n");
    printf("Cadastro de Territorios\n");
    printf("=======================\n");

    for (int i = 0; i < MaxTer; i++)
    {
        printf("\nTerritorio %d \n", i + 1);

        printf("Nome: ");
        scanf("%s", territorios[i].nome);

        printf("Cor: ");
        scanf("%s", territorios[i].cor);

        printf("Numero de Tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // limpar buffer
    }
}

void ExibicaoMapa()
{
    printf("\n\n==============================");
    printf("\n            MAPA             \n");
    printf("==============================\n");
    for (int i = 0; i < MaxTer; i++)
    {
        printf("\n(%d) %s Cor: %s Tropas: %d", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("\n==============================\n");
}

void Atacar(struct Territorio *Atacante, struct Territorio *Defensor)
{
    if (strcmp(Atacante->cor, Defensor->cor) == 0)
    {
        printf("\nExercitos aliados, ataque invalido!\n");
        return;
    }

    int dadoAtk = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;
    printf("\n%s rolou o dado e tirou: %d\n", Atacante->nome, dadoAtk);
    printf("%s rolou o dado e tirou: %d\n", Defensor->nome, dadoDef);

    if (dadoAtk > dadoDef)
    {
        printf("%s ganhou! %s perde 1 tropa.\n", Atacante->nome, Defensor->nome);
        Defensor->tropas--;
    }
    else if (dadoAtk < dadoDef)
    {
        printf("%s ganhou! %s perde 1 tropa.\n", Defensor->nome, Atacante->nome);
        Atacante->tropas--;
    }
    else
    {
        printf("\nEMPATE!\n");
    }

    if (Atacante->tropas == 0)
    {
        strcpy(Atacante->cor, Defensor->cor);
        Atacante->tropas = 1;
        printf("\nO territorio %s foi tomado pelo exercito: %s\n", Atacante->nome, Defensor->cor);
    }
    else if (Defensor->tropas == 0)
    {
        strcpy(Defensor->cor, Atacante->cor);
        Defensor->tropas = Atacante->tropas / 2;
        printf("\nO territorio %s foi tomado pelo exercito: %s\n", Defensor->nome, Atacante->cor);
    }

    ExibicaoMapa();
}

void LiberacaoMem()
{
    free(territorios);
    free(missaoJogador1);
    free(missaoJogador2);
}

void atribuirMissao(char **missao, char *missoes[], int totalMissoes)
{
    *missao = malloc(100 * sizeof(char));
    if (!*missao)
    {
        printf("Erro de alocacao de missao!\n");
        exit(1);
    }
    int sorteio = rand() % totalMissoes;
    strcpy(*missao, missoes[sorteio]);
}

int verificarMissao(char *missao, struct Territorio *mapa, int tamanho)
{
    // Lógica simples: apenas verificando se há alguma tropa vermelha
    if (strstr(missao, "Vermelho") != NULL)
    {
        for (int i = 0; i < tamanho; i++)
        {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0)
                return 0; // ainda existe vermelho
        }
        return 1; // missão cumprida
    }
    // Outras missões podem ser implementadas com regras próprias
    return 0;
}

void exibirMissao(char *missao)
{
    printf("%s\n", missao);
}