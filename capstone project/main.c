#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SEATS 56
#define MAX_BUSES 10

#define DATA_FILE "data.txt"
#define SCHEDULES_FILE "bus_schedules.txt"

// Structures to hold username and password, customer details, and bus schedules
struct Credentials {
    char username[20];
    char password[20];
};

struct Customer {
    char name[20];
    int idNumber;
    long long phoneNumber;
    int seatNumber;
};

struct BusSchedule {
    char busCode[10];
    char timeLine[20];
    char route[50];
};

struct UserData {
    struct Credentials credentials;
    struct BusSchedule busSchedules[MAX_BUSES];
    int numSchedules; // Keep track of the number of schedules in the array.
};

struct UserData userData; // Instance to store user data
struct Credentials userCredentials;


// Function declarations(prototype)
void loadUserData();
void saveUserData();
void loadBusSchedules();
void saveBusSchedules();
void bookBus(struct Customer *customer);
void bus();
void addBusSchedule();
void viewBusSchedules();
void deleteBusSchedule();
void printReceipt(struct Customer *customer, struct BusSchedule *busSchedule);

int main() {
    loadUserData();
    struct Credentials commonCredentials = {"admin", "Administrator254"};
    struct Credentials userCredentials;

    int loggedIn = 0;
    loadBusSchedules(); // Load bus schedules from the file

    while (!loggedIn) {
        char username[20];
        char password[20];

        printf("================BUS RESERVATION SYSTEM LOGIN================\n");
        printf("\t\tEnter your username: ");
        scanf("%s", username);

        printf("\t\tEnter your password: ");
        scanf("%s", password);

        // Check if the entered username and password match the common ones or the user's saved credentials
        if ((strcmp(username, userCredentials.username) == 0 && strcmp(password, userCredentials.password) == 0)||
            (strcmp(username, commonCredentials.username) == 0 && strcmp(password, commonCredentials.password) == 0)
            ) {
            printf("||\t\t\tLogin successful!\t\t\t||\n");
            loggedIn = 1;
        } else {
            printf("\n[!! Login failed. Please try again. !!]\n");
        }
    }

    int choice, modify;
    struct Customer customer; // Create an instance of the Customer struct

fpage:
    bus();

    printf("Enter a choice: ");
    scanf("%d", &choice);

    int index;
    switch (choice) {
        case 1:
            viewBusSchedules();
            break;
        case 2:
            if (userData.numSchedules > 0) {
                    printf("Bus Schedules:\n");
                    printf("Index Bus Code Timeline      Route\n");
                    for (int i = 0; i < userData.numSchedules; i++) {
                            printf("%-6d %-9s %-19s %s\n", i, userData.busSchedules[i].busCode, userData.busSchedules[i].timeLine, userData.busSchedules[i].route);
                    }
                    int chosenIndex;
                    do {
                            printf("Choose a bus index for booking: ");
                            scanf("%d", &chosenIndex);
                            if (chosenIndex < 0 || chosenIndex >= userData.numSchedules) {
                                    printf("Invalid index. Please choose a valid index.\n");
                            }
                            } while (chosenIndex < 0 || chosenIndex >= userData.numSchedules);
                            bookBus(&customer);
                            printReceipt(&customer, &userData.busSchedules[chosenIndex]);
                            } else {
                                printf("No bus schedules available.\n");
                                }
            break;
        case 3:
            printf("Cancel booking");
            break;
        case 4:
            addBusSchedule();
            goto fpage;
            break;
        case 5:
            viewBusSchedules();
            printf("Enter the index of the schedule to delete: ");
            scanf("%d", &index);
            deleteBusSchedule(index);
            goto fpage;
            break;
        case 6:
             system("cls");
             printf("================ACCOUNT MODIFICATION================\n");
             printf("\t\t\tEnter your new username: ");
             scanf("%s", userCredentials.username);
             printf("\t\t\tEnter your new password: ");
             scanf("%s", userCredentials.password);
             // Save the user's new credentials to a file for future logins
             FILE *file = fopen(DATA_FILE, "wb");
             if (file != NULL) {
                    // Save the user's credentials in the userData structure as well
                    strcpy(userData.credentials.username, userCredentials.username);
                    strcpy(userData.credentials.password, userCredentials.password);
                    fwrite(&userData, sizeof(struct UserData), 1, file);
                    fclose(file);
                    printf("\n\t\tUsername and password updated and saved for future logins.\n");
                } else {
                    printf("Failed to save user data to file.\n");
                    }
                goto fpage;
            break;
        case 7:
            saveUserData();
            exit(1);
            break;
        default:
            printf("Invalid choice");
    }
    saveUserData();
    return 0;
}

