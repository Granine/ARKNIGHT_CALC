/*
* Author: GRANINE
* Date Start: Oct 31, 2020
* Last Modify: Jan, 02, 2022
* Drop Data time: Dec, 24, 2020, GLOBAL server
* Purpuse:	1: calculate sanity return per stage based on document input or keyboard input and print to document
*		2: Calculate store item sanity efficiency -UD
*
* Tasks remain:
*	3. Create new calculation mode for different certificate stores
*/
/* Item code lookup table
* 01=Kohl
* 02=Manganese
* 03=Grindstone
* 04=RMA70
* 05=Orirock
* 06=Device
* 07=Polyester
* 08=Sugar
* 09=Oriron
* 10=Keton
* 11=Gel
* 12=Alloy
* 13=Part(Furnature Part)
* 14=Skill
* 15=Records
* 16=LMD
* 17=1Reserved
* 18=2Reserved
* 19=3Reserved
* 20=4Reserved
*/


#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Special constants
#define TRUE 1
#define FALSE 0
#define MAIN 0
#define END 1
#define RUN 2
#define HOLDER 0
#define SAME 0
#define TOKEN -660066

//Chance constants 
//Kohl
#define blueKOHL 41.95 //6-11
#define purpleKOHL 127.60 //c

//Manganese
#define blueMANG 39.95 //3-2
#define purpleMANG 153.34 //c

//Grindstone
#define blueGRIND 46.08 //3-3
#define purpleGRIND 151.17 //c

//RMA70
#define blueRMA 53.41 //2-10
#define purpleRMA 141.78 //c

//Orirock
#define whiteORIROCK 3.29 //s2-5
#define greenORIROCK 4.83 //1-7
#define blueORIROCK 24.15 //c
#define purpleORIROCK 96.6 //c

//Device
#define whiteDEVICE 7.79 //2-3
#define greenDEVICE 15.53 //s3-4
#define blueDEVICE 52.85 //3-4
#define purpleDEVICE 147.23 //c

//Polyester
#define whitePOLY  5.07 //s2-7
#define greenPOLY 9.91 //s3-2
#define bluePOLY 31.49 //2-6
#define purplePOLY 150.47 //c

//Sugar
#define whiteSUGAR 4.81 //s2-6
#define greenSUGAR 9.67 //s3-1
#define blueSUGAR 32.24 //2-5
#define purpleSUGAR 153.67 //c

//Oriron
#define whiteORIRON 6.45 //1-3
#define greenORIRON 12.31 //s3-3
#define blueORIRON 49.24 //c
#define purpleOriron 182.82 //c

//Keton
#define whiteKETON 5.86 //s2-9
#define greenKETON 12.25 //3-7
#define blueKETON 40.07 //3-1
#define purpleKETON 152.33 //c

//Gel
#define blueGEL 63.87 //s4-10
#define purpleGEL 149.56 //c

//Alloy
#define blueALLOY 36.45 //s3-6
#define purpleALLOY 135.38 //c

//Skill Certificate
#define greenSKILL 3.00 //ca-2
#define blueSKILL 6.67 //ca-3
#define purpleSKILL 15 //ca-5

//Record
#define greenRECORD 1.0 //convert
#define blueRECORD 2.00 //convert
#define purpleRECORD 5.00 //convert
#define specialRECORD 10.00 //ls-5

//Carbon (white =1 part green = 4 part, blue =8, purple=12)//Weighted interms of parts
#define whiteCARBON 0.5925	//sk-5			//SK-5 gives 30 sanity / 50.6336 part, may be more if consider byproduct
#define greenCARBON 2.37			
#define blueCARBON 4.74 //convert
#define purpleCARBON 7.11 //sk-5

//LMD (gold = 500 LMD)
#define LMD 0.004					//LS-5 gives 30 sanity /7500 LMD
#define GOLD 2.00					//Weighted interms of LMD, time to sell not considered, may be less
 
//Enter the avaerage drop cost for your special item, replacing 0, call with 1res, 2res, 3res and 4res

//Reserved item list 1	
#define RESONEW 0
#define RESONEG 0
#define RESONEB 0
#define RESONEP 0

//Reserved item list 2
#define RESTWOW 0
#define RESTWOG 0
#define RESTWOB 0
#define RESTWOP 0

//Reserved item list 3
#define RESTHREEW 0
#define RESTHREEG 0
#define RESTHREEB 0
#define RESTHREEP 0

//Reserved item list 4
#define RESFOURW 0
#define RESFOURG 0
#define RESFOURB 0
#define RESFOURP 0

int main(void) {
	int mode = 1;
	int status = RUN;					//0=main menu, 1=terminate program, 2=running (no prompt)

	printf("          ----------Arknights----------\n");
	printf("Initiation completed. Greetings, Doctor. I am Ptilopsis, how may I assist you?\n");		//Ptil greeting	

	while (status != END) {

		if (status == MAIN) {													//if the user want to visit main menu display options
			printf("Enter 1 to access stage sanity calculator.\n");
			printf("Enter 2 to access complex stage sanity calculator.\n");
			printf("Enter 3 to access shopping calculator\n");
			printf("Enter 0 to discharge Ptilopsis\n");
			scanf(" %d", &mode);
		}
		switch (mode) {
		case 0:
			status = END;
			break;
		case 1:
			status = stageSanityCalc(FALSE);
			break;
		case 2:
			status = stageSanityCalc(TRUE);
			break;
		case 3:
			status = shopCalculator();
			break;
		default:
			printf("Error. Ptilopsis cannot interperate your input\n\n");
		}

	}
	printf("All task complete, system...is now in Sleep Mode... Zzzzz...\n");

	return 0;
}

