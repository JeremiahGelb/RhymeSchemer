/* 
 * File:   RhymeSchemer.c
 * Author: Jeremiah
 *
 * Created on December 20, 2016, 11:31 PM
 * 
 * 
 */

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define dictionary "C:/Users/Jeremiah/Desktop/rhyme_schemer_docs/dictionary.txt" // Change to to the directory where you saved the dictonary
#define engdoc "C:/Users/Jeremiah/Desktop/rhyme_schemer_docs/bible.txt" // Change to the directory where you saved the bible
#define dictionarylength 133779    // http://www.speech.cs.cmu.edu/cgi-bin/cmudict 
#define maxprecs 3334 //How many preceders each word has 
#define maxlines 50 // Max lines in a poem


#define EXCH(x,y) ({struct Word* tmp; tmp = x; x = y; y = tmp;}) // Switches two word* in the word* array
#define LESS(x,y) (strcmp(x->spelling,y->spelling)<0) // Compares values of two words alphabetically

struct preceder{ // Holds a word that has come before another word
    int count; // How many times
    struct Word* word; // What it is
};

struct Word{ // Holds a word 
    char spelling[35]; // Spelling of the word
    int phonemecount; // How many phonemes (sounds)
    int syllables; // How many syllables
    int precedercount; // How many unique words come before it in the text
    char phonemes[33][4]; // Array of strings that hold the phonemes
    struct preceder preceders[maxprecs]; // Array of preceders
};

struct line{ // Holds information about a line of poetry
    int syllables; // How many syllables are in the line
    struct Word * rhymer; // What word the line needs to rhyme with
};
    

char* Allup(char string[]); //Converts words to all uppercase
int isVowel(char*); // Returns 1 if the phoneme is a vowel
struct Word* findWord(char* theword, struct Word* Words[],int size); // Returns pointer to word in the word* array
int lightSort(struct Word* Words[], int size); // Sorts the word* array using one pass of bubble sort, returns 1 if it changed anything 
                                                // Very quick if it's already very close to sorted (as the default dictionary is)

int rhymesWith(struct Word* word1, struct Word* word2); // Returns 1 if the words rhyme 0 else
struct Word* choosePreceder(struct Word* wrd); // Returns pointer to preceder, or NULL if none found
void fillDictionary(struct Word* Words[]); // Fills up spelling, phonemecount, syllables, phonemes
void cleanup(struct Word* Words[], int size); // Frees all allocated memory
void learnEnglish(struct Word* Words[]); // Reads the text to fill preceders
int placePreceder(struct Word* Words[], struct Word* preceder, struct Word* current); // Returns 1 if successfull, 0 else

void genSentence(int syllables, struct Word* endswith, struct Word* Words[]); // Prints a sentence to std out ending with the chosen word
struct Word* findRhymer(struct Word* wrd, struct Word* Words[]); // Randomly finds a word that rhymes
void genPoem(struct Word* Words[]); // Prompts the user and generates a poem

/*TESTERS*/
void rhymetest(struct Word* Words[]);

int main() {
    
    struct Word* Words[dictionarylength]; // This is the number of words in the dictionary
    
    srand(time(NULL)); // Seed rng
    fillDictionary(Words); // FIll up words array from dictionary file
    while(lightSort(Words,dictionarylength)); /* Sorts dictionary - WILL TAKE EONS ON LONG UNSORTED DICTIONARIES */
    learnEnglish(Words);    // Reads the text to figure out preceders
    
    while(1){
        char c[2];
        int syls;
        char wrd[50];
        struct Word * rhymer;
        
        printf("p for poem , q for quit: ");
        scanf("%1s",c);
       
        if(c[0]=='q'){
            break;
        }
        
        if(c[0]=='p'){
            genPoem(Words);            
        }
        else continue;
    }
     
    
    cleanup(Words,dictionarylength); // Free all the used memory
    return 0;
}

char* Allup(char string[]){
    int i;
    for(i=0;i<strlen(string);i++){
        string[i] = (char)toupper((int)string[i]);
    }
    return string;
}

int isVowel(char* phoneme){
    char c = phoneme[0];
    if(c=='A'||c=='E'||c=='I'||c=='O'||c=='U')return 1; // If the first character of the phoneme is a vowel
    else return 0;  
}

struct Word* findWord(char* theword, struct Word* Words[],int size){ // Binary search
    int whichway;
    
    whichway = strcmp(theword,((Words[size/2])->spelling)); // Compares the word to middle of array
   
    if(whichway==0) return Words[size/2]; // If you found the word, return a pointer to it
    
