# Poker Multiplayer com Sockets em C++

## Descrição

Este projeto foi desenvolvido como parte da disciplina de Redes, com o objetivo de criar um jogo de poker multiplayer em C++ utilizando sockets. A implementação inclui a lógica do jogo de poker e a comunicação em rede para permitir partidas em tempo real.

## Funcionalidades

- **Comunicação em Rede:** Utilização de sockets para permitir a comunicação entre jogadores em diferentes máquinas.
  
- **Lógica do Jogo:** Implementação das regras do poker, incluindo distribuição de cartas, avaliação de mãos e determinação do vencedor.

## Estrutura do Projeto

- `server.cpp`: Código-fonte do servidor que gerencia a lógica do jogo e coordena a comunicação entre os clientes.
  
- `client.cpp`: Código-fonte do cliente que se conecta ao servidor para participar de partidas de poker.

## Como Executar

1. **Compilação:**
    - Compile o código-fonte do servidor:
        ```bash
        g++ -o server server.cpp
        ```
    - Compile o código-fonte do cliente:
        ```bash
        g++ -o client client.cpp
        ```

2. **Execução do Servidor:**
    ```bash
    ./server
    ```

3. **Execução do Cliente:**
    ```bash
    ./client
    ```

4. **Início do Jogo:**
    - Inicie dois clientes para que o jogo comece. O servidor coordenará a comunicação entre os clientes e gerenciará o estado do jogo.

5. **Divirta-se:** Desfrute de partidas de poker com amigos em tempo real!

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para sugerir melhorias na lógica do jogo, na comunicação em rede ou na interface gráfica. Abra uma issue ou envie um pull request para colaborar.

## Licença

Este projeto é distribuído sob a [Licença MIT](LICENSE), garantindo liberdade para utilizar, modificar e distribuir o código-fonte.

## Autores

Maria Eduarda Bessa Teixeira e Gabriel Henrique Rocha

---

**Nota:** Certifique-se de ter as dependências necessárias instaladas antes de compilar e executar o jogo. Consulte o código-fonte e as instruções para obter detalhes adicionais.

