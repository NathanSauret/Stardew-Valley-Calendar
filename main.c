#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAXCHAR 1000    // the max char for csv files
#define seasonLength 28     // the length of seasons


void clear()
// clear display
{
    for (int i=0; i<100; i++) { // margin
        printf("\n");
    }
    printf("\e[1;1H\e[2J");     // scroll to clear the display
}


int get_csv_row_length(char *csvName)
{
    // display plants list
    FILE *CSV;  // used to read the file
    char row[MAXCHAR];      // used to read rows
    char *token;            // used to read elements of rows

    CSV = fopen(csvName ,"r");   // open the CSV file

    int rowI = 0;
    while (feof(CSV) != true) {
        fgets(row, MAXCHAR, CSV);   // get the row
        rowI++;
    }

    return rowI;
}


void display_list()
// display the list to the user.
{
    // open plantList CSV (read mode)
    FILE *plantListCsvRead;  // used to read the file
    char rowPlantList[MAXCHAR];      // used to read rows
    plantListCsvRead = fopen("plant_list.csv","r");   // open the CSV file

    // error message if can't open
    if(plantListCsvRead == NULL){
        printf("\e[1;1H\e[2J");     // scroll to clear the display
        printf("Couldn't open file\n");
        fflush(stdout);
        sleep(2);
        return;
    }


    // open seasonal crop CSV (read mode)
    FILE *seasonalCropCsvRead;  // used to read the file
    char rowSeasonalCrop[MAXCHAR];      // used to read rows
    char *token;            // used to read elements of rowschar*
    seasonalCropCsvRead = fopen("seasonal_crop.csv","r");   // open the CSV file

    // error message if can't open
    if(seasonalCropCsvRead == NULL){
        printf("\e[1;1H\e[2J");     // scroll to clear the display
        printf("Couldn't open file\n");
        fflush(stdout);
        sleep(2);
        return;
    }

    
    char *quitChoice;
    while (quitChoice != 'q') {
        printf("\e[1;1H\e[2J");     // scroll to clear the display
        // display plants list
        plantListCsvRead = fopen("plant_list.csv","r");   // open the CSV file
        int rowI = 0;
        while (feof(plantListCsvRead) != true) {
            fgets(rowPlantList, MAXCHAR, plantListCsvRead);   // get the row
            
            if (rowI != 0 && strstr(rowPlantList,",")) {

                char *name = strtok(rowPlantList, ",");     // get the name
                char *season = strtok(NULL, ",");           // get the season
                char *day = strtok(NULL, ",");              // get the day
                char *number = strtok(NULL, ",");           // get the number

                // search for the sell price and the days to grow
                char *daysToGrow;                   // days to grow
                daysToGrow = (char*)malloc(20);
                char *sellPrice;                    // sell price
                sellPrice = (char*)malloc(20);
                seasonalCropCsvRead = fopen("seasonal_crop.csv","r");   // re-open the CSV file to go to the begining
                while (feof(seasonalCropCsvRead) != true) {
                    fgets(rowSeasonalCrop, MAXCHAR, seasonalCropCsvRead);   // get the row

                    if (strstr(rowSeasonalCrop, name)) {
                        token = strtok(rowSeasonalCrop, ",");
                        token = strtok(NULL, ",");
                        token = strtok(NULL, ",");
                        strcpy(daysToGrow, token);   // get days to grow
                        token = strtok(NULL, ",");
                        token = strtok(NULL, ",");
                        strcpy(sellPrice, token);   // get sell price

                        break;
                    }
                }

                // shift date if the plantation ready after the end of the season
                int dayReady;
                char *seasonReady;
                if (atoi(day)+atoi(daysToGrow) <= 28) {
                    dayReady = atoi(day)+atoi(daysToGrow);
                    seasonReady = season;
                }
                else {
                    dayReady = (atoi(day)+atoi(daysToGrow))-28;
                    if (strstr(season,"spring")) {
                        seasonReady = "summer";
                    }
                    else if (strstr(season,"summer")) {
                        seasonReady = "fall";
                    }
                    else if (strstr(season,"fall")) {
                        seasonReady = "winter";
                    }
                    else {
                        seasonReady = "spring";
                    }
                }

                printf("%s * %s", name, number);                                     // display the name of the planntation
                printf("Planted at day %d of %s\n", atoi(day), season);             // display the day and season of plantation
                printf("Will be ready at day %d of %s\n", dayReady, seasonReady);   // display when it will be ready
                printf("Can be sold for %dG\n\n", atoi(sellPrice)*atoi(number));    // display how much the user can sell
            }
            rowI++; // update rowI
        }
        printf("Type 'q' to quit: ");
        quitChoice = getchar();
    }
}


