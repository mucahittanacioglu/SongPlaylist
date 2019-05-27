#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>//for random number seed

struct Song{
    char name[25];
    int duration;
    struct Song *chrone_next;
    struct Song *alpha_next;
    struct Song *duration_next;
    struct Song *random_next;
}typedef Song;

void printAllPlaylists();

void printPlaylistsWithType(Song *,char);

void insertToRandom(Song song, Song **root);

void insertToChrone(Song song, Song **root);

void insertToAlphabetic(Song song, Song **root);

void insertToDuration(Song song, Song **root);

char* getDurasMinSec(int a);

void insertNode(char *name,int duration);

void deleteNode(char *name);

void deleteFromChrone(char *name, Song **chroneroot);

void deleteFromAlphabetic(char *name, Song **root);

void deleteFromDuration(char *name, Song **root);

void deleteFromRandom(char *name, Song **root);

char* getOnlyName(char* input);

int stringToInt(char* duration);

int getOnlyDuration(char* input,int nameSize);

void readSongsFromFile(char* path);

void writeThroughFile(char *outputfile);

void printPlaylistsThroughFile(Song *root,char type,char *output);

void shuffleRandomPlayList();

int isIn(int*,int,int);

void insertNewSongsFromFile();

Song *random_root=NULL,*alpha_root=NULL,*duration_root=NULL,*chrone_root=NULL;

int main() {
    static char filename[] = "songs.txt";
    int option=10;
    char *nameForMenu,*outputfile;
    char buffer[120]={'\0'},buffer2[50]={'\0'};
    readSongsFromFile(filename);
    shuffleRandomPlayList();
       do{
        printf("\nEnter your choice:\n"
               "1 to insert a song into\n"
               "2 to delete a song from\n"
               "3 to print the songs in the list\n"
               "4 to print the songs to an output file\n"
               "5 to insert songs from a file\n"
               "6 to end.\n?");
        scanf("%d",&option);
        getchar();
        switch(option){
            case 1:
                printf("\nEnter a song name with duration:\n");
                fgets(buffer, sizeof buffer,stdin);

                nameForMenu=getOnlyName(buffer);

                insertNode(getOnlyName(buffer),getOnlyDuration(buffer,strlen(nameForMenu)));
                memset(buffer,sizeof buffer,0);
                break;
            case 2:
                printf("\nEnter a song name :\n");
                fgets(buffer, sizeof buffer,stdin);
                nameForMenu=getOnlyName(buffer);
                deleteNode(nameForMenu);
                break;
            case 3:
                printAllPlaylists();
                break;
            case 4:
                printf("Please enter output file name:\n");
                scanf("%s", outputfile);
                getchar();
                writeThroughFile(outputfile);
                printf("The songs in list exported to  \"%s\"..\n",outputfile);
                break;
            case 5:
                printf("Please enter input file name:\n");
                fgets(buffer2,50,stdin);//reading line max 50 character
                readSongsFromFile(getOnlyName(buffer2));//reading songs from given file
                printf("The songs in \"%s\" imported to list..\n",getOnlyName(buffer2));
                break;
            default:
                printf("Please select proper option!\n");
                break;
        }
    }while(option!=6);
    return 0;
}


void shuffleRandomPlayList(){
    //Getting list size create an integer array to store random indises later;
    Song *iter = random_root;

    int arraySize=0,iterIndex=0,tempRandom,tempIndexer;

    for(arraySize ; iter!=NULL ; arraySize++ , iter=iter->random_next);

    int *randomIndex = malloc(sizeof(int)*arraySize);
    memset(randomIndex,0, sizeof(int)*arraySize);

    srand(time(NULL));//getting random seed with time

    while(1){
        tempRandom=rand()%(arraySize);
        if(!isIn(randomIndex,arraySize,tempRandom)){//Checking whether index already in array or not.
            randomIndex[iterIndex]=tempRandom;//adding index to random index array if its not already included.
            iterIndex++;
        }
        if(iterIndex>=(arraySize-1)){//if array is full break
            break;
        }
    }

    random_root=NULL;//random list root has set to null to recreate list.
    for(int i = 0; i<arraySize;i++){
        iter=duration_root; //i use duration ordered list to get songs it can be duration or alphabetic as well.
        tempIndexer=randomIndex[i]; //getting first element index from random inedx array

        for (int k = 0; k < tempIndexer; k++)
            iter = iter->duration_next;//getting node base on random index array

        insertToRandom(*iter,&random_root);//creating random list from scratch.
    }



}

int isIn(int* indexes,int size,int number){
    for(int i = 0; i<size;i++) //a small method to check whether given number included given integer array or not(used on shuffle method).
        if(indexes[i]==number)
            return 1;
    return 0;
}


