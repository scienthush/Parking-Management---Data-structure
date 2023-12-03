#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void loadParkingSpaceIds();
struct Vehicle
{
    char ticket[10];
    char licensePlate[15];
    int type;
};

struct ParkingSpace
{
    char spaceID[5];
    int isOccupied;
    int vehicleType;
};

struct Node
{
    struct Vehicle data;
    struct Node *next;
};

// Function to create a new node
struct Node *createNode(struct Vehicle data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Global variables
struct Node *vehicles = NULL;
struct ParkingSpace parkingSpaces[100];
int totalSpaces = 100;

// Function to register a vehicle entry
void registerEntry()
{
    struct Vehicle newVehicle;

    printf("Enter the license plate number: ");
    scanf("%s", newVehicle.licensePlate);

    printf("Select the type of vehicle:\n");
    printf("1. Car\n2. Motorcycle\n3. Truck\n");
    scanf("%d", &newVehicle.type);

    sprintf(newVehicle.ticket, "%04d-%c%c", rand() % 10000, 'A' + rand() % 26,
            'A' + rand() % 26);

    struct Node *newNode = createNode(newVehicle);
    if (vehicles == NULL)
    {
        vehicles = newNode;
    }
    else
    {
        struct Node *current = vehicles;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }

    printf("\n\nVehicle registered successfully.....\n", newVehicle.ticket);
    printf("---------------------------------------\n", newVehicle.ticket);
    printf("Ticket:                  | %s\n", newVehicle.ticket);
    allocateParkingSpace(newVehicle);
    saveVehicleDataToFile();
}

// Function to allocate a parking space for a vehicle
void allocateParkingSpace(struct Vehicle vehicle)
{
    int i;
    loadParkingSpaceIds();
    for (i = 0; i < totalSpaces; i++)
    {
        if (parkingSpaces[i].isOccupied == 0)
        {
            parkingSpaces[i].isOccupied = 1;
            parkingSpaces[i].vehicleType = vehicle.type;
            printf("---------------------------------------\n");
            printf("Allocated parking space: | %s\n", parkingSpaces[i].spaceID);
            printf("---------------------------------------\n\n");
            printf("\n\nHit enter to continue......\n");
            getch();
            system("cls");
            // Save parking space ID to file
            FILE *file = fopen("parking_space_ids.txt", "a");
            if (file == NULL)
            {
                printf("Error: Unable to open file for writing.\n");
                return;
            }
            fprintf(file, "%s %d\n", parkingSpaces[i].spaceID,parkingSpaces[i].vehicleType);
            fclose(file);
            break;
        }
    }
    if (i == totalSpaces)
    {
        printf("Sorry, the parking lot is full. Cannot allocate a space for this "
               "vehicle.\n");
        getch();
        system("cls");
    }
}

// Function to validate a parking ticket
int validateParkingTicket(char ticket[10])
{
    struct Node *current = vehicles;

    while (current != NULL)
    {
        if (strcmp(current->data.ticket, ticket) == 0)
        {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

// Function to calculate the parking fee for a vehicle
double calculateParkingFee(struct Vehicle vehicle)
{

    double fee = 0.0;

    switch (vehicle.type)
    {
    case 1: // Car
        fee = 20.0;
        break;
    case 2: // Motorcycle
        fee = 10.0;
        break;
    case 3: // Truck
        fee = 50.0;
        break;
    default:
        printf("Invalid vehicle type.\n");
        break;
    }

    return fee;
}


// Function to handle vehicle exit
void exitVehicle()
{
    char ticket[10];

    printf("Enter the parking ticket: ");
    scanf("%s", ticket);

    if (validateParkingTicket(ticket))
    {

        struct Node *current = vehicles;
        struct Vehicle exitingVehicle;
        int found = 0;

        while (current != NULL)
        {
            if (strcmp(current->data.ticket, ticket) == 0)
            {
                exitingVehicle = current->data;
                found = 1;
                break;
            }
            current = current->next;
        }

        if (found)
        {


            double parkingFee = calculateParkingFee(exitingVehicle);
            printf("-----------------------------------------------------\n");
            printf("Vehicle exited successfully. Parking fee: $%.2f\n", parkingFee);
            printf("-----------------------------------------------------\n\n\n");
        }
        else
        {
            printf("-------------------------------------------------------\n");
            printf("Error: Vehicle not found with the provided ticket.\n");
            printf("-------------------------------------------------------\n\n\n");
        }
    }
    else
    {
        printf("\n----------------------------------------------\n");
        printf("  Error !!! Invalid or expired parking ticket. \n");
        printf("----------------------------------------------\n\n\n");

    }
    printf("Hit enter to continue......\n");
    getch();
    system("cls");

}

// Function to display the parking lot status
void displayParkingLotStatus()
{
    printf("Parking Lot Status:\n");
    loadParkingSpaceIds();
    for (int i = 0; i < totalSpaces; i++)
    {
        if (parkingSpaces[i].isOccupied)
        {
            printf("Space %s: Occupied by a ", parkingSpaces[i].spaceID);
            switch (parkingSpaces[i].vehicleType)
            {
            case 1:
                printf("Car\n");
                break;
            case 2:
                printf("Motorcycle\n");
                break;
            case 3:
                printf("Truck\n");
                break;
            default:
                printf("Unknown Vehicle Type\n");
            }
        }
        else
        {
            printf("Space %s: Vacant\n", parkingSpaces[i].spaceID);
        }
    }
    printf("Enter any button to continue.......");
    getch();
    system("cls");
}

// Function to search for a vehicle based on ticket or license plate number
void searchVehicle(char key[10])
{
    struct Node *current = vehicles;
    int found = 0;

    while (current != NULL)
    {
        if (strcmp(current->data.ticket, key) == 0 ||
                strcmp(current->data.licensePlate, key) == 0)
        {
            found = 1;
            system("cls");

            printf("==============================================\n");
            printf("              Vehicle Details:\n");
            printf("==============================================\n");
            printf("   Ticket:          |  %s\n", current->data.ticket);
            printf("   License Plate:   |  %s\n", current->data.licensePlate);
            printf("   Vehicle Type:    |  ");
            switch (current->data.type)
            {
            case 1:
                printf("Car\n");
                break;
            case 2:
                printf("Motorcycle\n");
                break;
            case 3:
                printf("Truck\n");
                break;
            default:
                printf("Unknown Vehicle Type\n");

            }
            printf("==============================================\n");
            break;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("\nVehicle not found with the provided ticket or license plate !!!\n");
    }
    getch();
    system("cls");

}



// Function to generate reports
void generateReports()
{

    int totalVehicles = 0;
    int cars = 0, motorcycles = 0, trucks = 0;
    double totalRevenue = 0.0;

    struct Node *current = vehicles;

    while (current != NULL)
    {
        totalVehicles++;
        switch (current->data.type)
        {
        case 1:
            cars++;
            break;
        case 2:
            motorcycles++;
            break;
        case 3:
            trucks++;
            break;
        default:
            break;
        }
        totalRevenue += calculateParkingFee(current->data);

        current = current->next;
    }
    system("cls");
    printf("======================================================\n");
    printf("*                      Reports:                      *\n");
    printf("======================================================\n");
    printf("Total Vehicles Parked: |    %d\n", totalVehicles);
    printf("-----------------------------------------------------\n");
    printf("Cars:                  |    %d\n", cars);
    printf("-----------------------------------------------------\n");
    printf("Motorcycles:           |    %d\n", motorcycles);
    printf("-----------------------------------------------------\n");
    printf("Trucks:                |    %d\n", trucks);
    printf("-----------------------------------------------------\n");
    printf("Total Revenue:         |    $%.2f\n", totalRevenue);
    printf("-----------------------------------------------------\n\n\n");

    printf("Hit enter to continue........\n");
    getch();
    system("cls");
}

//function to read  vehicle data from file
void readVehicleFile()
{
    FILE *file = fopen("vehicle_data.txt", "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file for reading.\n");
        return;
    }

    int i = 0,d;
    char l[15],t[10];
    struct Node *current = vehicles;
    while (fscanf(file, "%s %s %d", t,l, &d) != EOF)
    {
        struct Vehicle newVehicle;
        strcpy(newVehicle.licensePlate,l);
        strcpy(newVehicle.ticket,t);
        newVehicle.type=d;
        struct Node *newNode = createNode(newVehicle);
        if (vehicles == NULL)
        {
            vehicles = newNode;
        }
        else
        {
            struct Node *current = vehicles;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newNode;
        }
    }
    fclose(file);
}

// Function to save vehicle data to a file
void saveVehicleDataToFile()
{
    FILE *file = fopen("vehicle_data.txt", "w");
    if (file == NULL)
    {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    struct Node *current = vehicles;

    while (current != NULL)
    {
        fprintf(file, "%s %s %d\n", current->data.ticket, current->data.licensePlate, current->data.type);
        current = current->next;
    }

    fclose(file);
}

// Function to load parking space IDs from file
void loadParkingSpaceIds()
{
    FILE *file = fopen("parking_space_ids.txt", "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file for reading.\n");
        return;
    }

    int i = 0,d;
    char line[10];

    while (i < totalSpaces && fscanf(file, "%s %d", line, &d) != EOF)
    {
        strcpy(parkingSpaces[i].spaceID, line);
        parkingSpaces[i].vehicleType=d;
        parkingSpaces[i].isOccupied = 1;
        i++;
    }

    fclose(file);
}

// Function to authenticate user
int authenticateUser(char username[20], char password[20])
{
    const char correctUsername[] = "admin";
    const char correctPassword[] = "admin";

    if (strcmp(username, correctUsername) == 0 &&
            strcmp(password, correctPassword) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//main function
int main()
{
    char username[20];
    char password[20];
    char searchKey[10];

    printf("\n\n\n\n=======================================================\n");
    printf("|     ***      Parking Management System       ***    |\n");
    printf("=======================================================\n\n");

    printf("                       Login                       \n");
    printf("           ----------------------------             \n");
    printf("              Enter username: ");
    scanf("%s", username);
    printf("              Enter password: ");
    scanf("%s", password);

    if (authenticateUser(username, password))
    {

        printf("\n      Authentication successful........\n");
        clock_t startTime = clock();
        while ((double)(clock() - startTime) / CLOCKS_PER_SEC < 1.0);
        system("cls");
        readVehicleFile();
        // Initialize parking spaces
        for (int i = 0; i < totalSpaces; i++)
        {
            sprintf(parkingSpaces[i].spaceID, "P%03d", i + 1);
            parkingSpaces[i].isOccupied = 0;
            parkingSpaces[i].vehicleType = 0;
        }

        while (1)
        {
            int choice;
            printf("----------------------------------------------------------------------------\n");
            printf("|                       Parking management System                          |\n");
            printf("----------------------------------------------------------------------------\n");
            printf("|                             Main Menu                                    |\n");
            printf("|                          ---------------                                 |\n");
            printf("|                       1. Register Entry                                  |\n");
            printf("|                       2. Exit Vehicle                                    |\n");
            printf("|                       3. Check Parking Lot Status                        |\n");
            printf("|                       4. Search for a Vehicle                            |\n");
            printf("|                       5. Generate Reports                                |\n");
            printf("|                       6. Exit                                            |\n");
            printf("----------------------------------------------------------------------------\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                registerEntry();
                break;
            case 2:
                exitVehicle();
                break;
            case 3:
                displayParkingLotStatus();
                break;
            case 4:
                printf("Enter the ticket or license plate number to search: ");
                scanf("%s", searchKey);
                searchVehicle(searchKey);
                break;
            case 5:
                generateReports();
                break;
            case 6:
                // Save vehicle data to file before exiting
                saveVehicleDataToFile();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
            }
        }
    }
    else
    {
        printf("\nAuthentication failed !! Access denied......\n");
    }

    return 0;
}
