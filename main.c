#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 1000
#define TAM_ENDERECO 200

// Estruturas
typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int codigo;
    char nome[50];
    char sigla[10];
} Profissao;

typedef struct {
    int matricula;
    char cpf[15];
    char nome[100];
    int prof_codigo;
    char numRP[20];
    char tipo[30];
    Data dataNasc;
    char email[100];
    char fone[15];
} Profissional;

typedef struct {
    int codigo;
    char nome[100];
    Data dataNasc;
    int idade;
    char email[100];
    char fone[15];
    char celular[15];
    char endereco[TAM_ENDERECO];
} Cliente;

typedef struct {
    int numero;
    int prof_matricula;
    int cliente_codigo;
    Data data;
    char descricao[300];
} Atendimento;

// Vetores dinâmicos
Profissao *profissoes = NULL;
Profissional *profissionais = NULL;
Cliente *clientes = NULL;
Atendimento *atendimentos = NULL;

int numProfissoes = 0, numProfissionais = 0, numClientes = 0, numAtendimentos = 0;
int capProfissoes = 0, capProfissionais = 0, capClientes = 0, capAtendimentos = 0;

// Funções auxiliares
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int lerInteiro(const char *msg) {
    int valor;
    printf("%s", msg);
    while (scanf("%d", &valor) != 1) {
        printf("Entrada invalida. Tente novamente: ");
        limparBuffer();
    }
    limparBuffer();
    return valor;
}

void lerString(const char *msg, char *dest, int tam) {
    printf("%s", msg);
    fgets(dest, tam, stdin);
    dest[strcspn(dest, "\n")] = '\0';
    if (strlen(dest) == 0) {
        strcpy(dest, "N/A");
    }
}

int compararDatas(Data a, Data b) {
    if (a.ano != b.ano) return a.ano - b.ano;
    if (a.mes != b.mes) return a.mes - b.mes;
    return a.dia - b.dia;
}

// Funções de realocação
void realocarProfissoes() {
    capProfissoes = (capProfissoes == 0) ? 10 : capProfissoes * 2;
    profissoes = realloc(profissoes, capProfissoes * sizeof(Profissao));
}

void realocarProfissionais() {
    capProfissionais = (capProfissionais == 0) ? 10 : capProfissionais * 2;
    profissionais = realloc(profissionais, capProfissionais * sizeof(Profissional));
}

void realocarClientes() {
    capClientes = (capClientes == 0) ? 10 : capClientes * 2;
    clientes = realloc(clientes, capClientes * sizeof(Cliente));
}

void realocarAtendimentos() {
    capAtendimentos = (capAtendimentos == 0) ? 10 : capAtendimentos * 2;
    atendimentos = realloc(atendimentos, capAtendimentos * sizeof(Atendimento));
}

// === CADASTRO ===
void cadastrarProfissao() {
    if (numProfissoes >= capProfissoes) realocarProfissoes();
    Profissao p;
    p.codigo = numProfissoes + 1;
    lerString("Nome da profissao: ", p.nome, 50);
    lerString("Sigla (CRM, CRO, etc): ", p.sigla, 10);
    profissoes[numProfissoes++] = p;
    printf("Profissao cadastrada com sucesso! Codigo: %d\n", p.codigo);
}

void cadastrarProfissional() {
    if (numProfissoes == 0) {
        printf("Cadastre pelo menos uma profissao primeiro!\n");
        return;
    }
    if (numProfissionais >= capProfissionais) realocarProfissionais();
    Profissional p;
    p.matricula = numProfissionais + 1;
    lerString("CPF: ", p.cpf, 15);
    lerString("Nome: ", p.nome, 100);
    printf("Profissoes disponiveis:\n");
    for (int i = 0; i < numProfissoes; i++) {
        printf("  [%d] %s (%s)\n", profissoes[i].codigo, profissoes[i].nome, profissoes[i].sigla);
    }
    p.prof_codigo = lerInteiro("Codigo da profissao: ");
    lerString("Numero do registro profissional (NRP): ", p.numRP, 20);
    lerString("Tipo (medico, enfermeiro, etc): ", p.tipo, 30);
    p.dataNasc.dia = lerInteiro("Dia nascimento: ");
    p.dataNasc.mes = lerInteiro("Mes nascimento: ");
    p.dataNasc.ano = lerInteiro("Ano nascimento: ");
    lerString("E-mail: ", p.email, 100);
    lerString("Telefone: ", p.fone, 15);
    profissionais[numProfissionais++] = p;
    printf("Profissional cadastrado! Matricula: %d\n", p.matricula);
}

