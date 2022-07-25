#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Exemplo de rede neural com duas camadas de neurônios
// uma camada oculta e uma de saída

//-------------------------------------------------------------------
// constantes para parametrização do algoritmo
//-------------------------------------------------------------------
#define numeroEntradas 4 // define o número de entradas da rede neural
#define numeroSaidas 3 // define o número de saídas da rede
#define numeroNeuroniosCamadaOculta 16 // define o número de neurônios na camada oculta
#define numeroAmostrasTreinamento 16 // define o número de amostra de dados para treinamento
#define taxaAprendizado 0.5 // define a taxa de aprendizado para a rede

//-------------------------------------------------------------------
// variáveis globais
//-------------------------------------------------------------------
// dados para treinamento da rede
double amostrasTreinamento[numeroAmostrasTreinamento][numeroEntradas]; // matriz que contém os dados de entrada para treinamento da rede
double saidasTreinamento[numeroAmostrasTreinamento][numeroSaidas]; // vetor que contém os dados de saida para treinamento da rede

// pesos das sinapses
double pesosCamadaOculta[numeroEntradas][numeroNeuroniosCamadaOculta];
double novosPesosCamadaOculta[numeroEntradas][numeroNeuroniosCamadaOculta];
double pesosLimiarCamadaOculta[numeroNeuroniosCamadaOculta];
double novosPesosLimiarCamadaOculta[numeroNeuroniosCamadaOculta];
double pesosCamadaSaida[numeroNeuroniosCamadaOculta][numeroSaidas];
double novosPesosCamadaSaida[numeroNeuroniosCamadaOculta][numeroSaidas];
double pesosLimiarCamadaSaida[numeroSaidas];
double novosPesosLimiarCamadaSaida[numeroSaidas];

// somatórios das sinapses de cada neurônio
double somaNeuronioCamadaOculta[numeroNeuroniosCamadaOculta];
double somaNeuronioCamadaSaida[numeroSaidas];

// saídas de cada neurônio em cada camada
double saidaNeuronioCamadaOculta[numeroNeuroniosCamadaOculta];
double saidaNeuronioSaida[numeroSaidas];

// entradas da rede neural
double entradas[numeroEntradas];

// saída desejada para a entrada atual
double saidasDesejadas[numeroSaidas];

// erro de cada saída com relação a amostra
//double erroNeuronioCamadaOculta[numeroNeuroniosCamadaOculta];
double erroSaida[numeroSaidas];
// erro total da amostra
double erroTotal;

//-------------------------------------------------------------------
// Declaração da funções
//-------------------------------------------------------------------
// calcula um valor aleatório entre 0 e 1
double aleatorio()
{
  return((double)rand()/RAND_MAX);
}

int inteiroAleatorio(int maximo)
{
  return(rand() % (maximo+1));
}

// calcula a sigmoide de um valor
double sigmoid(double x)
{
  return(1.0 / (1.0 + exp(-x)));
}

// calcula os valores de saída da camada oculta
void calculaSaidaCamadaOculta()
{
  int contEntradas; // conta as entradas de cada neurônio
  int contNeuronios; // conta os neurônio da camada oculta
  double somatorio; // armazena o somatório das entradas de cada neurônio
  for(contNeuronios=0; contNeuronios<numeroNeuroniosCamadaOculta; contNeuronios++)
  {
    //conta os neuronios da camada oculta
    somatorio=0.0; //inicia o somatório das entradas do neurônio com 0.0
    for(contEntradas=0; contEntradas<numeroEntradas; contEntradas++)
    {
      //conta as entradas do neurônio
      //fas o somatório dos produtos das entradas pelos pesos das sinapses do neurônio
      somatorio=somatorio+(entradas[contEntradas]*pesosCamadaOculta[contEntradas][contNeuronios]);
    }
    // soma a entrada de limiar do neurônio
    somatorio=somatorio+pesosLimiarCamadaOculta[contNeuronios];
    // armazena o somatório para apresentar na tela
    somaNeuronioCamadaOculta[contNeuronios]=somatorio;
    // aplica a função de ativação no somatório a armazena o serultado na saída do neurônio
    saidaNeuronioCamadaOculta[contNeuronios]=sigmoid(somatorio);
  }
}                                                                                                                               

