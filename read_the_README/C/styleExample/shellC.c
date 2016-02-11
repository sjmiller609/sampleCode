/**
Author: Steven Miller.5252
2/23/15
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 80 /* 80 chars per line, per command, should be enough. */
#define HISTORY_SIZE 12

/** The setup() routine reads in the next command line string storing it in the input buffer.
The line is separated into distinct tokens using whitespace as delimiters.  Setup also 
modifies the args parameter so that it holds points to the null-terminated strings which 
are the tokens in the most recent user command line as well as a NULL pointer, indicating the
end of the argument list, which comes after the string pointers that have been assigned to
args. ***/

/*define new type for commmand history*/
/*convention: history[*].number = 0 means that it shouldn't be printed because it's a duplicate or an unwritten spot in the array history[]*/
typedef struct{
	int number;
	char command[MAXLINE+1];
}prevcomm;


void setup(int *lengthptr, char iBuffer[], char *args[],int *bgrnd)
{

        int i,       /* Index for iBuffer arrray          */
        start,   /* Beginning of next command parameter           */
        j;       /* Where to place the next parameter into args[] */
	int length = *lengthptr;
    /* Read what the user enters */

    j = 0;
    start = -1;

    if (length == 0)
        exit(0);            /* Cntrl-d was entered, end of user command stream */

    if (length < 0){
        perror("error reading command");
	exit(-1);           /* Terminate with error code of -1 */
    }
    
    /* Examine every character in the input buffer */
    for (i = 0; i < length; i++) {
 
        switch (iBuffer[i]){
        case ' ':
        case '\t' :          /* Argument separators */

            if(start != -1){
                args[j] = &iBuffer[start];    /* Set up pointer */
                j++;
            }

            iBuffer[i] = '\0'; /* Add a null char; make a C string */
            start = -1;
            break;

        case '\n':             /* Final char examined */
            if (start != -1){
                args[j] = &iBuffer[start];     
                j++;
            }

            iBuffer[i] = '\0';
            args[j] = NULL; /* No more arguments to this command */
            break;

        case '&':
            *bgrnd = 1;
            iBuffer[i] = '\0';
            break;
            
        default :             /* Some other character */
            if (start == -1)
                start = i;
	}
 
    }    
    args[j] = NULL; /* Just in case the input line was > 80 */
} 

/*swaps two elements of prevcomm array*/
void swap(void *historyv, int A, int B){

	prevcomm *history = historyv;
	prevcomm temp;
	temp = history[A];
	history[A] = history[B];
	history[B] = temp;

}

/*this method moves commands that are marked with zero to the next overwrite sections*/
void moveZeros(void *historyv, int historySize){

	prevcomm *history = historyv;
	int i;
	int j;

	/*bubble sort, could be optimized*/
	for(i=0;i<historySize;i++){
		if(history[i].number==0){
			for(j=i;j<historySize-1;j++){
				swap(history,j,j+1);
			}
		}
	}
}

/*marks elements that match elements at history[0] with history.number = 0*/
void deleteDuplicates(void* historyPtr,int* historySizePtr,int* commandcountPtr){

	int i;
	prevcomm *history = historyPtr;

	for(i=1;i < *historySizePtr;i++){
		if(!strcmp(history[0].command,history[i].command)){
			history[i].number = 0;
		}
	}
	/*now we need to move the elements of the history array that history.number = 0 so that they are overwritten first*/
	moveZeros(history, *historySizePtr); 
}

/*method for r - execute command from histroy
strategy: call setup on old string again to get old args*/
void commandr(void *historyv, char** args, int commandcount, int historySize, int* lengthPtr, int* bgrndPtr, char* iBuffer){
	prevcomm *history = historyv;
	int i;
	int check =0;
	char* temp;
	if(args[1]!=NULL){
		for(i=0; i<commandcount && i<historySize;i++){
			/*if the string contains arg[1] at the beginning*/
			temp = strstr(history[i].command,args[1]);
			if(temp!=NULL && !strcmp(temp,history[i].command)){
				/*replace the string with the number of the command*/
				*lengthPtr = strlen(history[i].command);
				strcpy(iBuffer ,history[i].command);
				setup(lengthPtr, iBuffer, args, bgrndPtr);       /* Get next command */
				i=commandcount;
				check = 1;
			}
		}
	}

	if(!check && (args[1]==NULL||args[1]==""||atoi(args[1])>commandcount||atoi(args[1])<(commandcount+1-historySize)||atoi(args[1])<1)){

		printf("incorrect argument or out of range\n"); 
	}else if(!check){
	/*replace r <argument> with the string from history*/
		for(i=0; i<commandcount && i<historySize;i++){
			if(atoi(args[1])==history[i].number){
				/*replace the string with the number of the command*/
				*lengthPtr = strlen(history[i].command);
				strcpy(iBuffer ,history[i].command);
				setup(lengthPtr, iBuffer, args, bgrndPtr);       /* Get next command */
				i=commandcount;
			}
		}

	}
}		

