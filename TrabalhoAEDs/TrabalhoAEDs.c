#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTES 100
#define MAX_FUNCIONARIOS 50
#define MAX_ESTADIAS 200
#define MAX_QUARTOS 50


typedef struct {
  int codigo;
  char nome[50];
  char endereco[100];
  char telefone[15];
} Cliente;

typedef struct {
  int codigo;
  char nome[50];
  char telefone[15];
  char cargo[30];
  float salario;
} Funcionario;

typedef struct {
  int numero;
  int qtdHospedes;
  float valorDiaria;
  char status[20];
} Quarto;

typedef struct {
  int codigo;
  char dataEntrada[11];
  char dataSaida[11];
  int qtdDiarias;
  int codigoCliente;
  int numeroQuarto;
  float valorTotal;
} Estadia;

void cadastrarCliente(Cliente clientes[], int *qtdClientes) {
  if (*qtdClientes >= MAX_CLIENTES) {
    printf("\tLimite de clientes atingido.\n");
    return;
  }

  Cliente novoCliente;
  novoCliente.codigo = *qtdClientes + 1;
  printf("\t__________________________\n");
  printf("\tCadastro de Cliente\n");
  printf("\t__________________________\n");
  printf("\tNome: ");
  scanf(" %[^\n]", novoCliente.nome);
  printf("\tEndereco: ");
  scanf(" %[^\n]", novoCliente.endereco);
  printf("\tTelefone: ");
  scanf(" %[^\n]", novoCliente.telefone);

  clientes[*qtdClientes] = novoCliente;
  (*qtdClientes)++;

  printf("\tCliente cadastrado com sucesso!\n\tCodigo do Cliente: %d\n\n",
         novoCliente.codigo);
}

void cadastrarFuncionario(Funcionario funcionarios[], int *qtdFuncionarios) {
  if (*qtdFuncionarios >= MAX_FUNCIONARIOS) {
    printf("\tLimite de funcionarios atingido.\n");
    return;
  }

  Funcionario novoFuncionario;
  novoFuncionario.codigo = *qtdFuncionarios + 1;
  printf("\t__________________________\n");
  printf("\tCadastro de Funcionario\n");
  printf("\t__________________________\n");
  printf("\tNome: ");
  fflush(stdout);
  scanf(" %[^\n]", novoFuncionario.nome);
  printf("\tTelefone: ");
  fflush(stdout);
  scanf(" %[^\n]", novoFuncionario.telefone);
  printf("\tCargo (Recepcionista, Auxiliar de limpeza, Garçom e Gerente): ");
  fflush(stdout);
  scanf(" %[^\n]", novoFuncionario.cargo);
  printf("\tSalario: ");
  fflush(stdout);
  scanf("%f", &novoFuncionario.salario);

  funcionarios[*qtdFuncionarios] = novoFuncionario;
  (*qtdFuncionarios)++;

  printf("\tFuncionario cadastrado com sucesso!\n\tCodigo do Funcionario: %d\n",
         novoFuncionario.codigo);
}

void salvarQuartoEmArquivo(Quarto quartos[], int qtdQuartos, int numeroQuarto,
                           int codigoEstadia) {
  FILE *arquivo;
  arquivo = fopen("quarto.txt", "a");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo quarto.txt.\n");
    return;
  }

  for (int i = 0; i < qtdQuartos; i++) {
    if (quartos[i].numero == numeroQuarto) {
      fprintf(arquivo, "QUARTO = %d, %d, %.2f, %s, %d\n", quartos[i].numero,
              quartos[i].qtdHospedes, quartos[i].valorDiaria, quartos[i].status,
              codigoEstadia);
      break;
    }
  }

  fclose(arquivo);
}

