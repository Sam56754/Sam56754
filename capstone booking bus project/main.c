#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define ROWS 14
#define SEATS_PER_ROW 4
#define TOTAL_SEATS (ROWS * SEATS_PER_ROW)
#define MAX_BUS_ROUTE_SIZE 50
#define MAX_BUS_TIME_SIZE 10
#define MAX_BUS_DATE_SIZE 20
#define MAX_BOOKED_SEATS 20

char busRoute[MAX_BUS_ROUTE_SIZE];
char busTimeFrom[MAX_BUS_TIME_SIZE];
char busTimeTo[MAX_BUS_TIME_SIZE];
char busDate[MAX_BUS_DATE_SIZE];
char* getSeatingFileName(char* busCode);
float busFare;

// Define a structure to hold customer details
struct Customer {
    char name[30];
    char idNumber[9];  // 8 digits + null terminator
    char phoneNumber[11];  // 10 digits + null terminator
    int seatNumber;
};
struct BookingDetails {
    int busIndex;  // Index of the selected bus schedule
    int *seatNumbers;  //to store booked seat numbers
    int seatNumber;
    struct Customer customer;  // Customer details
    char busRoute[50];  // Bus route
    float busFare;  // Bus fare
    char busTimeFrom[10];  // Bus departure time
    char busTimeTo[10];  // Bus arrival time
    char busDate[20];  // Bus date
};

// Initialize the global booking details
struct BookingDetails currentBooking = {0};



// Function declarations
void getPassword(char *password);
int isUserExist(char *username);
void signUp();
void Login();
void Menu();
void addBusSchedule();
void viewBusSchedule();
void deleteBusSchedule();
void bookBus();
void viewBookingHistory();
void cancelBooking();
void reviewTicketing();
void storeBookingDetails(struct BookingDetails booking);
void modifyAccount();
void changeUsername();
void changePassword();

// Global variables
char seatingChart[ROWS][SEATS_PER_ROW][30];  // To store customer names for each seat


int main() {
    int choice, menu;

    login:
    printf("\tWELCOME TO SAM@TRAVELLERS BUS SERVICES\n");
    printf("\t\t1. Sign Up\n");
    printf("\t\t2. Login\n");
    printf("\t\t3. Exit\n");
    printf("\tEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            signUp();
            goto login;
            break;
        case 2:
            if (login()) {
                back:
                Menu();
                scanf("%d", &menu);
                switch (menu) {
                    case 1:
                        addBusSchedule();
                        printf("press 1 to continue to main menu:");
                        scanf("%d",&choice);
                        if(choice==1){
                                goto back;
                        }else
                        break;
                    case 2:
                        viewBusSchedule();
                        printf("press 1 to continue to main menu:");
                        scanf("%d",&choice);
                        if(choice==1){
                                goto back;
                        }else
                        break;
                    case 3:
                        deleteBusSchedule();
                        printf("press 1 to continue to main menu:");
                        scanf("%d",&choice);
                        if(choice==1){
                                goto back;
                        }else
                        break;
                    case 4:
                        bookBus();
                        printf("press 1 to continue to main menu:");
                        scanf("%d",&choice);
                        if(choice==1){
                                goto back;
                        }else
                        break;
                    case 5:
                        viewBookingHistory();
                        printf("press 1 to continue to main menu:");
                        scanf("%d",&choice);
                        if(choice==1){
                                goto back;
                        }else
                        break;
                    case 6:
                        cancelBooking();
                        printf("press 1 to continue to main menu:");
                        scanf("%d",&choice);
                        if(choice==1){
                                goto back;
                        }else
                        break;
                    case 7:
                        reviewTicketing();
                        printf("press 1 to continue to main menu:");
                        scanf("%d",&choice);
                        if(choice==1){
                                goto back;
                        }else
                        break;
                    case 8:
                        modifyAccount();
                        break;
                    case 9:
                        exit(1);
                        break;
                    // Add other cases as needed
                    default:
                        printf("Invalid option!!");
                        goto back;
                }
            }
            break;
        case 3:
            printf("\t\tExiting program.\n");
            return 0;  // Exiting the program
        default:
            printf("\t\tInvalid choice. Please try again.\n");
    }

    return 0;
}
int login() {
    char username[30];
    char password[30];
    int attempts = 3;  // Maximum number of login attempts

    do {
        printf("\nEnter your username: ");
        scanf("%s", username);

        if (!isUserExist(username)) {
            printf("\nUsername does not exist. Please sign up first.\n");
            return 0;  // Login failed
        }

        printf("Enter your password: ");
        getPassword(password);

        FILE *file = fopen("user_data.txt", "r");
        if (file == NULL) {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        char storedUsername[30], storedPassword[30];
        int authenticated = 0;

        while (fscanf(file, "%s %s", storedUsername, storedPassword) == 2) {
            if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, password) == 0) {
                authenticated = 1;
                break;
            }
        }

        fclose(file);

        if (authenticated) {
            printf("\nLogin successful!\n");
            return 1;  // Login successful
        } else {
            printf("\nIncorrect username or password. Please try again. Attempts left: %d\n", --attempts);
        }
    } while (attempts > 0);

    printf("\nMaximum login attempts reached. Exiting program.\n");
    exit(EXIT_FAILURE);
}