/*method to for command "rr"*/
void commandrr(char** args, char* iBuffer, int* bgrndPtr, int* lengthPtr, int commandcount, int historySize, void *historyv){
	prevcomm *history = historyv;
	/*make sure there is a previous command*/
		if(commandcount > 0){
		/*replace rr with the previous command*/
			*lengthPtr = strlen(history[0].command);
			strcpy(iBuffer ,history[0].command);
			setup(lengthPtr, iBuffer, args, bgrndPtr);       /* Get next command */
		}else{
			printf("no previous commands\n");
		}
}

/*method for sethistory*/
void sethistory(char** args, void *historyv, int* historySizePtr){
	int i;
	prevcomm* history = historyv;
	/*make sure ok argument*/
	if(args[1]==NULL||atoi(args[1])<1){
		printf("incorrect argument for sethistory\n");
	}else{
	/*reallocate size of history*/

		history = (prevcomm *) realloc(history, (atoi(args[1])) * sizeof(prevcomm));

		if(atoi(args[1])>*historySizePtr){
			for(i=*historySizePtr;i < atoi(args[1]);i++){
				history[i].number=0;
			}
		}
		*historySizePtr = atoi(args[1]);
	}
}

/*this method must be follwed by replacing the data in the front of history*/
void vacateFront(void* historyv, int historySize){
	prevcomm* history = historyv;
	int i;
	for(i=historySize-1;i>0;i--){
		swap(history,i,i-1);
	}
}

/*method for adding commands to history*/
void addCommand(char** args, void *historyv, int historySize, int commandcount, int bgrnd){

	prevcomm* history = historyv;
	prevcomm new;
	/*vacate front of array*/
        vacateFront(history, historySize);
	/*going to concat to an empty string*/
	history[0].command[0] = '\0';

	int i = 0;
	/*loop through args, add to appropriate index of history[]*/
	while(args[i]!=NULL){
		strcat(history[0].command, args[i]);
		strcat(history[0].command, " ");
		i++;
	}
	/*& is not in args, need to check for that*/
	if(bgrnd){
		strcat(history[0].command,"&");
	}
	history[0].number = 1+commandcount;
}

/*history structure: int (sethistoryout), int (commandcount),int (historysize), int, 81 chars, int, 81 chars...*/
int loadhistory(void* historyv, int* commandcountptr, int* historySizePtr, int sethistoryout){

	FILE *fileptr;
	if(access("./.history.txt", F_OK) != -1){
		fileptr = fopen("./.history.txt","r");
		int i = 0;
		int tempint;
		prevcomm *history = historyv;

		/*read commandcount and history size*/
		fread(&sethistoryout, sizeof(int), 1 , fileptr);
		fread(commandcountptr, sizeof(int), 1 , fileptr);
		fread(historySizePtr, sizeof(int), 1 ,fileptr);
		
		/*going to iterate min(commandcount,historysize) times*/
		if(*commandcountptr > *historySizePtr){
			tempint = *historySizePtr;
		}else{
			tempint = *commandcountptr;
		}
		if(sethistoryout > 0){
			tempint = sethistoryout;
		}
		/*loop @iterations times to read in data for history*/
		for(i=0;i<tempint;i++){
			fread(&(history[i].number), sizeof(int), 1, fileptr); 
			fread(history[i].command, sizeof(char)*(MAXLINE+1), 1, fileptr);
		}
		fclose(fileptr);
		return(0);
	}
	return(-1);
}

/*this function persists the history*/
int persist(void* historyv,int historySize,int commandcount, int sethistoryout){

	FILE *fileptr;
	int tempint;
	int i;
	prevcomm *history = historyv;

	/*open the file, fileptr now points to it
	and it's opened in overwriting mode*/
	fileptr = fopen("./.history.txt","w+");
	fwrite(&sethistoryout, sizeof(int), 1, fileptr);
	fwrite(&commandcount, sizeof(int), 1, fileptr);
	fwrite(&historySize, sizeof(int), 1, fileptr);

	if(commandcount < historySize){
		tempint = commandcount;
	}else{
		tempint = historySize;
	}
	if(sethistoryout > 0){
		tempint = sethistoryout;
	}
	
	for(i=0;i<tempint;i++){
		fwrite(&(history[i].number), sizeof(int), 1, fileptr);
		fwrite(history[i].command, (sizeof(char))*(MAXLINE+1), 1, fileptr);
	}

	fclose(fileptr);
	return(0);
}

