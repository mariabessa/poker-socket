#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // Para inet_pton
#include <limits>  // Adicione esta linha para incluir o cabeçalho <limits>

// Função para limpar o terminal
void clearTerminal() {
    // Limpar o terminal no Linux ou Windows
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    // Configuração do cliente
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    int playerPoints = 0;
    // Conectar ao servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }
    bool continuePlaying = true;
    bool receiveEndSignal = false;

    while (continuePlaying && !receiveEndSignal) {
        clearTerminal();

        // Receber as cartas do servidor
        int cards[5], naipes[5];
        if (recv(clientSocket, cards, sizeof(cards), 0) == -1) {
            std::cerr << "Error receiving cards from server\n";
            close(clientSocket);
            return -1;
        }
        if (recv(clientSocket, naipes, sizeof(naipes), 0) == -1) {
            std::cerr << "Error receiving naipes from server\n";
            close(clientSocket);
            return -1;
        }
        if (recv(clientSocket, &playerPoints, sizeof(playerPoints), 0) == -1){
            std::cerr << "Error receiving points from server\n";
            close(clientSocket);
            return -1; 
        }
        
        // Exibir as cartas sorteadas para o jogador
        std::cout << "Suas cartas: ";
        for (int i = 0; i < 5; ++i) {
            //Valor da carta
            if(cards[i] == 11)
                std::cout << "J ";
                else if(cards[i] == 12)
                    std::cout << "Q ";
                else if(cards[i] == 13)
                    std::cout << "K ";
                else if(cards[i] == 14)
                    std::cout << "A ";
                else
                    std::cout << cards[i] << " ";

            //Naipe da carta
            if(naipes[i] == 1 || i == 5)
                std::cout << "Copas";
                else if(naipes[i] == 2)
                    std::cout << "Ouros";
                else if(naipes[i] == 3)
                    std::cout << "Espadas";
                else if(naipes[i] == 4)
                    std::cout << "Paus";
            
            if(i != 4)
                std::cout << ", ";
        }
        std::cout << std::endl;
        // Lógica do jogo - permitir ao usuário escolher apostar ou não
        int choice;
        std::cout << "Seus pontos atuais sao: " << playerPoints << std::endl;
        std::cout << "Escolha (0 para não apostar, 1 para apostar): " << std::flush;
        std::cin >> choice;
        // Receber o resultado do servidor

        if (send(clientSocket, &choice, sizeof(int), 0) == -1) {
            std::cerr << "Error sending choice to server\n";
            close(clientSocket);
            return -1;
        }

        // Enviar a pontuação do jogador para o servidor
        // if (send(clientSocket, &playerPoints, sizeof(int), 0) == -1) {
        //     std::cerr << "Error sending playerPoints to server\n";
        //     close(clientSocket);
        //     return -1;
        // }

    // Enviar as cartas para o servidor
        if (send(clientSocket, cards, sizeof(cards), 0) == -1) {
            std::cerr << "Error sending cards to server\n";
            close(clientSocket);
            return -1;
        }
        
        if (send(clientSocket, naipes, sizeof(cards), 0) == -1) {
            std::cerr << "Error sending naipes to server\n";
            close(clientSocket);
            return -1;
        }

        int winner;
        if (recv(clientSocket, &winner, sizeof(int), 0) == -1) {
            std::cerr << "Error receiving winner from server\n";
            close(clientSocket);
            return -1;
        }
        // Exibir o resultado para o jogador
        if (winner == 0) {
            std::cout << "Empate!\n";
        } else if (winner == 1) {
            std::cout << "Jogador 1 ganhou!!!\n";
        } else if (winner == 2) {
            std::cout << "Jogador 2 ganhou!!!\n";
        }
        std::cout << "Deseja continuar jogando? (1 para sim, 0 para não): " << std::flush;
        std::cin >> continuePlaying;

        // Limpar o buffer de entrada
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (send(clientSocket, &continuePlaying, sizeof(int), 0) == -1) {
            std::cerr << "Error sending continuePlaying choice to server\n";
            close(clientSocket);
            return -1;
        }
        // Se o jogador optar por continuar, envie a escolha para o servidor
        // if (continuePlaying) {
        //     int continueChoice = 1; // representando continuar
        //     if (send(clientSocket, &continueChoice, sizeof(int), 0) == -1) {
        //         std::cerr << "Error sending continuePlaying choice to server\n";
        //         close(clientSocket);
        //         return -1;
        //     }
        // }
                // Verificar se recebeu o sinal do servidor para exibir o relatório final
        recv(clientSocket, &receiveEndSignal, sizeof(bool), 0);

        // Se recebeu o sinal, exiba o relatório final
        if (receiveEndSignal) {
            // continuePlaying = 0;
            int player1Wins, player1Losses, player1Draws, player1Points;
            int player2Wins, player2Losses, player2Draws, player2Points;
            int totalPartidas;
            // Receba as pontuações dos jogadores
            recv(clientSocket, &player1Points, sizeof(int), 0);
            recv(clientSocket, &player2Points, sizeof(int), 0);

            recv(clientSocket, &totalPartidas, sizeof(int), 0);

            // Receba as estatísticas do jogador 1
            recv(clientSocket, &player1Wins, sizeof(int), 0);
            recv(clientSocket, &player1Losses, sizeof(int), 0);
            recv(clientSocket, &player1Draws, sizeof(int), 0);

            // Receba as estatísticas do jogador 2
            recv(clientSocket, &player2Wins, sizeof(int), 0);
            recv(clientSocket, &player2Losses, sizeof(int), 0);
            recv(clientSocket, &player2Draws, sizeof(int), 0);
            std::cout << "Algum dos jogadores encerrou o jogo. Obrigado por jogar!\n";

            // Imprima o relatório final na tela
            std::cout << "Relatório Final:\n";
            std::cout << "Total de partidas: " << totalPartidas << std::endl;
            std::cout << "Jogador 1 - Pontuação Total: " << player1Points << ", Vitórias: " << player1Wins << ", Derrotas: " << player1Losses << ", Empates: " << player1Draws << "\n";
            std::cout << "Jogador 2 - Pontuação Total: " << player2Points << ", Vitórias: " << player2Wins << ", Derrotas: " << player2Losses << ", Empates: " << player2Draws << "\n";
            close(clientSocket);

        }

    }
    // Lógica para relatório e continuar a jogar
    // Fechar o socket
    close(clientSocket);

    return 0;
}
