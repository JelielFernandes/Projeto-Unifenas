#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CLIENTES 100
#define MAX_CARROS 100
#define MAX_VENDAS 100


typedef struct
{
    int id;
    char modelo[50];
    float preco;
    int quantidade_estoque;
} Carro;

typedef struct
{
    int id;
    char nome[50];
    char telefone[20];
    float valor_gasto;
} Cliente;

typedef struct
{
    int id;
    int idCliente;
    int idCarro;
    struct tm dataHora;
} Venda;


struct tm obterDataHoraAtual()
{
    time_t t;
    struct tm *dataHora;

    time(&t);
    dataHora = localtime(&t);

    return *dataHora;
}

char *formatarDataHora(struct tm dataHora)
{
    static char dataHoraFormatada[20];
    strftime(dataHoraFormatada, sizeof(dataHoraFormatada), "%d/%m/%Y %H:%M:%S", &dataHora);
    return dataHoraFormatada;
}

const char *obterNomeCliente(int idCliente, Cliente clientes[], int numClientes)
{
    for (int i = 0; i < numClientes; i++)
    {
        if (clientes[i].id == idCliente)
        {
            return clientes[i].nome;
        }
    }
    return "Cliente Desconhecido";
}

const char *obterNomeCarro(int idCarro, Carro carros[], int numCarros)
{
    for (int i = 0; i < numCarros; i++)
    {
        if (carros[i].id == idCarro)
        {
            return carros[i].modelo;
        }
    }
    return "Carro Desconhecido";
}

void listarVendas(Venda vendas[], int numVendas, Cliente clientes[], int numClientes, Carro carros[], int numCarros)
{
    printf("\nLista de Vendas:\n");
    printf("ID\tCliente\t\tCarro\t\tValor Carro\tData\n");

    for (int i = 0; i < numVendas; i++)
    {
        printf("%d\t%s\t\t%s\t\t%.2f\t\t%s\n", vendas[i].id,
               obterNomeCliente(vendas[i].idCliente, clientes, numClientes),
               obterNomeCarro(vendas[i].idCarro, carros, numCarros),
               carros[vendas[i].idCarro - 1].preco,
               formatarDataHora(vendas[i].dataHora));
    }
}

void adicionarCarro(Carro carros[], int *numCarros, const char *nomeArquivo)
{
    Carro novoCarro;

    
    printf("Informe o modelo do carro: ");
    scanf("%49s", novoCarro.modelo);
    printf("Informe o preco do carro: ");
    scanf("%f", &novoCarro.preco);
    printf("Informe a quantidade em estoque do carro: ");
    scanf("%d", &novoCarro.quantidade_estoque);

    
    novoCarro.id = *numCarros + 1;

    
    carros[*numCarros] = novoCarro;
    (*numCarros)++;

    
    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    
    fprintf(arquivo, "%d,%s,%.2f,%d\n", novoCarro.id, novoCarro.modelo, novoCarro.preco, novoCarro.quantidade_estoque);

    fclose(arquivo);

    printf("Carro adicionado com sucesso!\n");
}

void atualizarCarro(Carro carros[], int numCarros)
{
    
}

void cadastrarCliente(Cliente clientes[], int *numClientes, const char *nomeArquivo)
{
    Cliente novoCliente;

   
    printf("Informe o nome do cliente: ");
    scanf("%49s", novoCliente.nome);
    printf("Informe o telefone do cliente: ");
    scanf("%19s", novoCliente.telefone);
    novoCliente.valor_gasto = 0.00;

   
    novoCliente.id = *numClientes + 1;

    
    clientes[*numClientes] = novoCliente;
    (*numClientes)++;

    
    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

   
    fprintf(arquivo, "%d,%s,%s,%.2f\n", novoCliente.id, novoCliente.nome, novoCliente.telefone, novoCliente.valor_gasto);

    fclose(arquivo);

    printf("Cliente cadastrado com sucesso!\n");
}



void listarClientes(Cliente clientes[], int numClientes)
{
    printf("\nLista de Clientes:\n");
    printf("ID\tNome\t\tTelefone\tValor Gasto\n");
    for (int i = 0; i < numClientes; i++)
    {
        printf("%d\t%s\t\t%s\t\t%.2f\n", clientes[i].id, clientes[i].nome, clientes[i].telefone, clientes[i].valor_gasto);
    }
}

void listarCarros(Carro carros[], int numCarros)
{
    printf("\nLista de Carros:\n");
    printf("ID\tModelo\t\tPreco\tQuantidade em Estoque\n");
    for (int i = 0; i < numCarros; i++)
    {
        printf("%d\t%s\t\t%.2f\t%d\n", carros[i].id, carros[i].modelo, carros[i].preco, carros[i].quantidade_estoque);
    }
}