//Purpose: Calculate the returned sanities of a stage
//Param: int mode: mode of the calculator, 0 for normal, 1 for complex, -1 fpr test								//U.D
//Return: What user wants to do next, see main variable mode and status for more info
int stageSanityCalc(int mode) {
	FILE* SanityCalcHistory;

	char stageCode[8];
	char confirm[10] = { "no" };
	char itemReport[15][20] = { "Holder" };				//holder at 0th location so other numbers match input number, max 14 (1-15) input
	double chanceReport[15] = { 0 };					
	double averageSanity[15] = { 0 };					//store avaerge drop chance of item
	double sanityReturn[15] = { 0 };					//sanity returened per item

	int sanity = 0;
	int itemCount = 1;
	double netSanityReturn;

	int count;											//counters
	int count2;

	char itemColor[6][9] = {
		"Holder",
		"White-",					//1
		"Green-",
		"Blue-",
		"Purple-",
		"Special-",					//5
	};
	char itemIndexTable[21][12] = {
		"Holder",
		"Kohl",					//1
		"Manganese",
		"Grindstone",
		"RMA70",
		"Orirock",				//5
		"Device",
		"Polyester",
		"Sugar",
		"Oriron",
		"Keton",				//10
		"Gel",
		"Alloy",
		"Carbon",
		"Skill",
		"Records",				//15
		"LMD",
		"1RSV",					//user can change those to items they want
		"2RSV",
		"3RSV",
		"4RSV",					//20
	};

	//stage and sanity data collect, must enter exactly "yes" to leave loop
	while (strcmp(confirm, "yes") != SAME) {								
		printf("First enter the stage code or enter \"main\" to select mode\n");
		scanf(" %7[^\n]*c", stageCode);

		_strupr(stageCode);

		if (strcmp(stageCode, "MAIN") == SAME) {			//return to main menu if user wants to
			return MAIN;
		}
		printf("Now, enter the sanity cost for the stage\n");				//stage sanity cost prompt
		scanf(" %d", &sanity);

		printf("%s cost %d sanity, is that corrrect?(Yes/No)\n", stageCode, sanity);			//confirm: valid input: "yes", "YeS" etc
		scanf(" %10[^\n]*c", confirm);

		_strlwr(confirm);
	}
	printf("Next, enter the item (color then name of the item), for example: bkohl, b kohl, blue kohl or blue-kohl. \nOr, enter \"re\" to remove last result, enter \"over\" to indicate completion\n");
	printf("Then Enter the drop rate for the item and repeat until you have entered all drops\n\n");
	// item and rate collect, leave loop only with "confirm"
	while ((strcmp(confirm, "confirm") != SAME)) {						
		printf("Enter name for item #%d\n", (itemCount));								//prompt item name
		scanf(" %19[^\n]*c", itemReport[itemCount]);

		inputChecker(itemReport[itemCount]);					//check and correct commen inputs

		if ((strcmp(itemReport[itemCount], "over") != SAME) && (strcmp(itemReport[itemCount], "re") != SAME)) {				//check if input is valid and prompt drop chance
			averageSanity[itemCount] = averageSanityFinder(itemReport[itemCount]);				//store item's average drop

			if (averageSanity[itemCount] != 0) {								//if the item did not match any known, re prompt again
				printf("Enter the drop rate for ");
				fullNamePrinter(FALSE, itemReport[itemCount]);
				printf(" in %%\n");
				scanf("%lf", &chanceReport[itemCount]);

				sanityReturn[itemCount] = averageSanity[itemCount] * (chanceReport[itemCount] / 100);			//store sanity return
			}
			else {
				itemCount--;
			}
		}
		if ((strcmp(itemReport[itemCount], "re") == SAME) && (itemCount > 1)) {
			printf("Item %s deleted\n", itemReport[itemCount - 1]);
			itemCount -= 2;
		}
		else if (strcmp(itemReport[itemCount], "re") == SAME) {
			itemCount--;
			printf("Error: deleting when list is empty have been detected. Please confirm.\n\n");
		}
		if (strcmp(itemReport[itemCount], "over") == SAME) {				//confirm reset if user demands
			itemCount--;										//ignore input over in count

			printf("Successfully built package using a total of %d new data entered, confirm or would you like to edit the list?(confirm/edit)\n", itemCount);
			scanf(" %10[^\n]*c", confirm);

			while ((strcmp(confirm, "confirm") != SAME) && (strcmp(confirm, "erase") != SAME)) {
				count = 0;

				while (count < itemCount) {
					count++;

					printf("%d.", count);
					count2 = fullNamePrinter(FALSE, itemReport[count]);					//Print item name
					while (count2 < 18) {																	//print spaces after item to line up
						printf(" ");
						count2++;
					}
					printf(": %-8.2lf%% This item's estimated sanity cost across all level is %-7.3lf\n", chanceReport[count], averageSanity[count]);
				}
				printf("Docter, please enter the item number of the data you want to append, enter \"erase\" to delete all input or \"confirm\" to continue.\n");
				scanf(" %10[^\n]*c", confirm);

				if (strcmp(confirm, "confirm") != SAME && strcmp(confirm, "erase") != SAME) {
					count = confirm[0] - '0';						//change item location counter to what user entered

					if (count == itemCount + 1) {
						itemCount++;
					}

					if (count > 0 && count <= itemCount) {
						printf("Enter name for item #%d\n", (count));								//prompt item to replace
						scanf(" %19[^\n]*c", itemReport[count]);

						inputChecker(itemReport[count]);											//normalize input

						averageSanity[count] = averageSanityFinder(itemReport[count]);				//store item's average drop

						if (averageSanity[count] == 0) {
							count--;
						}
						else {
							printf("Enter the drop rate for ");										//prompt chance
							fullNamePrinter(FALSE, itemReport[count]);
							printf(" in %%\n");
							scanf("%lf", &chanceReport[count]);

							printf("Successfully built package using new data.\n");

							sanityReturn[count] = averageSanity[count] * (chanceReport[count] / 100);			//store sanity return
						}
					}
					else {
						printf("Error: unkown item number has been detected. The list can only expand by one item at a time.\n\n");
					}
				}
				else if (strcmp(confirm, "erase") == SAME) {
					itemCount = 0;
					printf("Reconfiguring to last normal system bootup point.\n\n");
				}
			}
		}
		itemCount++;
	}
	itemCount--;								//compensate for the last ++

	printf("Reading package data... Resource expended for stage: %s which cost %d sanity\n", stageCode, sanity);
	printf("   Item Name            Drop     Sanity   Average  Sanity\n");			// Item Name, Drop chance, sanity needed for one item, average cost of item (based on regular stage sanity cost for one item), refunded sanity for this item
	printf("                       Chance   Required  Sanity   Return\n");
	count = 0;
	//print result
	while (count < itemCount) {			//print details for each item
		count++;

		printf("%2d.", count);

		count2 = fullNamePrinter(FALSE, itemReport[count]);					//Print item name
		while (count2 < 18) {																	//print spaces after item to line up
			printf(" ");
			count2++;
		}
		printf("%8.2lf%% ", chanceReport[count]);
		printf("%8.2lf ", (100.0 / chanceReport[count]) * sanity);
		printf("%8.2lf ", averageSanity[count]);
		printf("%8.2lf\n", sanityReturn[count]);
	}
	netSanityReturn = netSanity(sanityReturn, itemCount);
	printf("The total sanity returned for stage %s is: %5.4lf or %0.3lf%% \n", stageCode, netSanityReturn, ((netSanityReturn / sanity) * 100.0));		//Print the stage summary in snaity returned and return ratio

	printf("Doctor, would you like to store the data in the disk array SanityCalcHistory.txt(Yes/No)?\n");
	scanf(" %10[^\n]*c", confirm);
	_strlwr(confirm);

	//store result to file
	if (strcmp(confirm, "yes") == SAME) {									
		SanityCalcHistory = fopen("SanityCalcHistory.txt", "a");			//Open and check file
		if (SanityCalcHistory == NULL) {
			printf("A fatal error has occured during file accessing. Please check the file SanityCalcHistory.txt exsist and reboot the system.\n\n");
			return END;
		}
		else {

			count = 0;			//Repeat the printing stage again, but to text file for record
			fprintf(SanityCalcHistory, "Stage %s cost %d sanity\n", stageCode, sanity);
			fprintf(SanityCalcHistory, "   Item Name            Drop     Sanity   Average  Sanity\n");
			fprintf(SanityCalcHistory, "                       Chance   Required  Sanity   Return\n");


			while (count < itemCount) {
				count++;

				fprintf(SanityCalcHistory, "%d.", count);
				count2 = fullNamePrinter(SanityCalcHistory, itemReport[count]);
				while (count2 < 18) {																	//print spaces after item to line up
					fprintf(SanityCalcHistory, " ");
					count2++;
				}

				fprintf(SanityCalcHistory, "%8.2lf%% ", chanceReport[count]);
				fprintf(SanityCalcHistory, "%8.2lf ", (100.0 / chanceReport[count]) * sanity);
				fprintf(SanityCalcHistory, "%8.2lf ", averageSanity[count]);
				fprintf(SanityCalcHistory, "%8.2lf\n", sanityReturn[count]);
			}
			fprintf(SanityCalcHistory, "The total sanity returned for stage %s is: %5.4lf or %0.3lf%% \n\n\n", stageCode, netSanityReturn, ((netSanityReturn / sanity) * 100.0));
		}
		fclose(SanityCalcHistory);
		printf("Loading completed. File successfully stored. Doctor, what can Ptilopsis do for you now?\n");
	}			
	printf("Enter \"Main\" to return to main menu, \"Re\" to restart calculator or \"Exit\" to terminate program\n");
	scanf(" %10[^\n]*c", confirm);

	_strlwr(confirm);

	if (strcmp(confirm, "main") == SAME) {
		return MAIN;
	}
	else if (strcmp(confirm, "re") == SAME) {
		return RUN;
	}
	else {
		return END;
	}
}