// calcula os valores de saída da camada de saida
void calculaSaidaCamadaSaida()
{
  int contSaidas; // conta as entradas de cada neurônio
  int contSinapses; // conta as sinapses de entrada de cada neuronio da saída
  double somatorio; // armazena o somatório das entradas de cada neurônio
  for(contSaidas=0; contSaidas<numeroSaidas; contSaidas++)
  {
    //conta os neuronios da camada de saida
    somatorio=0.0; //inicia o somatório das entradas do neurônio com 0.0
    for(contSinapses=0; contSinapses<numeroNeuroniosCamadaOculta; contSinapses++)
    {
      //conta as entradas de cada neuronio da saída
      //fas o somatório dos produtos das entradas pelos pesos das sinapses do neurônio
      somatorio=somatorio+(saidaNeuronioCamadaOculta[contSinapses]*pesosCamadaSaida[contSinapses][contSaidas]);
    }
    // soma a entrada de limiar do neurônio
    somatorio=somatorio+pesosLimiarCamadaSaida[contSaidas];
    // armazena o somatório para apresentar na tela
    somaNeuronioCamadaSaida[contSaidas]=somatorio;
    // aplica a função de ativação no somatório a armazena o serultado na saída do neurônio
    saidaNeuronioSaida[contSaidas]=sigmoid(somatorio);
  }
}

// Imprime Todos os Valores da rede
void imprimeTodosValores()
{
  int contSaidas; // conta as saídas
  int contEntradas; // conta as entradas de cada neurônio
  int contSinapses; // conta as sinapses de entrada de cada neuronio da saída
  int contNeuronios; // conta os neurônio da camada oculta
  // imprime entradas
  printf("-------------------------------------------------------------\n");
  printf("Entradas\n");
  for(contEntradas=0; contEntradas<numeroEntradas; contEntradas++)
  {
    printf("\tEntrada %d = %f\n",contEntradas,entradas[contEntradas]);
  }
  // imprime saidas desejadas
  printf("Saidas desejadas\n");
  for(contSaidas=0; contSaidas<numeroSaidas; contSaidas++)
  {
    printf("\tsaida Desejada %d = %f\n",contSaidas,saidasDesejadas[contSaidas]);
  }
  // imprime valores da camada oculta
  printf("\nCamada Oculta\n");
  for(contNeuronios=0; contNeuronios<numeroNeuroniosCamadaOculta; contNeuronios++)
  {
    printf("Neuronio %d pesos (",contNeuronios);
    for(contEntradas=0; contEntradas<numeroEntradas; contEntradas++)
    {
      printf(" %f",pesosCamadaOculta[contEntradas][contNeuronios]);
    }
  printf(" <%f>",pesosLimiarCamadaOculta[contNeuronios]);
  printf(")\n\t somatorio = %f",somaNeuronioCamadaOculta[contNeuronios]);
  printf(" saida = %f\n",saidaNeuronioCamadaOculta[contNeuronios]);
  }
  // imprime valores da camada de saída
  printf("\nCamada saida\n");
  for(contSaidas=0; contSaidas<numeroSaidas; contSaidas++)
  {
    printf("Neuronio %d pesos (",contSaidas);
    for(contSinapses=0; contSinapses<numeroNeuroniosCamadaOculta; contSinapses++)
    {
      printf(" %f",pesosCamadaSaida[contSinapses][contSaidas]);
    }
    printf(" <%f>",pesosLimiarCamadaSaida[contSaidas]);
    printf(")\n\t somatorio = %f",somaNeuronioCamadaSaida[contSaidas]);
    printf(" <{saida = %f}>",saidaNeuronioSaida[contSaidas]);
    printf(" erro = %f\n",erroSaida[contSaidas]);
  }
  printf("ErroTotal = %f\n",erroTotal);
}