void display_calendar()
// display the calendar to the user.
{
   // open plantList CSV (read mode)
    FILE *plantListCsvRead;  // used to read the file
    char rowPlantList[MAXCHAR];      // used to read rows
    plantListCsvRead = fopen("plant_list.csv","r");   // open the CSV file

    // error message if can't open
    if(plantListCsvRead == NULL){
        printf("\e[1;1H\e[2J");     // scroll to clear the display
        printf("Couldn't open file\n");
        fflush(stdout);
        sleep(2);
        return;
    }


    // open seasonal crop CSV (read mode)
    FILE *seasonalCropCsvRead;  // used to read the file
    char rowSeasonalCrop[MAXCHAR];      // used to read rows
    char *token;            // used to read elements of rowschar*
    seasonalCropCsvRead = fopen("seasonal_crop.csv","r");   // open the CSV file

    // error message if can't open
    if(seasonalCropCsvRead == NULL){
        printf("\e[1;1H\e[2J");     // scroll to clear the display
        printf("Couldn't open file\n");
        fflush(stdout);
        sleep(2);
        return;
    }


    char *season = "spring";

    while (1) {
        clear();    // clear display
        printf("\t\t\t\t\t\t\t  ~%s~\n", season);
        int dayInCalendar = 1;
        for (int week=0; week<4; week++) {

            // separation bars
            for (int i=0; i<7; i++) {
                printf("--------------------");
            }
            printf("-\n");  // return to line

            // days
            for (int dayHeight=0; dayHeight<3; dayHeight++) {
                for (int weekDay=1; weekDay<8; weekDay++) {
                    
                    // set variables for display
                    char *plantName;                        // plant name
                    plantName = (char*)malloc(20);

                    char *plantSeason;                      // plant season
                    plantSeason = (char*)malloc(20);

                    char *plantDay;                         // plant day
                    plantDay = (char*)malloc(20);

                    char *plantNumber;                      // plant number
                    plantNumber = (char*)malloc(20);

                    char *plantDaysToGrow;                  // days to grow
                    plantDaysToGrow = (char*)malloc(20);

                    char *plantSellPrice;                   // sell price
                    plantSellPrice = (char*)malloc(20);

                    char *dayReady;                         // day ready
                    dayReady = (char*)malloc(20);

                    char *seasonReady;                      // season ready
                    seasonReady = (char*)malloc(20);


                    // get plantation infos
                    seasonalCropCsvRead = fopen("seasonal_crop.csv","r");   // open the CSV file
                    plantListCsvRead = fopen("plant_list.csv","r");   // open the CSV file
                    int rowI = 0;
                    while (feof(plantListCsvRead) != true) {
                        fgets(rowPlantList, MAXCHAR, plantListCsvRead);   // get the row
                        if (rowI != 0 && strstr(rowPlantList,",")) {

                            plantName = strtok(rowPlantList, ",");     // get the name
                            plantSeason = strtok(NULL, ",");           // get the season
                            plantDay = strtok(NULL, ",");              // get the day
                            plantNumber = strtok(NULL, ",");           // get the number

                            // search for the sell price and the days to grow
                            seasonalCropCsvRead = fopen("seasonal_crop.csv","r");   // re-open the CSV file to go to the begining
                            while (feof(seasonalCropCsvRead) != true) {
                                fgets(rowSeasonalCrop, MAXCHAR, seasonalCropCsvRead);   // get the row

                                if (strstr(rowSeasonalCrop, plantName)) {
                                    token = strtok(rowSeasonalCrop, ",");
                                    token = strtok(NULL, ",");
                                    token = strtok(NULL, ",");
                                    strcpy(plantDaysToGrow, token);   // get days to grow
                                    token = strtok(NULL, ",");
                                    token = strtok(NULL, ",");
                                    strcpy(plantSellPrice, token);   // get sell price

                                    break;
                                }
                            }

                            // shift date if the plantation ready after the end of the season
                            if (atoi(plantDay)+atoi(plantDaysToGrow)+atoi(plantDaysToGrow) <= 28) {
                                sprintf(dayReady, "%d", atoi(plantDay)+atoi(plantDaysToGrow)+atoi(plantDaysToGrow));
                                seasonReady = plantSeason;
                            }
                            else {
                                sprintf(dayReady, "%d", (atoi(plantDay)+atoi(plantDaysToGrow)+atoi(plantDaysToGrow))-28);
                                if (strstr(plantSeason,"spring")) {
                                    seasonReady = "summer";
                                }
                                else if (strstr(plantSeason,"summer")) {
                                    seasonReady = "fall";
                                }
                                else if (strstr(plantSeason,"fall")) {
                                    seasonReady = "winter";
                                }
                                else {
                                    seasonReady = "spring";
                                }
                            }

                            // if there is a plantation that day
                            if (atoi(dayReady) == dayInCalendar && strstr(seasonReady,season)) {
                                break;
                            }
                        }
                        rowI++; // update rowI
                    }


                    /*display infos*/
                    // dayHeight == 0
                    if (dayHeight == 0) {
                        if (dayInCalendar < 10) {
                            printf("|[%d]                ", dayInCalendar); // space for days
                        }
                        else {
                            printf("|[%d]               ", dayInCalendar); // space for days
                        }
                    }

                    // dayHeight == 1
                    if (dayHeight == 1) {
                        // if plantation that day
                        if (atoi(dayReady) == dayInCalendar && strstr(seasonReady,season)) {
                            int spaces = strlen(plantName)+strlen(plantNumber)+1;   // number of spaces
                            printf("|%s*%d", plantName, atoi(plantNumber)); // space for days
                            for (int i=0; i<20-spaces; i++) {
                                printf(" ");
                            }
                        }
                        // if not
                        else {
                            printf("|                   ");
                        }
                    }

                    // dayHeight == 2
                    if (dayHeight == 2) {
                        // if plantation that day
                        if (atoi(dayReady) == dayInCalendar && strstr(seasonReady,season)) {
                            int spaces = strlen(plantSellPrice)+7;   // number of spaces
                            printf("|sell: %dG", atoi(plantSellPrice)); // display sell price
                            for (int i=0; i<20-spaces; i++) {
                                printf(" ");
                            }
                        }
                        // if not
                        else {
                            printf("|                   ");
                        }
                    }

                    // if at the end of the week, decrease dayInCalendar
                    if (weekDay == 7 && dayHeight != 2) {
                        dayInCalendar -= 6;
                    }
                    else {
                        dayInCalendar++;
                    }
                    
                    fflush(stdout);

                }
                printf("|\n");  // return to line
                
            }

        }

        // last separation bars
        for (int i=0; i<7; i++) {
            printf("--------------------");
        }
        printf("-\n");
        printf("[<'p' previous season] [next season 'n'>] | 'q': quit\n");
        char *userChoice = getchar();

        if (userChoice == 'q') {
            return;
        }
        else if (userChoice == 'p') {
            if (season == "spring") {
                season = "winter";
            }
            else if (season == "summer") {
                season = "spring";
            }
            else if (season == "fall") {
                season = "summer";
            }
            else if (season == "winter") {
                season = "fall";
            }
        }
        else if (userChoice == 'n') {
            if (season == "spring") {
                season = "summer";
            }
            else if (season == "summer") {
                season = "fall";
            }
            else if (season == "fall") {
                season = "winter";
            }
            else if (season == "winter") {
                season = "spring";
            }
        }
            
        fflush(stdout);
    }
    sleep(20);
}