    if (size==0) return NULL; // Fail if the array has no size
    
    if (whichway>0) return findWord(theword, Words+(size/2)+1, (size/2)-1); // If the word is greater than the middle, search the right half (recursively)
    else return findWord(theword, Words, (size/2)); // Else search the left half (recursively)
   
}

int lightSort(struct Word* Words[], int size){ // Runs one pass of bubble sort, returns 1 if it did something (Quick for nearly sorted arrays)
    int i = 0;
    int didsomething = 0;
    for(i=0;i<size-1;i++){
        if(LESS(Words[i+1],Words[i])){
            EXCH(Words[i+1],Words[i]);
            didsomething = 1;
        }
    }
        return didsomething;
}

int rhymesWith(struct Word* word1, struct Word* word2){
    struct Word * shorter, * longer;
    int i = 1;
    if(word1->phonemecount>word2->phonemecount){ // Figure out which one has more phonemes
        longer = word1;
        shorter = word2;
    }
    else{
        longer = word2;
        shorter = word1;
    }
    
    while(!isVowel(shorter->phonemes[shorter->phonemecount-i])) i++; // Sets i to be offset from phonemecount to the last vowel in shorter
    
    while(i>0){
        if(strcmp(shorter->phonemes[shorter->phonemecount-i],longer->phonemes[longer->phonemecount-i])){ //If any of the corresponding phonemes are mismatched
            return 0;
        }
        i--;
    }
    
    return 1;
}

struct Word* choosePreceder(struct Word* wrd){
    int divider = 0;
    int dividers[maxprecs] = {0};
    int i=0;
    
    /* Fill up dividers with divider count */
    
    for(i=0;i<wrd->precedercount;i++){
        divider += (wrd->preceders[i]).count;
        dividers[i] = divider;
    }
        
    divider = (rand()%dividers[i-1]); /* Generate a random number between 0 and #of occurences */
    
     for(i=0;i<wrd->precedercount;i++){
         if(divider<dividers[i]) return (wrd->preceders[i]).word;
     }
    
    return NULL;
}

void rhymetest(struct Word* Words[])
{
     while(1)
    {
        int cont;
        printf("continue? ");
        scanf("%d",&cont);
        if(!cont) break;
        
        char wrd1[50];
        char wrd2[50];
        
        printf("First word? ");
        scanf("%50s",wrd1);
        printf("Second word? ");
        scanf("%50s",wrd2);
        
        struct Word *tempay1, *tempay2;
        tempay1 = findWord(Allup(wrd1),Words,dictionarylength);
        tempay2 = findWord(Allup(wrd2),Words,dictionarylength);
        
        if(tempay1 == NULL){
            printf("Unable to find word: '%s'\n",wrd1);
            continue;
        }
        
        if(tempay2 == NULL){
            printf("Unable to find word: '%s'\n",wrd2);
            continue;
        }
        
        if(rhymesWith(tempay1,tempay2)){
            printf("Yes, %s rhymes with %s\n",wrd1,wrd2);
        }
        
        else printf("NO, %s DOESN'T RHYME with %s\n",wrd1,wrd2);
        
    }
}

void fillDictionary(struct Word* Words[]){
    char temp[40]; // Holds the spelling of words as they are read in
    char tempchar = 0; // Holds a character
    
    int i = 0; // Counter
    int j = 0; // Counter
    
    FILE * fp;
    fp = fopen(dictionary,"r"); // Opening Dictionary File
    
    if (fp==NULL) printf("Error Opening File \n");
    
    
    while (1){ 
        fscanf(fp,"%s",temp);
        Words[i] = (struct Word*) calloc(1,sizeof(struct Word)); //Allocates memory for new word
        strcpy(Words[i]->spelling, temp); // puts spelling in word
        
        j = 0;
        tempchar = 0;
        while(tempchar != '\n'){ // For every phoneme
            fscanf(fp,"%s", Words[i]->phonemes[j]); // Scan in the phoneme
            Words[i]->phonemecount++; // Add to the phoneme count
            if(isVowel(Words[i]->phonemes[j])){
              Words[i]->syllables++; // If phoneme is vowel increase syllables
              Words[i]->phonemes[j][2] = (char)0; // Deletes stress of vowel
            } 
            j++; // Go down a line
            tempchar = fgetc(fp);  //Eat the whitespace character to check if the line is over
            if (tempchar==EOF) break;
        }
            if (tempchar==EOF) break;
        i++; // Put it in the next slot of the array
    }
    
    fclose(fp);
    
}