// inicializa os pesos com valores aleatórios
void inicializaPesos()
{
  // inicializa todos os pesos com valores entre -0.5 e 0.5
  int contSaidas; // conta as entradas de cada neurônio
  int contEntradas; // conta as entradas de cada neurônio
  int contSinapses; // conta as sinapses de entrada de cada neuronio da saída
  int contNeuronios; // conta os neurônio da camada oculta
  for(contNeuronios=0; contNeuronios<numeroNeuroniosCamadaOculta; contNeuronios++)
  {
    for(contEntradas=0; contEntradas<numeroEntradas; contEntradas++)
    {
      pesosCamadaOculta[contEntradas][contNeuronios]=aleatorio()-0.5;
    }
    pesosLimiarCamadaOculta[contNeuronios]=aleatorio()-0.5;
  }
  for(contSaidas=0; contSaidas<numeroSaidas; contSaidas++)
  {
    for(contSinapses=0; contSinapses<numeroNeuroniosCamadaOculta; contSinapses++)
    {
      pesosCamadaSaida[contSinapses][contSaidas]=aleatorio()-0.5;
    }
    pesosLimiarCamadaSaida[contSaidas]=aleatorio()-0.5;
  }
}

// inicializa dado de treinamento
void inicializaDadosTreinamento()
{
  amostrasTreinamento[0][0]=0.0;
  amostrasTreinamento[0][1]=0.0;
  amostrasTreinamento[0][2]=0.0;
  amostrasTreinamento[0][3]=0.0;
  saidasTreinamento[0][0]=1.0;
  saidasTreinamento[0][1]=0.0;
  saidasTreinamento[0][2]=1.0;

  amostrasTreinamento[1][0]=0.0;
  amostrasTreinamento[1][1]=0.0;
  amostrasTreinamento[1][2]=0.0;
  amostrasTreinamento[1][3]=1.0;
  saidasTreinamento[1][0]=1.0;
  saidasTreinamento[1][1]=0.0;
  saidasTreinamento[1][2]=1.0;

  amostrasTreinamento[2][0]=0.0;
  amostrasTreinamento[2][1]=0.0;
  amostrasTreinamento[2][2]=1.0;
  amostrasTreinamento[2][3]=0.0;
  saidasTreinamento[2][0]=1.0;
  saidasTreinamento[2][1]=0.0;
  saidasTreinamento[2][2]=0.0;

  amostrasTreinamento[3][0]=0.0;
  amostrasTreinamento[3][1]=0.0;
  amostrasTreinamento[3][2]=1.0;
  amostrasTreinamento[3][3]=1.0;
  saidasTreinamento[3][0]=1.0;
  saidasTreinamento[3][1]=0.0;
  saidasTreinamento[3][2]=1.0;

  amostrasTreinamento[4][0]=0.0;
  amostrasTreinamento[4][1]=1.0;
  amostrasTreinamento[4][2]=0.0;
  amostrasTreinamento[4][3]=0.0;
  saidasTreinamento[4][0]=1.0;
  saidasTreinamento[4][1]=0.0;
  saidasTreinamento[4][2]=0.0;

  amostrasTreinamento[5][0]=0.0;
  amostrasTreinamento[5][1]=1.0;
  amostrasTreinamento[5][2]=0.0;
  amostrasTreinamento[5][3]=1.0;
  saidasTreinamento[5][0]=1.0;
  saidasTreinamento[5][1]=0.0;
  saidasTreinamento[5][2]=0.0;

  amostrasTreinamento[6][0]=0.0;
  amostrasTreinamento[6][1]=1.0;
  amostrasTreinamento[6][2]=1.0;
  amostrasTreinamento[6][3]=0.0;
  saidasTreinamento[6][0]=1.0;
  saidasTreinamento[6][1]=0.0;
  saidasTreinamento[6][2]=0.0;
  

  amostrasTreinamento[7][0]=0.0;
  amostrasTreinamento[7][1]=1.0;
  amostrasTreinamento[7][2]=1.0;
  amostrasTreinamento[7][3]=1.0;
  saidasTreinamento[7][0]=1.0;
  saidasTreinamento[7][1]=0.0;
  saidasTreinamento[7][2]=1.0;

  amostrasTreinamento[8][0]=1.0;
  amostrasTreinamento[8][1]=0.0;
  amostrasTreinamento[8][2]=0.0;
  amostrasTreinamento[8][3]=0.0;
  saidasTreinamento[8][0]=1.0;
  saidasTreinamento[8][1]=0.0;
  saidasTreinamento[8][2]=1.0;

  amostrasTreinamento[9][0]=1.0;
  amostrasTreinamento[9][1]=0.0;
  amostrasTreinamento[9][2]=0.0;
  amostrasTreinamento[9][3]=1.0;
  saidasTreinamento[9][0]=1.0;
  saidasTreinamento[9][1]=0.0;
  saidasTreinamento[9][2]=1.0;

  amostrasTreinamento[10][0]=1.0;
  amostrasTreinamento[10][1]=0.0;
  amostrasTreinamento[10][2]=1.0;
  amostrasTreinamento[10][3]=0.0;
  saidasTreinamento[10][0]=1.0;
  saidasTreinamento[10][1]=0.0;
  saidasTreinamento[10][2]=0.0;

  amostrasTreinamento[11][0]=1.0;
  amostrasTreinamento[11][1]=0.0;
  amostrasTreinamento[11][2]=1.0;
  amostrasTreinamento[11][3]=1.0;
  saidasTreinamento[11][0]=1.0;
  saidasTreinamento[11][1]=0.0;
  saidasTreinamento[11][2]=1.0;

  amostrasTreinamento[12][0]=1.0;
  amostrasTreinamento[12][1]=1.0;
  amostrasTreinamento[12][2]=0.0;
  amostrasTreinamento[12][3]=0.0;
  saidasTreinamento[12][0]=0.0;
  saidasTreinamento[12][1]=1.0;
  saidasTreinamento[12][2]=0.0;

  amostrasTreinamento[13][0]=1.0;
  amostrasTreinamento[13][1]=1.0;
  amostrasTreinamento[13][2]=0.0;
  amostrasTreinamento[13][3]=1.0;
  saidasTreinamento[13][0]=0.0;
  saidasTreinamento[13][1]=1.0;
  saidasTreinamento[13][2]=0.0;

  amostrasTreinamento[14][0]=1.0;
  amostrasTreinamento[14][1]=1.0;
  amostrasTreinamento[14][2]=1.0;
  amostrasTreinamento[14][3]=0.0;
  saidasTreinamento[14][0]=0.0;
  saidasTreinamento[14][1]=1.0;
  saidasTreinamento[14][2]=0.0;

  amostrasTreinamento[15][0]=1.0;
  amostrasTreinamento[15][1]=1.0;
  amostrasTreinamento[15][2]=1.0;
  amostrasTreinamento[15][3]=1.0;
  saidasTreinamento[15][0]=0.0;
  saidasTreinamento[15][1]=1.0;
  saidasTreinamento[15][2]=1.0;
}

