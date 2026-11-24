#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#define GREEN "\033[42m" //green background     |
#define YELLOW "\033[43m" //yellow background   |----> macros
#define GRAY "\033[100m" // gray nackground     |
#define RESET "\033[0m" // back to normal       |
#define TEXT_GREEN "\033[1;32m"
#define TEXT_RED "\033[1;31m"


void print_feedback(const char *guess, const char *letter_store)
{
    for(int i = 0; i < 5; i++)
    {
        if(letter_store[i] == 'G')
            printf(GREEN);
        else if(letter_store[i] == 'Y')
            printf(YELLOW);
        else
            printf(GRAY);

        printf(" %c " RESET, guess[i]);
    }
    printf("\n");
}


int is_valid(const char *guess, const char list[][6], int count)
{
    int found = 0;

    for(int i = 0; i < count; i++)
    {
        if (strcmp(guess, list[i]) == 0)
        {
            return 1;
        }

    }
    return 0;

}





int main(void) 
{

    char list[20000][6];

    int count = 0;

    char line[64];

    char letter_store[5];

    char guess[64];

    int attempts = 0;

    bool won = false;

    FILE *fp = fopen("words.txt", "r");

    if (fp == NULL)
    {
        printf("couldn't open the file\n");
        return 1;
    }

    while(fgets(line, sizeof line, fp) != NULL && count < 20000)
    {   
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) != 5) continue;
        strcpy(list[count], line);
        count++;
    }
    fclose(fp);

    srand(time(NULL));

    const char *word = list[rand() % count];

while(attempts < 6) 
{
    int counts[26] = {0};

    printf("guess your word: ");
    scanf("%63s", guess);

    if (strlen(guess) != 5)
    {
        printf(TEXT_RED "enter a 5 letter word\n" RESET);
        continue;
    }

    for (int i =0; i < 5; i++)
    {
        guess[i] = tolower(guess[i]);
    }

    //check the user word is legit
   if (!is_valid(guess, list, count))
   {
        printf("the word is not in wordlist\n");
        continue;
   }


    /*to intialize everything to '-' */
    for (int i = 0; i < 5; i++)
    {
        letter_store[i] = '-';
    } 

    /*Green pass*/
    for (int i = 0; i < 5; i++)
    {
        if (guess[i] == word[i])
        {
            letter_store[i] = 'G';
            counts[guess[i] - 'a']--; /*find the letter slot and if it matches subtract one from its tally*/
        }

    }

    for (int i = 0; i < 5; i++)
    {
        counts[word[i] - 'a']++; /*find the letter slot and add one to its tally*/

    }


    /*yellow pass*/
    for (int i =0; i < 5; i++)
    {
        if (letter_store[i] == '-' && counts[guess[i] - 'a'] > 0) /*if the letter slot is still greater than 0 in tally*/
        {
            letter_store[i] = 'Y';
            counts[guess[i] - 'a']--;
        }
    }

    /*color feedback*/
    print_feedback(guess, letter_store);


attempts++;

if (strcmp(guess, word) == 0)
{
        printf(TEXT_GREEN "congratulations! \n" RESET);
        won = true;
        break;
}

}
if(!won)
    printf(TEXT_RED "game over!\nthe word was: %s\n" RESET, word);

}
