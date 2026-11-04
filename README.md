# Implementação 4 — Sincronização do Metrô (Infraestrutura de Software)

**Aluno:** Davi Maltez Costa Vasconcelos
**Curso:** Ciências da Computação — CESAR School
**Disciplina:** Infraestrutura de Software

## Descrição

Solução em C para o problema de sincronização entre vagões de metrô e passageiros da METROREC. Cada vagão e cada passageiro é uma thread independente. As funções de sincronização garantem que o embarque ocorra de forma ordenada, sem busy-wait, usando apenas **um mutex** e **duas variáveis de condição** por `struct station`.

## Arquivos

| Arquivo | Descrição |
|---|---|
| `sync_car.h` | Declaração da `struct station` e dos protótipos das funções. |
| `sync_car.c` | Implementação de `station_init`, `station_load_car`, `station_wait_for_car` e `station_on_board`. |
| `sync_car_runner.c` | Programa de teste fornecido pelo professor (contém o `main`). Cria 100 threads de passageiros e simula vagões com quantidades aleatórias de assentos livres. |
| `Makefile` | Compila o projeto (gera o executável `sync_car`) e limpa os arquivos gerados. |
| `README.md` | Este arquivo. |

## Funções implementadas

- **`station_init(station *)`**: inicializa os contadores, o mutex e as duas variáveis de condição.
- **`station_load_car(station *, int count)`**: chamada pelo vagão ao chegar, com `count` assentos livres. Libera os passageiros que aguardam e só retorna quando todos os que embarcaram já estão sentados e o vagão está cheio ou a fila esvaziou. Retorna imediatamente se `count == 0` ou se não há passageiros esperando.
- **`station_wait_for_car(station *)`**: chamada pelo passageiro ao chegar. Bloqueia até haver um vagão na estação com assento livre. Vários passageiros podem retornar dessa função antes de qualquer um chamar `station_on_board`.
- **`station_on_board(station *)`**: chamada pelo passageiro após sentar. Avisa o vagão quando o último passageiro embarcou e a condição de partida foi atingida.

### Estrutura de sincronização

Campos da `struct station`:

- `qtd_pessoas_fila`: passageiros esperando na estação.
- `qtd_assentos_disponiveis`: assentos livres do vagão atual (0 quando não há vagão).
- `passageiros_embarcando`: passageiros que já ocuparam um assento, mas ainda não chamaram `station_on_board`.
- `mutex`: único lock da estação.
- `cond_passageiros`: passageiros aguardam um vagão com assentos.
- `cond_vagao`: o vagão aguarda os passageiros terminarem de embarcar.

O vagão parte quando **não há ninguém no corredor** (`passageiros_embarcando == 0`) **e** (não há assentos livres **ou** a fila está vazia).

Restrições do enunciado atendidas: apenas as funções `pthread_mutex_*` e `pthread_cond_*` permitidas, sem semáforos, um único lock por estação e sem busy-wait (toda espera é feita com `pthread_cond_wait` dentro de um `while`).


## Como compilar

```bash
make
```

Isso gera o executável `sync_car`.

Para remover os arquivos compilados:

```bash
make clean
```

## Como executar

```bash
./sync_car
```

A saída mostra cada vagão entrando na estação, quantos assentos livres tinha e quantos passageiros embarcaram. Se tudo estiver correto, o programa termina com:

```
Looks good!
```

Em caso de erro, uma mensagem é escrita em `stderr` e o programa encerra com código diferente de zero.

## Como testar

1. **Teste do professor**: `./sync_car`. Como o número de assentos é aleatório, vale repetir várias vezes:

   ```bash
   for i in $(seq 1 100); do ./sync_car > /dev/null || echo "falhou na execução $i"; done
   ```

2. **Sanitizers (opcional)**:

   ```bash
   gcc -g -fsanitize=address,undefined -pthread -o sync_car_asan sync_car_runner.c sync_car.c && ./sync_car_asan
   gcc -g -fsanitize=thread -pthread -o sync_car_tsan sync_car_runner.c sync_car.c && ./sync_car_tsan
   ```

   Observação: o ThreadSanitizer reporta data races nas variáveis `volatile` do `sync_car_runner.c`, que não usa mutex para elas. Nenhuma ocorre em `sync_car.c`.

3. **Cenários verificados** (testes adicionais):
   - passageiros bloqueiam enquanto não há vagão;
   - vagão com N assentos e mais de N passageiros: embarcam exatamente N;
   - todos os passageiros passam por `station_wait_for_car` antes de qualquer `station_on_board`;
   - vagão com mais assentos que passageiros retorna quando todos embarcaram;
   - fila vazia ou `count == 0`: retorno imediato;
   - ausência de busy-wait (uso de CPU próximo de zero enquanto o vagão espera).

## Relatório

O relatório em PDF está neste repositório: `[dmcv].pdf`.