void cadastrarCliente() {
    if (numClientes >= capClientes) realocarClientes();
    Cliente c;
    c.codigo = numClientes + 1;
    lerString("Nome: ", c.nome, 100);
    c.dataNasc.dia = lerInteiro("Dia nascimento: ");
    c.dataNasc.mes = lerInteiro("Mes nascimento: ");
    c.dataNasc.ano = lerInteiro("Ano nascimento: ");
    c.idade = 2025 - c.dataNasc.ano; // Ajuste simplificado
    lerString("E-mail: ", c.email, 100);
    lerString("Telefone: ", c.fone, 15);
    lerString("Celular: ", c.celular, 15);
    lerString("Endereco (logradouro, num, bairro, cidade, estado, CEP): ", c.endereco, TAM_ENDERECO);
    clientes[numClientes++] = c;
    printf("Cliente cadastrado! Codigo: %d\n", c.codigo);
}

void cadastrarAtendimento() {
    if (numProfissionais == 0 || numClientes == 0) {
        printf("Cadastre profissionais e clientes primeiro!\n");
        return;
    }
    if (numAtendimentos >= capAtendimentos) realocarAtendimentos();
    Atendimento a;
    a.numero = numAtendimentos + 1;
    printf("Profissionais:\n");
    for (int i = 0; i < numProfissionais; i++) {
        printf("  [%d] %s\n", profissionais[i].matricula, profissionais[i].nome);
    }
    a.prof_matricula = lerInteiro("Matricula do profissional: ");
    printf("Clientes:\n");
    for (int i = 0; i < numClientes; i++) {
        printf("  [%d] %s\n", clientes[i].codigo, clientes[i].nome);
    }
    a.cliente_codigo = lerInteiro("Codigo do cliente: ");
    a.data.dia = lerInteiro("Dia atendimento: ");
    a.data.mes = lerInteiro("Mes atendimento: ");
    a.data.ano = lerInteiro("Ano atendimento: ");
    lerString("Descricao do atendimento: ", a.descricao, 300);
    atendimentos[numAtendimentos++] = a;
    printf("Atendimento registrado! Numero: %d\n", a.numero);
}

// === RELATÓRIOS ===
void listagemProfissionais() {
    printf("\n=== LISTAGEM DE PROFISSIONAIS ===\n");
    printf("%-8s %-30s %-20s\n", "Matricula", "Nome", "Tipo");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < numProfissionais; i++) {
        printf("%-8d %-30s %-20s\n", profissionais[i].matricula, profissionais[i].nome, profissionais[i].tipo);
    }
}

void aniversariantesInstituicao() {
    int dia = lerInteiro("Dia do aniversario: ");
    int mes = lerInteiro("Mes do aniversario: ");
    printf("\n=== ANIVERSARIANTES DA INSTITUICAO (%02d/%02d) ===\n", dia, mes);
    int encontrado = 0;
    for (int i = 0; i < numProfissionais; i++) {
        if (profissionais[i].dataNasc.dia == dia && profissionais[i].dataNasc.mes == mes) {
            printf("Profissional: %s\n", profissionais[i].nome);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("Nenhum aniversariante encontrado.\n");
}

void atendimentoGeral() {
    printf("\n=== ATENDIMENTO GERAL ===\n");
    printf("%-25s %-25s %-12s\n", "Cliente", "Profissional", "Data");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < numAtendimentos; i++) {
        char nomeProf[100] = "Desconhecido";
        char nomeCli[100] = "Desconhecido";
        for (int j = 0; j < numProfissionais; j++) {
            if (profissionais[j].matricula == atendimentos[i].prof_matricula) {
                strcpy(nomeProf, profissionais[j].nome);
                break;
            }
        }
        for (int j = 0; j < numClientes; j++) {
            if (clientes[j].codigo == atendimentos[i].cliente_codigo) {
                strcpy(nomeCli, clientes[j].nome);
                break;
            }
        }
        printf("%-25s %-25s %02d/%02d/%d\n", nomeCli, nomeProf,
               atendimentos[i].data.dia, atendimentos[i].data.mes, atendimentos[i].data.ano);
    }
}

void atendimentoPorPeriodo() {
    Data inicio, fim;
    printf("Periodo inicial:\n");
    inicio.dia = lerInteiro("Dia: ");
    inicio.mes = lerInteiro("Mes: ");
    inicio.ano = lerInteiro("Ano: ");
    printf("Periodo final:\n");
    fim.dia = lerInteiro("Dia: ");
    fim.mes = lerInteiro("Mes: ");
    fim.ano = lerInteiro("Ano: ");

    printf("\n=== ATENDIMENTOS NO PERIODO ===\n");
    printf("%-25s %-25s %-12s\n", "Cliente", "Profissional", "Data");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < numAtendimentos; i++) {
        int cmpInicio = compararDatas(atendimentos[i].data, inicio);
        int cmpFim = compararDatas(atendimentos[i].data, fim);
        if (cmpInicio >= 0 && cmpFim <= 0) {
            char nomeProf[100] = "Desconhecido";
            char nomeCli[100] = "Desconhecido";
            for (int j = 0; j < numProfissionais; j++) {
                if (profissionais[j].matricula == atendimentos[i].prof_matricula) {
                    strcpy(nomeProf, profissionais[j].nome);
                    break;
                }
            }
            for (int j = 0; j < numClientes; j++) {
                if (clientes[j].codigo == atendimentos[i].cliente_codigo) {
                    strcpy(nomeCli, clientes[j].nome);
                    break;
                }
            }
            printf("%-25s %-25s %02d/%02d/%d\n", nomeCli, nomeProf,
                   atendimentos[i].data.dia, atendimentos[i].data.mes, atendimentos[i].data.ano);
        }
    }
}

