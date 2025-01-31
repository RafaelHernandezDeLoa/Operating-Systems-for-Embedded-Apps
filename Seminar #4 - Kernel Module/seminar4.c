#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>


#include "ioct_test.h"

static char hangman[1000] ="\n            \n             \n             \n              \n              \n __________________\n\n            \n|            \n|            \n|             \n|             \n|__________________\n\n ___________\n|            \n|            \n|             \n|             \n|__________________\n\n ___________\n|           |\n|            \n|             \n|             \n|__________________\n\n ___________\n|           |\n|           O\n|             \n|             \n|__________________\n\n ___________\n|           |\n|           O\n|           | \n|             \n|__________________\n\n ___________\n|           |\n|           O\n|          /| \n|             \n|__________________\n\n ___________\n|           |\n|           O\n|          /|\\\n|             \n|__________________\n\n ___________\n|           |\n|           O\n|          /|\\\n|          /  \n|__________________\n\n ___________\n|           |\n|           O\n|          /|\\\n|          / \\\n|__________________\n";  

int main()
{
    srand(time(NULL));
    int rand_num =(rand() % 7) + 1;

    char word [10];

    //printf ("%d\n", rand_num);
    //printf ("%s\n",an);

    //open the module
    int fd = open("/dev/seminar_mod",O_RDWR);
    if (fd == -1){
        printf("ERROR\n");
        return -1;
    }
    //send the random number to the kernel so a word can be generated from the word bank
    ioctl(fd, WR_VALUE, &rand_num);

    //Read the generated word from the kernel (saved in output)
    printf("Now receiving word from device;\n");
    
    memset(word, 0, strlen(word));
    read(fd,word,10);
    //printf("%s\n",word);
    
    close(fd);

    //start of hangman application

    printf("Welcome to Hangman!\n");// going to use module to keep word and change word when done and clear its buffer (to prevent write overflow...) with help of ioctl
    //note: I haven't implemented the 
    printf("You get 9 guesses to guess the word. If you get a guess correct, you still have the same amount of guesses as you had before.\n");
    printf("\n");
    //char word[] = "random word generator function here"; //insert a word from module here... I made cmd 2
    for (int i = 0; i< 92; i++)
        printf("%c", hangman[i]);
    printf("Your word is: ");
    int display_size = sizeof(word)/sizeof(*word)*2;
    char display[display_size];

    int num_letters = 0;
    
    for (int i = 0; i<sizeof(word)/sizeof(*word); i++) {
        if (word[i] == ' '|| word[i] == '\0')//(strcmp(word[i]," ")==0)
        {
            printf("  ");
            display[i*2] = ' ' ;
            display[i*2+1] = ' ';
        }
        else {
            printf("_ ");
            display[i*2] = '_';
            display[i*2+1] = ' ';
            ++num_letters;
        }
    }
    printf("\n");

    int errors = 0;
    int successes = 0;
    char guess;
    int win = 0;
    char wrong_guesses[26];
    int flag;

    
    while((num_letters != successes) && errors < 9)
    {   flag = 1;
        printf("enter a lowercase letter (you can also type * to quit): ");
        
        scanf("%s", &guess);
        while (flag == 1){
            for (int i = 0; i < 9; ++i){
                if(guess == wrong_guesses[i*2] || guess == display[i]){
                    flag = 1;
                    printf ("You've entered the letter '%c' already\n", guess);
                    printf("Enter another lowercase letter: ");
                    scanf("%s", &guess);
                    break;
                }
                else flag = 0;
            }
        }

        if (guess == '*'){//(strcmp(guess, "*") == 0){
            win = -1;
            printf("quitting\n");
            break;
        }
        else if((int)guess >= (int)'a' && (int)guess <= (int)'z') {
            int old_num_of_suc = successes;
            for (int i = 0; i < sizeof(word)/sizeof(*word); i++)
            {
                if((int)guess == (int)word[i])//(strcmp(guess, word[i]) == 0)
                {
                    successes++;
                    display[i*2] = guess;
                }
            }
            if (old_num_of_suc == successes && successes != num_letters)
            {
                printf("\nThere is no '%c' in this word\n\n", guess);//mention no letter of sorts in this word and need counter to lower the amount of guesses left
                wrong_guesses[errors*2] = guess;
                wrong_guesses[errors*2 + 1] = ' ';
                errors++;
            }
            else if (successes - old_num_of_suc == 1){
                printf("\nyour guess '%c' is in this WORD!\n\n", guess);
                if (successes == num_letters)
                    win = 1;
            }
            else {
                printf("\nThere are %d '%c's in this WORD!\n\n", successes - old_num_of_suc, guess);
                if (successes == num_letters)
                    win = 1;
            }
            for (int i = 92* errors; i< 92 * errors + 92; i++)
                printf("%c", hangman[i]);
            printf ("You have %d guesses remaining.\n ", 9-errors);
            printf ("Wrong Guesses: %s\n\n", wrong_guesses);
            printf("Your word is: ");
            for (int i = 0; i<sizeof(word)/sizeof(*word); i++) {
                printf("%c ",display[i*2]);
            }
            printf("\n\n");
            
            //print words I have left and the letters called out... and the errors made previously...
            //note: refer to end of last example for how to output from module...
        }
        else{
            printf("Invalid input entered. Try again.\n\n");
        }

        scanf("%c", &guess); //the thing to get rid of /0
    }
    if (win == 1)
        printf("congrats! You guessed the word!\n The word is %s\n\n", word);
    else if (win == 0)
        printf("You lost.\nThe word was %s\n\n", word);

    return 0;
}