void cadastrarEstadia(Estadia estadias[], int *qtdEstadias, Cliente clientes[],
                      int qtdClientes, Quarto quartos[], int qtdQuartos) {
  if (*qtdEstadias >= MAX_ESTADIAS) {
    printf("\tLimite de estadias atingido.\n");
    return;
  }

  Estadia novaEstadia;
  novaEstadia.codigo = *qtdEstadias + 1;
  printf("\t__________________________\n");
  printf("\tCadastro de Estadia\n");
  printf("\t__________________________\n");
  printf("\tCodigo do cliente: ");
  fflush(stdout);
  scanf("%d", &novaEstadia.codigoCliente);

  int clienteExiste = 0;
  for (int i = 0; i < qtdClientes; i++) {
    if (clientes[i].codigo == novaEstadia.codigoCliente) {
      clienteExiste = 1;
      break;
    }
  }

  if (!clienteExiste) {
    printf("\tCliente nao encontrado.\n");
    return;
  }

  int qtdHospedes;
  printf("\tQuantidade de hospedes: ");
  fflush(stdout);
  scanf("%d", &qtdHospedes);

  printf("\tData de entrada (dia/mês/ano): ");
  fflush(stdout);
  scanf(" %[^\n]", novaEstadia.dataEntrada);
  printf("\tData de saida (dia/mês/ano): ");
  fflush(stdout);
  scanf(" %[^\n]", novaEstadia.dataSaida);


  struct tm dataEnt = {0}, dataSai = {0};
  fflush(stdout);
  sscanf(novaEstadia.dataEntrada, "%d/%d/%d", &dataEnt.tm_mday, &dataEnt.tm_mon,
         &dataEnt.tm_year);
  fflush(stdout);
  sscanf(novaEstadia.dataSaida, "%d/%d/%d", &dataSai.tm_mday, &dataSai.tm_mon,
         &dataSai.tm_year);
  dataEnt.tm_mon -= 1;
  dataEnt.tm_year -= 1900;
  dataSai.tm_mon -= 1;
  dataSai.tm_year -= 1900;
  time_t entrada = mktime(&dataEnt);
  time_t saida = mktime(&dataSai);


  if (saida <= entrada) {
    printf("\tData de saída deve ser posterior à data de entrada.\n");
    return;
  }

  double diff = difftime(saida, entrada);
  novaEstadia.qtdDiarias = (int)(diff / (60 * 60 * 24));


  printf("\tQuartos disponíveis:\n");
  for (int i = 0; i < qtdQuartos; i++) {
    if (strcmp(quartos[i].status, "desocupado") == 0 &&
        quartos[i].qtdHospedes >= qtdHospedes) {
      printf("\tQuarto %d: Capacidade %d pessoas, Diária R$%.2f\n",
             quartos[i].numero, quartos[i].qtdHospedes, quartos[i].valorDiaria);
    }
  }

  int numeroQuarto;
  printf("\tEscolha o numero do quarto desejado: ");
  fflush(stdout);
  scanf("%d", &numeroQuarto);

  int quartoEncontrado = 0;
  for (int i = 0; i < qtdQuartos; i++) {
    if (quartos[i].numero == numeroQuarto &&
        strcmp(quartos[i].status, "desocupado") == 0 &&
        quartos[i].qtdHospedes >= qtdHospedes) {
      novaEstadia.numeroQuarto = quartos[i].numero;
      strcpy(quartos[i].status, "ocupado");
      quartoEncontrado = 1;
      break;
    }
  }

  if (!quartoEncontrado) {
    printf("\tQuarto nao disponivel ou nao atende a quantidade de hospedes ""desejada.\n");
    return;
  }

  novaEstadia.valorTotal = novaEstadia.qtdDiarias *
                           quartos[novaEstadia.numeroQuarto - 1].valorDiaria;

  estadias[*qtdEstadias] = novaEstadia;
  (*qtdEstadias)++;

  salvarQuartoEmArquivo(quartos, qtdQuartos, novaEstadia.numeroQuarto,
                        novaEstadia.codigo);

  printf("\t__________________________\n");
  printf("\tEstadia cadastrada com sucesso!\n");
  printf("\tCodigo da Estadia: %d\n", novaEstadia.codigo);
  printf("\tCodigo do Cliente: %d\n", novaEstadia.codigoCliente);
  printf("\tNumero do Quarto: %d\n", novaEstadia.numeroQuarto);
  printf("\tValor Total: %.2f\n", novaEstadia.valorTotal);
}