//Purpose: standarize item drop input including common misspells and cases
//Param: char itemReport[20]: Current user input 
//
void inputChecker(char itemReport[20]) {
	int count = 0;
	int count2 = 0;

	_strlwr(itemReport);													//lowercase all to standard
	if (strncmp(itemReport, "white", 4) == SAME || (strncmp(itemReport, "purple", 5) == SAME) || (strncmp(itemReport, "blue", 3) == SAME)) {		//shorten whole color word to first letter
		while (itemReport[count + 1] != 'e' && itemReport[count + 1] != NULL) {
			itemReport[count + 1] = ' ';
			count++;
		}
		itemReport[count + 1] = ' ';
	}
	else if (strncmp(itemReport, "green", 4) == SAME) {
		while (itemReport[count + 1] != 'n' && itemReport[count + 1] != NULL) {
			itemReport[count + 1] = ' ';
			count++;
		}
		itemReport[count + 1] = ' ';
	}

	for (count = 0; itemReport[count] != NULL;) {						//remove space and specific characters
		if ((itemReport[count] == ' ') || (itemReport[count] == ':') || (itemReport[count] == '-')) {							//did the user input empty space or : between item and color?
			for (count2 = count; itemReport[count2] != NULL; count2++) {											//take out white space or "-" by shifting the item name locate left by one space
				itemReport[count2] = itemReport[count2 + 1];
			}
		}
		if ((itemReport[count] != ' ') && (itemReport[count] != ':') && (itemReport[count] != '-')) {
			count++;
		}
	}
	if (strstr(itemReport, "rock") != NULL && itemReport[1] != 'o') {					//change each word to proper input
		itemReport[1] = '\0';
		strcat(itemReport, "orirock");
		printf("Ptilopsis detected an invalid input and altered it to %s.\n", itemReport);
	}
	else if (strstr(itemReport, "ore") != NULL && itemReport[1] != 'm') {
		itemReport[1] = '\0';
		strcat(itemReport, "manganese");
		printf("Ptilopsis detected an invalid input and altered it to %s.\n", itemReport);
	}
	else if (strstr(itemReport, "ester") != NULL && itemReport[1] != 'p') {
		itemReport[1] = '\0';
		strcat(itemReport, "polyester");
		printf("Ptilopsis detected an invalid input and altered it to %s.\n", itemReport);
	}
	else if (strstr(itemReport, "part") != NULL) {
		itemReport[1] = '\0';
		strcat(itemReport, "carbon");
		printf("Ptilopsis detected an invalid input and altered it to %s (which is equal to furnature parts).\n", itemReport);
	}
	else if (strstr(itemReport, "gold") != NULL) {
		itemReport[1] = '\0';
		strcat(itemReport, "lmd");
		printf("Ptilopsis detected an invalid input and altered it to %s (which is equal to pure gold).\n", itemReport);
	}
	else if (strstr(itemReport, "battle record") != NULL) {
		itemReport[1] = '\0';
		strcat(itemReport, "record");
		printf("Ptilopsis detected an invalid input and altered it to %s.\n", itemReport);
	}
	else if (strstr(itemReport, "money") != NULL) {
		itemReport[1] = '\0';
		strcat(itemReport, "lmd");
		printf("Ptilopsis detected an invalid input and altered it to %s (which is equal to Lungmen dollars).\n", itemReport);
	}
	else if (strstr(itemReport, "mang") != NULL) {
		itemReport[1] = '\0';
		strcat(itemReport, "manganese");
		printf("Ptilopsis detected an invalid input and altered it to %s.\n", itemReport);
	}
	else if (strstr(itemReport, "res") != NULL && itemReport[1] == 'r') {				//change res1 to 1res
		itemReport[1] = itemReport[4];
		itemReport[2] = '\0';
		strcat(itemReport, "res");
		printf("Ptilopsis detected an invalid input and altered it to %s.\n", itemReport);
	}
}

