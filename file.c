#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("contacts.txt", "w");
    if (!fptr) {
        printf("Error opening file for writing!\n");
        return;
    }

    // Write contact count first
    fprintf(fptr, "%d\n", addressBook->contactCount);

    // Write each contact
    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
   // printf("Contacts saved successfully!\n");
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("contacts.txt", "r");
    if (!fptr) {
        printf("No saved contacts found.\n");
        return;
    }

    addressBook->contactCount = 0; // start fresh

    char line[200];
    int totalContacts = 0;

    // Read first line as contact count
    if (fgets(line, sizeof(line), fptr)) {
        totalContacts = atoi(line);
    }

    for (int i = 0; i < totalContacts; i++) {
        if (!fgets(line, sizeof(line), fptr)) break; // no more lines

        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Split line by commas
        char *name = strtok(line, ",");
        char *phone = strtok(NULL, ",");
        char *email = strtok(NULL, ",");

        if (!name || !phone || !email) {
           // printf("Skipping invalid contact at line %d\n", i + 2);
            continue;
        }

        strncpy(addressBook->contacts[addressBook->contactCount].name, name, 49);
        strncpy(addressBook->contacts[addressBook->contactCount].phone, phone, 19);
        strncpy(addressBook->contacts[addressBook->contactCount].email, email, 49);
        addressBook->contacts[addressBook->contactCount].serialNo = addressBook->contactCount + 1;

        addressBook->contactCount++;
    }

    fclose(fptr);
    printf("Successfully loaded Contacts count: %d\n", addressBook->contactCount);
}