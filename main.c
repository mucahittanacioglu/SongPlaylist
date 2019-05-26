#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Song{
    char name[25];
    int duration;
    struct Song *chrone_next;
    struct Song *alpha_next;
    struct Song *duration_next;
    struct Song *random_next;
}typedef Song;
Song *random_root=NULL,*alpha_root=NULL,*duration_root=NULL,*chrone_root=NULL;

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

int main() {
    static char filename[] = "songs.txt";
    int option=10;
    char *nameForMenu,*durationForMenu;
    char *inputSong;

    readSongsFromFile(filename);

    do{
        printf("Enter your choice:\n"
               "1 to insert a song into\n"
               "2 to delete a song from\n"
               "3 to print the songs in the list\n"
               "4 to print the songs to an output file\n"
               "5 to end.\n?");
        scanf("%d",&option);
        switch(option){
            case 1:
                printf("\nEnter a song name with duration:\n");

                printf("%s",inputSong);
                //nameForMenu=getOnlyName(inputSong);
                //durationForMenu=getOnlyDuration(inputSong,strlen(getOnlyName(inputSong)));
               // insertNode(nameForMenu,durationForMenu);
                break;
            case 2:
                break;
            case 3:
                printAllPlaylists();
                break;
            case 4:
                break;
            default:
                break;
        }
    }while(option!=5);

    return 0;
}
void readSongsFromFile(char* path){
    char *name;

    //int dur;
    FILE *filePtr = fopen ( path, "r" );
    if (filePtr != NULL){
        char line [ 128 ];
        while ( fgets ( line, sizeof line, filePtr ) != NULL ){
            name=getOnlyName(line);
            //dur=getOnlyDuration(line,strlen(name));
            insertNode(getOnlyName(line),getOnlyDuration(line,strlen(name)));
        }
        fclose ( filePtr );
    }else{
        perror ( path );
    }
}

void deleteNode(char *name) {
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
            iter->random_next=iter->random_next->random_next;
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
            iter->duration_next=iter->duration_next->duration_next;
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
            iter->alpha_next=iter->alpha_next->alpha_next;
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
            iter->chrone_next=iter->chrone_next->chrone_next;
            free(temp);
        }
    }
}

void insertToRandom(Song song, Song **root) {

    if(*root==NULL){
        *root = (Song*)malloc(sizeof(Song));
        strcpy((*root)->name,song.name);
        (*root)->duration=song.duration;

        (*root)->random_next=NULL;
    }
}

void insertToChrone(Song song, Song **root) {
    Song *temp,*iter;
    iter=*root;
    temp = (Song*)malloc(sizeof(Song));
    strcpy((temp)->name,song.name);
    temp->duration=song.duration;

    if(*root==NULL){
        *root = temp;    //Creating list and changing root
        temp->chrone_next=NULL;
        return;
    }else{
        while(iter->chrone_next !=NULL )
            iter=iter->chrone_next;//Finding end of list.
        temp->chrone_next=NULL;
        iter->chrone_next=temp;

    }
}

void insertToAlphabetic(Song song, Song **root) {
    Song *temp,*iter;
    iter=*root;
    temp = (Song*)malloc(sizeof(Song));
    strcpy((temp)->name,song.name);
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
            temp->alpha_next = iter->alpha_next;
            iter->alpha_next = temp;
        }
    }
}

void insertToDuration(Song song, Song **root) {
    Song *temp,*iter;
    iter=*root;
    temp = (Song*)malloc(sizeof(Song));
    strcpy((temp)->name,song.name);
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
        temp->duration_next=iter->duration_next;
        iter->duration_next=temp;
    }


}
char* getDurasMinSec(int a){
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
    temp.duration=duration;
    insertToRandom(temp,random_root);
    insertToChrone(temp,chrone_root);
    insertToAlphabetic(temp,alpha_root);
    insertToDuration(temp,duration_root);
}

void insertNode(char *name,int duration){
    insertNodeWithRoots(name,duration,&alpha_root,&duration_root,&chrone_root,&random_root);
}

void printPlaylistsWithType(Song *root,char type){
    int index=1;
    switch(type) {
        case'D':
            index=1;
            printf("The list in duration-time order:\n");
            while (root != NULL) {
                printf("%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));
                root = root->duration_next;
                index++;
            }
            printf("\n");
            break;
        case'A':
            index=1;
            printf("The list in alphabetical order:\n");
            while (root != NULL) {
                printf("%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));
                root = root->alpha_next;
                index++;
            }
            printf("\n");
            break;
        case'C':
            index=1;
            printf("The list in choronological order:\n");
            while (root != NULL) {
                printf("%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));
                root = root->chrone_next;
                index++;
            }
            printf("\n");
            break;
        case'R':
            index=1;
            printf("The list in random order:\n");
            while (root != NULL) {
                printf("%d. %s\t%s\n",index,root->name,getDurasMinSec(root->duration));
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

void printAllPlaylists() {
    printPlaylistsWithType(chrone_root,'C');
    printPlaylistsWithType(alpha_root,'A');
    printPlaylistsWithType(duration_root,'D');
    printPlaylistsWithType(random_root,'R');
}

char* getOnlyName(char* input){
    char* name;
    int nameSize;
    for(nameSize = 0 ;nameSize<60;nameSize++)
        if((input[nameSize]==' ' && input[nameSize+1]==' ') || input[nameSize]=='\t')
            break;

    name= malloc(sizeof(char)*(nameSize+1));
    name[nameSize]='\0';
    for(int i = 0 ;i<nameSize;i++)
        name[i]=input[i];
    return name;

}

int stringToInt(char* duration){
    int k = (((duration[0]-'0')*10)+(duration[1]-'0'))*60+(((duration[3]-'0')*10)+duration[4]-'0');
    return k;
}

int getOnlyDuration(char* input,int nameSize){
    char* duration=malloc(sizeof(char)*6);
    duration[5]='\0';
    int iter=0;
    for(int i = nameSize;i <= strlen(input)-1;i++)
        if( input[i]!='\t') {
            duration[iter] = input[i];
            iter++;
        }
    return stringToInt(duration);
}

