# Resolução do Problema da Mochila Multidimensional (MKP) aplicada à Alocação de Recursos em Nuvem com Simulated Annealing

Este repositório contém uma implementação em C++ de um resolvedor para o **Problema da Mochila Multidimensional (Multidimensional Knapsack Problem - MKP)** utilizando a meta-heurística probabilística **Simulated Annealing**. 

O projeto foi desenvolvido para a disciplina de **Algoritmos e Estruturas de Dados III (AED 3)**.

---

## 📌 Contexto do Problema: Alocação em Nuvem
O Problema da Mochila Multidimensional mapeia-se perfeitamente ao desafio de **Alocação de Recursos em Servidores de Nuvem**:
* **Itens (n):** Máquinas Virtuais (VMs) ou tarefas a serem alocadas.
* **Benefício (Lucro):** Faturamento ou prioridade de alocação de cada VM.
* **Múltiplos Recursos (m):** Restrições multidimensionais do servidor físico (ex: limites de CPU, RAM, Armazenamento, Banda de Rede).
* **Capacidades:** A capacidade máxima física de cada recurso no servidor.

O objetivo é maximizar o lucro total das VMs alocadas sem que nenhuma das $m$ restrições do servidor seja violada.

---

## 🛠️ Detalhes da Implementação

### 1. Simulated Annealing
O algoritmo utiliza transições estocásticas baseadas no **Critério de Metropolis** ($P = e^{\Delta E / T}$) para escapar de ótimos locais na busca de soluções de alta qualidade.
* **Vizinhança:** Mutação por inversão de um único bit (flip).
* **Esquema de Resfriamento:** Resfriamento geométrico clássico ($T_{novo} = T_{atual} \times \alpha$).

### 2. Heurística de Reparação Gulosa (Constraint Handling)
Como as restrições multidimensionais são rígidas, vizinhos inválidos gerados por flips aleatórios são corrigidos por um operador de reparação guloso. A relevância de remoção de cada item selecionado é calculada pela razão custo-benefício multidimensional normalizada:
$$\text{Ratio}(j) = \frac{\text{Profit}(j)}{\sum_{i=1}^{m} \frac{\text{Usage}(i, j)}{\text{Capacity}(i)}}$$
Os itens com as piores razões (que consomem mais recursos relativos para pouco lucro) são desmarcados sequencialmente até restabelecer a viabilidade completa da solução.

---

## 📂 Estrutura do Repositório

```text
multiDKnapsack/
├── include/
│   ├── instance.h       # Definição das estruturas e métodos do parser
│   ├── solution.h       # Gerenciamento de soluções e heurística de reparação
│   └── sa.h             # Parâmetros e núcleo do Simulated Annealing
├── src/
│   ├── instance.cpp     # Implementação do parser de instâncias OR-Library
│   ├── solution.cpp     # Implementação da avaliação e reparação
│   ├── sa.cpp           # Loop principal do Simulated Annealing
│   └── main.cpp         # Ponto de entrada interativo da CLI
├── data/                # Pasta sugerida para os benchmarks (.txt)
├── Makefile             # Script de compilação rápida otimizada (-O3)
└── README.md            # Este arquivo
```

---

## 🚀 Como Compilar e Executar

### Pré-requisitos
* Compilador GCC compatível com C++17.
* Utilitário `make` instalado.

### Passo 1: Compilação
Abra o terminal na pasta do projeto e execute o comando:
```bash
make
```
Isso gerará o executável binário `multi_knapsack` na pasta raiz.

### Passo 2: Execução
Execute o programa:
```bash
./multi_knapsack
```

---

## 📊 Formato das Instâncias de Teste
O parser é compatível com os arquivos de dados padrão da **OR-Library** (como `mknap1.txt` e a biblioteca de Chu & Beasley `mknapcb1.txt`). 

A entrada lê dados no seguinte formato:
1. Quantidade de problemas no arquivo ($K$).
2. Para cada problema: número de variáveis ($n$), número de restrições ($m$) e ótimo conhecido ($opt$).
3. Vetor de lucros.
4. Matriz de consumos (recurso por recurso).
5. Vetor de capacidades máximas.

---

## 📋 Resultados Acadêmicos e Execução em Lote
O resolvedor conta com um **Modo Batch (Lote)**. Ao selecionar essa opção, ele resolve todas as instâncias contidas no arquivo em sequência e gera uma tabela formatada em Markdown pronta para colar em relatórios técnicos, contendo:
* O lucro encontrado.
* A qualidade percentual em relação ao ótimo global (quando cadastrado).
* O tempo de processamento preciso em milissegundos.
* A confirmação de viabilidade (viavel = Sim/Não).