// inicializa as entradas e saidas
void inicializaEntradasESaidas(int numeroAmostra)
{
  entradas[0]=amostrasTreinamento[numeroAmostra][0];
  entradas[1]=amostrasTreinamento[numeroAmostra][1];
  entradas[2]=amostrasTreinamento[numeroAmostra][2];
  entradas[3]=amostrasTreinamento[numeroAmostra][3];
  saidasDesejadas[0]=saidasTreinamento[numeroAmostra][0];
  saidasDesejadas[1]=saidasTreinamento[numeroAmostra][1];
  saidasDesejadas[2]=saidasTreinamento[numeroAmostra][2];
}

// calcula erro total
void calculaErroTotal()
{
  // a expressão do erro total é: erroTotal=somatorio(1/2(saidaDesejada-saidaAtual)^2)
  int cont;
  double soma=0;
  double diferenca; // armazena a diferença entre o valor desejado e a saidatemporariamente
  double erroAtual;
  for(cont=0; cont<numeroSaidas; cont++)
  {
    diferenca=saidasDesejadas[cont]-saidaNeuronioSaida[cont];
    erroAtual=(0.5*(diferenca*diferenca));
    erroSaida[cont]=erroAtual;
    soma=soma+erroAtual;
  }
  erroTotal=soma;
}