void list_or_calendar()
// ask to the user if he want to see plantations from a list or a calendar.
{
    char *choice = NULL;
    while (1) {
        clear();    // clear display
        printf("[1] List\n");
        printf("[2] Calendar\n\n");
        printf("How would you like to see the plantations? ([1-2]/n): ");

        choice = getchar();

        if (choice == '1') {
            display_list();
        }
        else if (choice == '2') {
            display_calendar();
        }
        else if (choice == 'n') {
            return;
        }
    }
}


int element_in_double_list(char *el, char **list, int len)
// check if an element is in a list.
{
    for (int i=0; i<len; i++) {
        if (el == list[i][0]) {
            return 1;
        }
    }
    return 0;
}


void new_plantation()
// create a new plantation and update plant_list.csv.
{
    // get user choice (season)
    char *seasonChoice = '0'; // user choice variable
    while (seasonChoice<'1' || seasonChoice>'4') {
        clear();    // clear display
        printf("[1] Spring\n");
        printf("[2] Summer\n");
        printf("[3] Fall\n");
        printf("[4] Winter\n");
        printf("What season? ([1-4]/n): ");

        seasonChoice = getchar();
        if (seasonChoice == 'n') {
            return;
        }
    }

    // set the season
    if (seasonChoice == '1') {
        seasonChoice = "spring";
    }
    else if (seasonChoice == '2') {
        seasonChoice = "summer";
    }
    else if (seasonChoice == '3') {
        seasonChoice = "fall";
    }
    else {
        seasonChoice = "winter";
        printf("\e[1;1H\e[2J");     // scroll to clear the display
        printf("Work in progress...");
        fflush(stdout);
        sleep(2);
        return;
    }

    // get seasonal crop file
    FILE *seasonalCrop;  // used to read the file
    char row[MAXCHAR];      // used to read rows
    char *token;            // used to read elements of rowschar*)
    seasonalCrop = fopen("seasonal_crop.csv","r");   // open the CSV file

    // error message if can't open
    if(seasonalCrop == NULL){
        printf("Couldn't open file\n");
        fflush(stdout);
        sleep(2);
        return;
    }

    //get crop list of the season
    char *plantList[36];
    int rowI = 0;
    int i = 0;
    while (feof(seasonalCrop) != true) {
        fgets(row, MAXCHAR, seasonalCrop);   // get the row

        if (i!=0) {
            // get name
            token = strtok(row, ",");   // tokenize the row

            // copy the plant name into a variable
            char *plantName;
            plantName = (char*)malloc(20);
            strcpy(plantName, token);

            // get season
            token = strtok(NULL, ",");   // tokenize the row
            
            // if seasons the same, add to list
            if (strstr(token, seasonChoice)!=NULL) {
                plantList[rowI] = plantName;
                rowI++;
            }
        }
        i++;
    }

    // get user choice (plant)
    char plantChoice[3] = "0"; // user choice variable
    while (atoi(plantChoice)<1 || atoi(plantChoice)>rowI) {
        clear();    // clear display

        // list plants
        int i;
        for (i=0; i<rowI; i++) {
            printf("[%d] %s\n", i+1, plantList[i]);
            fflush(stdout);
        }

        printf("What plant? ([1-%d]/n): ", i);

        scanf("%s", &plantChoice);
        if (strstr(plantChoice, "n")) {
            return;
        }
    }

    // get user choice (day)
    char dayChoice[3] = "0"; // user choice variable
    while (atoi(dayChoice)<1 || atoi(dayChoice)>28) {
        clear();    // clear display

        printf("What day? ([1-28]/n): ", i);

        scanf("%s", &dayChoice);
        if (strstr(dayChoice, "n")) {
            return;
        }
    }

    // get user choice (number)
    char numberChoice[5] = "0"; // user choice variable
    while (atoi(numberChoice)<1 || atoi(numberChoice)>1000) {
        clear();    // clear display

        printf("How many? ([1-1000]/n): ", i);

        scanf("%s", &numberChoice);
        if (strstr(numberChoice, "n")) {
            return;
        }
    }


    // get back to the first row of csv
    seasonalCrop = fopen("seasonal_crop.csv","r");   // open the CSV file

    // error message if can't open
    if(seasonalCrop == NULL){
        printf("Couldn't open file\n");
        fflush(stdout);
        sleep(2);
        return;
    }

    // check if user have the time until next season
    while (feof(seasonalCrop) != true) {
        fgets(row, MAXCHAR, seasonalCrop);   // get the row

        if (strstr(row, plantList[atoi(plantChoice)-1])) {

            token = strtok(row, ",");   // tokenize the row
            token = strtok(NULL, ",");   // tokenize the row
            // get days to grow
            token = strtok(NULL, ",");   // tokenize the row

            if (atoi(dayChoice) + atoi(token) > 28) {
                char *yesNoChoice = NULL;
                while (1) {
                    clear();    // clear display
                    printf("Warning: if you plant this plant now it will die at the next season\nDo you really want to add it to the calendar? (y/n) : ");
                    yesNoChoice = getchar();

                    if (yesNoChoice == 'n') {
                        clear();    // clear display
                        printf("Plantation cancelled!");
                        fflush(stdout);
                        sleep(2);
                        return;
                    }
                    else if (yesNoChoice == 'y') {
                        break;
                    }
                }
            }
            break;
        }
    }

    // verification
    char *yesNoChoice = NULL;
    while (1) {
        clear();    // clear display
        printf("You want to plant %s %s at day %s of %s, is that right? (y/n): ",numberChoice, plantList[atoi(plantChoice)-1], dayChoice, seasonChoice);
        yesNoChoice = getchar();
        
        if (yesNoChoice == 'n') {
            clear();    // clear display
            printf("Plantation cancelled!");
            fflush(stdout);
            sleep(2);
            return;
        }
        else if (yesNoChoice == 'y') {
            break;
        }
    }

    
    // open plant list csv
    FILE *plantListCsvAppend;  // used to read the file
    plantListCsvAppend = fopen("plant_list.csv","a");   // open the CSV file

    // error message if can't open
    if(plantListCsvAppend == NULL){
        printf("Couldn't open file\n");
        fflush(stdout);
        sleep(2);
        return;
    }

    // add new plantation to list
    fprintf(plantListCsvAppend, "%s,%s,%s,%s\n",plantList[atoi(plantChoice)-1], seasonChoice, dayChoice, numberChoice);
    fclose(plantListCsvAppend);     // close the csv file

    clear();    // clear display
    printf("Plantation added!\n");
    fflush(stdout);
    sleep(2);
}