//Purpose: print name of the item based on shortened form
//Param: FILE** SanityCalcHistory: File pointer, if passed as 0 print to screen, if passed as file pointer print to that file
//Param: char itemReport[20]: User input
//Return: number of characters printed, 0 if an error occured
int fullNamePrinter(FILE* SanityCalcHistory, char itemReport[20]) {
	char fullName[25];
	char itemColor[6][9] = {
		"Holder",
		"White-",					//1
		"Green-",
		"Blue-",
		"Purple-",
		"Special-",					//5
	};
	char itemIndexTable[21][12] = {
		"Holder",
		"Kohl",					//1
		"Manganese",
		"Grindstone",
		"RMA70",
		"Orirock",				//5
		"Device",
		"Polyester",
		"Sugar",
		"Oriron",
		"Keton",				//10
		"Gel",
		"Alloy",
		"Carbon",
		"Skill",
		"Records",				//15
		"LMD",
		"1RSV",					//user can change those to items they want
		"2RSV",
		"3RSV",
		"4RSV",					//20
	};

	switch (itemReport[0]) {				//give code quality value of hundredth place
	case 'w':
		strcpy(fullName, itemColor[1]);		//White-
		break;
	case 'g':
		strcpy(fullName, itemColor[2]);		//Green-
		break;
	case 'b':
		strcpy(fullName, itemColor[3]);		//Blue-
		break;
	case 'p':
		strcpy(fullName, itemColor[4]);		//Purple-
		break;
	case 's':
		strcpy(fullName, itemColor[5]);		//Special-
		break;
	default:
		printf("Error: unkown input has been detected. Please confirm and enter \"re\".\n\n");		//return 0 if input does not match any items
		return 0;
		break;
	}
	switch (itemReport[1]) {				//check second letter of the item, only first few letters must match
	case 'k':
		if (itemReport[2] == 'o') { strcat(fullName, itemIndexTable[1]); } //ko hl
		if (itemReport[2] == 'e') { strcat(fullName, itemIndexTable[10]); } //ke ton
		break;
	case 'm':
		strcat(fullName, itemIndexTable[2]); //m anganese
		break;
	case 'g':
		if (itemReport[2] == 'r') { strcat(fullName, itemIndexTable[3]); }//gr indstone
		if (itemReport[2] == 'e') { strcat(fullName, itemIndexTable[11]); }//ge l
		if (itemReport[2] == 'o') { strcat(fullName, itemIndexTable[16]); }//go ld
		break;
	case 'r':
		if (itemReport[2] == 'm') { strcat(fullName, itemIndexTable[4]); }//rm d
		if (itemReport[2] == 'e') { strcat(fullName, itemIndexTable[15]); }//re cord
		break;
	case 'o':
		if (itemReport[6] == 'c') { strcat(fullName, itemIndexTable[5]); }//oriroc k
		if (itemReport[6] == 'n') { strcat(fullName, itemIndexTable[9]); }//oriron
		break;
	case 'd':
		strcat(fullName, itemIndexTable[6]);//d evice
		break;
	case 'p':
		if (itemReport[2] == 'o') { strcat(fullName, itemIndexTable[7]); } //po lyester
		if (itemReport[2] == 'a') { strcat(fullName, itemIndexTable[13]); }//pa rt
		break;
	case 'c':
		strcat(fullName, itemIndexTable[13]);//c arbon
		break;
	case 's':
		if (itemReport[2] == 'u') { strcat(fullName, itemIndexTable[8]); } //su gar
		if (itemReport[2] == 'k') { strcat(fullName, itemIndexTable[14]); }//sk ill
		break;
	case 'a':
		strcat(fullName, itemIndexTable[12]);//a lloy
		break;
	case 'l':
		strcat(fullName, itemIndexTable[16]);//l md
		break;
	case '1':
		strcat(fullName, itemIndexTable[17]);//reserved one
		break;
	case '2':
		strcat(fullName, itemIndexTable[18]);//reserved two
		break;
	case '3':
		strcat(fullName, itemIndexTable[19]);//reserved three
		break;
	case '4':
		strcat(fullName, itemIndexTable[20]);//reserved four
		break;
	default:
		printf("Error: unkown input has been detected. Please confirm and enter \"re\".\n\n");
		return 0;
		break;
	}
	if (SanityCalcHistory != 0) {
		return fprintf(SanityCalcHistory,"%s", fullName);
	}
	return printf("%s", fullName);
}

