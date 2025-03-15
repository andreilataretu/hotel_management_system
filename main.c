#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Reservation {
    int roomNumber;
    char guestName[50];
    struct Reservation *next;
} Reservation;

Reservation *head = NULL;

Reservation *createNode(int roomNumber, char *guestName) {
    Reservation *newNode = (Reservation *)malloc(sizeof(Reservation));
    if (newNode == NULL) {
        printf("Error at memory allocation\n");
        exit(1);
    }

    newNode->roomNumber = roomNumber;
    strncpy(newNode->guestName, guestName, sizeof(newNode->guestName) - 1);
    newNode->guestName[sizeof(newNode->guestName) - 1] = '\0';
    newNode->next = NULL;
    return newNode;
}

void addReservation(int roomNumber, char *guestName) {
    Reservation *newReservation = createNode(roomNumber, guestName);

    if (head == NULL) {
        head = newReservation;
    } else {
        newReservation->next = head;
        head = newReservation;
    }

    printf("Reservation added with success!\n");
}

void viewReservations() {
    Reservation *current = head;
    if (current == NULL) {
        printf("No reservations found!\n");
        return;
    }

    printf("\nReservation list:\n");
    while (current != NULL) {
        printf("Room: %d\tGuest: %s\n", current->roomNumber, current->guestName);
        current = current->next;
    }
}

void searchReservation(int roomNumber) {
    Reservation *current = head;

    while (current != NULL) {
        if (current->roomNumber == roomNumber) {
            printf("Reservation found - Room: %d\tGuest: %s\n", current->roomNumber, current->guestName);
            return;
        }
        current = current->next;
    }

    printf("Reservation doesn't exist for room %d.\n", roomNumber);
}

void deleteReservation(int roomNumber) {
    Reservation *current = head, *prev = NULL;

    while (current != NULL) {
        if (current->roomNumber == roomNumber) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Reservation deleted with success!\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Reservation doesn't exist for room %d.\n", roomNumber);
}

void saveToFile() {
    FILE *file = fopen("reservations.txt", "w");
    if (file == NULL) {
        printf("Error at file opening!\n");
        return;
    }

    Reservation *current = head;
    while (current != NULL) {
        fprintf(file, "%d %s\n", current->roomNumber, current->guestName);
        current = current->next;
    }
    fclose(file);
    printf("Reservations saved!\n");
}

void loadFromFile() {
    FILE *file = fopen("reservations.txt", "r");
    if (file == NULL) {
        return;
    }

    int roomNumber;
    char guestName[50];

    while (fgets(guestName, sizeof(guestName), file) != NULL) {
        if (sscanf(guestName, "%d %[^\n]", &roomNumber, guestName) == 2) {
            addReservation(roomNumber, guestName);
        }
    }
    fclose(file);
}

int main() {
    int option, roomNumber;
    char guestName[50];

    loadFromFile();

    do {
        printf("\nHotel Management System\n");
        printf("1. Add reservation\n");
        printf("2. See reservations\n");
        printf("3. Search for reservation\n");
        printf("4. Delete Reservation\n");
        printf("5. Save and exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);
        getchar(); 

        switch (option) {
            case 1:
                printf("Room number: ");
                scanf("%d", &roomNumber);
                getchar(); 
                printf("Guest name: ");
                fgets(guestName, sizeof(guestName), stdin);
                guestName[strcspn(guestName, "\n")] = '\0'; 
                addReservation(roomNumber, guestName);
                break;
            case 2:
                viewReservations();
                break;
            case 3:
                printf("Room number to search: ");
                scanf("%d", &roomNumber);
                searchReservation(roomNumber);
                break;
            case 4:
                printf("Room number to delete: ");
                scanf("%d", &roomNumber);
                deleteReservation(roomNumber);
                break;
            case 5:
                saveToFile();
                printf("Exit... Reservations saved!\n");
                break;
            default:
                printf("Invalid option, try again.\n");
        }

    } while (option != 5);

    return 0;
}
