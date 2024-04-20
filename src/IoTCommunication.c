// Define todas as bibliotecas usadas
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "list.h" // Essa biblioteca foi feita na unidade curricular de Programação e Algoritmos, é usada para comandar uma lista ligada simples

#ifdef _WIN32
    #define CLEAR_SCREEN "cls" // Se for Windows, define o comando para limpar a tela
#else
    #define CLEAR_SCREEN "clear" // Se for Linux, define o comando para limpar a tela
#endif

typedef struct Event_ *Event; // Defino um ponteiro para a struct Event_

struct Event_ // Defino a struct Event_
{
    char *name;  // Nome do evento
    time_t time; // Tempo do evento
};

void free_event(void *event) // Função para liberar a memória alocada para o evento
{
    free(event);
}

int main(int argc, char **argv)
{
    char *array_commands[] = {"Identidade Válida  ", "Identidade Inválida"}; // Array com os nomes dos eventos
    List list = list_create();                                               // Crio uma lista ligada simples
    // Código para fazer tudo funcionar (não vou explicar tudo, porque nem eu entendi muito bem, mas servem para fazer a comunicação entre o terminal e o arduino funcionar)
    struct termios tio;
    struct termios stdio;
    struct termios old_stdio;
    int tty_fd; // File descriptor para a comunicação com o arduino
    system(CLEAR_SCREEN); // Limpa a tela do terminal
    printf("   _____            _             _        _____ ____ _______\n");
    printf("  / ____|          | |           | |      |_   _/ __ \\__   __|\n");
    printf(" | |     ___  _ __ | |_ _ __ ___ | | ___    | || |  | | | |   \n");
    printf(" | |    / _ \\| '_ \\| __| '__/ _ \\| |/ _ \\   | || |  | | | |   \n");
    printf(" | |___| (_) | | | | |_| | | (_) | |  __/  _| || |__| | | |   \n");
    printf("  \\_____\\___/|_| |_|\\__|_|  \\___/|_|\\___| |_____|\\____/ |_|   \n");
    printf("\n");
    printf("Comandos de Controle:\n");
    printf("i - Ligar Led Vermelho\n");
    printf("o - Desligar Led Vermelho\n");
    printf("k - Ligar Led Verde\n");
    printf("l - Desligar Led Verde\n");
    printf("z - Mostrar Tabela de Eventos\n");
    printf("c - Mostrar Controles\n");
    printf("q - Sair\n");
    printf("\n");

    unsigned char c = 'D'; // Caractere que vai ser lido do terminal
    tcgetattr(STDOUT_FILENO, &old_stdio);

    memset(&stdio, 0, sizeof(stdio));
    stdio.c_iflag = 0;
    stdio.c_oflag = 0;
    stdio.c_cflag = 0;
    stdio.c_lflag = 0;
    stdio.c_cc[VMIN] = 1;
    stdio.c_cc[VTIME] = 0;
    tcsetattr(STDOUT_FILENO, TCSANOW, &stdio);
    tcsetattr(STDOUT_FILENO, TCSAFLUSH, &stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // Faz a entrada ser não bloqueante

    memset(&tio, 0, sizeof(tio));
    tio.c_iflag = 0;
    tio.c_oflag = 0;
    tio.c_cflag = CS8 | CREAD | CLOCAL;
    tio.c_lflag = 0;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 5;

    tty_fd = open(argv[1], O_RDWR | O_NONBLOCK); // Abre a comunicação com o arduino
    cfsetospeed(&tio, B9600);                    // 9600 baud
    cfsetispeed(&tio, B9600);                    // 9600 baud

    tcsetattr(tty_fd, TCSANOW, &tio);
    // Fim do código para fazer tudo funcionar
    while (c != 'q') // Enquanto o usuário não digitar 'q', o programa vai continuar rodando
    {
        if (read(tty_fd, &c, 1) > 0 && c != '0' && c != '1' && c != '2' && c != '3') // Se o arduino mandar um caractere, ele vai ser escrito no terminal, exceto os caracteres de ID enviados juntos com a mensagem
        {
            write(STDOUT_FILENO, &c, 1); // Escreve o caractere no terminal
        }

        if (read(STDIN_FILENO, &c, 1) > 0) // Se o usuário digitar algo, o caractere vai ser enviado para o arduino
        {
            write(tty_fd, &c, 1); // Escreve o caractere no arduino
        }

        if (c == '0') // Se o caractere enviado pelo Arduino junto com a mensagem for '0', o evento "Identidade Válida" é adicionado à lista, com a respectiva data e hora
        {
            Event event = malloc(sizeof(struct Event_)); // Aloca memória para o evento
            event->name = array_commands[0];             // Define o nome do evento
            event->time = time(NULL);                    // Define a data e hora do evento
            list_insert_last(list, event);               // Adiciona o evento à lista
        }
        if (c == '1') // Se o caractere enviado pelo Arduino junto com a mensagem for '1', o evento "Identidade Inválida" é adicionado à lista, com a respectiva data e hora
        {
            Event event = malloc(sizeof(struct Event_)); // Aloca memória para o evento
            event->name = array_commands[1];             // Define o nome do evento
            event->time = time(NULL);                    // Define a data e hora do evento
            list_insert_last(list, event);               // Adiciona o evento à lista
        }
        if (c == '2') // Se o caractere enviado pelo Arduino junto com a mensagem for '2', a tabela de eventos é mostrada no terminal
        {
            if (list_is_empty(list)) // Se a lista estiver vazia, uma mensagem é mostrada no terminal
            {
                write(STDOUT_FILENO, "Nenhum evento registrado\n\r", 27); // Escreve "Nenhum evento registrado" no terminal
            }
            else // Se a lista não estiver vazia, os eventos são mostrados no terminal
            {
                list_iterator_start(list);                         // Inicia o iterador da lista
                write(STDOUT_FILENO, "\n\r", 2);                   // Pula uma linha no terminal
                write(STDOUT_FILENO, "Lista de Eventos:\n\r", 19); // Escreve "Lista de Eventos:" no terminal
                for (int i = 0; i < list_size(list); i++)          // Para cada evento na lista, o nome e a data e hora do evento são mostrados no terminal
                {
                    Event event = list_iterator_get_next(list);                             // Pega o elemento atual da lista e avança o iterador
                    write(STDOUT_FILENO, event->name, strlen(event->name));                 // Escreve o nome do evento no terminal
                    write(STDOUT_FILENO, " - ", 3);                                         // Escreve um traço no terminal
                    write(STDOUT_FILENO, ctime(&event->time), strlen(ctime(&event->time))); // Escreve a data e hora do evento no terminal
                    write(STDOUT_FILENO, "\r ", 1);                                         // Retorna a posicao do cursor para o início da linha
                }
            }
        }
        if(c == '3') //Se o caractere pressionado for c, os comandos de controle são mostrados no terminal
        {
            // Escreve os comandos de controle no terminal
            write(STDIN_FILENO, "\n\r", 1);
            write(STDIN_FILENO, "Comandos de Controle:\n\r", 23);
            write(STDIN_FILENO, "i - Ligar Led Vermelho\n\r", 25);
            write(STDIN_FILENO, "o - Desligar Led Vermelho\n\r", 28);
            write(STDIN_FILENO, "k - Ligar Led Verde\n\r", 22);
            write(STDIN_FILENO, "l - Desligar Led Verde\n\r", 25);
            write(STDIN_FILENO, "z - Mostrar Tabela de Eventos\n\r", 32);
            write(STDIN_FILENO, "c - Mostrar Controles\n\r", 24);
            write(STDIN_FILENO, "q - Sair\n\r", 11);
        }
    }

    list_destroy(list, free_event); // Libera a memória alocada para a lista
    // Mais código para fazer tudo funcionar

    close(tty_fd);                                 // Fecha a comunicação com o arduino
    tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio); // Restaura as configurações do terminal

    // Fim do código para fazer tudo funcionar

    return EXIT_SUCCESS; // Retorna 0
}