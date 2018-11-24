#ifndef MINHASERIAL_H
#define MINHASERIAL_H

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>

template <class T>
class MinhaSerial {
	public:
	    MinhaSerial();
	    ~MinhaSerial();
	    int AbreDispositivo(const char *path);
	    int Envia(const void *buf, size_t nbytes);
	    int Recebe(void *buf, size_t nbytes);
	    void RecebeMedida(float &valorVolts);
	    int IniciaColeta();
	    int ParaColeta();
	    
	private:
	    int file; // ponteiro para o dispositivo
	    
};

template <class T>
MinhaSerial<T>::MinhaSerial() : file( 0 ) {}

template <class T>
MinhaSerial<T>::~MinhaSerial(){
	close(file);
}

template <class T>
int MinhaSerial<T>::AbreDispositivo(const char *path){
    if ((file = open(path, O_RDWR | O_NOCTTY | O_NDELAY))<0){
        perror("UART: Falha ao abrir o arquivo.\n");
        return -1;
    }
    struct termios options;             // cria estruturas para configurar a comunicacao
    tcgetattr(file, &options);          // ajusta os parametros do arquivo
    // Configura a comunicacao
    // 115200 baud, 8-bit, enable receiver, no modem control lines
    options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR | ICRNL;   // ignora erros de paridade
    tcflush(file, TCIFLUSH);            // descarta informacao no arquivo
    tcsetattr(file, TCSANOW, &options); // aplica alteracoes imediatamente
    return 0;
}

template <class T>
int MinhaSerial<T>::Envia(const void *buf, size_t nbytes){
	int count;
    if ( (count = write(file, buf, nbytes)) < 0 ){
        perror("Falha ao escrever na saida.\n");
        return -1;
    }
    return count;
}

template <class T>
int MinhaSerial<T>::Recebe(void *buf, size_t nbytes){
	int count;
	if ((count = read(file, buf, nbytes))<0){        // recebe os dados
	    perror("Falha ao ler da entrada\n");
		return -1;
    }
    return count;
}

template <class T>
void MinhaSerial<T>::RecebeMedida(float &valorVolts){
	unsigned char receive[2];        // cria um buffer para receber os dados
	int count, valorRecebido;
	count = Recebe((void*)receive, 2);
    if ( count == 2 ) {
        valorRecebido = receive[0] | receive[1] << 8;
		if ( valorRecebido >= 0 && valorRecebido <= 1023 )
		{
			valorVolts = valorRecebido * 5.0 / 1023.0;
		}
    }
    else
    {
		valorVolts = -1;
	}
}

template <class T>
int MinhaSerial<T>::IniciaColeta(){
	return Envia("i",1);      // inicia a coleta no Arduino
}

template <class T>
int MinhaSerial<T>::ParaColeta(){
	return Envia("p",1);      // termina a coleta no Arduino
}
	    
#endif