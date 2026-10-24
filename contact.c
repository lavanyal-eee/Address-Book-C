#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
#include "populate.h"


// ----------------- Helper validation functions (reused) -----------------
int isValidName(const char *name) 
{
    if (strlen(name) == 0) 
    return 0;
    for (int i = 0; name[i] != '\0'; i++) 
    {
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }
    return 1;
}

int isValidPhone(const char *phone) 
{
    if (strlen(phone) != 10) 
    return 0;
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i]))
            return 0;
    }
    return 1;
}

int isUniquePhone(AddressBook *ab, const char *phone)
{
    for (int i = 0; i < ab->contactCount; i++) {
        if (strcmp(ab->contacts[i].phone, phone) == 0)
            return 0;
    }
    return 1;
}

int isValidEmail(const char *email)
{
    char *at = strchr(email, '@');
    char *dot = strstr(email, ".com");
    if (!at || !dot) 
    return 0;
    if (at == email) 
    return 0;               // nothing before @
    if (dot <= at + 1) 
    return 0;              // no text between @ and .com
    if (*(dot + 4) != '\0') 
    return 0;         // must end with .com
    return 1;
}
int isUniqueEmail(AddressBook *ab, const char *email)
{
    for (int i = 0; i < ab->contactCount; i++) {
        if (strcmp(ab->contacts[i].email, email) == 0)
            return 0; // already exists
    }
    return 1; // unique
}

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria
    printf("+------+----------------+--------------+-----------------------+\n");
    printf("| S.No | Name           | Phone        | Email                 |\n");
    printf("+------+----------------+--------------+-----------------------+\n");
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        printf("| %-4d | %-14s | %-12s | %-21s |\n", addressBook->contacts[i].serialNo, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
    printf("+------+----------------+--------------+-----------------------+\n");
    
}

void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
   // loadContactsFromFile(addressBook);
    
    // Restore serial numbers

    for (int i = 0; i < addressBook->contactCount; i++) {
        addressBook->contacts[i].serialNo = i + 1;
    }
}

void saveAndExit(AddressBook *addressBook) 
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    // ----------------- Create Contact -----------------
    if (addressBook->contactCount >= MAX_CONTACTS) 
    {
        printf("Address Book is full!\n");
        return;
    }

    Contact c;
    c.serialNo = addressBook->contactCount + 1;

    // Name input
    do 
    {
        printf("Enter name [Maximum 49 characters]: ");
        scanf(" %[^\n]", c.name);
        if (!isValidName(c.name)) {
            printf("Invalid name. Please enter only alphabetic characters and spaces.\n");
        }
    } 
    while (!isValidName(c.name));

    // Phone input
    do 
    {
        printf("Enter phone number [Maximum 10 digits]: ");
        scanf("%s", c.phone);
        if (!isValidPhone(c.phone)) 
        {
            printf("Invalid phone number. Must be exactly 10 digits and only numbers.\n");
            continue;
        }
        if (!isUniquePhone(addressBook, c.phone)) 
        {
            printf("This phone number already exists. Please enter a unique number.\n");
            continue;
        }
        break;
    } 
    while (1);

    // Email input
    do 
    {
        printf("Enter email [Maximum 49 characters]: ");
        scanf("%s", c.email);
        if (!isValidEmail(c.email)) 
        {
            printf("Invalid email format. Make sure it has proper structure (e.g. a@b.com).\n");
            continue;
        }
        if (!isUniqueEmail(addressBook, c.email)) {
            printf("This email already exists. Enter a unique one.\n");
            continue;
        }
        break;
    } 
    while (1);

    addressBook->contacts[addressBook->contactCount++] = c;
    printf("Contact added successfully\n");
}