void writeThroughFile(char *outputfile) {//methdo automatically calls write to file methdods with all root of lists.

    printPlaylistsThroughFile(duration_root,'D',outputfile);
    printPlaylistsThroughFile(alpha_root,'A',outputfile);
    printPlaylistsThroughFile(chrone_root,'C',outputfile);
    printPlaylistsThroughFile(random_root,'R',outputfile);

}

void printPlaylistsThroughFile(Song *root,char type,char *output){
    int index=1;//the order number on the lists
    FILE *cfPtr;//file pointer
    switch(type) {
        case'D':
            cfPtr= fopen(output,"w");//each function call if given output file same as before its remove everything inside first than appen for others
            index=1;
            fprintf(cfPtr,"The list in duration-time order:\n");
            while (root != NULL) {
                fprintf(cfPtr,"\t%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));
                root = root->duration_next;
                index++;
            }
            fprintf(cfPtr,"\n");
            fclose(cfPtr);
            break;
        case'A'://appending list
            cfPtr= fopen(output,"a+");
            index=1;
            fprintf(cfPtr,"The list in alphabetical order:\n");
            while (root != NULL) {
                fprintf(cfPtr,"\t%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));
                root = root->alpha_next;
                index++;
            }
            fprintf(cfPtr,"\n");
            fclose(cfPtr);
            break;
        case'C'://appending list
            cfPtr= fopen(output,"a+");
            index=1;
            fprintf(cfPtr,"The list in choronological order:\n");
            while (root != NULL) {
                fprintf(cfPtr,"\t%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));
                root = root->chrone_next;
                index++;
            }
            fprintf(cfPtr,"\n");
            fclose(cfPtr);
            break;
        case'R'://appending list
            shuffleRandomPlayList();//shuffling before write
            cfPtr= fopen(output,"a+");
            index=1;
            fprintf(cfPtr,"The list in random order:\n");
            while (root != NULL) {
                fprintf(cfPtr,"\t%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));
                root = root->random_next;
                index++;
            }
            fprintf(cfPtr,"\n");
            fclose(cfPtr);
            break;
        default:
            printf("Please give valid type parameter.('C' for chronical,'A' for alphabetic,'D' for duration,'R' for random list.");
            break;
    }
}

void readSongsFromFile(char* path){
    char *name;//name field for song on given file

    int dur;
    FILE *filePtr = fopen ( path, "r" );
    if (filePtr != NULL){
        char line [ 128 ];//creating buffer to store our lines
        while ( fgets ( line, sizeof line, filePtr ) != NULL ){//reading max 128 char from each line and store it in buffer which is line char array
            name=getOnlyName(line);//calls function which seperate name part from duration on given file and return it
            dur=getOnlyDuration(line,strlen(name));// function that seperate duration part from file and return it
            insertNode(getOnlyName(line),dur);//inserting function base on given song properties.
        }
        fclose ( filePtr );//closing file
        free(filePtr);
    }else{
        perror ( path );//if file couldnt find error message
    }
}

void deleteNode(char *name) {//delete given node from every list
    deleteFromChrone(name,&chrone_root);
    deleteFromAlphabetic(name,&alpha_root);
    deleteFromDuration(name,&duration_root);
    deleteFromRandom(name,&random_root);
}

void deleteFromRandom(char *name, Song **root) {
    Song * temp,*iter;
    iter=*root;
    if(strcmp((*root)->name,name)==0) {//the song we would remove is root so root gonna change
        temp = *root;
        *root=(*root)->random_next;
        free(temp);
        return;
    }else{//we  try to find song here
        while(iter->random_next !=NULL && strcmp(iter->random_next->name,name)!=0)
            iter=iter->random_next;
        if(iter->random_next==NULL && strcmp(iter->name,name)!=0)//song is not in list.
            printf("There is no song named as: %s\n\n",name);
        else{
            temp=iter->random_next;
            iter->random_next=iter->random_next->random_next;//deleting node and rearrange list
            free(temp);
        }
    }
}

void deleteFromDuration(char *name, Song **root) {
    Song * temp,*iter;
    iter=*root;
    if(strcmp((*root)->name,name)==0) {//the song we would remove is root so root gonna change
        temp = *root;
        *root=(*root)->duration_next;
        free(temp);
        return;
    }else{//we  try to find song here
        while(iter->duration_next !=NULL && strcmp(iter->duration_next->name,name)!=0)
            iter=iter->duration_next;
        if(iter->duration_next==NULL && strcmp(iter->name,name)!=0)//song is not in list.
            printf("There is no song named as: %s\n\n",name);
        else{
            temp=iter->duration_next;
            iter->duration_next=iter->duration_next->duration_next;//deleting node and rearrange list
            free(temp);
        }
    }
}

void deleteFromAlphabetic(char *name, Song **root) {
    Song * temp,*iter;
    iter=*root;
    if(strcmp((*root)->name,name)==0) {//the song we would remove is root so root gonna change
        temp = *root;
        *root=(*root)->alpha_next;
        free(temp);
        return;
    }else{//we  try to find song here
        while(iter->alpha_next !=NULL && strcmp(iter->alpha_next->name,name)!=0)
            iter=iter->alpha_next;
        if(iter->alpha_next==NULL && strcmp(iter->name,name)!=0)//song is not in list.
            printf("There is no song named as: %s\n\n",name);
        else{
            temp=iter->alpha_next;
            iter->alpha_next=iter->alpha_next->alpha_next;//deleting node and rearrange list
            free(temp);
        }
    }
}

void deleteFromChrone(char *name, Song **root) {
    Song * temp,*iter;
    iter=*root;
    if(strcmp((*root)->name,name)==0) {//the song we would remove is root so root gonna change
        temp = *root;
        *root=(*root)->chrone_next;
        free(temp);
        return;
    }else{//we  try to find song here
        while(iter->chrone_next !=NULL && strcmp(iter->chrone_next->name,name)!=0)
            iter=iter->chrone_next;
        if(iter->chrone_next==NULL && strcmp(iter->name,name)!=0)//song is not in list.
            printf("There is no song named as: %s\n\n",name);
        else{
            temp=iter->chrone_next;
            iter->chrone_next=iter->chrone_next->chrone_next;//deleting node and rearrange list
            free(temp);
        }
    }
}

void insertToRandom(Song song, Song **root) {
    Song *temp,*iter;
    iter=*root;
    temp = (Song*)malloc(sizeof(Song));//creating song(node) to insert
    strcpy((temp)->name,song.name);
    temp->duration=song.duration;

    if(*root==NULL){
        *root = temp;    //Creating list and changing root
        temp->random_next=NULL;
        return;
    }else{
        while(iter->random_next !=NULL )
            iter=iter->random_next;//Finding end of list.
        temp->random_next=NULL;//rearrange list
        iter->random_next=temp;

    }
}

void insertToChrone(Song song, Song **root) {
    Song *temp,*iter;
    iter=*root;
    temp = (Song*)malloc(sizeof(Song));
    strcpy((temp)->name,song.name);//creating song(node) to insert
    temp->duration=song.duration;

    if(*root==NULL){
        *root = temp;    //Creating list and changing root
        temp->chrone_next=NULL;
        return;
    }else{
        while(iter->chrone_next !=NULL )
            iter=iter->chrone_next;//Finding end of list.
        temp->chrone_next=NULL;//rearrange list
        iter->chrone_next=temp;

    }
}

void insertToAlphabetic(Song song, Song **root) {
    Song *temp,*iter;
    iter=*root;
    temp = (Song*)malloc(sizeof(Song));
    strcpy((temp)->name,song.name);//creating song(node) to insert
    temp->duration=song.duration;

    if(*root==NULL){
        *root = temp;
        temp->alpha_next=NULL;   //Inserting first song to list which is changes root
        return;
    }
    if(strcmp(((*root)->name),temp->name) == 1) {//Cehecking is new song's name comes before current root song's name with dictionary order.
        temp->alpha_next=*root;
        *root=temp;
    }else{//If its not comes before than root then we  gonna find suitable place to insert
        while(iter->alpha_next != NULL  && strcmp(iter->alpha_next->name,temp->name) == -1)
            iter = iter->alpha_next;
        if(iter->alpha_next==NULL) {//Here we check is song gonna be instert to tale or not
            temp->alpha_next=NULL;
            iter->alpha_next=temp;
        }else{
            temp->alpha_next = iter->alpha_next;//rearrange list
            iter->alpha_next = temp;
        }
    }
}

void insertToDuration(Song song, Song **root) {
    Song *temp,*iter;
    iter=*root;
    temp = (Song*)malloc(sizeof(Song));
    strcpy((temp)->name,song.name);//creating song(node) to insert
    temp->duration=song.duration;

    if(*root==NULL){
        *root = temp;
        temp->duration_next=NULL;   //Inserting first song to list which is changes root
        return;
    }
    //If new song's duration smaller than current smallest root gonna change here.
    if((*root)->duration > temp->duration){
        temp->duration_next=*root;
        *root=temp;
        //If its not smaller than root here we find suitable location
    }else{
        while(iter->duration_next !=NULL && (iter->duration_next->duration) < (temp->duration))
            iter = iter->duration_next;
        if(iter->duration_next ==NULL) {//if new song going to add tale
            temp->duration_next=NULL;
            iter->duration_next=temp;
            return;
        }
        //Here if songs going to add between 2 song
        temp->duration_next=iter->duration_next;//rearrange list
        iter->duration_next=temp;
    }


}

char* getDurasMinSec(int a){//this method simply take integer as time and return it format of mm:ss as char array length of 5
    char* duration= malloc(sizeof(char)*6);
    duration[5]='\0';
    duration[2]=':';
    duration[0]='0';
    duration[1]='0'+(a/60);
    duration[3]='0'+(a%60)/10;
    duration[4]='0'+(a%60)%10;
    return duration;

}

void insertNodeWithRoots(char * name, int duration, Song **alpha_root, Song **duration_root, Song **chrone_root, Song **random_root){
    //Creating new node to insert list.
    Song temp={'\0'};
    strcpy(temp.name,name);
    temp.duration=duration;//creating song(node) to insert

    insertToRandom(temp,random_root);
    insertToChrone(temp,chrone_root);//calling insert function for all nodes
    insertToAlphabetic(temp,alpha_root);
    insertToDuration(temp,duration_root);
}

void insertNode(char *name,int duration){//this method simply call original insert function to make easy use for user
    insertNodeWithRoots(name,duration,&alpha_root,&duration_root,&chrone_root,&random_root);
}

void printPlaylistsWithType(Song *root,char type){//this method prints all lists base on given type there is printAllList()f function which calls this method for all lists.
    int index=1;
    switch(type) {
        case'D':
            index=1;
            printf("The list in duration-time order:\n");
            while (root != NULL) {
                printf("\t%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));//Printing Duration based list.
                root = root->duration_next;
                index++;
            }
            printf("\n");
            break;
        case'A':
            index=1;
            printf("The list in alphabetical order:\n");
            while (root != NULL) {
                printf("\t%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));//Printing Alphabetic order based list.
                root = root->alpha_next;
                index++;
            }
            printf("\n");
            break;
        case'C':
            index=1;
            printf("The list in choronological order:\n");
            while (root != NULL) {
                printf("\t%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));//Printing choronological based list.
                root = root->chrone_next;
                index++;
            }
            printf("\n");
            break;
        case'R':
            shuffleRandomPlayList();//shuffling random list before print
            index=1;
            printf("The list in random order:\n");
            while (root != NULL) {
                printf("\t%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));//Printing random order based list.
                root = root->random_next;
                index++;
            }
            printf("\n");
            break;
        default:
            printf("Please give valid type parameter.('C' for chronical,'A' for alphabetic,'D' for duration,'R' for random list.");
            break;
    }
}

void printAllPlaylists() {//print function which calls original one for all lists
    printPlaylistsWithType(chrone_root,'C');
    printPlaylistsWithType(alpha_root,'A');
    printPlaylistsWithType(duration_root,'D');
    printPlaylistsWithType(random_root,'R');
}

char* getOnlyName(char* input) {
    char *name;
    int nameSize;
    for (nameSize = 0; nameSize < 60; nameSize++) {//here  we seperate given input whether name and duration seperated with \t or two ' ' or one ' ' character and base onthis stores total lenght of given song name
        if ((input[nameSize] ==' ' && input[nameSize + 1] ==' '))
            break;
        else if(input[nameSize]==' ' && ((input[nameSize + 1] - '0') <= 9 && (input[nameSize + 1] - '0') >= 0))
            break;
        else if((input[nameSize] == '\t') || (input[nameSize] == '\n') )
            break;
    }
    name= malloc(sizeof(char)*(nameSize+2));//creating new char array to store name of song
    memset(name,0,nameSize+2);
    name[nameSize+1]='\0';
    for(int i = 0 ;i<nameSize ;i++)//copying name of song o only name array
        name[i]=input[i];

    return name;

}

int stringToInt(char* duration){//this function simply calculate total second value of given time as format of mm:ss
    int k = (((duration[0]-'0')*10)+(duration[1]-'0'))*60+(((duration[3]-'0')*10)+duration[4]-'0');
    return k;
}

int getOnlyDuration(char* input,int nameSize){//this method seperates duration part of given input with using name lenght of song which we calculate before in getOnlyName() function.
    char* duration=malloc(sizeof(char)*6);
    duration[5]='\0';//here we create duration chararray which will store duration of song as formta of mm:ss
    int iter=0;
    for(int i = nameSize;i <= strlen(input)-1;i++)//we seperate song's duration part.
        if( input[i]!=' ' && input[i]!='\t') {
            duration[iter] = input[i];
            iter++;
        }
    return stringToInt(duration);//and we send this duration as format of mm:ss to stringToInt() functin which calculates time string as  second with integer datatype.
}