// calcula novos pesos da camada de saída
void CalculaPesosSaida()
{
  // calcula derivada parcial de casa sinapse do neuronio
  // com relação sua saída deste neuronio
  // = -1*(saidaDesejada-saidaAtual)*saidaAtual*(1-saidaAtual)*saidaNeuronioAnterior
  // conforme teoria apresentada em sala de aula
  int contSaidas; // conta os neurônios da camada de saída
  int contSinapses; // conta as entradas de cada neurônio da camada de saída
  double delta; // valor que deve ser alterado no peso atual para ajustar
  for(contSaidas=0; contSaidas<numeroSaidas; contSaidas++)
  {
    for(contSinapses=0; contSinapses<numeroNeuroniosCamadaOculta; contSinapses++)
    {
      delta=-1*(saidasDesejadas[contSaidas]-saidaNeuronioSaida[contSaidas]);
      delta=delta*saidaNeuronioSaida[contSaidas]*(1-saidaNeuronioSaida[contSaidas]);
      delta=delta*saidaNeuronioCamadaOculta[contSinapses];
      //printf("\n<<S delta[%d][%d] = %.20f>>",contSaidas,contSinapses,delta);
      // armazena os novos pesos para futura atualização
      novosPesosCamadaSaida[contSinapses][contSaidas]=pesosCamadaSaida[contSinapses][contSaidas]-(taxaAprendizado*delta);
      //printf("<<novo peso[%d][%d] = %.20f>>",contSaidas,contSinapses,novosPesosCamadaSaida[contSinapses][contSaidas]);
    }
    // calcula novo valor do limiar
    delta=-1*(saidasDesejadas[contSaidas]-saidaNeuronioSaida[contSaidas]);
    delta=delta*saidaNeuronioSaida[contSaidas]*(1-saidaNeuronioSaida[contSaidas]);
    // armazena o novo valor de limiar
    novosPesosLimiarCamadaSaida[contSaidas]=pesosLimiarCamadaSaida[contSaidas]-(taxaAprendizado*delta);
  }
}

