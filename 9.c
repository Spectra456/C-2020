#include <mpi.h> 
#include <stdio.h>
#define leng 100

int main( int argc, char **argv ) {
    char message[leng] = "hello";
    char rec_message[leng];
    int key = 2;
    encryption(message, key);

    double t,t2;
    int i,rank,size;
    //char WR[leng];
    
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    //sprintf(message,"Hello from %d",rank); 
    t=MPI_Wtime(); 
    MPI_Send(message,leng,MPI_CHAR,size-(rank+1),rank,MPI_COMM_WORLD); 
    MPI_Recv(rec_message,leng,MPI_CHAR,rank,size-(rank+1),MPI_COMM_WORLD,&status);
     
    decryption(rec_message, key);

    t2=MPI_Wtime(); 

    printf("From processor %d\nEncrypted message: %s\n",rank,message); 
    printf("From processor %d\n",rank+1); 
    printf("Decrypted message: %s \n", rec_message);

    MPI_Finalize();

    return 0;
}

int encryption(char *buff, int key) {
	char message[leng], ch;
	int i;
	strcpy(message, buff);
	
	for(i = 0; message[i] != '\0'; ++i){
		ch = message[i];
		
		if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;
			
			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}
			
			message[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;
			
			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}
			
			message[i] = ch;
		}
	}
	
    strcpy(buff, message);
	return 0;
}

int decryption(char *buff, int key){

	char message[100], ch;
	int i;
	
    strcpy(message, buff);

	for(i = 0; message[i] != '\0'; ++i){
		ch = message[i];
		
		if(ch >= 'a' && ch <= 'z'){
			ch = ch - key;
			
			if(ch < 'a'){
				ch = ch + 'z' - 'a' + 1;
			}
			
			message[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch - key;
			
			if(ch < 'A'){
				ch = ch + 'Z' - 'A' + 1;
			}
			
			message[i] = ch;
		}
	}
	
    strcpy(buff, message);

	return 0;
}