void darBaixaEstadia(Estadia estadias[], int *qtdEstadias, Quarto quartos[],
                     int qtdQuartos) {
  int codigoEstadia;
  printf("\t__________________________\n");
  printf("\tBaixa de Estadia\n");
  printf("\t__________________________\n");
  printf("\tDigite o codigo da estadia: ");
  fflush(stdout);
  scanf("%d", &codigoEstadia);

  for (int i = 0; i < *qtdEstadias; i++) {
    if (estadias[i].codigo == codigoEstadia) {
      printf("\tEstadia encontrada:\n");
      printf("\tCodigo: %d\n", estadias[i].codigo);
      printf("\tCodigo do Cliente: %d\n", estadias[i].codigoCliente);
      printf("\tNumero do Quarto: %d\n", estadias[i].numeroQuarto);
      printf("\tData de Entrada: %s\n", estadias[i].dataEntrada);
      printf("\tData de Saida: %s\n", estadias[i].dataSaida);
      printf("\tQuantidade de Diarias: %d\n", estadias[i].qtdDiarias);
      printf("\tValor Total: %.2f\n", estadias[i].valorTotal);


      for (int j = 0; j < qtdQuartos; j++) {
        if (quartos[j].numero == estadias[i].numeroQuarto) {
          strcpy(quartos[j].status, "desocupado");
          break;
        }
      }


      FILE *arquivoTemp = fopen("quarto_temp.txt", "w");
      FILE *arquivoQuarto = fopen("quarto.txt", "r");
      if (arquivoQuarto == NULL || arquivoTemp == NULL) {
        printf("Erro ao abrir o arquivo quarto.txt.\n");
        return;
      }

      char linha[100];
      while (fgets(linha, sizeof(linha), arquivoQuarto)) {
        int numero;
        fflush(stdout);
        sscanf(linha, "QUARTO = %d", &numero);
        if (numero != estadias[i].numeroQuarto) {
          fputs(linha, arquivoTemp);
        }
      }

      fclose(arquivoQuarto);
      fclose(arquivoTemp);

      remove("quarto.txt");
      rename("quarto_temp.txt", "quarto.txt");


      FILE *arquivoEstadias = fopen("estadias.txt", "w");
      if (arquivoEstadias == NULL) {
        printf("Erro ao abrir o arquivo estadias.txt.\n");
        return;
      }

      for (int j = 0; j < *qtdEstadias; j++) {
        if (estadias[j].codigo != codigoEstadia) {
          fprintf(arquivoEstadias, "%d,%s,%s,%d,%d,%d,%.2f\n",
                  estadias[j].codigo, estadias[j].dataEntrada,
                  estadias[j].dataSaida, estadias[j].qtdDiarias,
                  estadias[j].codigoCliente, estadias[j].numeroQuarto,
                  estadias[j].valorTotal);
        }
      }

      fclose(arquivoEstadias);

      printf("\t__________________________\n");
      printf("\tEstadia dada baixa com sucesso!\n");
      printf("\tValor Total a ser pago: %.2f\n", estadias[i].valorTotal);

      for (int j = i; j < *qtdEstadias - 1; j++) {
        estadias[j] = estadias[j + 1];
      }
      (*qtdEstadias)--;
      return;
    }
  }

  printf("\tEstadia nao encontrada.\n");
}

void mostrarEstadiasCliente(Estadia estadias[], int qtdEstadias,
                            Cliente clientes[], int qtdClientes) {
  int codigoCliente;
  printf("\t__________________________\n");
  printf("\tEstadias de um Cliente\n");
  printf("\t__________________________\n");
  printf("\tDigite o codigo do cliente: ");
  fflush(stdout);
  scanf("%d", &codigoCliente);

  int clienteEncontrado = 0;
  for (int i = 0; i < qtdClientes; i++) {
    if (clientes[i].codigo == codigoCliente) {
      clienteEncontrado = 1;
      break;
    }
  }

  if (!clienteEncontrado) {
    printf("\tCliente nao encontrado.\n");
    return;
  }

  int estadiasEncontradas = 0;
  for (int i = 0; i < qtdEstadias; i++) {
    if (estadias[i].codigoCliente == codigoCliente) {
      if (!estadiasEncontradas) {
        printf("\tEstadias do Cliente com codigo %d:\n", codigoCliente);
      }
      printf("\t__________________________\n");
      printf("\tCodigo da Estadia: %d\n", estadias[i].codigo);
      printf("\tNumero do Quarto: %d\n", estadias[i].numeroQuarto);
      printf("\tData de Entrada: %s\n", estadias[i].dataEntrada);
      printf("\tData de Saida: %s\n", estadias[i].dataSaida);
      printf("\tQuantidade de Diarias: %d\n", estadias[i].qtdDiarias);
      printf("\tValor Total: %.2f\n", estadias[i].valorTotal);
      estadiasEncontradas++;
    }
  }

  if (estadiasEncontradas == 0) {
    printf("\tNao existem estadias cadastradas para este cliente.\n");
  }

  printf("\t__________________________\n");
}