// calcula novos pesos da camada oculta
void CalculaPesosCamadaOculta()
{
  int contSaidas; // conta os neurônios da camada de saída
  int contEntradas; // conta o número de entradas de cada neurônio da camada oculta
  int contNeuronioOculto; // conta os neuronios da camada oculta
  double delta; // valor que deve ser alterado no peso atual para ajustar
  for(contNeuronioOculto=0; contNeuronioOculto<numeroNeuroniosCamadaOculta;contNeuronioOculto++)
  {
    for(contEntradas=0; contEntradas<numeroEntradas; contEntradas++)
    {
      delta=0;
      for(contSaidas=0; contSaidas<numeroSaidas; contSaidas++)
      {
        // calcula o delta com relação ao erro de todas as saídas
        delta=delta+((-1*(saidasDesejadas[contSaidas]-saidaNeuronioSaida[
        contSaidas]))*(saidaNeuronioSaida[contSaidas]*(1-saidaNeuronioSaida[contSaidas]))*(pesosCamadaSaida[contNeuronioOculto][contSaidas]));
      }
      delta=delta*saidaNeuronioCamadaOculta[contNeuronioOculto]*(1-saidaNeuronioCamadaOculta[contNeuronioOculto]);
      delta=delta*entradas[contEntradas];
      //printf("\n<<O delta[%d][%d] = %.20f>>",contNeuronioOculto,contEntradas,delta);
      // armazena os novos pesos para futura atualização
      novosPesosCamadaOculta[contEntradas][contNeuronioOculto]=pesosCamadaOculta[
      contEntradas][contNeuronioOculto]-(taxaAprendizado*delta);
      //printf("<<novo peso[%d][%d] = %.20f>>",contNeuronioOculto,contEntradas,novosPesosCamadaOculta[contEntradas][contNeuronioOculto]);
    }
    // calcula novo valor do limiar
    delta=0;
    for(contSaidas=0; contSaidas<numeroSaidas; contSaidas++)
    {
      // calcula o delta com relação ao erro de todas as saídas
      delta=delta+((-1*(saidasDesejadas[contSaidas]-saidaNeuronioSaida[contSaidas]))*(saidaNeuronioSaida[contSaidas]*(1-saidaNeuronioSaida[contSaidas]))*(pesosCamadaSaida[contNeuronioOculto][contSaidas]));
    }
    delta=delta*saidaNeuronioCamadaOculta[contNeuronioOculto]*(1-saidaNeuronioCamadaOculta[contNeuronioOculto]);
    // armazena o novo valor de limiar
    novosPesosLimiarCamadaOculta[contNeuronioOculto]=pesosLimiarCamadaOculta[contNeuronioOculto]-(taxaAprendizado*delta);
  }
}

// atualiza todos os pesos com o valor dos novos pesos
void atualizaPesos()
{
  int contaSaidas;
  int contaOcultos;
  int contaEntradas;
  for(contaSaidas=0; contaSaidas<numeroSaidas; contaSaidas++)
  {
    for(contaOcultos=0; contaOcultos<numeroNeuroniosCamadaOculta; contaOcultos++)
    {
      pesosCamadaSaida[contaOcultos][contaSaidas]=novosPesosCamadaSaida[contaOcultos][contaSaidas];
    }
    pesosLimiarCamadaSaida[contaSaidas]=novosPesosLimiarCamadaSaida[contaSaidas];
    }
    for(contaOcultos=0; contaOcultos<numeroNeuroniosCamadaOculta; contaOcultos++)
    {
      for(contaEntradas=0; contaEntradas<numeroEntradas; contaEntradas++)
      {
      pesosCamadaOculta[contaEntradas][contaOcultos]=novosPesosCamadaOculta[contaEntradas][contaOcultos];
      }
    pesosLimiarCamadaOculta[contaOcultos]=novosPesosLimiarCamadaOculta[contaOcultos];
  }
}

int main()
{
  srand (time(NULL));// inicializa a semente do gerador de números aleatórios
  int cont;
  
  //inicializa as variáveos
  inicializaDadosTreinamento();
  inicializaPesos();
  imprimeTodosValores();

  // realiza o treinamento
  cont=100000;
  while(cont>0)
  {
    inicializaEntradasESaidas(inteiroAleatorio(15));
    calculaSaidaCamadaOculta();
    calculaSaidaCamadaSaida();
    calculaErroTotal();
    CalculaPesosSaida();
    CalculaPesosCamadaOculta();
    atualizaPesos();
    cont--;
  }

  // apresenta a rede treinada 0101=100
  inicializaEntradasESaidas(5);
  calculaSaidaCamadaOculta();
  calculaSaidaCamadaSaida();
  calculaErroTotal();
  imprimeTodosValores();

  // apresenta a rede treinada 1001=101
  inicializaEntradasESaidas(9);
  calculaSaidaCamadaOculta();
  calculaSaidaCamadaSaida();
  calculaErroTotal();
  imprimeTodosValores();
  return 0;
}