void getPassword(char *password) {
    int i = 0;
    char ch;

    while (1) {
        ch = _getch(); // Use _getch() to read a character without echoing
        if (ch == 13) // Enter key
            break;
        else if (ch == 8) // Backspace key
        {
            if (i > 0) {
                printf("\b \b"); // Move the cursor back, print a space, move the cursor back again
                i--;
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0'; // Null-terminate the password string
}

int isUserExist(char *username) {
    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, username) != NULL) {
            fclose(file);
            return 1; // Username exists
        }
    }

    fclose(file);
    return 0; // Username does not exist
}

void signUp() {
    char username[30];
    char password[30];

    printf("\nEnter a username: ");
    fflush(stdin);  // Clear input buffer
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';  // Remove newline character

    if (isUserExist(username)) {
        printf("\nUsername already exists. Please choose another username.\n");
        return;
    }

    printf("Enter a password: ");
    getPassword(password);

    FILE *file = fopen("user_data.txt", "a");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %s\n", username, password);

    fclose(file);
    printf("\nAccount created successfully!\n");
}


void Login() {
    char username[30];
    char password[30];

    printf("\nEnter your username: ");
    scanf("%s", username);

    if (!isUserExist(username)) {
        printf("\nUsername does not exist. Please sign up first.\n");
        return;
    }

    printf("Enter your password: ");
    getPassword(password);

    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char storedUsername[30], storedPassword[30];
    int authenticated = 0;

    while (fscanf(file, "%s %s", storedUsername, storedPassword) == 2) {
        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, password) == 0) {
            authenticated = 1;
            break;
        }
    }

    fclose(file);

    if (authenticated)
        printf("\nLogin successful!\n");
    else
        printf("\nIncorrect username or password. Please try again.\n");
}


