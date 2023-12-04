#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ROLLS 3
#define MAX_DICE 5
#define MAX_PLAYERS 10
#define MAX_NAME_LENGTH 50
#define SENTINEL 0

void howToPlay(){
	printf("Yahtzee is a game played with 5 dice. The objective is to get the highest score possible. With the five dice you can shake up to 3 times to get ");
	}

void commands(){
	printf("Here is the list of commands to use while playing this game:\nRules: Lists the rules of Yahtzee\nScore: Prints out the score of each player\nCommands: Will reprint this list\n");
	}

void printCard(int score[MAX_PLAYERS][15],int player){
	printf("1.  Aces:           %d\n", score[player][1]);
        printf("2.  Twos:           %d\n", score[player][2]);
        printf("3.  Threes:         %d\n", score[player][3]);
        printf("4.  Fours:          %d\n", score[player][4]);
        printf("5.  Fives:          %d\n", score[player][5]);
        printf("6.  Sixes:          %d\n", score[player][6]);
        printf("7.  3 of a Kind:    %d\n", score[player][7]);
        printf("8.  4 of a Kind:    %d\n", score[player][8]);
        printf("9.  Full House:     %d\n", score[player][9]);
        printf("10. Short Straight: %d\n", score[player][10]);
        printf("11. Long Straight:  %d\n", score[player][11]);
        printf("12. Yahtzee:        %d\n", score[player][12]);
        printf("13. Chance:         %d\n", score[player][13]);
	}

int rollDie(){
	return rand() % 6 + 1;
}

