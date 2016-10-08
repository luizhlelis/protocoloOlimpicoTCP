#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcoes.h"

#define MAXBUF 100

int main(int argc, char **argv){

        char *MsgRead = malloc (MAXBUF*sizeof(char));
        char *MsgWrite = malloc (MAXBUF*sizeof(char));
        struct sockaddr_in6 ServerAddr, ClientAddr;
        int Addrlen = sizeof(ClientAddr);
        int Sd =-1, Port, Client_s;

        Port = atoi(argv[1]);
        int Position, i, TimeMiliseconds;
        Record AthleteTime;

        Sd = socket(AF_INET6, SOCK_STREAM, 0);

        /* Foi atribuido in6addr_any ao endereço pois ele permite (por padrão) estabelecer conexoes
        de qualquer cliente IPv4 e IPv6 que especificar a porta igual ao segundo argumento(argv[2])*/

        memset(&ServerAddr, 0, sizeof(ServerAddr));
        ServerAddr.sin6_family = AF_INET6;
        ServerAddr.sin6_port   = htons(Port);

        ServerAddr.sin6_addr   = in6addr_any;

        bind(Sd,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));

        listen(Sd, 5); // Permite que o servidor aceite conexoes de entrada do cliente

        while(1){

            Client_s = accept(Sd,(struct sockaddr*)&ClientAddr, &Addrlen);  //Aceita uma solicitacao de conexao de entrada
            Table *AthletesPos = malloc(sizeof(Table));

             do{

                read(Client_s, MsgRead, MAXBUF);  // Le a mensagem escrita pelo cliente

                TimeMiliseconds = ChangeToMilisec(MsgRead, strlen(MsgRead));

                if(TimeMiliseconds>=0){

                    AthleteTime.Key = TimeMiliseconds;

                    InsertTable(AthleteTime, AthletesPos);

                    InsertionSort(AthletesPos->Item, AthletesPos->n);

                    Position = BinarySearch(AthleteTime.Key, AthletesPos);

                    sprintf(MsgWrite,"%d", Position);

                }else{
                    sprintf(MsgWrite,"%d", TimeMiliseconds);
                }

                write(Client_s, MsgWrite, strlen(MsgWrite)+1);

            }while(TimeMiliseconds>=0);

            close(Client_s);

            strcpy(MsgRead, "");
            strcpy(MsgWrite, "");
            free(AthletesPos);

        }

        free(MsgRead);
        free(MsgWrite);

}