//Purpose: Find average dropping snaity of the item in normal stages
//Param: char itemReport[20]: User input 
//Return: average dropping sanity of the item, 0 if an error occured
double averageSanityFinder(char itemReport[20]) {
//Ko hl
if (strncmp(itemReport, "bkohl", 3) == SAME) return blueKOHL;
if (strncmp(itemReport, "pkohl", 3) == SAME) return purpleKOHL;
//Man ganese
if (strncmp(itemReport, "bmanganese", 3) == SAME) return blueMANG;
if (strncmp(itemReport, "pmanganese", 3) == SAME) return purpleMANG;
//Gr indstone
if (strncmp(itemReport, "bgrindstone", 3) == SAME) return blueGRIND;
if (strncmp(itemReport, "pgrindstone", 3) == SAME) return purpleGRIND;
//RM A70
if (strncmp(itemReport, "brma70", 3) == SAME) return blueRMA;
if (strncmp(itemReport, "prma70", 3) == SAME) return purpleRMA;
//Oriroc k
if (strncmp(itemReport, "worirock", 7) == SAME) return whiteORIROCK;
if (strncmp(itemReport, "gorirock", 7) == SAME) return greenORIROCK;
if (strncmp(itemReport, "borirock", 7) == SAME) return blueORIROCK;
if (strncmp(itemReport, "porirock", 7) == SAME) return purpleORIROCK;
//De vice
if (strncmp(itemReport, "wdevice", 3) == SAME) return whiteDEVICE;
if (strncmp(itemReport, "gdevice", 3) == SAME) return greenDEVICE;
if (strncmp(itemReport, "bdevice", 3) == SAME) return blueDEVICE;
if (strncmp(itemReport, "pdevice", 3) == SAME) return purpleDEVICE;
//Po lyester
if (strncmp(itemReport, "wpolyester", 3) == SAME) return whitePOLY;
if (strncmp(itemReport, "gpolyester", 3) == SAME) return greenPOLY;
if (strncmp(itemReport, "bpolyester", 3) == SAME) return bluePOLY;
if (strncmp(itemReport, "ppolyester", 3) == SAME) return purplePOLY;
//Su gar
if (strncmp(itemReport, "wsugar", 3) == SAME) return whiteSUGAR;
if (strncmp(itemReport, "gsugar", 3) == SAME) return greenSUGAR;
if (strncmp(itemReport, "bsugar", 3) == SAME) return blueSUGAR;
if (strncmp(itemReport, "psugar", 3) == SAME) return purpleSUGAR;
//Or iron
if (strncmp(itemReport, "woriron", 3) == SAME) return whiteORIRON;
if (strncmp(itemReport, "goriron", 3) == SAME) return greenORIRON;
if (strncmp(itemReport, "boriron", 3) == SAME) return blueORIRON;
if (strncmp(itemReport, "poriron", 3) == SAME) return purpleOriron;
//Ke ton
if (strncmp(itemReport, "wketon", 3) == SAME) return whiteKETON;
if (strncmp(itemReport, "gketon", 3) == SAME) return greenKETON;
if (strncmp(itemReport, "bketon", 3) == SAME) return blueKETON;
if (strncmp(itemReport, "pketon", 3) == SAME) return purpleKETON;
//Ge l
if (strncmp(itemReport, "bgel", 3) == SAME) return blueGEL;
if (strncmp(itemReport, "pgel", 3) == SAME) return purpleGEL;
//Al loy
if (strncmp(itemReport, "balloy", 3) == SAME) return blueALLOY;
if (strncmp(itemReport, "palloy", 3) == SAME) return purpleALLOY;
//Ca rbon (g=4,b=8,p=12,)
if (strncmp(itemReport, "wcarbon", 3) == SAME) return whiteCARBON;
if (strncmp(itemReport, "gcarbon", 3) == SAME) return greenCARBON;
if (strncmp(itemReport, "bcarbon", 3) == SAME) return blueCARBON;
if (strncmp(itemReport, "pcarbon", 3) == SAME) return purpleCARBON;
//Sk ill
if (strncmp(itemReport, "gskill", 3) == SAME) return greenSKILL;
if (strncmp(itemReport, "bskill", 3) == SAME) return blueSKILL;
if (strncmp(itemReport, "pskill", 3) == SAME) return purpleSKILL;
//Re cord
if (strncmp(itemReport, "grecord", 3) == SAME) return greenRECORD;
if (strncmp(itemReport, "brecord", 3) == SAME) return blueRECORD;
if (strncmp(itemReport, "precord", 3) == SAME) return purpleRECORD;
//LMD
if (strcmp(itemReport, "wlmd") == SAME) return LMD;
if (strcmp(itemReport, "plmd") == SAME) return GOLD;
//Res1
if (strcmp(itemReport, "w1res") == SAME) return RESONEW;
if (strcmp(itemReport, "g1res") == SAME) return RESONEG;
if (strcmp(itemReport, "b1res") == SAME) return RESONEB;
if (strcmp(itemReport, "p1res") == SAME) return RESONEP;
//Res2
if (strcmp(itemReport, "w2res") == SAME) return RESTWOW;
if (strcmp(itemReport, "g2res") == SAME) return RESTWOG;
if (strcmp(itemReport, "b2res") == SAME) return RESTWOB;
if (strcmp(itemReport, "p2res") == SAME) return RESTWOP;
//Res3
if (strcmp(itemReport, "w3res") == SAME) return RESTHREEW;
if (strcmp(itemReport, "g3res") == SAME) return RESTHREEG;
if (strcmp(itemReport, "b3res") == SAME) return RESTHREEB;
if (strcmp(itemReport, "p3res") == SAME) return RESTHREEP;
//Res4
if (strcmp(itemReport, "w4res") == SAME) return RESFOURW;
if (strcmp(itemReport, "g4res") == SAME) return RESFOURG;
if (strcmp(itemReport, "b4res") == SAME) return RESFOURB;
if (strcmp(itemReport, "p4res") == SAME) return RESFOURP;

printf("Error. %s is an unkown input. Please confirm.\n\n", itemReport);		// if nothing match, return error message
return 0;
}

//Purpose: Sum all sanity returned values
//Param: char itemReport[20]: User input
//Param: int itemCount: total number of items
//Return: sum of the returned sanity
double netSanity(double sanityReturn[20], int itemCount) {
	double netSanityReturn = 0;
	int count;
	for (count = 1; count < itemCount + 1; count++) {
		netSanityReturn += sanityReturn[count];
	}
	return netSanityReturn;
}

//Purpose: Show and search for items efficientcies in the shop
//Return: what user wnat to do next, see main function status and modes variables for details
int shopCalculator(void) {
	char input[20] = "\0";
	int mode = 0;

	printf("Docter, which store do you need advise on?\n");				//prompt store
	printf("1/Green/Commendations\n2/Yellow/Distinctions\n3/Red/Vouchers\n4/Orange/Parametric Models\n5/F/Credit\n0/Main to return to main\n");
	scanf(" %20[^\n]*c", input);
	_strlwr(input);

	if ((strcmp(input, "main") == SAME) || (input[0] == '0')) {			//if input =main return, else check stores
	return MAIN;
	}
	else {
		mode = storePromptReader(input);								//else associate input with a store

		if (mode == 0) {
			printf("Error. Invalid input has been detected. Please confirm\n");		//if input does not match any, re prompt
			return RUN;
		}
	}

	printf("Would you like to see all the items or look up a specific one?\n");		//prompt look up mode
	printf("All/One\n");
	scanf(" %20[^\n]*c", input);
	_strlwr(input);

	if (strcmp(input, "all") == SAME) {
	}
	else if (strcmp(input, "one") == SAME) {				//prompt and check item input
		do {
			printf("Which item in your select store? (color+name, eg: bgel, purple keton, etc.)\n");				//prompyitem to look up, loop until valid input
			scanf(" %20[^\n]*c", input);
			_strlwr(input);

			inputChecker(input);					//check for common misspells and mistakes
		} while (averageSanityFinder(input) == 0);
	}
	else {
		printf("Error. Invalid input has been detected. Please confirm\n");
		return RUN;
	}

	switch (mode) {									//direct user to stores based on store number inputted
		case 1:
			greenStore(input);
			break;
		case 2:
			yellowStore(input);
			break;
		case 3:
			redStore(input);
			break;
		case 4:
			orangeStore(input);
			break;
		case 5:
			fStore(input);
			break;
		case -1:
			holder1Store(input);
			break;
		case -2:
			holder2Store(input);
			break;
		default:
			printf("Error. Invalid input has been detected. Please confirm\n");
			return RUN;
	}
	
	printf("Task completed. Doctor, what can Ptilopsis do for you now?\n");			//prompt following action
	printf("Enter \"Main\" to return to main menu, \"Re\" to see another store or \"Exit\" to terminate program\n");
	scanf(" %10[^\n]*c", input);

	_strlwr(input);

	if (strcmp(input, "main") == SAME) {
		return MAIN;
	}
	else if (strcmp(input, "re") == SAME) {
		return RUN;
	}
	else {
		return END;
	}
}