void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    // ----------------- Search Contact -----------------
    int option;
    char searchValue[50];
    int found = 0;

    printf("Search by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");

    if (scanf("%d", &option) != 1 || option < 1 || option > 3)
    {
        printf("Invalid input. Please enter a number between 1 and 3.\n");
        while (getchar() != '\n');
        return;
    }

    switch (option) 
    {
        case 1:
            printf("Enter name to search: ");
            scanf(" %[^\n]", searchValue);
            break;

        case 2:
            printf("Enter phone number to search: ");
            scanf("%s", searchValue);
            break;

        case 3:
            printf("Enter email to search: ");
            scanf("%s", searchValue);
            break;
    }

    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        int match = 0;

        if (option == 1 && strcasecmp(addressBook->contacts[i].name, searchValue) == 0) 
        {
            match = 1;
        } else if (option == 2 && strcmp(addressBook->contacts[i].phone, searchValue) == 0) 
        {
            match = 1;
        } else if (option == 3 && strcmp(addressBook->contacts[i].email, searchValue) == 0) 
        {
            match = 1;
        }

        if (match) 
        {
            if (!found) // Print header once
            { 
                printf("+------+----------------+--------------+-----------------------+\n");
                printf("| S.No | Name           | Phone        | Email                 |\n");
                printf("+------+----------------+--------------+-----------------------+\n");
            }
            printf("| %-4d | %-14s | %-12s | %-21s |\n",
                   addressBook->contacts[i].serialNo,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
            found = 1;
        }
    }

    if (found) 
    {
        printf("+------+----------------+--------------+-----------------------+\n");
    } 
    else 
    {
        printf("No contact found.\n");
    }
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    
    int searchOption;
    char searchValue[50];
    int matchIndexes[100];
    int matchCount = 0;

    printf("Search by to edit:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");

    if (scanf("%d", &searchOption) != 1 || searchOption < 1 || searchOption > 3) 
    {
        printf("Invalid choice.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter the value to search: ");
    scanf(" %[^\n]", searchValue);

    // Search and store matching indexes
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if ((searchOption == 1 && strcasecmp(addressBook->contacts[i].name, searchValue) == 0) ||
            (searchOption == 2 && strcmp(addressBook->contacts[i].phone, searchValue) == 0) ||
            (searchOption == 3 && strcmp(addressBook->contacts[i].email, searchValue) == 0)) 
        {
            matchIndexes[matchCount++] = i;
        }
    }

    if (matchCount == 0) 
    {
        printf("No contacts found.\n");
        return;
    }

    // Display all matches
    printf("+------+----------------+--------------+-----------------------+\n");
    printf("| S.No | Name           | Phone        | Email                 |\n");
    printf("+------+----------------+--------------+-----------------------+\n");
    for (int i = 0; i < matchCount; i++) 
    {
        int idx = matchIndexes[i];
        printf("| %-4d | %-14s | %-12s | %-21s |\n",
               idx,
               addressBook->contacts[idx].name,
               addressBook->contacts[idx].phone,
               addressBook->contacts[idx].email);
    }
    printf("+------+----------------+--------------+-----------------------+\n");

    // Ask user to choose index to edit
    int editIndex;
    printf("\nEnter the index of contact to edit: ");
    scanf("%d", &editIndex);

    int validChoice = 0;
    for (int i = 0; i < matchCount; i++) 
    {
        if (editIndex == matchIndexes[i]) 
        {
            validChoice = 1;
            break;
        }
    }
    if (!validChoice) {
        printf("Invalid index.\n");
        return;
    }

    // Ask what to edit
    int editField;
    printf("What do you want to edit?\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &editField);

    char newValue[50];
    switch (editField) {
        case 1:
            do 
            {
                printf("Enter new name: ");
                scanf(" %[^\n]", newValue);
                if (!isValidName(newValue))
                    printf("Invalid name. Only alphabets and spaces allowed.\n");
            } 
            while (!isValidName(newValue));
            strcpy(addressBook->contacts[editIndex].name, newValue);
            printf("Name updated successfully.\n");
            break;

        case 2:
            do 
            {
                printf("Enter new phone number: ");
                scanf("%s", newValue);
                if (!isValidPhone(newValue)) {
                    printf("Invalid phone. Must be 10 digits.\n");
                    continue;
                }
                if (!isUniquePhone(addressBook, newValue)) {
                    printf("Phone number already exists.\n");
                    continue;
                }
                break;
            } 
            while (1);
            strcpy(addressBook->contacts[editIndex].phone, newValue);
            printf("Phone number updated successfully.\n");
            break;

        case 3:
            do 
            {
                printf("Enter new email: ");
                scanf("%s", newValue);
                if (!isValidEmail(newValue))
                    printf("Invalid email format.\n");
            } 
            while (!isValidEmail(newValue));
            strcpy(addressBook->contacts[editIndex].email, newValue);
            printf("Email updated successfully.\n");
            break;

        default:
            printf("Invalid choice.\n");
    }
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    if (addressBook->contactCount == 0) 
    {
        printf("Address Book is empty. Nothing to delete.\n");
        return;
    }

    int option;
    char searchValue[50];
    int foundIndexes[MAX_CONTACTS];
    int foundCount = 0;

    printf("Delete contact by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");

    if (scanf("%d", &option) != 1 || option < 1 || option > 3) 
    {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter the value to search: ");
    if (option == 1)
        scanf(" %[^\n]", searchValue);
    else
        scanf("%s", searchValue);

    // Search for matching contacts
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        int match = 0;
        if (option == 1 && strcasecmp(addressBook->contacts[i].name, searchValue) == 0)
            match = 1;
        else if (option == 2 && strcmp(addressBook->contacts[i].phone, searchValue) == 0)
            match = 1;
        else if (option == 3 && strcmp(addressBook->contacts[i].email, searchValue) == 0)
            match = 1;

        if (match) 
        {
            foundIndexes[foundCount++] = i;
        }
    }

    if (foundCount == 0) 
    {
        printf("No matching contact found.\n");
        return;
    }

    // Display matches
    printf("Matching contact(s):\n");
    printf("+------+----------------+--------------+-----------------------+\n");
    printf("| S.No | Name           | Phone        | Email                 |\n");
    printf("+------+----------------+--------------+-----------------------+\n");
    for (int k = 0; k < foundCount; k++) 
    {
        int idx = foundIndexes[k];
        printf("| %-4d | %-14s | %-12s | %-21s |\n",
               addressBook->contacts[idx].serialNo,
               addressBook->contacts[idx].name,
               addressBook->contacts[idx].phone,
               addressBook->contacts[idx].email);
    }
    printf("+------+----------------+--------------+-----------------------+\n");

    int deleteIndex;
    if (foundCount == 1)
    {
        deleteIndex = foundIndexes[0];
    } 
    else 
    {
        printf("Multiple matches found. Enter the S.No to delete: ");
        int serialChoice;
        scanf("%d", &serialChoice);

        deleteIndex = -1;
        for (int k = 0; k < foundCount; k++) 
        {
            if (addressBook->contacts[foundIndexes[k]].serialNo == serialChoice)
            {
                deleteIndex = foundIndexes[k];
                break;
            }
        }
        if (deleteIndex == -1) 
        {
            printf("Invalid S.No entered.\n");
            return;
        }
    }

    // Shift contacts after deleteIndex
    for (int i = deleteIndex; i < addressBook->contactCount - 1; i++) 
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--;

    // Update serial numbers
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        addressBook->contacts[i].serialNo = i + 1;
    }

    printf("Contact deleted successfully.\n");
    
}
   