void realizarVenda(Venda vendas[], int *numVendas, Cliente clientes[], int numClientes,
                   Carro carros[], int numCarros, const char *nomeArquivoVendas, const char *nomeArquivoClientes, const char *nomeArquivoCarros)
{
    Venda novaVenda;

    
    printf("Informe o ID do cliente: ");
    scanf("%d", &novaVenda.idCliente);

    printf("Informe o ID do carro: ");
    scanf("%d", &novaVenda.idCarro);

   
    novaVenda.dataHora = obterDataHoraAtual();

   
    if (novaVenda.idCliente >= 1 && novaVenda.idCliente <= numClientes &&
        novaVenda.idCarro >= 1 && novaVenda.idCarro <= numCarros)
    {
        clientes[novaVenda.idCliente - 1].valor_gasto += carros[novaVenda.idCarro - 1].preco;

       
        carros[novaVenda.idCarro - 1].quantidade_estoque--;

        
        novaVenda.id = *numVendas + 1;

      
        vendas[*numVendas] = novaVenda;
        (*numVendas)++;

        
        FILE *arquivoVendas = fopen(nomeArquivoVendas, "a");
        if (arquivoVendas == NULL)
        {
            printf("Erro ao abrir o arquivo de vendas para escrita.\n");
            return;
        }

       
        fprintf(arquivoVendas, "%d,%d,%d,%d-%d-%d %d:%d:%d\n", novaVenda.id, novaVenda.idCliente, novaVenda.idCarro,
                novaVenda.dataHora.tm_year, novaVenda.dataHora.tm_mon, novaVenda.dataHora.tm_mday,
                novaVenda.dataHora.tm_hour, novaVenda.dataHora.tm_min, novaVenda.dataHora.tm_sec);

        fclose(arquivoVendas);

        
        FILE *arquivoClientes = fopen(nomeArquivoClientes, "w");
        if (arquivoClientes == NULL)
        {
            printf("Erro ao abrir o arquivo de clientes para atualizacao.\n");
            return;
        }

        
        for (int i = 0; i < numClientes; i++)
        {
            fprintf(arquivoClientes, "%d,%s,%s,%.2f\n", clientes[i].id, clientes[i].nome,
                    clientes[i].telefone, clientes[i].valor_gasto);
        }

        fclose(arquivoClientes);

        
        FILE *arquivoCarros = fopen(nomeArquivoCarros, "w");
        if (arquivoCarros == NULL)
        {
            printf("Erro ao abrir o arquivo de carros para atualizacao.\n");
            return;
        }

        
        for (int i = 0; i < numCarros; i++)
        {
            fprintf(arquivoCarros, "%d,%s,%.2f,%d\n", carros[i].id, carros[i].modelo,
                    carros[i].preco, carros[i].quantidade_estoque);
        }

        fclose(arquivoCarros);

        printf("Venda realizada com sucesso!\n");
    }
    else
    {
        printf("ID de cliente ou carro invalido.\n");
    }
}


int lerClientes(Cliente clientes[], const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Arquivo nao encontrado ou vazio.\n");
        return 0;
    }

    int numClientes = 0;

    while (fscanf(arquivo, "%d,%49[^,],%19[^,],%f\n", &clientes[numClientes].id,
                  clientes[numClientes].nome, clientes[numClientes].telefone, &clientes[numClientes].valor_gasto) == 4)
    {
        numClientes++;
        if (numClientes >= MAX_CLIENTES)
        {
            printf("Limite maximo de clientes atingido.\n");
            break;
        }
    }

    fclose(arquivo);
    return numClientes;
}

int lerCarros(Carro carros[], const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Arquivo nao encontrado ou vazio.\n");
        return 0;
    }

    int numCarros = 0;

    while (fscanf(arquivo, "%d,%49[^,],%f,%d\n", &carros[numCarros].id,
                  carros[numCarros].modelo, &carros[numCarros].preco, &carros[numCarros].quantidade_estoque) == 4)
    {
        numCarros++;
        if (numCarros >= MAX_CARROS)
        {
            printf("Limite maximo de carros atingido.\n");
            break;
        }
    }

    fclose(arquivo);
    return numCarros;
}

int lerVendas(Venda vendas[], const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL)
    {
        printf("Arquivo nao encontrado ou vazio.\n");
        return 0;
    }

    int numVendas = 0;

    while (fscanf(arquivo, "%d,%d,%d,%d-%d-%d %d:%d:%d\n", &vendas[numVendas].id,
                  &vendas[numVendas].idCliente, &vendas[numVendas].idCarro,
                  &vendas[numVendas].dataHora.tm_year, &vendas[numVendas].dataHora.tm_mon,
                  &vendas[numVendas].dataHora.tm_mday, &vendas[numVendas].dataHora.tm_hour,
                  &vendas[numVendas].dataHora.tm_min, &vendas[numVendas].dataHora.tm_sec) == 9)
    {
        numVendas++;
        if (numVendas >= MAX_VENDAS)
        {
            printf("Limite maximo de vendas atingido.\n");
            break;
        }
    }

    fclose(arquivo);
    return numVendas;
}
int main()
{
    Carro carros[100];
    Venda vendas[100];
    Cliente clientes[MAX_CLIENTES];

    int numCarros = 0;
    int numClientes = 0;
    int numVendas = 0;

    int opcao;

    numClientes = lerClientes(clientes, "clientes.txt");
    numCarros = lerCarros(carros, "carros.txt");
    numVendas = lerVendas(vendas, "vendas.txt");

    do
    {
        
        printf("\nMenu:\n");
        printf("1. Adicionar Carro\n");
        printf("2. Cadastrar Cliente\n");
        printf("3. Listar Clientes\n");
        printf("4. Realizar Venda\n");
        printf("5. Listar Vendas\n");
        printf("6. Listar Carros\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

       
        switch (opcao)
        {
        case 1:
            adicionarCarro(carros, &numCarros, "carros.txt");
            break;
        case 2:
            cadastrarCliente(clientes, &numClientes, "clientes.txt");
            break;
        case 3:
            listarClientes(clientes, numClientes);
            break;
        case 4:
            realizarVenda(vendas, &numVendas, clientes, numClientes, carros, numCarros, "vendas.txt", "clientes.txt", "carros.txt");
            break;
        case 5:
            listarVendas(vendas, numVendas, clientes, numClientes, carros, numCarros);
            break;
        case 6:
            listarCarros(carros, numCarros);
            break;
        case 7:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 7);

    return 0;
}