//Purpose: Commendation store
//Param: input[]: user's choice of item
void greenStore(char input[20]) {
	char strItmGreen[13][12] = {
	"holder",
	"borirock",
	"bsugar",
	"bpolyester",
	"boriron",
	"bketon",		//5
	"bdevice",
	"bkohl",
	"bmanganese",
	"bgrindstone",
	"brma",			//10
	"bgel",
	"balloy" };
	double strCostGreen[13] = {
		HOLDER,
		25,
		30,
		30,
		35,
		35,				//5
		45,
		30,
		35,
		40,
		45,				//10
		40,
		35 };
	int count = 1;
	int count2 = 0;
	int efficiencyOrder[13] = {1};
	
	efficiencyOrderer(13, strItmGreen, strCostGreen, efficiencyOrder);					//order item from highest effciency 

	if (strcmp(input, "all") == SAME) {
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");			//print format

		while (count < 13) {
			printf("%2d: ", count);													//Rank
			count2 = fullNamePrinter(FALSE, strItmGreen[efficiencyOrder[count]]);					//Print item name
			while (count2 < 18) {																	//print spaces after item to line up
				printf(" ");
				count2++;
			}
			printf("%6.2lf", strCostGreen[efficiencyOrder[count]]);
			printf("%8.2lf\n", averageSanityFinder(strItmGreen[efficiencyOrder[count]]) / strCostGreen[efficiencyOrder[count]]);
			count++;
		}
	}
	else {
		while (count < 13 && strcmp(input, strItmGreen[count]) != SAME) { count++; }		//find location of item form list
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");

		for (count2 = 1; (count2 < 13 && count != efficiencyOrder[count2]); count2++);				//find item location from the efficiency order

		printf("%2d: ", count2);

		count2 = fullNamePrinter(FALSE, strItmGreen[count]);					//Print item name
		while (count2 < 18) {																	//print spaces after item to line up
			printf(" ");
			count2++;
		}

		printf("%6.2lf", strCostGreen[count]);
		printf("%8.2lf\n", averageSanityFinder(strItmGreen[count]) / strCostGreen[count]);
	}	
}

//Purpose: Distinction store
//Param: input[]: user's choice of item
void yellowStore(char input[20]) {
	char strItmYellow[13][12] = {
		"holder",
		"pkohl",
		"pmanganese",
		"pgrindstone",
		"prma",
		"porirock",		//5
		"pgel",
		"pdevice",
		"ppolyester",
		"psugar",
		"poriron",		//10
		"pketon",
		"palloy",
	};
	double strCostYellow[13] = {
		HOLDER,
		10,
		10,
		10,
		15,
		10,		//5
		15,
		20,
		10,
		10,
		15,		//10
		15,
		15 };
	int count = 1;
	int count2 = 0;
	int efficiencyOrder[13] = { 1 };

	efficiencyOrderer(13, strItmYellow, strCostYellow, efficiencyOrder);

	if (strcmp(input, "all") == SAME) {
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");			//print format

		while (count < 13) {
			printf("%2d: ", count);													//Rank
			count2 = fullNamePrinter(FALSE, strItmYellow[efficiencyOrder[count]]);					//Print item name
			while (count2 < 18) {																	//print spaces after item to line up
				printf(" ");
				count2++;
			}
			printf("%6.2lf", strCostYellow[efficiencyOrder[count]]);
			printf("%8.2lf\n", averageSanityFinder(strItmYellow[efficiencyOrder[count]]) / strCostYellow[efficiencyOrder[count]]);
			count++;
		}
	}
	else {
		while (count < 13 && strcmp(input, strItmYellow[count]) != SAME) { count++; }		//find location of item form list
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");

		for (count2 = 1; (count2 < 13 && count != efficiencyOrder[count2]); count2++);				//find item location from the efficiency order

		printf("%2d: ", count2);

		count2 = fullNamePrinter(FALSE, strItmYellow[count]);					//Print item name
		while (count2 < 18) {																	//print spaces after item to line up
			printf(" ");
			count2++;
		}

		printf("%6.2lf", strCostYellow[count]);
		printf("%8.2lf\n", averageSanityFinder(strItmYellow[count]) / strCostYellow[count]);
	}
}

//Purpose: Voucher store
//Param: input[]: user's choice of item
void redStore(char input[20]) {
	char strItmRed[2][12] = {
	"holder",
	"Underdev" };
	double strCostRed[13] = {
	HOLDER,
	-1 };
	int count = 1;
	int count2 = 0;
	int efficiencyOrder[2] = { 1 };

	printf("This store is underdevelopment, you will be directed back\n");
	return;
	efficiencyOrderer(2, strItmRed, strCostRed, efficiencyOrder);

	if (strcmp(input, "all") == SAME) {
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");			//print format

		while (count < 2) {
			printf("%2d: ", count);													//Rank
			count2 = fullNamePrinter(FALSE, strItmRed[efficiencyOrder[count]]);					//Print item name
			while (count2 < 18) {																	//print spaces after item to line up
				printf(" ");
				count2++;
			}
			printf("%6.2lf", strCostRed[efficiencyOrder[count]]);
			printf("%8.2lf\n", averageSanityFinder(strItmRed[efficiencyOrder[count]]) / strCostRed[efficiencyOrder[count]]);
			count++;
		}
	}
	else {
		while (count < 2 && strcmp(input, strItmRed[count]) != SAME) { count++; }		//find location of item form list
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");

		for (count2 = 1; (count2 < 2 && count != efficiencyOrder[count2]); count2++);				//find item location from the efficiency order

		printf("%2d: ", count2);

		count2 = fullNamePrinter(FALSE, strItmRed[count]);					//Print item name
		while (count2 < 18) {																	//print spaces after item to line up
			printf(" ");
			count2++;
		}

		printf("%6.2lf", strCostRed[count]);
		printf("%8.2lf\n", averageSanityFinder(strItmRed[count]) / strCostRed[count]);
	}
}