void Menu() {
    system("cls");
    printf("***. Please choose one of the options below*****\n");
    printf("\t\t\t[1]=> Add a new bus schedules\n");
    printf("\t\t\t[2]=> View bus schedule\n");
    printf("\t\t\t[3]=> Delete a bus schedule\n");
    printf("\t\t\t[4]=> Book a bus\n");
    printf("\t\t\t[5]=> View booking history\n");
    printf("\t\t\t[6]=> Cancel booking\n");
    printf("\t\t\t[7]=> Review ticketing\n");
    printf("\t\t\t[8]=> Modify account\n");
    printf("\t\t\t[9]=> Exit\n");
}
void addBusSchedule() {
    char busCode[10];
    float busFare;
    int choice;

    // Get bus details from the user
    printf("Enter Bus Code: ");
    scanf("%s", busCode);

    // Create seating chart file for the new bus schedule
    FILE *seatingFile = fopen(getSeatingFileName(busCode), "w");
    if (seatingFile == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    // Initialize seating chart as empty
    memset(seatingChart, 0, sizeof(seatingChart));

    fwrite(seatingChart, sizeof(seatingChart), 1, seatingFile);
    fclose(seatingFile);

    printf("Enter Bus Route (From and To): ");
    fflush(stdin);
    fgets(busRoute, sizeof(busRoute), stdin);
    busRoute[strcspn(busRoute, "\n")] = '\0';

    printf("Enter Bus Fare: ");
    scanf("%f", &busFare);

    printf("Enter Bus Time (From): ");
    scanf("%s", busTimeFrom);

    printf("Enter Bus Time (To): ");
    scanf("%s", busTimeTo);

    printf("Enter Bus Date: ");
    scanf("%s", busDate);

    // Save bus details to the global structure
    currentBooking.busIndex = choice;
    strcpy(currentBooking.busRoute, busRoute);
    currentBooking.busFare = busFare;
    strcpy(currentBooking.busTimeFrom, busTimeFrom);
    strcpy(currentBooking.busTimeTo, busTimeTo);
    strcpy(currentBooking.busDate, busDate);

    // Save bus details to the file
    FILE *file = fopen("bus_schedule.txt", "a");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %s %.2f %s %s %s\n", busCode, busRoute, busFare, busTimeFrom, busTimeTo, busDate);

    fclose(file);
    printf("\nBus schedule added successfully!\n");
}


void viewBusSchedule() {
    FILE *file = fopen("bus_schedule.txt", "r");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    printf("\n%-5s %-15s %-20s %-10s %-10s %-10s %-20s\n", "Index", "Bus Code", "Bus Route", "Fare", "Time From", "Time To", "Date");
    printf("----------------------------------------------------------------------------------------\n");

    int index = 1;
    char busCode[10], busRoute[50], busTimeFrom[10], busTimeTo[10], busDate[20];
    float busFare;

    while (fscanf(file, "%s %s %f %s %s %s", busCode, busRoute, &busFare, busTimeFrom, busTimeTo, busDate) == 6) {
        printf("%-5d %-15s %-20s %-10.2f %-10s %-10s %-20s\n", index++, busCode, busRoute, busFare, busTimeFrom, busTimeTo, busDate);
    }

    fclose(file);
}
void deleteBusSchedule() {
    viewBusSchedule();

    int choice;
    printf("\nEnter the index of the bus schedule you want to delete: ");
    scanf("%d", &choice);

    char busCode[10];
    sprintf(busCode, "%d", choice);  // Use the bus schedule index as the bus code

    // Delete the seating chart file
    char fileName[50];
    snprintf(fileName, sizeof(fileName), "seating_chart_%s.txt", busCode);
    remove(fileName);

    FILE *file = fopen("bus_schedule.txt", "r");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    FILE *tempFile = fopen("temp_schedule.txt", "w");
    if (tempFile == NULL) {
        perror("Error");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int index = 1;
    char busRoute[50], busTimeFrom[10], busTimeTo[10], busDate[20];
    float busFare;

    while (fscanf(file, "%s %s %f %s %s %s", busCode, busRoute, &busFare, busTimeFrom, busTimeTo, busDate) == 6) {
        if (index != choice) {
            fprintf(tempFile, "%s %s %.2f %s %s %s\n", busCode, busRoute, busFare, busTimeFrom, busTimeTo, busDate);
        }
        index++;
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the modified temporary file
    remove("bus_schedule.txt");
    rename("temp_schedule.txt", "bus_schedule.txt");

    printf("\nBus schedule deleted successfully!\n");
}

void displaySeatingChart() {
    printf("\n***** Seating Chart *****\n");
    printf("\n");

    int seatNumber = 1;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < SEATS_PER_ROW; ++j) {
            if (seatNumber < 10) {
                printf(" ");
            }

            // Print seat number and customer name with fixed width
            printf("%d.%-*s", seatNumber, 30, seatingChart[i][j][0] == '\0' ? "--[Empty]--" : seatingChart[i][j]);

            seatNumber++;
        }
        printf("\n");
    }
}

char* getSeatingFileName(char* busCode) {
    static char fileName[50];
    snprintf(fileName, sizeof(fileName), "seating_chart_%s.txt", busCode);
    return fileName;
}

// ...
void bookBus() {
    // Display available bus schedules
    viewBusSchedule();

    int choice;
    printf("\nEnter the index of the bus schedule you want to book: ");
    scanf("%d", &choice);

    // Load seating chart from the file (if available)
    char busCode[10];
    sprintf(busCode, "%d", choice);  // Use the bus schedule index as the bus code

    FILE *seatingFile = fopen(getSeatingFileName(busCode), "r");
    if (seatingFile != NULL) {
        fread(seatingChart, sizeof(seatingChart), 1, seatingFile);
        fclose(seatingFile);
    }

    displaySeatingChart();

    // Get the number of seats to book from the user
    int numSeats;
    printf("\nEnter the number of seats you want to book (1-%d): ", MAX_BOOKED_SEATS);
    scanf("%d", &numSeats);

    // Validate the number of seats
    if (numSeats < 1 || numSeats > MAX_BOOKED_SEATS) {
        printf("\nInvalid number of seats. Please try again.\n");
        return;
    }

    // Get customer details
    struct Customer customer;
    printf("Enter your name: ");
    scanf("%s", customer.name);

    printf("Enter your ID number (8 digits): ");
    scanf("%s", customer.idNumber);

    printf("Enter your phone number (10 digits): ");
    scanf("%s", customer.phoneNumber);

    // Retrieve bus details from the file
    FILE *busScheduleFile = fopen("bus_schedule.txt", "r");
    if (busScheduleFile == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    int index = 1;
    char busCodeRead[10], busRouteRead[MAX_BUS_ROUTE_SIZE], busTimeFromRead[MAX_BUS_TIME_SIZE], busTimeToRead[MAX_BUS_TIME_SIZE], busDateRead[MAX_BUS_DATE_SIZE];
    float busFareRead;

    while (fscanf(busScheduleFile, "%s %s %f %s %s %s", busCodeRead, busRouteRead, &busFareRead, busTimeFromRead, busTimeToRead, busDateRead) == 6) {
        if (index == choice) {
            strcpy(currentBooking.busRoute, busRouteRead);
            currentBooking.busFare = busFareRead;
            strcpy(currentBooking.busTimeFrom, busTimeFromRead);
            strcpy(currentBooking.busTimeTo, busTimeToRead);
            strcpy(currentBooking.busDate, busDateRead);
            break;
        }
        index++;
    }

    fclose(busScheduleFile);

    // Calculate total fare based on the number of booked seats
    float totalFare = currentBooking.busFare * numSeats;

    // Save bus details to the global structure
    currentBooking.busIndex = choice;
    currentBooking.customer = customer;
    currentBooking.busFare = totalFare;
    currentBooking.seatNumbers = (int *)malloc(sizeof(int) * numSeats);

    // Save seat numbers to the global structure
    for (int i = 0; i < numSeats; ++i) {
        int seatNumber;
        printf("Enter the seat number for booking %d (1-%d): ", i + 1, TOTAL_SEATS);
        scanf("%d", &seatNumber);

        // Validate seat number
        if (seatNumber < 1 || seatNumber > TOTAL_SEATS || seatingChart[(seatNumber - 1) / SEATS_PER_ROW][(seatNumber - 1) % SEATS_PER_ROW][0] != '\0') {
            printf("\nInvalid seat number or the seat is already booked. Please try again.\n");
            return;
        }

        // Assign seat number directly to the structure
        currentBooking.seatNumbers[i] = seatNumber;

        // Update seating chart for the booked seat
        strcpy(seatingChart[(seatNumber - 1) / SEATS_PER_ROW][(seatNumber - 1) % SEATS_PER_ROW], customer.name);
    }

    // Save seating chart to the file
    FILE *seatingFileWrite = fopen(getSeatingFileName(busCode), "w");
    if (seatingFileWrite != NULL) {
        fwrite(seatingChart, sizeof(seatingChart), 1, seatingFileWrite);
        fclose(seatingFileWrite);
    }

    // Save booking details to a file
    storeBookingDetails(currentBooking);
    system("cls");

    // Display updated seating chart
    displaySeatingChart();

    printf("\n*********** BOOKING DETAILS ***********\n");
    printf("\t=customer details=\n");
    printf("\tNumber of Seats Booked: %d\n", numSeats);
    printf("\tCustomer Name: %s\n", customer.name);
    printf("\tID Number: %s\n", customer.idNumber);
    printf("\tPhone Number: %s\n\n", customer.phoneNumber);
    printf("\t=bus details=\n");
    printf("\tBus Route: %s\n", currentBooking.busRoute);
    printf("\tBus Time: %s - %s\n", currentBooking.busTimeFrom, currentBooking.busTimeTo);
    printf("\tBus Date: %s\n", currentBooking.busDate);
    printf("\tTotal Fare: %.2f\n", totalFare);
    printf("******************************************\n");
}
void viewBookingHistory() {
    // Display available bus schedules
    viewBusSchedule();

    int choice;
    printf("\nEnter the index of the bus schedule you want to view the booking history: ");
    scanf("%d", &choice);

    // Load seating chart from the file (if available)
    char busCode[10];
    sprintf(busCode, "%d", choice);  // Use the bus schedule index as the bus code

    FILE *seatingFile = fopen(getSeatingFileName(busCode), "r");
    if (seatingFile != NULL) {
        fread(seatingChart, sizeof(seatingChart), 1, seatingFile);
        fclose(seatingFile);
    }

    displaySeatingChart();
}
void cancelBooking() {
    // Display available bus schedules
    viewBusSchedule();

    int choice;
    printf("\nEnter the index of the bus schedule you want to cancel booking: ");
    scanf("%d", &choice);

    // Load seating chart from the file (if available)
    char busCode[10];
    sprintf(busCode, "%d", choice);  // Use the bus schedule index as the bus code

    FILE *seatingFile = fopen(getSeatingFileName(busCode), "r");
    if (seatingFile != NULL) {
        fread(seatingChart, sizeof(seatingChart), 1, seatingFile);
        fclose(seatingFile);
    }

    displaySeatingChart();

    // Get seat number from the user
    int seatNumber;
    printf("\nEnter the seat number you want to cancel (1-%d): ", TOTAL_SEATS);
    scanf("%d", &seatNumber);

    // Validate seat number
    if (seatNumber < 1 || seatNumber > TOTAL_SEATS || seatingChart[(seatNumber - 1) / SEATS_PER_ROW][(seatNumber - 1) % SEATS_PER_ROW][0] == '\0') {
        printf("\nInvalid seat number or the seat is already empty. Please try again.\n");
        return;
    }

    // Get customer details from the seat
    struct Customer customer;
    strcpy(customer.name, seatingChart[(seatNumber - 1) / SEATS_PER_ROW][(seatNumber - 1) % SEATS_PER_ROW]);

    // Update seating chart and save it to the file
    seatingChart[(seatNumber - 1) / SEATS_PER_ROW][(seatNumber - 1) % SEATS_PER_ROW][0] = '\0';

    FILE *seatingFileWrite = fopen(getSeatingFileName(busCode), "w");
    if (seatingFileWrite != NULL) {
        fwrite(seatingChart, sizeof(seatingChart), 1, seatingFileWrite);
        fclose(seatingFileWrite);
    }

    // Display updated seating chart
    displaySeatingChart();

    // Display ticket details
    printf("\n***** Ticket Details *****\n");
    printf("Bus Schedule: %d\n", choice);
    printf("Seat Number: %d\n", seatNumber);
    printf("Customer Name: %s\n", customer.name);
    printf("ID Number: %s\n", currentBooking.customer.idNumber);  // Use the stored customer ID
    printf("Phone Number: %s\n", currentBooking.customer.phoneNumber);  // Use the stored customer phone number
    printf("Booking canceled successfully.\n");
}
void reviewTicketing() {
    // Display available bus schedules
    viewBusSchedule();

    int choice;
    printf("\nEnter the index of the bus schedule for ticket review: ");
    scanf("%d", &choice);

    // Load seating chart from the file (if available)
    char busCode[10];
    sprintf(busCode, "%d", choice);  // Use the bus schedule index as the bus code

    FILE *seatingFile = fopen(getSeatingFileName(busCode), "r");
    if (seatingFile != NULL) {
        fread(seatingChart, sizeof(seatingChart), 1, seatingFile);
        fclose(seatingFile);
    }

    displaySeatingChart();

    // Get seat number from the user
    int seatNumber;
    printf("\nEnter the seat number for ticket review (1-%d): ", TOTAL_SEATS);
    scanf("%d", &seatNumber);

    // Validate seat number
    if (seatNumber < 1 || seatNumber > TOTAL_SEATS || seatingChart[(seatNumber - 1) / SEATS_PER_ROW][(seatNumber - 1) % SEATS_PER_ROW][0] == '\0') {
        printf("\nInvalid seat number or the seat is empty. Please try again.\n");
        return;
    }

    // Create a new booking details structure for this ticket
    struct BookingDetails reviewBooking;
    reviewBooking.busIndex = choice;
    reviewBooking.seatNumber = 1;  // Only reviewing one seat
    reviewBooking.seatNumbers = (int *)malloc(sizeof(int) * reviewBooking.seatNumber);
    reviewBooking.seatNumbers[0] = seatNumber;

    // Get customer details from the stored seat
    struct Customer customer;
    strcpy(customer.name, seatingChart[(seatNumber - 1) / SEATS_PER_ROW][(seatNumber - 1) % SEATS_PER_ROW]);

    // Retrieve bus details from the file
    FILE *busScheduleFile = fopen("bus_schedule.txt", "r");
    if (busScheduleFile == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    int index = 1;
    char busCodeRead[10], busRouteRead[MAX_BUS_ROUTE_SIZE], busTimeFromRead[MAX_BUS_TIME_SIZE], busTimeToRead[MAX_BUS_TIME_SIZE], busDateRead[MAX_BUS_DATE_SIZE];
    float busFareRead;

    while (fscanf(busScheduleFile, "%s %s %f %s %s %s", busCodeRead, busRouteRead, &busFareRead, busTimeFromRead, busTimeToRead, busDateRead) == 6) {
        if (index == choice) {
            strcpy(reviewBooking.busRoute, busRouteRead);
            reviewBooking.busFare = busFareRead;
            strcpy(reviewBooking.busTimeFrom, busTimeFromRead);
            strcpy(reviewBooking.busTimeTo, busTimeToRead);
            strcpy(reviewBooking.busDate, busDateRead);
            break;
        }
        index++;
    }

    fclose(busScheduleFile);

    // Save customer details to the new structure
    reviewBooking.customer = customer;

    system("cls");
    displaySeatingChart();

    // Display the details for the reviewed ticket
    printf("\n*********** TICKET REVIEW ***********\n");
    printf("\t=customer details=\n");
    printf("\tSeat Number: %d\n", seatNumber);
    printf("\tCustomer Name: %s\n", reviewBooking.customer.name);
    printf("\tID Number: %s\n", reviewBooking.customer.idNumber);  // Note: ID Number is not available in this context
    printf("\tPhone Number: %s\n\n", reviewBooking.customer.phoneNumber);
    printf("\t=bus details=\n");
    printf("\tBus Route: %s\n", reviewBooking.busRoute);
    printf("\tBus Time: %s - %s\n", reviewBooking.busTimeFrom, reviewBooking.busTimeTo);
    printf("\tBus Date: %s\n", reviewBooking.busDate);
    printf("\tBus Fare: %.2f\n", reviewBooking.busFare);
    printf("**************************************\n");

    // Free memory for seatNumbers
    free(reviewBooking.seatNumbers);
}


void storeBookingDetails(struct BookingDetails booking) {
    FILE *bookingFile = fopen("booking_details.txt", "a");
    if (bookingFile == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    // Print the bus index, seat numbers, and other details to the file
    fprintf(bookingFile, "%d %d ", booking.busIndex, booking.seatNumber);
    for (int i = 0; i < booking.seatNumber; ++i) {
        fprintf(bookingFile, "%d ", booking.seatNumbers[i]);
    }
    fprintf(bookingFile, "%s %s %s %s %s %s %.2f\n",
            booking.customer.name,
            booking.customer.idNumber,
            booking.customer.phoneNumber,
            booking.busRoute,
            booking.busTimeFrom,
            booking.busTimeTo,
            booking.busDate,
            booking.busFare);

    fclose(bookingFile);
    free(booking.seatNumbers);  // Free memory for seatNumbers
}
void modifyAccount() {
    int modifyChoice;

    printf("\n***** Modify Account *****\n");
    printf("1. Change Username\n");
    printf("2. Change Password\n");
    printf("3. Back to Main Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &modifyChoice);

    switch (modifyChoice) {
        case 1:
            changeUsername();
            break;
        case 2:
            changePassword();
            break;
        case 3:
            // Back to main menu
            break;
        default:
            printf("Invalid option. Please try again.\n");
    }
}

void changeUsername() {
    char newUsername[30];

    printf("Enter a new username: ");
    fflush(stdin);
    fgets(newUsername, sizeof(newUsername), stdin);
    newUsername[strcspn(newUsername, "\n")] = '\0';

    if (isUserExist(newUsername)) {
        printf("Username already exists. Please choose another username.\n");
        return;
    }

    // Read the current username from the file
    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char currentUsername[30], password[30];
    fscanf(file, "%s %s", currentUsername, password);
    fclose(file);

    // Write the new username to the file
    file = fopen("user_data.txt", "w");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %s\n", newUsername, password);
    fclose(file);

    printf("Username changed successfully!\n");
}

void changePassword() {
    char newPassword[30];

    printf("Enter a new password: ");
    getPassword(newPassword);

    // Read the current username from the file
    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char username[30], currentPassword[30];
    fscanf(file, "%s %s", username, currentPassword);
    fclose(file);

    // Write the new password to the file
    file = fopen("user_data.txt", "w");
    if (file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %s\n", username, newPassword);
    fclose(file);

    printf("Password changed successfully!\n");
}