// Function to load user data (both login credentials and bus schedules)
void loadUserData() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file != NULL) {
        fread(&userData, sizeof(struct UserData), 1, file);
        fclose(file);

        // Assign the user's credentials to userCredentials structure
        strcpy(userCredentials.username, userData.credentials.username);
        strcpy(userCredentials.password, userData.credentials.password);
    } else {
        // Initialize with default values if the file doesn't exist
        strcpy(userData.credentials.username, "admin");
        strcpy(userData.credentials.password, "Administrator254");
        userData.numSchedules = 0;
    }
}

// Function to save user data
void saveUserData() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file != NULL) {
        fwrite(&userData, sizeof(struct UserData), 1, file);
        fclose(file);
    }
}

void bus() {
    system("cls");
    printf("*****Welcome to Digitravellers. Please choose one of the options below*****\n");
    printf("\t\t\t[1]=> View bus schedules\n");
    printf("\t\t\t[2]=> Book a bus\n");
    printf("\t\t\t[3]=> Cancel booking\n");
    printf("\t\t\t[4]=> Add a new bus schedule\n");
    printf("\t\t\t[5]=> Delete a bus schedule\n");
    printf("\t\t\t[6]=> Modify account\n");
    printf("\t\t\t[7]=> Exit\n");
}

void bookBus(struct Customer *customer) {
    printf("Enter customer's name: ");
    scanf(" %[^\n]", customer->name);

    // Input validation for 8-digit ID number
    do {
      printf("Enter customer's 8-digit ID number: ");
    } while (scanf("%8d", &(customer->idNumber)) != 1 || customer->idNumber < 10000000 || customer->idNumber > 99999999);

    // Input validation for 10-digit phone number
    do {
      printf("Enter customer's 10-digit phone number: ");
    } while (scanf("%lld", &(customer->phoneNumber)) != 1 || customer->phoneNumber < 1000000000LL || customer->phoneNumber > 9999999999LL);

    //create a seat format for empty and taken seats

    printf("Enter seat number: ");
    scanf("%d", &(customer->seatNumber));
}


void viewBusSchedules() {
    if (userData.numSchedules > 0) {
        printf("Bus Schedules:\n");
        printf("Index  Bus Code  Timeline            Route\n");
        for (int i = 0; i < userData.numSchedules; i++) {
            printf("%-6d %-9s %-19s %s\n", i, userData.busSchedules[i].busCode, userData.busSchedules[i].timeLine, userData.busSchedules[i].route);
        }
    } else {
        printf("No bus schedules available.\n");
    }
}

void loadBusSchedules() {
    FILE *file = fopen(SCHEDULES_FILE, "rb");
    if (file == NULL) {
        // Handle the case when the file doesn't exist
        return;
    }

    userData.numSchedules = 0;
    while (fread(&userData.busSchedules[userData.numSchedules], sizeof(struct BusSchedule), 1, file) == 1) {
        userData.numSchedules++;
    }

    fclose(file);
}
void printReceipt(struct Customer *customer, struct BusSchedule *busSchedule){
    system("cls"); // Clear the screen
    printf("================= Booking Receipt =================\n");
    printf("************ Thank you for choosing Digitravellers ************\n");

    printf("Bus Schedule Details:\n");
    printf("Bus Code: %s\n", busSchedule->busCode);
    printf("Timeline: %s\n", busSchedule->timeLine);
    printf("Route: %s\n", busSchedule->route);

    printf("Customer Details:\n");
    printf("Name: %s\n", customer->name);
    printf("ID Number: %d\n", customer->idNumber);
    printf("Phone Number: %lld\n", customer->phoneNumber);
    printf("Seat Number: %d\n", customer->seatNumber);

    printf("************ Enjoy your journey with Digitravellers! ************\n");
}


void saveBusSchedules() {
    FILE *file = fopen(SCHEDULES_FILE, "wb");
    if (file == NULL) {
        return;
    }

    for (int i = 0; i < userData.numSchedules; i++) {
        fwrite(&userData.busSchedules[i], sizeof(struct BusSchedule), 1, file);
    }

    fclose(file);
}

void addBusSchedule() {
    if (userData.numSchedules < MAX_BUSES) {
        struct BusSchedule newSchedule;

        printf("Enter timeline: ");
        scanf("%s", newSchedule.timeLine);

        printf("Enter route: ");
        scanf("%s", newSchedule.route);

        printf("Enter bus code: ");
        scanf("%s", newSchedule.busCode);

        userData.busSchedules[userData.numSchedules] = newSchedule;
        userData.numSchedules++;
        printf("Schedule added successfully!\n");
    } else {
        printf("The schedule list is full. Unable to add more schedules.\n");
    }
}//payment to be added on the schedule as per the journey(admins decision)

void deleteBusSchedule(int index) {
    if (index >= 0 && index < userData.numSchedules) {
        for (int i = index; i < userData.numSchedules - 1; i++) {
            userData.busSchedules[i] = userData.busSchedules[i + 1];
        }
        userData.numSchedules--;
        printf("Schedule deleted successfully!\n");
    } else {
        printf("Invalid schedule index.\n");
    }
}
