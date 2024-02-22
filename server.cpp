#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>
#include <random>
#include <chrono> //fazer contagem de tempo da partida

enum PokerHand {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

// Função para determinar a combinação da mão de poker
PokerHand evaluatePokerHand(const std::vector<int>& hand, const std::vector<int>& naipes) {
    // Ordenar as cartas em ordem decrescente
    std::vector<int> sortedHand = hand;
    std::sort(sortedHand.rbegin(), sortedHand.rend());

    // Verificar combinações
    bool isFlush = false;
    bool isStraight = false;

    //Flush
    int count = 0, flush = 0, straight = 0;
    while(naipes[count] == naipes[count + 1]){
        flush++;
        std::cout << "Flush: " << flush << std::endl;
        count++;
    }
    if(flush == 4)
        isFlush = true;
    

    //Straight
    count = 4;

    while(sortedHand[count] - 1 == sortedHand[count-1]){
        straight++;
        std::cout << "Straight: " << straight << std::endl;
        count--;
    }

    if(straight == 4)
        isStraight = true;

    if (isFlush && isStraight) {
        if (sortedHand[0] == 14) {
            std::cout << "Temos um Royal Flush em jogo" << std::endl; 
            return ROYAL_FLUSH;
        } else {
            std::cout << "Temos um Straight Flush em jogo" << std::endl; 
            return STRAIGHT_FLUSH;
        }
    } else if (isFlush) {
        std::cout << "Temos um flush em jogo" << std::endl; 
        return FLUSH;
    } else if (isStraight) {
        std::cout << "Temos um straight em jogo" << std::endl; 
        return STRAIGHT;
    }

    // Contar ocorrências de cada valor
    std::vector<int> valueCounts(15, 0);
    for (int value : sortedHand) {
        valueCounts[value]++;
    }

    int numPairs = std::count(valueCounts.begin(), valueCounts.end(), 2);
    int numThreeOfAKind = std::count(valueCounts.begin(), valueCounts.end(), 3);
    int numFourOfAKind = std::count(valueCounts.begin(), valueCounts.end(), 4);

    if (numFourOfAKind > 0) {
        std::cout << "Temos uma quadra em jogo" << std::endl; 
        return FOUR_OF_A_KIND;
    } else if (numThreeOfAKind > 0 && numPairs > 0) {
        std::cout << "Temos uma fullhose em jogo" << std::endl; 
        return FULL_HOUSE;
    } else if (numThreeOfAKind > 0) {
        std::cout << "Temos uma tripla em jogo" << std::endl; 
        return THREE_OF_A_KIND;
    } else if (numPairs == 2) {
        std::cout << "Temos duas duplas em jogo" << std::endl; 
        return TWO_PAIR;
    } else if (numPairs == 1) {
        std::cout << "Temos uma dupla em jogo" << std::endl; 
        return ONE_PAIR;
    } else {
        std::cout << "Que a maior cartas venca" << std::endl; 
        return HIGH_CARD;
    }
}

// Função para lidar com a lógica do jogo de poker
int playGame(int player1Choice, int player2Choice, const std::vector<int>& player1Cards, const std::vector<int>& player2Cards, const std::vector<int>& player1Naipes, const std::vector<int>& player2Naipes) {
    if (player1Choice == 0 && player2Choice == 0) {
        // Ambos não apostam
        return 0;  // Empate
    }
    PokerHand player1Hand = evaluatePokerHand(player1Cards, player1Naipes);
    PokerHand player2Hand = evaluatePokerHand(player2Cards, player2Naipes);
    if (player1Choice == 1 && player2Choice == 1) {
        if (player1Hand > player2Hand) {
            return 1;  // Jogador 1 vence
        } else if (player1Hand < player2Hand) {
            return 2;  // Jogador 2 vence
        } else {
            // Se as mãos têm a mesma combinação, compare os valores das cartas
            std::vector<int> sortedPlayer1Cards = player1Cards;
            std::vector<int> sortedPlayer2Cards = player2Cards;
            std::sort(sortedPlayer1Cards.rbegin(), sortedPlayer1Cards.rend());
            std::sort(sortedPlayer2Cards.rbegin(), sortedPlayer2Cards.rend());

            for (size_t i = 0; i < sortedPlayer1Cards.size(); ++i) {
                if (sortedPlayer1Cards[i] > sortedPlayer2Cards[i]) {
                    return 1;  // Jogador 1 vence
                } else if (sortedPlayer1Cards[i] < sortedPlayer2Cards[i]) {
                    return 2;  // Jogador 2 vence
                }
            }

        return 0;  // Empate
        }
    }else {
        // Um apostou e o outro não
        if (player1Choice == 1) {
            return 1;  // Jogador 1 vence
        } else {
            return 2;  // Jogador 2 vence
        }
    }
}

// Função para embaralhar as cartas
void shuffleDeck(std::vector<int> &deck, std::vector<int> &naipe) {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Embaralhar o baralho usando o mecanismo de geração de números aleatórios padrão
    std::shuffle(deck.begin(), deck.end(), gen);

    // Gerar valores aleatórios para o naipe variando de 1 a 8
    std::uniform_int_distribution<> dis(1, 4);
    for (auto& card : naipe) {
        card = dis(gen);
    }
}

// // Função para lidar com a lógica do jogo
// int playGame(int player1Choice, int player2Choice, const std::vector<int> &player1Cards, const std::vector<int> &player2Cards) {
//     // Implementação da lógica do jogo
//     // Retorna 1 se o jogador 1 vencer, 2 se o jogador 2 vencer, 0 se for empate

//     if (player1Choice == 0 && player2Choice == 0) {
//         // Ambos não apostam
//         return 0;  // Empate
//     } else if (player1Choice == 1 && player2Choice == 1) {
//         // Ambos apostam

//         // Ordenar as cartas em ordem decrescente
//         std::vector<int> sortedPlayer1Cards = player1Cards;
//         std::vector<int> sortedPlayer2Cards = player2Cards;

//         std::sort(sortedPlayer1Cards.rbegin(), sortedPlayer1Cards.rend());
//         std::sort(sortedPlayer2Cards.rbegin(), sortedPlayer2Cards.rend());

//         // Comparar as cartas para determinar o vencedor
//         for (int i = 0; i < 5; ++i) {
//             if (sortedPlayer1Cards[i] > sortedPlayer2Cards[i]) {
//                 std::cout << sortedPlayer1Cards[i] << " x " << sortedPlayer2Cards[i] << std::endl;
//                 return 1;  // Jogador 1 vence
//             } else if (sortedPlayer1Cards[i] < sortedPlayer2Cards[i]) {
//                 std::cout << sortedPlayer1Cards[i] << " x " << sortedPlayer2Cards[i] << std::endl;
//                 return 2;  // Jogador 2 vence
//             }
//         }

//         // Se todas as cartas forem iguais, é um empate
//         std::cout << "Empatou" << std::endl;
//         return 0;  // Empate
//     } else {
//         // Um apostou e o outro não
//         if (player1Choice == 1) {
//             return 1;  // Jogador 1 vence
//         } else {
//             return 2;  // Jogador 2 vence
//         }
//     }
// }


int main() {
    // Configuração do servidor
    int serverSocket, clientSocket1, clientSocket2;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating server socket\n";
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    int player1Points = 0, player2Points = 0;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding server socket\n";
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 2) == -1) {
        std::cerr << "Error listening for connections\n";
        close(serverSocket);
        return -1;
    }
    std::cout << "Aguardando jogadores...\n";

    // Aceitar conexão do primeiro cliente
    clientSocket1 = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
    std::cout << "Player 1 connected\n";

    // Limpar a estrutura clientAddr antes de usá-la novamente
    memset(&clientAddr, 0, sizeof(clientAddr));

    // Aceitar conexão do segundo cliente
    clientSocket2 = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
    std::cout << "Player 2 connected\n";
    std::cout << "Iniciando partida...\n";

    // Variáveis adicionais para estatísticas e tempo
    int player1Wins = 0, player1Losses = 0, player1Draws = 0;
    int player2Wins = 0, player2Losses = 0, player2Draws = 0, totalPartidas = 0;
    bool continuePlaying1 = true, continuePlaying2 = true;

    while (continuePlaying1 && continuePlaying2) {
        totalPartidas++;
        auto startTime = std::chrono::steady_clock::now();

        std::vector<int> deck{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};  // Simplificando para cartas numeradas
        std::vector<int> naipe{1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
        shuffleDeck(deck, naipe);

        // Envie as cartas para os jogadores
        std::cout << "Enviando cartas aos jogadores e aguardando apostas\n";

        send(clientSocket1, deck.data(), 5 * sizeof(int), 0);
        send(clientSocket1, naipe.data(), 5 * sizeof(int), 0); 
        send(clientSocket1, &player1Points, sizeof(int), 0); 
        send(clientSocket2, deck.data() + 5, 5 * sizeof(int), 0);
        send(clientSocket2, naipe.data() + 5, 5 * sizeof(int), 0); 
        send(clientSocket2, &player2Points, sizeof(int), 0); 

        // Aguarde as escolhas dos jogadores e determine o vencedor
        int player1Choice, player2Choice;
        std::vector<int> player1Cards(5);
        std::vector<int> player2Cards(5);
        std::vector<int> player1Naipes(5);
        std::vector<int> player2Naipes(5);

        recv(clientSocket1, &player1Choice, sizeof(int), 0);

        recv(clientSocket2, &player2Choice, sizeof(int), 0);

        // Receber as cartas dos jogadores
        recv(clientSocket1, player1Cards.data(), 5 * sizeof(int), 0); 
        recv(clientSocket2, player2Cards.data(), 5 * sizeof(int), 0); 
        std::cout << "Calculando ganhador\n";

        recv(clientSocket1, player1Naipes.data(), 5 * sizeof(int), 0); 
        recv(clientSocket2, player2Naipes.data(), 5 * sizeof(int), 0); 

        int winner;
        // Verificar se ambos os jogadores optaram por não apostar
        if (player1Choice == 0 && player2Choice == 0) {
            // Ambos não apostam, ambos perdem 1 ponto
            winner = 0;
            player1Points -= 1;
            player2Points -= 1;
            player1Draws++;
            player2Draws++;
            std::cout << "EMPATE.\n";
        } else if (player1Choice == 1 && player2Choice == 1) {
            // Ambos apostam
            winner = playGame(player1Choice, player2Choice, player1Cards, player2Cards, player1Naipes, player2Naipes);

            if (winner == 1) {
                player1Points += 3;
                player2Points -= 2;
                player1Wins++;
                player2Losses++;
                std::cout << "VITÓRIA JOGADOR 1...\n";
            } else if (winner == 2) {
                player1Points -= 2;
                player2Points += 3;
                player2Wins++;
                player1Losses++;
                std::cout << "VITÓRIA JOGADOR 2...\n";
            } else {
                player1Draws++;
                player2Draws++;
                std::cout << "EMPATE...\n";
            }
        } else {
            // Apenas um jogador aposta, não há necessidade de chamar playGame
            if (player1Choice == 1) {
                winner = 1;
                player1Points += 3;
                player2Points -= 1;
                player1Wins++;
                player2Losses++;
                std::cout << "VITÓRIA JOGADOR 1...\n";
            } else {
                winner = 2;
                player2Points += 3;
                player1Points -= 1;
                player2Wins++;
                player1Losses++;
                std::cout << "VITÓRIA JOGADOR 2...\n";
            }
        }

        
        // Envie o resultado para os jogadores
        send(clientSocket1, &winner, sizeof(int), 0);
        send(clientSocket2, &winner, sizeof(int), 0);
        std::cout << "Aguardando opção para continuar jogando\n";
        // Recebe a opção dos jogadores para continuar jogando
        recv(clientSocket1, &continuePlaying1, sizeof(int), 0);
        recv(clientSocket2, &continuePlaying2, sizeof(int), 0);
        
        bool sendEndSignal = !continuePlaying1 || !continuePlaying2;
        // manda o sinal para ambos os jogadores se algum deles deseja finalizar
        send(clientSocket1, &sendEndSignal, sizeof(bool), 0);
        send(clientSocket2, &sendEndSignal, sizeof(bool), 0);
        // Se os jogadores optarem por encerrar, atualize as variáveis de continuePlaying
        
        if (sendEndSignal) {
            auto endTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

            // Enviar relatório final
            std::cout << "Relatório Final:\n";
            std::cout << "Tempo de Jogo: " << elapsedTime << " segundos\n";
            std::cout << "Total de partidas: " << totalPartidas << std::endl;
            std::cout << "Jogador 1 - Pontuação Total: " << player1Points << " Vitórias: " << player1Wins << ", Derrotas: " << player1Losses << ", Empates: " << player1Draws << "\n";
            std::cout << "Jogador 2 - Pontuação Total: " << player2Points << " Vitórias: " << player2Wins << ", Derrotas: " << player2Losses << ", Empates: " << player2Draws << "\n";

            // Envie as estatísticas para os jogadores
            send(clientSocket1, &player1Points, sizeof(int), 0);
            send(clientSocket1, &player2Points, sizeof(int), 0);
            send(clientSocket2, &player1Points, sizeof(int), 0);
            send(clientSocket2, &player2Points, sizeof(int), 0);

            send(clientSocket1, &totalPartidas, sizeof(int), 0);
            send(clientSocket2, &totalPartidas, sizeof(int), 0);

            send(clientSocket1, &player1Wins, sizeof(int), 0);
            send(clientSocket1, &player1Losses, sizeof(int), 0);
            send(clientSocket1, &player1Draws, sizeof(int), 0);
            send(clientSocket1, &player2Wins, sizeof(int), 0);
            send(clientSocket1, &player2Losses, sizeof(int), 0);
            send(clientSocket1, &player2Draws, sizeof(int), 0);
            
            send(clientSocket2, &player1Wins, sizeof(int), 0);
            send(clientSocket2, &player1Losses, sizeof(int), 0);
            send(clientSocket2, &player1Draws, sizeof(int), 0);
            send(clientSocket2, &player2Wins, sizeof(int), 0);
            send(clientSocket2, &player2Losses, sizeof(int), 0);
            send(clientSocket2, &player2Draws, sizeof(int), 0);
        }
        else{
            std::cout << "Iniciando nova partida\n";

        }

    }
        
    // Fechar os sockets
    close(clientSocket1);
    close(clientSocket2);
    close(serverSocket);

    return 0;
}