void atendimentoPorMesAno() {
    int mes = lerInteiro("Mes (1-12): ");
    int ano = lerInteiro("Ano: ");
    int contagem[32] = {0};
    for (int i = 0; i < numAtendimentos; i++) {
        if (atendimentos[i].data.mes == mes && atendimentos[i].data.ano == ano) {
            contagem[atendimentos[i].data.dia]++;
        }
    }
    printf("\n=== ATENDIMENTOS POR DIA NO MES %02d/%d ===\n", mes, ano);
    int total = 0;
    for (int d = 1; d <= 31; d++) {
        if (contagem[d] > 0) {
            printf("Dia %02d: %d atendimento(s)\n", d, contagem[d]);
            total += contagem[d];
        }
    }
    printf("TOTAL NO MES: %d\n", total);
}

void estatisticaPorProfissional() {
    Data inicio, fim;
    printf("Periodo inicial:\n");
    inicio.dia = lerInteiro("Dia: ");
    inicio.mes = lerInteiro("Mes: ");
    inicio.ano = lerInteiro("Ano: ");
    printf("Periodo final:\n");
    fim.dia = lerInteiro("Dia: ");
    fim.mes = lerInteiro("Mes: ");
    fim.ano = lerInteiro("Ano: ");

    printf("\n=== ESTATISTICA DE ATENDIMENTOS POR PROFISSIONAL ===\n");
    for (int i = 0; i < numProfissionais; i++) {
        int cont = 0;
        for (int j = 0; j < numAtendimentos; j++) {
            if (atendimentos[j].prof_matricula == profissionais[i].matricula) {
                int cmpInicio = compararDatas(atendimentos[j].data, inicio);
                int cmpFim = compararDatas(atendimentos[j].data, fim);
                if (cmpInicio >= 0 && cmpFim <= 0) cont++;
            }
        }
        if (cont > 0) {
            printf("%s: %d atendimento(s)\n", profissionais[i].nome, cont);
        }
    }
}

void aniversariantesClientes() {
    int dia = lerInteiro("Dia do aniversario: ");
    int mes = lerInteiro("Mes do aniversario: ");
    printf("\n=== ANIVERSARIANTES DOS CLIENTES (%02d/%02d) ===\n", dia, mes);
    int encontrado = 0;
    for (int i = 0; i < numClientes; i++) {
        if (clientes[i].dataNasc.dia == dia && clientes[i].dataNasc.mes == mes) {
            printf("Nome: %s | Fone: %s | Cel: %s | Email: %s\n",
                   clientes[i].nome, clientes[i].fone, clientes[i].celular, clientes[i].email);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("Nenhum cliente aniversariante.\n");
}

// === MENU PRINCIPAL ===
int main() {
    int opcao;
    do {
        printf("\n");
        printf("=====================================\n");
        printf("   SISTEMA DE ATENDIMENTO EM SAUDE   \n");
        printf("=====================================\n");
        printf("1. Cadastrar Profissao\n");
        printf("2. Cadastrar Profissional\n");
        printf("3. Cadastrar Cliente\n");
        printf("4. Registrar Atendimento\n");
        printf("5. Listagem de Profissionais\n");
        printf("6. Aniversariantes da Instituicao\n");
        printf("7. Atendimento Geral\n");
        printf("8. Atendimento por Periodo\n");
        printf("9. Atendimentos por Mes/Ano\n");
        printf("10. Estatistica por Profissional\n");
        printf("11. Aniversariantes dos Clientes\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: cadastrarProfissao(); break;
            case 2: cadastrarProfissional(); break;
            case 3: cadastrarCliente(); break;
            case 4: cadastrarAtendimento(); break;
            case 5: listagemProfissionais(); break;
            case 6: aniversariantesInstituicao(); break;
            case 7: atendimentoGeral(); break;
            case 8: atendimentoPorPeriodo(); break;
            case 9: atendimentoPorMesAno(); break;
            case 10: estatisticaPorProfissional(); break;
            case 11: aniversariantesClientes(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    // Liberar memória
    free(profissoes);
    free(profissionais);
    free(clientes);
    free(atendimentos);

    return 0;
}