void yahtzee(int score[MAX_PLAYERS][15], char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH],int numPlayers){
	int rounds = 1;
	sleep(3);
	while(rounds <= 13){
		system("clear");
		sleep(1);
		printf("It is round %d out of 13\n", rounds);
		int player = 0;
		while(player < numPlayers){//while loop that goes through players
			int array[5][2];
			printf("%d\n",score[player][0]);
			int reroll[5];
			int input;
			int roll = 1;

			printf("It is %s's turn:\n",playerNames[player]);
			printCard(score,player);
			while(roll < MAX_ROLLS ){//while loop for rolling and rerolling dice
				printf("Rolling dice. . . . .\n");
				sleep(1);
				for(int i = 0; i < 5; i ++){
					if(roll == 1){
						array[i][1] = rollDie();
						array[i][2] = 0;
						//array[i][1] = i;
					}
					reroll[i] = 0;
					printf("%d ", array[i][1]);
				}
				printf("\nType in the dice you want to reroll with spaces between them. Type 0 when you are finshed to roll typed dice.\n");

				while(1){
					scanf("%d", &input);
					if(input == SENTINEL){
						break;
					}
					else{
						for(int i = 0; i < 5; i++){
							if(input == array[i][1] && array[i][2] == 0){//valid reroll
								reroll[i] = input;
								array[i][2] = 1; //So the same number cant be rerolled
								i = 5 ;
							}
							if(i ==4){
								printf("%d is not in the dice you rolled\n", input);
							}
						}
					}
				}
				for(int i = 0; i < 5; i++){//resetting dummy value used to prevent the same number being rerolled multiple times
                                        array[i][2] = 0;
                                }
				for(int i = 0; i < 5; i++){//rerolls dice that were selected
					if(reroll[i] != 0){
						array[i][1] = rollDie();
					}
				}
				roll++;
			}
			for(int i = 0; i < 4; i ++){
				for(int j = 0; j < 4 - i; j++){
					if(array[j][1] > array[j + 1][1]){
						int temp = array[j][1];
						array[j][1] = array[j+1][1];
						array[j+1][1] = temp;
					}
				}
			}
			sleep(0.5);
			printf("The scoring dice are:\n");
			for(int i = 0; i < 5; i ++){
                        	printf("%d  ", array[i][1]);
			}
			printf("\n");
			printCard(score,player);

			while(1){
			scanf("%d",&input);
			if(0 > input && input > 14 || score[player][input] != 0){
				printf("%d is an invalid input\n", input);
			}
			else if(0 < input && input < 14  && score[player][input] == 0){
				if( 0 < input && input < 7 ){
					for(int i = 0; i < 5; i++){
						if(array[i][1] == input){
							score[player][input] += input;
						}
					}
					score[player][0] += score[player][input];
					//break;
				}
				else if( 6 < input && input < 14 ){//The bottom half the score card
					if( input == 7 || input == 8){//3 & 4 of a kind
						int one = 0;
						for(int i = 0; i < 5; i++){
							int counter = 0;
							for(int j = 0; j < 5; j++){
								if(j == i){
									continue;
								}
								else if(array[i][1] == array[j][1]){
									counter++;
									printf("%d\n",counter);
									if((counter >= 2 && input == 7) || (counter >= 3 && input ==8)){
		                                                        	for(int w = 0; w < 5; w++){
                	                                                	score[player][input] += array[w][1];
										one++;
										}
                        	                                        	break;
									}
								}
							if(one > 1){
								break;
							}}
							//score[player][input] = 0;
						}
					}
				else if(input == 9 ){//fullhouse
					int counter = 0;
					int uniqueValues = 5;
					for(int i = 0; i < 4; i++){
						int dumby = 0;
						for(int j = 0; j < 5; j++){
							if(j == i){
								continue;
							}
							else if(array[i][1] == array[j][1]){
								uniqueValues --;
								counter++;
							}
							else{
								if(dumby == array[j][1]){
									uniqueValues --;
								}
								dumby = array[j][1];
							}
						}
						if(uniqueValues == 2 && (counter == 1 || counter == 2)){
							score[player][9] = 25;
							break;
						}
						else{
							score[player][9] = 0;
						}
					}
				}
				else if((input == 10 || input == 11)){//short & long straight check if the dice are in order
					int count = 0;
					for(int i = 0; i < 5; i++){
						if(array[i][1] + 1 == array[i+1][1]){
							count++;
						}
					}
					if(count >= 3 && input == 10){
						score[player][10] = 30;
					}
					else if(count == 4 && input == 11){
						score[player][11] = 40;
					}
					else{
						score[player][input] = 0;
					}
					//break;
				}
				else if(input == 12 ){//Yahtzee check if all the dice are the same
					for(int i = 0; i < 3; i++){
						if(array[i][1] != array[i+1][1]){
							score[player][12] = 0;
							break;
						}
						else if(i == 2){
							score[player][12] = 50;
						}
					}
				}
				else if(input == 13 ){//Chance add all the dice by interating through dice array and adding to score
					for(int i = 0; i < 5; i++){
						score[player][13] += array[i][1];
					}
				}
				score[player][14] += score[player][input];
				printf("%d was added to bottom\n",score[player][14]);
				}
				printf("%d was added to %s's score card in row %d\n", score[player][input],playerNames[player],input);
				printCard(score,player);
				player++;
				//printCard(score,player);
				sleep(5);
				system("clear");
				break;}
			}

		}
		rounds++;
	}
	for(int i = 0; i < numPlayers; i++){
			if(score[i][0] == 63 || score[i][0] > 63){
				score[i][0] += 35;
			}
		printf("%s's card:\n",playerNames[i]);
		printCard(score,i);
		printf("%s's final score:\nTop: %d\nBottom: %d\nTotal: %d",playerNames[i], score[i][0], score[i][14],score[i][0] + score[i][14]);
	}
	exit(1);
}



int main(){
	srand((unsigned int)time(NULL));
	char playerNames[MAX_PLAYERS][MAX_NAME_LENGTH];
	int numPlayers;

	printf("Enter the number of players (up to %d): ", MAX_PLAYERS);
	scanf("%d", &numPlayers);

	if(numPlayers < 1 || numPlayers > MAX_PLAYERS){
		fprintf(stderr, "Invalid number of players. Exiting. . .\n");
		return 1;
	}


	while (getchar() != '\n');

	for(int i = 0; i < numPlayers; i++){
		printf("Enter the name of Player %d: ", i + 1);
		fgets(playerNames[i], MAX_NAME_LENGTH, stdin);

		size_t len = strlen(playerNames[i]);
		if(len > 0 && playerNames[i][len - 1] == '\n'){
			playerNames[i][len - 1] = '\0';
		}
	}

	printf("\nPlayer Names:\n");
	for(int i = 0; i < numPlayers; ++i){
		printf("Player %d: %s\n", i + 1, playerNames[i]);
	}

	int score[numPlayers][15];
	for(int i = 0; i < numPlayers; i++){
		for(int j = 0; j < 15; j++){
			score[i][j] = 0;
		}
	}

	yahtzee(score,playerNames,numPlayers);

	return 0;
}