void pesquisarCliente(Cliente clientes[], int qtdClientes) {
  int codigo;
  char nome[50];
  int opcao;
  printf("\t__________________________\n");
  printf("\tPesquisa de Cliente\n");
  printf("\t__________________________\n");
  printf("\tPesquisar por:\n1. Codigo\n2. Nome\nEscolha uma opcao: ");
  fflush(stdout);
  scanf("%d", &opcao);

  if (opcao == 1) {
    printf("\tDigite o codigo do cliente: ");
    fflush(stdout);
    scanf("%d", &codigo);
    for (int i = 0; i < qtdClientes; i++) {
      if (clientes[i].codigo == codigo) {
        printf("__________________________\n");
        printf("\tCliente encontrado:\n");
        printf("\tCodigo: %d\n", clientes[i].codigo);
        printf("\tNome: %s\n", clientes[i].nome);
        printf("\tEndereco: %s\n", clientes[i].endereco);
        printf("\tTelefone: %s\n", clientes[i].telefone);
        return;
      }
    }
  } else if (opcao == 2) {
    printf("\tDigite o nome do cliente: ");
    fflush(stdout);
    scanf(" %[^\n]", nome);
    for (int i = 0; i < qtdClientes; i++) {
      if (strcmp(clientes[i].nome, nome) == 0) {
        printf("__________________________\n");
        printf("\tCliente encontrado:\n");
        printf("\tCodigo: %d\n", clientes[i].codigo);
        printf("\tNome: %s\n", clientes[i].nome);
        printf("\tEndereco: %s\n", clientes[i].endereco);
        printf("\tTelefone: %s\n", clientes[i].telefone);
        return;
      }
    }
  } else {
    printf("\tOpcao invalida!\n");
  }

  printf("\tCliente nao encontrado.\n");
}

void pesquisarFuncionario(Funcionario funcionarios[], int qtdFuncionarios) {
  int codigo;
  char nome[50];
  int opcao;

  printf("\t__________________________\n");
  printf("\tPesquisa de Funcionario\n");
  printf("\t__________________________\n");
  printf("\tPesquisar por:\n1. Codigo\n2. Nome\nEscolha uma opcao: ");
  fflush(stdout);
  scanf("%d", &opcao);

  if (opcao == 1) {
    printf("\tDigite o codigo do funcionario: ");
    fflush(stdout);
    scanf("%d", &codigo);
    for (int i = 0; i < qtdFuncionarios; i++) {
      if (funcionarios[i].codigo == codigo) {
        printf("__________________________\n");
        printf("\tFuncionario encontrado:\n");
        printf("\tCodigo: %d\n", funcionarios[i].codigo);
        printf("\tNome: %s\n", funcionarios[i].nome);
        printf("\tTelefone: %s\n", funcionarios[i].telefone);
        printf("\tCargo: %s\n", funcionarios[i].cargo);
        printf("\tSalario: %.2f\n", funcionarios[i].salario);
        return;
      }
    }
  } else if (opcao == 2) {
    printf("\tDigite o nome do funcionario: ");
    fflush(stdout);
    scanf(" %[^\n]", nome);
    for (int i = 0; i < qtdFuncionarios; i++) {
      if (strcmp(funcionarios[i].nome, nome) == 0) {
        printf("\t__________________________\n");
        printf("\tFuncionario encontrado:\n");
        printf("\tCodigo: %d\n", funcionarios[i].codigo);
        printf("\tNome: %s\n", funcionarios[i].nome);
        printf("\tTelefone: %s\n", funcionarios[i].telefone);
        printf("\tCargo: %s\n", funcionarios[i].cargo);
        printf("\tSalario: %.2f\n", funcionarios[i].salario);
        return;
      }
    }
  } else {
    printf("\tOpcao invalida!\n");
  }

  printf("\tFuncionario nao encontrado.\n");
}