void create_csv_copy(char *csvName)
// create a copy of a csv file
{
    // create new file
    FILE *plantListCsvTMP;
    plantListCsvTMP = fopen("TMP.csv","w");   // open the CSV file

    // open original file
    FILE *plantListCsvRead;  // used to read the file
    char row[MAXCHAR];      // used to read rows
    plantListCsvRead = fopen(csvName,"r");   // open the CSV file

    // error message if can't open
    if(plantListCsvRead == NULL){
        printf("Couldn't open file\n");
        fflush(stdout);
        sleep(2);
        return;
    }

    // copy file rows into copy
    int i=0;
    while (i < get_csv_row_length(csvName)-1) {
        fgets(row, MAXCHAR, plantListCsvRead);   // get the row
        fprintf(plantListCsvTMP, row);     // write the line on the original file
        i++;
    }
    fclose(plantListCsvTMP);   // close the copy file
}


void remove_plantation()
// remover a plant from plant_list.csv.
{
    char csvName[15] = "plant_list.csv";
    int rowLength = get_csv_row_length(csvName);       // get number of rows

    if (rowLength == 2) {
        clear();    // clear display
        printf("No plants in the calendar\n");
        fflush(stdout);
        sleep(2);
        return;
    }
    
    char choice[3] = "0"; // user choice variable
    while (atoi(choice)<1 || atoi(choice)>rowLength-2) {

        // open CSV file (read mode)
        FILE *plantListCsvRead;  // used to read the file
        char row[MAXCHAR];      // used to read rows
        char *token;            // used to read elements of rowschar*)
        plantListCsvRead = fopen("plant_list.csv","r");   // open the CSV file

        // error message if can't open
        if(plantListCsvRead == NULL){
            clear();    // clear display
            printf("Couldn't open file\n");
            fflush(stdout);
            sleep(2);
            return;
        }

        // display plants list
        clear();    // clear display
        int rowI = 0;
        char *name;
        name = (char*)malloc(20);
        char *season;
        season = (char*)malloc(20);
        char *day;
        day = (char*)malloc(20);
        char *number;
        number = (char*)malloc(20);
        while (feof(plantListCsvRead) != true) {
            fgets(row, MAXCHAR, plantListCsvRead);   // get the row
            
            if (rowI != 0 && strstr(row,",")) {
                printf("[%d]\n", rowI);    // print the row number

                name = strtok(row, ",");       // get name
                
                season = strtok(NULL, ",");    // get season
                
                day = strtok(NULL, ",");       // get day

                number = strtok(NULL, ",");    // get number

                printf("%s*%s", name, number);
                printf("Planted at day %s of %s\n\n", day, season);
            }
            rowI++; // update rowI
        }
        rowI--;

        // ask to the user his choice
        printf("\nWich plant do you want to delete? ([1-%d]/n): ", rowI-1);
        scanf("%s", &choice); // get the user choice
        
        //exit if 'n'
        if (strstr(choice, "n")) {
            return;
        }

        char *choice;
        while (1) {
            clear();    // clear display
            printf("Do you really want to delete %d %s planted at day %s of %s? (y/n): ", atoi(number), name, day, season);
            choice = getchar();

            if (choice == 'y') {
                break;
            }
            else if (choice == 'n') {
                return;
            }
        }
    }
    
    
    // create a copy of the csv file
    create_csv_copy(csvName);

    // if only one element in csv, just copy the first line
    if (get_csv_row_length(csvName) == 2) {
        // open TMP CSV file (read mode)
        FILE *TMP;  // used to read the file
        char rowTMP[MAXCHAR];      // used to read rows
        TMP = fopen("TMP.csv","r");   // open the CSV file

        // open CSV file (write mode)
        FILE *plantListCsvWrite;  // used to read the file
        plantListCsvWrite = fopen("plant_list.csv","w");   // open the CSV file

        fprintf(plantListCsvWrite, "name,season_of_plantation,day_of_plantation");     // write the line on the original file

        fclose(plantListCsvWrite);   // close the csv file
    }

    // if more than one element in csv
    else {
        // open TMP CSV file (read mode)
        FILE *TMP;  // used to read the file
        char rowTMP[MAXCHAR];      // used to read rows
        TMP = fopen("TMP.csv","r");   // open the CSV file

        // open CSV file (write mode)
        FILE *plantListCsvWrite;  // used to read the file
        plantListCsvWrite = fopen("plant_list.csv","w");   // open the CSV file

        // remove the line choosed by the user
        int rowI = 0;
        while (rowI < get_csv_row_length("TMP.csv")-1) {
            fgets(rowTMP, MAXCHAR, TMP);   // get the row of copy

            // copy line only if not the line we want to remove
            if (rowI != atoi(choice)) {
                fprintf(plantListCsvWrite, rowTMP);     // write the line on the original file
            }
            rowI++; // update rowI
        }
        fclose(plantListCsvWrite);   // close the csv file
    }
    remove("TMP.csv");  // remove the TMP csv file

    clear();    // clear display
    printf("Plantation removed!\n");
    fflush(stdout);
    sleep(2);
}