void cleanup(struct Word* Words[], int size){
    for(int i = 0; i<size ; i++){
        free(Words[i]);
    }
}

void learnEnglish(struct Word* Words[]){
    char temp[40]; // Holds the spelling of words as they are read in
    char tempchar = 0; // Holds a character
    
    int i = 0; // Counter
    
    
    FILE * fp;
    fp = fopen(engdoc,"r"); // Opening Dictionary File
    
    if (fp==NULL) printf("Error Opening engdoc File \n");
    
    char current[40] = {0};
    char preceder[40] = {0};
    struct Word * curp; // Pointer to the word that current holds
    struct Word * perp; // Pointer to the word that preceder holds
    
    char c;
    int new_sent = 1;
    
    while(fscanf(fp,"%s",current)!=EOF){
        //printf("%s ",current);
        if(new_sent){
            preceder[0] = (char)0; // At the be
            new_sent = 0;
        }
        
        c = current[strlen(current)-1]; // Checking the last character of the current word;
        
        if((c<'A')||(c>'z')||(c>'Z')&&(c<'a')){ // If the last character is non letter
            if((c=='.')||(c=='?')||(c=='!')||(c==';')){
                new_sent = 1;
            }
            current[strlen(current)-1] = (char)0;
        }
        
        curp = findWord(Allup(current),Words,dictionarylength);
        
        if(curp==NULL){
            //printf("%s wasn't in the dictionary \n",current);
            preceder[0] = (char)0;
            continue;
        }
        
        if(preceder[0]){
            perp = findWord(Allup(preceder),Words,dictionarylength);
            if(!placePreceder(Words, perp, curp)){
                printf("Failed to put %s behind %s \n",preceder,current);
            }   
        }
        
      strcpy(preceder,current);  
        
    }
    
    
}


int placePreceder(struct Word* Words[], struct Word* preceder, struct Word* current){
    int i;
    
    for(i=0;i<maxprecs;i++){
        
        if((current->preceders[i]).word == preceder){ // If the preceder is already in the list 
            (current->preceders[i]).count++; // Increase the count
            return 1; // we did it!
        }
        
        if((current->preceders[i]).word == NULL){ // If you come to an empty space
            
            (current->preceders[i]).word = preceder; // Put in the preceder
            (current->preceders[i]).count++; // Increase the count
            current->precedercount++;
            return 1; // we did it!
        }
        
    }
    return 0;
}

void genSentence(int syllables, struct Word* endswith, struct Word* Words[]){
     struct Word * sentence[50];
        int i = 0;
        int trycount = 0;
        int sylcpy = syllables;
        struct Word* current;
        current = endswith;
        
        while((current->syllables)>syllables){
            current = findRhymer(endswith,Words);
        }
        
        
        
        while(sylcpy>0){
   
            if(trycount>(maxprecs)){
                return genSentence(syllables,endswith,Words);
            }
            
            if(current->syllables <= sylcpy){
                trycount = 0;
                sentence[i] = current;
                sylcpy -= current->syllables;
                current = choosePreceder(current);
                i++;
            }
            else{
                current = choosePreceder(sentence[i-1]);
                trycount++;
            }
            
            }
       
        i--;
        while(i>=0){
            printf("%s ",sentence[i--]->spelling);
        }
        printf("\n");
}
struct Word* findRhymer(struct Word* wrd, struct Word* Words[]){
    int randpicker;
    int max_picks = dictionarylength*10;
    int i;
    
    for(i=0;i<max_picks;i++){
        randpicker = (rand()%dictionarylength);
        if (rhymesWith(wrd,Words[randpicker]) && Words[randpicker]->preceders[0].count){
            return Words[randpicker];
        }
    }
    return NULL;
    
}

void genPoem(struct Word* Words[]){
    struct line lines[maxlines] = {0};
    
    int linecount;
    int i;
    char wrd[50];
    struct line tmp;
    
    
    printf("How many lines should the poem be? (<= maxlines)  : ");
    scanf("%d",&linecount);
    
    for(i=0;i<linecount;i++){
        while(1){
            printf("Line %d, Syllables then rhyme word: \n",(i+1));
            scanf("%d %s",&tmp.syllables,wrd);
            tmp.rhymer = findWord(Allup(wrd),Words,dictionarylength);
            if(tmp.rhymer != NULL){
                lines[i] = tmp;
                break;
            }
            else printf("Unable to find word %s\n",wrd);
        }          
    }
    printf("\n");
    for(i=0;i<linecount;i++){
        genSentence(lines[i].syllables , findRhymer(lines[i].rhymer , Words) , Words);
    }
    
    
    printf("\n");
}