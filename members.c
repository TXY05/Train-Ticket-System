#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#pragma warning(disable:4996)
#pragma warning(disable:6031)
#pragma warning(disable:6054)

#define MAX_ID_LENGTH 6
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20
#define MAX_GENDER_LENGTH 7
#define MAX_IC_LENGTH 13
#define MAX_CONTACT_LENGTH 15
#define MAX_SECURITY_QUESTION_LENGTH 100
#define MAX_SECURITY_ANSWER_LENGTH 100
#define MAX_CHOICE_LENGTH 6

#define STATE_CODE_LENGTH 2

const char* STATE_CODES = "0123456789"; // State codes represented as numbers

#define MAX_STREETNAMESTAFF 30
#define MAX_RESIDENTAREASTAFF 30
#define MAX_CITYSTAFF 20
#define MAX_KEYSTAFF 10
#define MAX_IDSTAFF 7
#define MAX_NAMESTAFF 25
#define MAX_POSITIONSTAFF 20
#define MAX_PASSWORDSTAFF 30
#define MAX_STRINGSTAFF 30
#define MAX_FORGOTPASSSTAFF 30

#define MEMBER_INFO_FILE "members.txt"

typedef struct {
    int houseNum;
    char streetName[MAX_STREETNAMESTAFF];
    char residentArea[MAX_RESIDENTAREASTAFF];
    char city[MAX_CITYSTAFF];
}Address;

typedef struct {
    int num;
    char encryptionKey[MAX_KEYSTAFF];
    char id[MAX_IDSTAFF];
    char name[MAX_NAMESTAFF];
    char position[MAX_POSITIONSTAFF];
    char password[MAX_PASSWORDSTAFF];
    char forgotPass[MAX_FORGOTPASSSTAFF];
    int forgotPassQues;
    Address staffAddress;
}Staff;

typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
}logDate;