void clear_calendar()
// clear the plant_list.csv file wich contain all the plantations.
{
    char choice = NULL; // user choice variable
    while (choice!='y') {
        clear();    // clear display
        printf("All saved plantations will be lost.\nAre you sure? (y/n) : ");

        choice = getchar();
        if (choice == 'n') {
            return;
        }
    }

    // open CSV file (write mode)
    FILE *plantListCsvWrite;  // used to read the file
    plantListCsvWrite = fopen("plant_list.csv","w");   // open the CSV file

    fprintf(plantListCsvWrite, "name,season_of_plantation,day_of_plantation,number\n");     // write the line on the original file

    fclose(plantListCsvWrite);   // close the csv file

    clear();    // clear display
    printf("Calendar cleared!\n");
    fflush(stdout);
    sleep(2);
}



int main(int argc, char *argv[])
{
    char choice = '0'; // user choice variable
    
    // ask to the user what he want to do
    while (choice<'1' || choice>'5') {
        clear();    // clear display
        printf("what do you want to do?\n");
        printf("\t[1] See plantations\n");
        printf("\t[2] New plantation\n");
        printf("\t[3] Remove plantation\n");
        printf("\t[4] Clear plantations\n");
        printf("\t[5] Quit\n");

        choice = getchar();
    }

    if (choice == '1') {
        list_or_calendar(); // display the calendar
    }
    else if (choice == '2') {
        new_plantation();   // create a new plantation
    }
    else if (choice == '3') {
        remove_plantation();   // remove the plant
    }
    else if (choice == '4') {
        clear_calendar();   // clear the calendar
    }
    else {
        clear();    // clear display
        return 1;
    }

    return main(argc, argv);
}