//Purpose: Parametric model store
//Param: input[]: user's choice of item
void orangeStore(char input[20]) {
	char strItmOrange[37][12] = {
	"holder",
	"palloy",
	"pgel",
	"prma",
	"pgrindstone",
	"pmanganese",		//5
	"pkohl",
	"pdevice",
	"pketon",
	"poriron",
	"ppolyester",		//10
	"psugar",
	"porirock",
	"balloy",
	"bgel",
	"brma",				//15
	"bgrindstone",
	"bmanganese",
	"bkohl",
	"bdevice",
	"bketon",			//20
	"boriron",
	"bpolyester",
	"bsugar",
	"borirock",
	"gdevice",			//25
	"gketon",
	"goriron",
	"gpolyester",
	"gsugar",
	"gorirock",			//30
	"wdevice",
	"wketon",
	"woriron",
	"wpolyester",
	"wsugar",			//35
	"worirock" };
	double strCostOrange[37] = {
	HOLDER,
	75.0,
	65.0,
	80.0,
	75.0,
	80.0,					//5
	65.0,
	85.0,
	85.0,
	90.0,
	80.0,					//10
	75.0,
	60.0,
	40.0/2,
	50.0/2,
	60.0/2,					//15
	50.0/2,
	45.0/2,
	40.0/2,
	60.0/2,
	45.0/2,					//20
	45.0/2,
	35.0/2,
	35.0/2,
	30.0/2,
	40.0/4,					//25
	30.0/4,
	30.0/4,
	25.0/4,
	25.0/4,
	15.0/4,					//30
	40.0/8,
	30.0/8,
	30.0/8,
	25.0/8,
	25.0/8,					//35
	15.0/8 };
	int count = 1;
	int count2 = 0;
	int efficiencyOrder[37] = { 1 };

	efficiencyOrderer(37, strItmOrange, strCostOrange, efficiencyOrder);

	if (strcmp(input, "all") == SAME) {
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");			//print format

		while (count < 37) {
			printf("%2d: ", count);													//Rank
			count2 = fullNamePrinter(FALSE, strItmOrange[efficiencyOrder[count]]);					//Print item name
			while (count2 < 18) {																	//print spaces after item to line up
				printf(" ");
				count2++;
			}
			printf("%6.2lf", strCostOrange[efficiencyOrder[count]]);
			printf("%8.2lf\n", averageSanityFinder(strItmOrange[efficiencyOrder[count]]) / strCostOrange[efficiencyOrder[count]]);
			count++;
		}
	}
	else {
		while (count < 37 && strcmp(input, strItmOrange[count]) != SAME) { count++; }		//find location of item form list
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");

		for (count2 = 1; (count2 < 37 && count != efficiencyOrder[count2]); count2++);				//find item location from the efficiency order

		printf("%2d: ", count2);

		count2 = fullNamePrinter(FALSE, strItmOrange[count]);					//Print item name
		while (count2 < 18) {																	//print spaces after item to line up
			printf(" ");
			count2++;
		}

		printf("%6.2lf", strCostOrange[count]);
		printf("%8.2lf\n", averageSanityFinder(strItmOrange[count]) / strCostOrange[count]);
	}
}

//Purpose: credit store
//Param: input[]: user's choice of item
void fStore(char input[20]) {
	char strItmF[22][12] = {
	"holder",
	"wlmd",//1800
	"wcarbon",//20
	"plmd",//6			
	"grecord",//9
	"brecord",//9		//5
	"gskill",//5
	"bskill",//3
	"gcarbon",//5			
	"bcarbon",//3
	"worirock",//2		//10
	"wsugar",//2
	"wpolyester",//2
	"woriron",//2		
	"wketon",//2
	"wdevice",//1		//15
	"gorirock",//3
	"gsugar",//2
	"gpolyester",//2	
	"goriron",//2
	"gketon",//2		//20
	"gdevice" };

	double strCostF[22] = {
	HOLDER,
	100.0/1800,
	160.0/20,
	160.0/6,				
	100.0/9,
	200.0/9,				//5
	160.0/5,
	200.0/3,
	160.0/5,				
	200.0/3,
	80.0/2,				//10
	100.0/2,
	100.0/2,
	120.0/2,				
	120.0/2,
	80.0,					//15
	200.0/3,
	200.0/2,
	200.0/2,				
	240.0/2,
	240.0/2,				//20
	160.0};
	int count = 1;
	int count2 = 0;
	int efficiencyOrder[22] = { 1 };
	int discount = 0;

	efficiencyOrderer(22, strItmF, strCostF, efficiencyOrder);

	if (strcmp(input, "all") == SAME) {
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");			//print format

		while (count < 22) {
			printf("%2d: ", count);													//Rank
			count2 = fullNamePrinter(FALSE, strItmF[efficiencyOrder[count]]);					//Print item name
			while (count2 < 18) {																	//print spaces after item to line up
				printf(" ");
				count2++;
			}
			printf("%6.2lf", strCostF[efficiencyOrder[count]]);
			printf("%8.2lf\n", averageSanityFinder(strItmF[efficiencyOrder[count]]) / strCostF[efficiencyOrder[count]]);
			count++;
		}
	}
	else {
		printf("How much was the item discounted? (0 for none)\n");
		scanf(" %d", &discount);
		
		while (count < 22 && strcmp(input, strItmF[count]) != SAME) { count++; }		//find location of item form list
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");

		for (count2 = 1; (count2 < 22 && count != efficiencyOrder[count2]); count2++);				//find item location from the efficiency order

		printf("%2d: ", count2);

		count2 = fullNamePrinter(FALSE, strItmF[count]);					//Print item name
		while (count2 < 18) {																	//print spaces after item to line up
			printf(" ");
			count2++;
		}
		if (strcmp(input, "yes") == SAME) {

		}

		printf("%6.2lf", strCostF[count] - (strCostF[count] * (discount / 100.0)));
		printf("%8.2lf\n", averageSanityFinder(strItmF[count]) / (strCostF[count] - (strCostF[count] * (discount / 100.0))) );
	}
}