struct Member {
    char id[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char passwordRecovery[MAX_PASSWORD_LENGTH];
    char gender[MAX_GENDER_LENGTH];
    char ic[MAX_IC_LENGTH];
    char contact[MAX_CONTACT_LENGTH];
    char securityQuestion1[MAX_SECURITY_QUESTION_LENGTH];
    char securityAnswer1[MAX_SECURITY_ANSWER_LENGTH];
};

// Functions
void displayMembersMenu();
void displayAdminMenu(Staff* pStaffInfo);
void memberOptions(struct Member* member);
void loginMember();
void displayMemberDetails(struct Member member);
void modifyMemberDetails(struct Member* member);
void saveModifiedMemberDetails(struct Member* member);
void registerMember();
bool icValidate(const char* ic);
bool isIdTaken(const char* id);
char* generateUniqueId();
void searchMember();
void modifyMember(Staff* pStaffInfo);
void displayMembers();
void resetPassword();
void deleteMembers(Staff* pStaffInfo);

// Function to display menu options for members register/login mode
void displayMembersMenu() {
    updateValidDate();
    int choice;

    do {
        system("cls");
        welcome();
        printf("\n=== Members Register/Login Menu ===\n");
        printf("1. Register As Members\n");
        printf("2. Login To Members\n");
        printf("3. Reset Password\n");
        printf("4. Exit to Main Menu\n");
        line();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        rewind(stdin);
        system("cls");

        switch (choice) {
        case 1:
            registerMember();
            break;
        case 2:
            loginMember();
            break;
        case 3:
            resetPassword();
            break;
        case 4:
            break;
        default:
            printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4);
}

// Function to display menu options for Staff Admin
void displayAdminMenu(Staff* pStaffInfo) {
    int choice;

    do {
        system("cls");

        welcome();

        printf("========== Edit Member =========\n");
        printf("1. Search Member\n");
        printf("2. Modify Member\n");
        printf("3. Display All Members\n");
        printf("4. Delete Member\n");
        printf("5. Back\n");
        line();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        rewind(stdin);

        switch (choice) {
        case 1:
            searchMember();
            break;
        case 2:
            modifyMember(pStaffInfo);
            break;
        case 3:
            displayMembers();
            break;
        case 4:
            deleteMembers(pStaffInfo);
            break;
        case 5:
            break;
        default:
            printf("Invalid choice! Please enter a number between 1 and 5.\n");
        }
    } while (choice != 5);
}

// Function to display member options for student after login
void memberOptions(struct Member* member) {
    int choice;
    while (1) {
        system("cls");
        welcome();
        displayMemberDetails(*member);
        printf("\n=== Member Options ===\n");
        printf("1. Modify Details\n");
        printf("2. Check Train Schedule\n");
        printf("3. Check Ticket\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        rewind(stdin);

        switch (choice) {
        case 1:
            system("cls");
            modifyMemberDetails(member);
            break;
        case 2:
            system("cls");
            checkSchedule(member);
            break;
        case 3:
            system("cls");
            viewTicMenu(member);
            break;
        case 4:
            system("cls");
            return; // Logout
        default:
            printf("Invalid choice!\n");
            break;
        }
    }
}

// Function to login member
void loginMember() {
    char memberIc[MAX_IC_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    struct Member member;
    FILE* file;

    file = fopen(MEMBER_INFO_FILE, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    welcome();
    printf("========= Member Login =========\n");
    printf("Enter \"-1\" to Exit\n");
    line();

    do {
        // Reset the file pointer to the beginning
        rewind(file);

        // Prompt for IC Number and Password
        printf("\nEnter Your IC Number: ");
        scanf("%s", memberIc);
        rewind(stdin);
        if (strcmp(memberIc, "-1") == 0) {
            system("cls");
            fclose(file);
            return;
        }

        printf("Enter Password: ");
        scanf("%s", password);
        rewind(stdin);
        if (strcmp(password, "-1") == 0) {
            system("cls");
            fclose(file);
            return;
        }

        // Authenticate member
        while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", member.id, member.name, member.password, member.passwordRecovery, member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1) != EOF) {
            if (strcmp(member.ic, memberIc) == 0 && strcmp(member.password, password) == 0) {
                system("cls");
                printf("\nLogin successful!\n");

                // Display Member Menu when login successful
                memberOptions(&member);
                fclose(file);
                return;
            }
        }

        // If authentication fails
        printf("Invalid Member IC or Password!\n");

    } while (true);
    fclose(file);
    system("pause");
}

// Function to display member details
void displayMemberDetails(struct Member member) {
    printf("\n=== Member Details ===\n");
    printf("ID: %s\n", member.id);
    printf("Name: %s\n", member.name);
    printf("Gender: %s\n", member.gender);
    printf("IC: %s\n", member.ic);
    printf("Contact: %s\n", member.contact);
    printf("Password: %s\n", member.password);
    printf("Password Recovery: %s\n", member.passwordRecovery);
    printf("Security Question 1: %s\n", member.securityQuestion1);
    printf("Security Answer 1: %s\n", member.securityAnswer1);
}

// Function to modify member details
void modifyMemberDetails(struct Member* member) {
    struct Member tempMember;
    bool isValidIC = false;

    printf("\n=== Modify Member Details ===\n");
    printf("Enter '-1' to exit.\n");

    printf("\nEnter New Name: ");
    scanf(" %99[^\n]", tempMember.name);
    rewind(stdin);
    if (strcmp(tempMember.name, "-1") == 0) {
        strcpy(tempMember.name, member->name);
        system("cls");
        return;
    }

    printf("Enter New Gender: ");
    scanf(" %6[^\n]", tempMember.gender);
    rewind(stdin);
    if (strcmp(tempMember.gender, "-1") == 0) {
        strcpy(tempMember.gender, member->gender);
        system("cls");
        return;
    }

    do {
        printf("Enter New IC Number: ");
        scanf(" %14[^\n]", tempMember.ic);
        rewind(stdin);

        if (strcmp(tempMember.ic, "-1") == 0) {
            strcpy(tempMember.ic, member->ic);
            system("cls");
            return 0;
        }

        isValidIC = icValidate(tempMember.ic);
    } while (!isValidIC);

    printf("Enter New Contact Number: ");
    scanf(" %14[^\n]", tempMember.contact);
    rewind(stdin);
    if (strcmp(tempMember.contact, "-1") == 0) {
        strcpy(tempMember.contact, member->contact);
        system("cls");
        return;
    }

    char choice;

    do {
        printf("Do you want to change security question and answer? (Y=Yes/N=No/Z=Exit): ");
        scanf(" %c", &choice);
        rewind(stdin);

        if (toupper(choice) != 'Y' && toupper(choice) != 'N' && toupper(choice) != 'Z') {
            printf("Invalid input. Please enter 'Y', 'N', or 'Z'.\n");
        }
    } while (toupper(choice) != 'Y' && toupper(choice) != 'N' && toupper(choice) != 'Z');

    if (toupper(choice) == 'Y') {
        printf("\nEnter New Security Question (Don't Start With Z): ");
        scanf(" %99[^\n]", member->securityQuestion1);
        rewind(stdin);

        printf("\nEnter New Security Answer (Don't Start With Z): ");
        scanf(" %99[^\n]", member->securityAnswer1);
        rewind(stdin);
    }
    else if (toupper(choice) == 'Z') {
        system("cls");
        return;
    }

    saveModifiedMemberDetails(member);
}

// Function to save modified member details to file
void saveModifiedMemberDetails(struct Member* member) {
    // Update member details in the file
    FILE* readFile = fopen(MEMBER_INFO_FILE, "r");
    if (readFile == NULL) {
        printf("Error opening file for reading!\n");
        return;
    }

    // Open a temporary file for writing
    FILE* writeFile = fopen("temp.txt", "w");
    if (writeFile == NULL) {
        printf("Error opening temporary file for writing!\n");
        fclose(readFile);
        return;
    }

    // Structure to hold member details while reading
    struct Member tempMember;

    // Read from the original file, and write to the temporary file
    while (fscanf(readFile, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
        tempMember.id, tempMember.name, tempMember.password,
        tempMember.passwordRecovery, tempMember.gender, tempMember.ic,
        tempMember.contact, tempMember.securityQuestion1,
        tempMember.securityAnswer1) != EOF) {
        if (strcmp(tempMember.id, member->id) == 0) {
            // If the ID matches, write the modified member data
            fprintf(writeFile, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n",
                member->id, member->name, member->password,
                member->passwordRecovery, member->gender, member->ic,
                member->contact, member->securityQuestion1,
                member->securityAnswer1);
        }
        else {
            // Otherwise, write the original member data
            fprintf(writeFile, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n",
                tempMember.id, tempMember.name, tempMember.password,
                tempMember.passwordRecovery, tempMember.gender, tempMember.ic,
                tempMember.contact, tempMember.securityQuestion1,
                tempMember.securityAnswer1);
        }
    }

    // Close the files
    fclose(readFile);
    fclose(writeFile);

    // Open the temporary file for reading
    FILE* tempFile = fopen("temp.txt", "r");
    if (tempFile == NULL) {
        printf("Error opening temporary file for reading!\n");
        return;
    }

    // Open the original file for writing
    FILE* originalFile = fopen(MEMBER_INFO_FILE, "w");
    if (originalFile == NULL) {
        printf("Error opening original file for writing!\n");
        fclose(tempFile);
        return;
    }

    // Copy contents from temporary file to original file
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), tempFile) != NULL) {
        fprintf(originalFile, "%s", buffer);
    }

    // Close files
    fclose(tempFile);
    fclose(originalFile);

    printf("Member details modified successfully!\n");
}

// Function to register as member
void registerMember() {
    struct Member member;

    // Generate unique ID
    strcpy(member.id, generateUniqueId());

    welcome();
    printf("======== Member Register =======\n");
    printf("Enter \"-1\" to Exit\n");
    line();

    char choiceStr[MAX_CHOICE_LENGTH];
    printf("Enter Name: ");
    scanf(" %[^\n]", member.name);
    if (strcmp(member.name, "-1") == 0) {
        system("cls");
        return;
    }

    printf("Enter Password: ");
    scanf(" %[^\n]", member.password);
    if (strcmp(member.password, "-1") == 0) {
        system("cls");
        return;
    }

    printf("Enter Password Recovery: ");
    scanf(" %[^\n]", member.passwordRecovery);
    if (strcmp(member.passwordRecovery, "-1") == 0) {
        system("cls");
        return;
    }

    printf("Enter Gender: ");
    scanf(" %[^\n]", member.gender);
    if (strcmp(member.gender, "-1") == 0) {
        system("cls");
        return;
    }

    while (1) {
        printf("Enter IC Number: ");
        scanf(" %[^\n]", member.ic);
        if (strcmp(member.ic, "-1") == 0) {
            system("cls");
            return;
        }

        // Validate IC number
        if (icValidate(member.ic)) {
            break;
        }
        else {
            printf("\nInvalid IC number! Please enter a valid IC number.\n");
        }
    }

    printf("Enter Contact Number: ");
    scanf(" %[^\n]", member.contact);
    if (strcmp(member.contact, "-1") == 0) {
        system("cls");
        return;
    }

    printf("Enter Security Question 1: ");
    scanf(" %[^\n]", member.securityQuestion1);
    if (strcmp(member.securityQuestion1, "-1") == 0) {
        system("cls");
        return;
    }

    printf("Enter Security Answer 1: ");
    scanf(" %[^\n]", member.securityAnswer1);
    if (strcmp(member.securityAnswer1, "-1") == 0) {
        system("cls");
        return;
    }

    FILE* file = fopen(MEMBER_INFO_FILE, "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n",
        member.id, member.name, member.password, member.passwordRecovery,
        member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1);

    printf("Member registered successfully!\n");
    system("pause");
    fclose(file);
}

// Function to validate IC
bool icValidate(const char* ic) {
    // Check length
    if (strlen(ic) != 12) {
        printf("\nInvalid IC length!\n");
        return false;
    }

    // Check if all characters are digits
    for (int i = 0; ic[i] != '\0'; ++i) {
        if (!isdigit(ic[i])) {
            printf("\nIC contains non-digit characters!\n");
            return false;
        }
    }

    // All checks passed, IC is valid
    return true;
}

// Function to check if ID is taken
bool isIdTaken(const char* id) {
    FILE* file = fopen(MEMBER_INFO_FILE, "r");
    if (file == NULL) {
        return false; // ID is not taken if file does not exist
    }

    char currentId[MAX_ID_LENGTH];
    while (fscanf(file, " %[^|]", currentId) != EOF) {
        if (strcmp(currentId, id) == 0) {
            fclose(file);
            return true;
        }
        // Skip the rest of the line
        fscanf(file, "%*[^\n]");
    }

    fclose(file);
    return false;
}

// Function to generate a unique ID
char* generateUniqueId() {
    static char id[MAX_ID_LENGTH]; // Static variable to remain its value across function calls
    int num;
    srand((unsigned int)time(NULL)); // Seed the random number generator

    // Generate random 4-digit number until a unique one is found
    do {
        num = rand() % 10000; // Generate random number between 0 and 9999
        snprintf(id, sizeof(id), "M%04d", num); // Format the number to 4 digits with leading zeros
    } while (isIdTaken(id)); // Check if the generated ID is taken or not

    return id;
}

// Function to search for a member by ID
void searchMember() {
    struct Member member;
    char searchIc[MAX_IC_LENGTH];
    FILE* file;

    file = fopen(MEMBER_INFO_FILE, "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    system("cls");
    welcome();
    line();
    printf("Enter \"-1\" to Exit\n");
    line();

    printf("\nEnter Member's IC to search :");
    scanf("%s", searchIc);

    if (strcmp(searchIc, "-1") == 0) {
        system("cls");
        fclose(file);
        return;
    }

    // Search for member by ID
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", member.id, member.name, member.password, member.passwordRecovery, member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1) != EOF) {
        if (strcmp(member.ic, searchIc) == 0) {
            printf("\nMember Found!\n\n");
            printf("ID: %s\n", member.id);
            printf("Name: %s\n", member.name);
            printf("Gender: %s\n", member.gender);
            printf("IC: %s\n", member.ic);
            printf("Contact: %s\n", member.contact);
            printf("Password: %s\n", member.password);
            printf("Password Recovery: %s\n", member.passwordRecovery);
            printf("Security Question 1: %s\n", member.securityQuestion1);
            printf("Security Answer 1: %s\n", member.securityAnswer1);
            fclose(file);
            system("pause");
            return;
        }
    }

    printf("Member with ID %s not found!\n", searchIc);
    system("pause");
    fclose(file);
}

// Function to modify member information
void modifyMember(Staff* pStaffInfo) {
    struct Member member;
    char searchIc[MAX_IC_LENGTH];
    FILE* file, * tempFile;

    file = fopen(MEMBER_INFO_FILE, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Open temporary file in write mode
    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    system("cls");

    welcome();

    printf("\n======= Modify Member =======\n");
    printf("Enter \"-1\" to Exit\n");
    line();

    printf("\nEnter Member's IC to modify: ");
    scanf("%s", searchIc);



    if (strcmp(searchIc, "-1") == 0) {
        system("cls");
        fclose(file);
        fclose(tempFile);
        displayAdminMenu(pStaffInfo);
    }

    // Search for the member by IC
    bool found = false;
    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", member.id, member.name, member.password, member.passwordRecovery, member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1) != EOF) {
        if (strcmp(member.ic, searchIc) == 0) {
            found = true;
            // Modify member details
            printf("\n=== Modify Member Details ===\n");
            printf("Enter New Name: ");
            scanf(" %[^\n]", member.name);
            printf("Enter New Gender: ");
            scanf(" %[^\n]", member.gender);
            printf("Enter New Contact Number: ");
            scanf(" %[^\n]", member.contact);
            printf("Member details modified successfully!\n");
        }
        // Write member record to temp file
        fprintf(tempFile, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n", member.id, member.name, member.password, member.passwordRecovery, member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1);
    }

    fclose(file);
    fclose(tempFile);

    // Replace original file with temporary file if member was found and modified
    if (found) {
        remove(MEMBER_INFO_FILE);
        rename("temp.txt", MEMBER_INFO_FILE);
    }
    else {
        printf("Member with IC %s not found!\n", searchIc);
        remove("temp.txt"); // Delete the temporary file if member was not found
    }

    logDate date;
    sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

    FILE* addLog = fopen("staffLog.txt", "a");
    if (addLog == NULL) {
        printf("Unable to open staff.txt\n");
        exit(-1);
    }
    fprintf(addLog, "%s modified \"%s\" account|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, member.id, date.day, date.month, date.year, date.hour, date.minute);

    fclose(addLog);
    system("pause");
}

// Function to display all members
void displayMembers() {
    struct Member member;
    FILE* file;

    file = fopen(MEMBER_INFO_FILE, "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n=== All Members ===\n");

    // Display all members
    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", member.id, member.name, member.password, member.passwordRecovery, member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1) != EOF) {
        printf("ID: %s\n", member.id);
        printf("Name: %s\n", member.name);
        printf("Gender: %s\n", member.gender);
        printf("Password: %s\n", member.password);
        printf("Password Recovery: %s\n", member.passwordRecovery);
        printf("IC: %s\n", member.ic);
        printf("Contact: %s\n", member.contact);
        printf("Security Question 1: %s\n", member.securityQuestion1);
        printf("Security Answer 1: %s\n", member.securityAnswer1);
        printf("\n");
    }
    fclose(file);
    system("pause");
}

// Function to Reset Members Password
void resetPassword() {
    char memberIc[MAX_IC_LENGTH];
    char answer1[MAX_SECURITY_ANSWER_LENGTH];
    struct Member member;
    FILE* inputFile;
    FILE* tempFile;
    bool passwordReset = false; // Flag to track if the password has been reset

    inputFile = fopen(MEMBER_INFO_FILE, "r");
    if (inputFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");
        fclose(inputFile);
        return;
    }

    printf("========= Reset Password ========\n");
    printf("Enter \"-1\" to Exit\n");
    printf("\nEnter Member's IC Number: ");
    scanf("%14s", memberIc);
    if (strcmp(memberIc, "-1") == 0) {
        fclose(inputFile);
        fclose(tempFile);
        remove("temp.txt");
        return;
    }

    while (fscanf(inputFile, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]",
        member.id, member.name, member.password, member.passwordRecovery, member.gender, member.ic,
        member.contact, member.securityQuestion1, member.securityAnswer1) == 9) {
        if (strcmp(member.ic, memberIc) == 0) {
            printf("Answer the following security question to reset your password:\n");
            printf("Security Question: %s\n", member.securityQuestion1);
            printf("Your Answer: ");
            scanf(" %[^\n]", answer1);
            rewind(stdin);
            if (strcmp(answer1, "-1") == 0) {
                fclose(inputFile);
                fclose(tempFile);
                remove("temp.txt");
                return;
            }

            if (strcmp(answer1, member.securityAnswer1) == 0) {
                printf("Correct answer!\n");
                if (!passwordReset) {
                    printf("Enter your new password: ");
                    scanf(" %[^\n]", member.password);
                    rewind(stdin);
                    passwordReset = true; // Set the flag indicating password reset
                    printf("\nPassword reset successfuly\n");
                }
            }
            else {
                printf("Incorrect answer!\n");
            }
        }
        else {
            printf("\nInvalid IC number\n");
        }
        fprintf(tempFile, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n", member.id, member.name, member.password, member.passwordRecovery,
            member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1);
    }

    fclose(inputFile);
    fclose(tempFile);

    remove(MEMBER_INFO_FILE);
    rename("temp.txt", MEMBER_INFO_FILE);

    system("pause");
}

// Function to delete a member by IC number
void deleteMembers(Staff* pStaffInfo) {
    int valid = -1;
    char memberIc[MAX_IC_LENGTH];
    struct Member member;
    FILE* file, * tempFile;

    file = fopen(MEMBER_INFO_FILE, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Open temporary file in write mode
    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    system("cls");
    welcome();
    printf("========= Delete Member ========\n");
    printf("Enter \"-1\" to Exit\n");
    line();
    printf("\nEnter Member's IC to delete: ");
    scanf("%s", memberIc);
    if (strcmp(memberIc, "-1") == 0) {
        system("cls");
        fclose(file);
        fclose(tempFile);
        return;
    }

    // Search for the member by IC
    bool found = false;
    while (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", member.id, member.name, member.password, member.passwordRecovery, member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1) != EOF) {
        if (strcmp(member.ic, memberIc) == 0) {
            valid = 0;
            printf("Member found and deleted!\n");

            logDate date;
            sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

            FILE* addLog = fopen("staffLog.txt", "a");
            if (addLog == NULL) {
                printf("Unable to open staff.txt\n");
                exit(-1);
            }
            fprintf(addLog, "%s deleted \"%s\" account|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, member.id, date.day, date.month, date.year, date.hour, date.minute);

            fclose(addLog);

            found = true;
        }
        else {
            // Copy member record to temp file if not the member to delete
            fprintf(tempFile, "%s|%s|%s|%s|%s|%s|%s|%s|%s\n", member.id, member.name, member.password, member.passwordRecovery, member.gender, member.ic, member.contact, member.securityQuestion1, member.securityAnswer1);
        }
    }

    if (valid != 0) {
        printf("Member Not Found!\n");
    }

    fclose(file);
    fclose(tempFile);

    // Replace original file with temporary file if member was found and deleted
    if (found) {
        remove(MEMBER_INFO_FILE);
        rename("temp.txt", MEMBER_INFO_FILE);
    }
    else {
        remove("temp.txt"); // Delete the temporary file if member was not found
    }
    system("pause");
}