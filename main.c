#include <stdio.h>
#include <stdlib.h>
#include "contact.h"
#include "file.h"

int main() 
{
    int choice;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book
    loadContactsFromFile(&addressBook); // Load saved contacts automatically

    do 
    { printf("\n+---------------------------------------+\n");
        printf("|        Address Book Menu              |\n");
        printf("+---------------------------------------+\n");
        printf("| 1. Create contact                     |\n");
        printf("| 2. Search Contact                     |\n");
        printf("| 3. Edit contact                       |\n");
        printf("| 4. Delete contact                     |\n");
        printf("| 5. List all contacts                  |\n");
    	printf("| 6. Save and Exit                      |\n");	
        printf("+---------------------------------------+\n");	
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:          
                listContacts(&addressBook);
                break;
            case 6:
                {
                char saveChoice;
                printf("Do you want to save changes before exit? (y/n): ");
                scanf(" %c", &saveChoice);

                if (saveChoice == 'y' || saveChoice == 'Y') 
                {
                    saveContactsToFile(&addressBook);
                    printf("Contacts saved successfully.\n");
                } 
                else 
                {
                    printf("Changes not saved.\n");
                }

                printf("Exiting program. Goodbye!\n");
                exit(0);
            }
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } 
    while (1);
    
    return 0;
}