//Purpose: backup store 2
//Param: input[]: user's choice of item
void holder1Store(char input[20]) {
	char strItmH1[1][12] = {
	"holder"};
	double strCostH1[1] = {
	HOLDER};
	int count = 1;
	int count2 = 0;
	int efficiencyOrder[1] = { 1 };

	printf("The store data cannot be read, please check the database\n");
	return;

	efficiencyOrderer(1, strItmH1, strCostH1, efficiencyOrder);

	if (strcmp(input, "all") == SAME) {
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");			//print format

		while (count < 1) {
			printf("%2d: ", count);													//Rank
			count2 = fullNamePrinter(FALSE, strItmH1[efficiencyOrder[count]]);					//Print item name
			while (count2 < 18) {																	//print spaces after item to line up
				printf(" ");
				count2++;
			}
			printf("%6.2lf", strCostH1[efficiencyOrder[count]]);
			printf("%8.2lf\n", averageSanityFinder(strItmH1[efficiencyOrder[count]]) / strCostH1[efficiencyOrder[count]]);
			count++;
		}
	}
	else {
		while (count < 1 && strcmp(input, strItmH1[count]) != SAME) { count++; }		//find location of item form list
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");

		for (count2 = 1; (count2 < 1 && count != efficiencyOrder[count2]); count2++);				//find item location from the efficiency order

		printf("%2d: ", count2);

		count2 = fullNamePrinter(FALSE, strItmH1[count]);					//Print item name
		while (count2 < 18) {																	//print spaces after item to line up
			printf(" ");
			count2++;
		}

		printf("%6.2lf", strCostH1[count]);
		printf("%8.2lf\n", averageSanityFinder(strItmH1[count]) / strCostH1[count]);
	}
};

//Purpose: backup store 1
//Param: input[]: user's choice of item
void holder2Store(char input[20]) {
	char strItmH2[1][12] = {
	"holder" };
	double strCostH2[1] = {
	HOLDER };
	int count = 1;
	int count2 = 0;
	int efficiencyOrder[1] = { 1 };

	printf("The store data cannot be read, please check the database\n");
	return;

	efficiencyOrderer(1, strItmH2, strCostH2, efficiencyOrder);

	if (strcmp(input, "all") == SAME) {
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");			//print format

		while (count < 1) {
			printf("%2d: ", count);													//Rank
			count2 = fullNamePrinter(FALSE, strItmH2[efficiencyOrder[count]]);					//Print item name
			while (count2 < 18) {																	//print spaces after item to line up
				printf(" ");
				count2++;
			}
			printf("%6.2lf", strCostH2[efficiencyOrder[count]]);
			printf("%8.2lf\n", averageSanityFinder(strItmH2[efficiencyOrder[count]]) / strCostH2[efficiencyOrder[count]]);
			count++;
		}
	}
	else {

		while (count < 1 && strcmp(input, strItmH2[count]) != SAME) { count++; }		//find location of item form list
		printf("Rank   Item Name       Cost   Efficientcy(Compared to Sanity)\n");

		for (count2 = 1; (count2 < 1 && count != efficiencyOrder[count2]); count2++);				//find item location from the efficiency order

		printf("%2d: ", count2);

		count2 = fullNamePrinter(FALSE, strItmH2[count]);					//Print item name
		while (count2 < 18) {																	//print spaces after item to line up
			printf(" ");
			count2++;
		}

		printf("%6.2lf", strCostH2[count]);
		printf("%8.2lf\n", averageSanityFinder(strItmH2[count]) / strCostH2[count]);
	}
};

//Purpose: associate specific user input with store number
//Param: input[]: string of user input
//Return: store number
int storePromptReader(char input[20]) {
	if ((strncmp(input, "gre", 3)) == SAME || (strncmp(input, "com", 3) == SAME) || (input[0] == '1')) {
		return 1;
	}
	else if ((strncmp(input, "yel", 3) == SAME) || (strncmp(input, "dis", 3)) == SAME || (input[0] == '2')) {
		return 2;
	}
	else if ((strncmp(input, "red", 3) == SAME) || (strncmp(input, "vou", 3)) == SAME || (input[0] == '3')) {
		return 3;
	}
	else if ((strncmp(input, "ora", 3) == SAME) || (strncmp(input, "par", 3)) == SAME || (input[0] == '4')) {
		return 4;
	}
	else if ((strncmp(input, "f\n", 2) == SAME) || (strncmp(input, "cre", 3)) == SAME || (input[0] == '5')) {
		return 5;
	}
	else if ((strncmp(input, "1ho", 3) == SAME) || (strncmp(input, "ho1", 3)) == SAME || (strncmp(input, "-1", 2)) == SAME) {				//place holder for future stores
		return -1;
	}
	else if ((strncmp(input, "2ho", 3) == SAME) || (strncmp(input, "ho2", 3)) == SAME || (strncmp(input, "-1", 2)) == SAME) {				//place holder for future stores
		return -2;
	}
	else {
		return 0;
	}
};

//Purpose: Order the store items from highest efficient to lowest, store in an array
//Param: size: number of items in store
//Param: strItm: item name list
//Param: strCost: item store currency cost
//Param: efficiencyOrder: destination string
void efficiencyOrderer(int size, char strItm[][12], double strCost[], int efficiencyOrder[]) {
	int itmCount = 1;
	int count = 1;
	double highEff = 0;
	int highEffCount = 0;

	while (itmCount < size) {
		highEff = 0;

		for (count = 1; count < size; count++) {
			if (averageSanityFinder(strItm[count]) / strCost[count] > highEff) {
				highEff = averageSanityFinder(strItm[count]) / strCost[count];
				highEffCount = count;
			}
		}
		efficiencyOrder[itmCount] = highEffCount;

		strCost[highEffCount] = -strCost[highEffCount];		//marked used values negative so nolonger used again
		itmCount++;
	}
	for (count = 1; count < size; count++) {				//make all cost positive again
		strCost[count] = -strCost[count];
	}
}