/*this function sets the history to be saved to the value given by user*/
/*and it checks to make sure it's a valid input*/
void sethistoryoutf(char** args, int historySize, int* sethistoryoutPtr){
	
	if(args[1] == NULL){
		printf("incorrect number of arguments, leaving size at %d\n", historySize);
	}else if(atoi(args[1])>historySize){
		printf("that is greater than the history size. leaving size at %d\n", historySize);
	}else if(atoi(args[1])<0){
		printf("pick a size greater than zero. leaving size at %d\n", historySize);
	}else{
		*sethistoryoutPtr = atoi(args[1]);
	}

}

int main(void)
{
    int historySize = HISTORY_SIZE;
    char iBuffer[MAXLINE]; /* Buffer to hold the command entered */
    char *args[MAXLINE/2+1];/* Command line (of 80) has max of 40 arguments */
    int bgrnd;             /* Equals 1 if a command is followed by '&', else 0 */
    int pid;
    int *status;
    int length;
    /*counting int*/
    int i;
    /*keeps track of how many command have been executed*/
    int commandcount = 0;
    int sethistoryout = -1;

    /*array to store history*/
    /*each command is added to this array before the fork*/
    prevcomm *history = (prevcomm *) malloc(historySize * sizeof(prevcomm));

    /*initialize history.number to 0 for all elements*/
    for(i=0;i<historySize;i++){
	history[i].number=0;
    }

    /*initialize history from .history.txt*/
    loadhistory(history,&commandcount,&historySize,sethistoryout);

    while (1){            /* Program terminates normally inside setup */

	bgrnd = 0;

	printf("Sys2Sh: ");  /* Shell prompt */
        fflush(0);

	/*the below line was moved outside of the method setup()
	so that the method may take a string input from history more easily*/
    	length = read(STDIN_FILENO, iBuffer, MAXLINE);  
        setup(&length, iBuffer, args, &bgrnd);       /* Get next command */

	/*this if is to make sure there is an argument*/
	if(!(args[0]==NULL)){

		/*check if the command is r or rr
		do this before the fork so that parent can still
		know the status of bgrnd*/
		/*options for commands that dont need child process*/
		if(!strcmp(args[0],"r")){
			commandr(history, args, commandcount, historySize, &length, &bgrnd, iBuffer);

		}else if(!strcmp(args[0],"rr")){
			commandrr(args, iBuffer, &bgrnd, &length, commandcount, historySize, history);

		}else if(!strcmp(args[0],"sethistory")){
			sethistory(args, history, &historySize);

		}else if(!strcmp(args[0],"sethistoryout")){
			sethistoryoutf(args, historySize, &sethistoryout);
		}


		/*add current command to history*/
		addCommand(args, history, historySize, commandcount, bgrnd);

		/*incement commandcount*/
		commandcount++;

		/*delete duplicates*/
		deleteDuplicates(history, &historySize, &commandcount);

		/*persist history*/
		persist(history,historySize,commandcount,sethistoryout);

		/*fork child process*/
		pid = fork();
		/* fork() returning less than zero means child process couldn't be forked*/
		if(pid < 0){
			printf("fork() did not work.");

		}else if(!pid){
		/*child*/
			/*code to execute h*/
			if(!strcmp(args[0],"h")||!strcmp(args[0],"history")){
				for(i=historySize-1; i>=0 ;i--){
					/*number=0 entries are deleted repeats or not initialized*/
					if(history[i].number!=0){
						printf(" %d\t",history[i].number);
						printf(" %s ",history[i].command);
						printf("\n");
					}
				}
				exit(0);
			}else if(!strcmp(args[0],"sethistory")){
				exit(0);
			}else if(!strcmp(args[0],"sethistoryout")){
				exit(0);
			}else{		/*invoke execvp()*/
				execvp(args[0], args);
				/*won't get here unless execvp didn't work.*/
				printf("unknown command\n");
				exit(0);
			}	
		
		}else{
		/*parent*/
			/*print out the pid of child if background*/
			if(bgrnd){
				printf("%d",pid);
				printf("\n");
			}
			/*waits until child process is terminated if bgrnd is false*/
			while(!bgrnd){
				pid_t childpid = wait(&status);
				/*set background varable to 1 to escape the loop*/
				if(childpid == pid) bgrnd=1;
			}
		}
	}
    }
}