void pesquisarEstadia(Estadia estadias[], int qtdEstadias) {
  int codigo;

  printf("\t__________________________\n");
  printf("\tPesquisa de Estadia\n");
  printf("\t__________________________\n");
  printf("\tDigite o codigo da estadia: ");
  fflush(stdout);
  scanf("%d", &codigo);

  for (int i = 0; i < qtdEstadias; i++) {
    if (estadias[i].codigo == codigo) {
      printf("\t__________________________\n");
      printf("\tEstadia encontrada:\n");
      printf("\tCodigo: %d\n", estadias[i].codigo);
      printf("\tCodigo do Cliente: %d\n", estadias[i].codigoCliente);
      printf("\tNumero do Quarto: %d\n", estadias[i].numeroQuarto);
      printf("\tData de Entrada: %s\n", estadias[i].dataEntrada);
      printf("\tData de Saida: %s\n", estadias[i].dataSaida);
      printf("\tQuantidade de Diarias: %d\n", estadias[i].qtdDiarias);
      printf("\tValor Total: %.2f\n", estadias[i].valorTotal);
      return;
    }
  }

  printf("\tEstadia nao encontrada.\n");
}

void salvarDados(Cliente clientes[], int qtdClientes,
                 Funcionario funcionarios[], int qtdFuncionarios,
                 Estadia estadias[], int qtdEstadias) {
  FILE *fileClientes = fopen("clientes.txt", "w");
  FILE *fileFuncionarios = fopen("funcionarios.txt", "w");
  FILE *fileEstadias = fopen("estadias.txt", "w");

  if (fileClientes == NULL || fileFuncionarios == NULL ||
      fileEstadias == NULL) {
    printf("\tErro ao abrir os arquivos para escrita.\n");
    return;
  }

  for (int i = 0; i < qtdClientes; i++) {
    fprintf(fileClientes, "%d,%s,%s,%s\n", clientes[i].codigo, clientes[i].nome,
            clientes[i].endereco, clientes[i].telefone);
  }

  for (int i = 0; i < qtdFuncionarios; i++) {
    fprintf(fileFuncionarios, "%d,%s,%s,%s,%.2f\n", funcionarios[i].codigo,
            funcionarios[i].nome, funcionarios[i].telefone,
            funcionarios[i].cargo, funcionarios[i].salario);
  }

  for (int i = 0; i < qtdEstadias; i++) {
    fprintf(fileEstadias, "%d,%s,%s,%d,%d,%d,%.2f\n", estadias[i].codigo,
            estadias[i].dataEntrada, estadias[i].dataSaida,
            estadias[i].qtdDiarias, estadias[i].codigoCliente,
            estadias[i].numeroQuarto, estadias[i].valorTotal);
  }

  fclose(fileClientes);
  fclose(fileFuncionarios);
  fclose(fileEstadias);

}

void carregarDados(Cliente clientes[], int *qtdClientes,
                   Funcionario funcionarios[], int *qtdFuncionarios,
                   Estadia estadias[], int *qtdEstadias) {
  FILE *fileClientes = fopen("clientes.txt", "r");
  FILE *fileFuncionarios = fopen("funcionarios.txt", "r");
  FILE *fileEstadias = fopen("estadias.txt", "r");

  if (fileClientes == NULL || fileFuncionarios == NULL ||
      fileEstadias == NULL) {

    return;
  }

  *qtdClientes = 0;
  *qtdFuncionarios = 0;
  *qtdEstadias = 0;

  while (fscanf(fileClientes, "%d,%[^,],%[^,],%[^\n]\n",
                &clientes[*qtdClientes].codigo, clientes[*qtdClientes].nome,
                clientes[*qtdClientes].endereco,
                clientes[*qtdClientes].telefone) != EOF) {
    (*qtdClientes)++;
  }

  while (fscanf(fileFuncionarios, "%d,%[^,],%[^,],%[^,],%f\n",
                &funcionarios[*qtdFuncionarios].codigo,
                funcionarios[*qtdFuncionarios].nome,
                funcionarios[*qtdFuncionarios].telefone,
                funcionarios[*qtdFuncionarios].cargo,
                &funcionarios[*qtdFuncionarios].salario) != EOF) {
    (*qtdFuncionarios)++;
  }

  while (fscanf(fileEstadias, "%d,%[^,],%[^,],%d,%d,%d,%f\n",
                &estadias[*qtdEstadias].codigo,
                estadias[*qtdEstadias].dataEntrada,
                estadias[*qtdEstadias].dataSaida,
                &estadias[*qtdEstadias].qtdDiarias,
                &estadias[*qtdEstadias].codigoCliente,
                &estadias[*qtdEstadias].numeroQuarto,
                &estadias[*qtdEstadias].valorTotal) != EOF) {
    (*qtdEstadias)++;
  }

  fclose(fileClientes);
  fclose(fileFuncionarios);
  fclose(fileEstadias);

  ;
}

int main() {
  Cliente clientes[MAX_CLIENTES];
  Funcionario funcionarios[MAX_FUNCIONARIOS];
  Quarto quartos[MAX_QUARTOS] = {{1, 2, 100.0, "desocupado"},
                                 {2, 4, 200.0, "desocupado"},
                                 {3, 1, 150.0, "desocupado"},
                                 {4, 3, 180.0, "desocupado"},
                                 {5, 2, 120.0, "desocupado"}};
  Estadia estadias[MAX_ESTADIAS];

  int qtdClientes = 0;
  int qtdFuncionarios = 0;
  int qtdEstadias = 0;

  carregarDados(clientes, &qtdClientes, funcionarios, &qtdFuncionarios,
                estadias, &qtdEstadias);

  int opcao;

  do {
    printf("\t__________________________\n");
    printf("\tHotel Descanso Garantido\n");
    printf("\t__________________________\n");
    printf("\t1. Cadastrar Cliente\n");
    printf("\t2. Cadastrar Funcionario\n");
    printf("\t3. Cadastrar Estadia\n");
    printf("\t4. Pesquisar Cliente\n");
    printf("\t5. Pesquisar Funcionario\n");
    printf("\t6. Pesquisar Estadia\n");
    printf("\t7. Dar Baixa em Estadia\n");
    printf("\t8. Mostrar Estadias de Cliente\n");
    printf("\t9. Salvar e Sair\n");
    printf("\n");
    printf("\tEscolha uma opcao: ");

    while (scanf("%d", &opcao) != 1) {
      printf("\tEntrada inválida. Por favor, insira um numero.\n");
      while (getchar() != '\n')

      printf("\tEscolha uma opção: ");
    }

    switch (opcao) {
    case 1:
      cadastrarCliente(clientes, &qtdClientes);
      salvarDados(clientes, qtdClientes, funcionarios, qtdFuncionarios,
                  estadias, qtdEstadias);
      break;
    case 2:
      cadastrarFuncionario(funcionarios, &qtdFuncionarios);
      salvarDados(clientes, qtdClientes, funcionarios, qtdFuncionarios,
                  estadias, qtdEstadias);
      break;
    case 3:
      cadastrarEstadia(estadias, &qtdEstadias, clientes, qtdClientes, quartos,
                       MAX_QUARTOS);
      salvarDados(clientes, qtdClientes, funcionarios, qtdFuncionarios,
                  estadias, qtdEstadias);
      break;
    case 4:
      pesquisarCliente(clientes, qtdClientes);
      break;
    case 5:
      pesquisarFuncionario(funcionarios, qtdFuncionarios);
      break;
    case 6:
      pesquisarEstadia(estadias, qtdEstadias);
      break;
    case 7:
      darBaixaEstadia(estadias, &qtdEstadias, quartos, MAX_QUARTOS);
      salvarDados(clientes, qtdClientes, funcionarios, qtdFuncionarios,
                  estadias, qtdEstadias);
      break;
    case 8:
      mostrarEstadiasCliente(estadias, qtdEstadias, clientes, qtdClientes);
      break;
    case 9:
      salvarDados(clientes, qtdClientes, funcionarios, qtdFuncionarios,
                  estadias, qtdEstadias);
      printf("\tSaindo...\n");
      break;
    default:
      printf("\tOpção inválida! Tente novamente.\n");
    }
  } while (opcao != 9);

  return 0;
}
