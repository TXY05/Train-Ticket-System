#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <ctype.h>
#pragma warning(disable:4996)

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
#define MAX_CIPHERSTAFF 400
#define RED(string) "\x1b[31m" string "\x1b[0m"

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

typedef struct {
	int num;
	char encryptionKey[MAX_KEYSTAFF];
	char encryptData[MAX_CIPHERSTAFF];
}Cipher;

void loginStaff();
void forgotPassStaff();
int resetPassStaff(Staff pStaffInfo);
void displayStaffMode(Staff* pStaffInfo);
int getStaffModeOption();
void displayStaffMenu(Staff* pStaffInfo, int* deleteNum);
int getStaffMenuOption();
void viewStaff(Staff* pStaffInfo);
void addStaff(Staff* pStaffInfo);
void editStaff(Staff* pStaffInfo);
int getEditStaffOption(Staff* pStaffInfo);
void editStringInfo(char currentStringInfo[MAX_STRINGSTAFF], Staff* pStaffInfo, int option);
void editIntInfo(int currentIntInfo, Staff* pStaffInfo, int option);
void editAllInfo(Staff* pStaffInfo);
void removeStaff(Staff* pStaffInfo);
int deleteAcc(Staff* pStaffInfo);
void staffLog();

void generateKey(char* key);
int findCombination(char encryptionKey[MAX_KEYSTAFF]);
char random_char();
void addAll(int* temp);
void encryption(char encryptionKey[MAX_KEYSTAFF], char* data);
void decryption(char encryptionKey[MAX_KEYSTAFF], char* hexChar);
void dec_to_hexa_conversion(int array[200], int stringlen, char* data);
void hex_to_dec_conversion(int array[MAX_CIPHERSTAFF], char* hexChar);
void assignDataToStaff(const char* data, Staff* pStaffInfo);

int backOption(char input[MAX_STRINGSTAFF]);
int backIntOption(int input);
int resetPassOption(char input[MAX_STRINGSTAFF]);
void convertToUpper(char* str);
int isInvalidInput(const char* input);
int isContainBar(const char* input);
int validateNum(const char* text, int* num);

void sGetCurrentTime(int* day, int* month, int* year, int* hour, int* minute);
void line();
void line2();

void loginStaff() {
	Staff staffLogin; //For Input 
	Staff staffInfo;
	Cipher cipherInfo;
	int forgotPassResult = 1;
	system("cls");

	cipherInfo.num = 0;

	//Check if theres Staff in the File
	FILE* checkStaff = fopen("staff.txt", "r");
	if (checkStaff == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}

	fscanf(checkStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData);

	fclose(checkStaff);

	//Generate New Admin ID
	if (cipherInfo.num == 0) { //If Theres No Info in the File
		generateKey(cipherInfo.encryptionKey);

		printf("Welcome First Admin\n");
		printf("The \x1b[31mStaff ID\x1b[0m is \x1b[31m\"ST0001\"\x1b[0m\n");
		printf("The \x1b[31mPassword\x1b[0m is \x1b[31m\"admin123\"\x1b[0m\n");
		Sleep(1000);
		printf("Successfully Connected as Admin\n\n");

		FILE* addStaff = fopen("staff.txt", "w");
		if (addStaff == NULL) {
			printf("Unable to open staff.txt\n");
			exit(-1);
		}
		//Encrypt new admin info into the file
		cipherInfo.num = 1;
		strcpy(cipherInfo.encryptData, "ST0001|Admin|Administrator|admin123|Answer|1|1|Jalan Bukit Bintang|Taman Bukit Bintang|55100 Kuala Lumpur");

		//Assign information into staffInfo b4 Encryption
		staffInfo.num = cipherInfo.num;
		strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
		strcpy(staffInfo.id, "ST0001");
		strcpy(staffInfo.name, "Admin");
		strcpy(staffInfo.position, "Administrator");
		strcpy(staffInfo.password, "admin123");
		strcpy(staffInfo.forgotPass, "Answer");
		staffInfo.forgotPassQues = 1;
		staffInfo.staffAddress.houseNum = 1;
		strcpy(staffInfo.staffAddress.streetName, "Jalan Bukit Bintang");
		strcpy(staffInfo.staffAddress.residentArea, "Taman Bukit Bintang");
		strcpy(staffInfo.staffAddress.city, "55100 Kuala Lumpur");

		//Encryption
		encryption(staffInfo.encryptionKey, cipherInfo.encryptData);

		fprintf(addStaff, "%d|%s|%s\n", staffInfo.num, staffInfo.encryptionKey, cipherInfo.encryptData);

		fclose(addStaff);

		system("pause");
		displayStaffMode(&staffInfo);
		return;
	}
	do {
		FILE* readStaff = fopen("staff.txt", "r");
		if (readStaff == NULL) {
			printf("Unable to open staff.txt\n");
			exit(-1);
		}

		welcome();

		printf("========= Login Staff ==========\n");
		printf("Enter \"0\" if Forgot Password\n");
		printf("Enter \"-1\" to Exit\n");
		line();

		while (1) { //Validate if Theres Spaces
			printf("\nEnter your ID  : ");
			rewind(stdin);
			if (scanf(" %[^\n]", staffLogin.id) != 1) { //Ensure user Input Something
				continue;
			}
			if (isInvalidInput(staffLogin.id)) {
				printf("No space or | is allowed in ID\n");
				getchar(); //Clear the newline character from the buffer
			}
			else {
				break; //Input is valid, exit the loop
			}
		}

		if (backOption(staffLogin.id) == 0) {//If Input == "-1" Then Back To Main();
			return;
		}
		if (resetPassOption(staffLogin.id) == 0) {//If Input == "0" Go To Forgot Password
			forgotPassStaff();
			return;
		}

		convertToUpper(staffLogin.id); //Convert the ID to uppercase

		while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
			//Decrypt
			decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
			//Assign all data to Staff by using strtok
			staffInfo.num = cipherInfo.num;
			strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
			assignDataToStaff(cipherInfo.encryptData, &staffInfo);

			if (strcmp(staffLogin.id, staffInfo.id) == 0) { //Compare id

				while (1) { //Validate if Theres Spaces
					printf("\nEnter your password: "); //Get password
					rewind(stdin);
					if (scanf(" %[^\n]", staffLogin.password) != 1) {
						continue;
					}
					if (isInvalidInput(staffLogin.password)) {
						printf("No space or | is allowed in password\n");
						getchar();
					}
					else {
						break;
					}
				}

				if (backOption(staffLogin.password) == 0) {
					return;
				}
				if (resetPassOption(staffLogin.password) == 0) {
					forgotPassStaff();
					return;
				}

				if (strcmp(staffLogin.password, staffInfo.password) == 0) { //Compare password

					if (strcmp(staffInfo.position, "Administrator") == 0) { //If Admin
						printf("Connected to Admin\n");
					}
					else {
						printf("Successfully Connected\n");
					}
					Sleep(1000);
					displayStaffMode(&staffInfo); //Edit Staff Menu
					break;
				}
				else {
					printf("Wrong Password\n"); //If Wrong Password
					printf("One attempt left\n");

					while (1) { //Validate if Theres Spaces
						printf("\nEnter your password: ");
						rewind(stdin);
						if (scanf(" %[^\n]", staffLogin.password) != 1) {
							continue;
						}
						if (isInvalidInput(staffLogin.password)) {
							printf("No space or | is allowed in password\n");
							getchar();
						}
						else {
							break;
						}
					}

					if (backOption(staffLogin.password) == 0) {
						return;
					}
					if (resetPassOption(staffLogin.password) == 0) {
						forgotPassStaff();
						return;
					}

					if (strcmp(staffLogin.password, staffInfo.password) == 0) {
						if (strcmp(staffInfo.position, "Administrator") == 0) {
							printf("Connected to Admin\n");
						}
						else {
							printf("Successfully Connected\n");
						}
						Sleep(1000);
						displayStaffMode(&staffInfo);
						break;
					}
					else {
						printf("Wrong Password\n");
						system("pause");
						return;
					}
				}
			}
		}

		fclose(readStaff);

		//If the ID Does Not Exist
		if ((strcmp(staffLogin.id, staffInfo.id) != 0)) {
			printf("Invalid ID\n");
			system("pause");
			system("cls");
		}

	} while ((strcmp(staffLogin.id, staffInfo.id) != 0));

	return;
}

void forgotPassStaff() {
	Staff staffInfo;
	Cipher cipherInfo;
	char confirmAns[MAX_FORGOTPASSSTAFF], confirmID[MAX_IDSTAFF];
	int resetPassOption = 1;
	system("cls");

	do {
		printf("====== Password Recovery ======\n");
		printf("Enter \"-1\" to Main Menu\n");
		line();

		while (1) { //Validate if Theres Spaces
			printf("\nEnter your Staff ID: "); //Get ID
			rewind(stdin);
			if (scanf(" %[^\n]", confirmID) != 1) {
				continue;
			}
			if (isInvalidInput(confirmID)) {
				printf("No space or | is allowed in ID\n");
				getchar();
			}
			else {
				break;
			}
		}

		if (backOption(confirmID) == 0) {
			return;
		}

		convertToUpper(confirmID);

		FILE* readStaff = fopen("staff.txt", "r");
		if (readStaff == NULL) {
			printf("Unable to open staff.txt\n");
			exit(-1);
		}
		while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
			//Decrypt
			decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
			//Assign all data to Staff by using strtok
			staffInfo.num = cipherInfo.num;
			strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
			assignDataToStaff(cipherInfo.encryptData, &staffInfo);


			if (strcmp(confirmID, staffInfo.id) == 0) { //Compare ID

				//Find Saved Question
				if (staffInfo.forgotPassQues == 1) {
					printf("\nWhat was your childhood best friend's nickname?\n");
				}
				else if (staffInfo.forgotPassQues == 2) {
					printf("\nIn which city did your parents meet?\n");
				}
				else {
					printf("\nWhat's your neighbor's last name\n");
				}
				while (1) { //Validate
					printf("\nEnter your answer: ");
					rewind(stdin);
					if (scanf(" %[^\n]", confirmAns) != 1) { //Ensure user Input Something
						continue;
					}
					if (isContainBar(confirmAns)) { //Validate if Theres '|'
						getchar();
					}
					else {
						break;
					}
				}
				if (backOption(confirmAns) == 0) {
					return;
				}

				if (strcmp(confirmAns, staffInfo.forgotPass) == 0) {
					printf("Successfully Connected\n");
					system("pause");
					resetPassOption = resetPassStaff(staffInfo);
					break;
				}
				else {
					printf("Wrong Answer\n"); //If Wrong Answer
					printf("One attempt left\n");
					while (1) { //Validate
						printf("\nEnter your answer: ");
						rewind(stdin);
						if (scanf(" %[^\n]", confirmAns) != 1) { //Ensure user Input Something
							continue;
						}
						if (isContainBar(confirmAns)) { //Validate if Theres '|'
							getchar();
						}
						else {
							break;
						}
					}
					if (backOption(confirmAns) == 0) {
						return;
					}

					if (strcmp(confirmAns, staffInfo.forgotPass) == 0) {
						printf("Successfully Connected\n");
						system("pause");
						resetPassOption = resetPassStaff(staffInfo);
						break;
					}
					else {
						printf("Wrong Answer\n");
						system("pause");
						system("cls");
						return;
					}
				}
			}
		}
		fclose(readStaff);

		if (strcmp(confirmID, staffInfo.id) != 0) {
			printf("Invalid ID\n");
			system("pause");
			system("cls");
		}
	} while ((strcmp(confirmID, staffInfo.id) != 0) || resetPassOption != 0);

	return;
}

int resetPassStaff(Staff pStaffInfo) {
	Staff staffInfo;
	Cipher cipherInfo;
	char newPassword[MAX_PASSWORDSTAFF];
	char confirmPassword[MAX_PASSWORDSTAFF];
	system("cls");

	do {
		printf("======== Reset Password ========\n");
		printf("Enter \"-1\" to Exit\n");
		line();

		while (1) { //Validate if Theres Spaces
			printf("\nEnter new password(No Space): ");
			rewind(stdin);
			if (scanf(" %[^\n]", newPassword) != 1) {
				continue;
			}
			if (isInvalidInput(newPassword)) {
				printf("No space or | is allowed in password\n");
				getchar();
			}
			else {
				break;
			}
		}

		if (backOption(newPassword) == 0) {
			return 1;
		}

		while (1) { //Validate if Theres Spaces
			printf("\nConfirm new password(Type password again): "); //Confirm New Password
			rewind(stdin);
			if (scanf(" %[^\n]", confirmPassword) != 1) {
				continue;
			}
			if (isInvalidInput(confirmPassword)) {
				printf("No space or | is allowed in password\n");
				getchar();
			}
			else {
				break;
			}
		}

		if (backOption(confirmPassword) == 0) {
			return 1;
		}

		if (strcmp(newPassword, confirmPassword) == 0) { //Compare Input Password

			//Update into File
			FILE* readStaff = fopen("staff.txt", "r");
			if (readStaff == NULL) {
				printf("Unable to open staff.txt\n");
				exit(-1);
			}
			FILE* writeTemp = fopen("tempStaff.txt", "w");
			if (writeTemp == NULL) {
				printf("Unable to open tempStaff.txt\n");
				exit(-1);
			}
			while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
				//Decrypt
				decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
				//Assign all data to Staff by using strtok
				staffInfo.num = cipherInfo.num;
				strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
				assignDataToStaff(cipherInfo.encryptData, &staffInfo);

				if (strcmp(pStaffInfo.id, staffInfo.id) == 0) {
					sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", pStaffInfo.id, pStaffInfo.name, pStaffInfo.position, newPassword,
						pStaffInfo.forgotPass, pStaffInfo.forgotPassQues, pStaffInfo.staffAddress.houseNum, pStaffInfo.staffAddress.streetName,
						pStaffInfo.staffAddress.residentArea, pStaffInfo.staffAddress.city);
					//Encryption
					encryption(pStaffInfo.encryptionKey, cipherInfo.encryptData);

					fprintf(writeTemp, "%d|%s|%s\n", pStaffInfo.num, pStaffInfo.encryptionKey, cipherInfo.encryptData);
				}
				else {
					sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", staffInfo.id, staffInfo.name, staffInfo.position, staffInfo.password, staffInfo.forgotPass,
						staffInfo.forgotPassQues, staffInfo.staffAddress.houseNum, staffInfo.staffAddress.streetName, staffInfo.staffAddress.residentArea, staffInfo.staffAddress.city);
					//Encryption
					encryption(staffInfo.encryptionKey, cipherInfo.encryptData);

					fprintf(writeTemp, "%d|%s|%s\n", staffInfo.num, staffInfo.encryptionKey, cipherInfo.encryptData);
				}
			}
			fclose(readStaff);
			fclose(writeTemp);

			FILE* readFile = fopen("tempStaff.txt", "r");
			if (readFile == NULL) {
				printf("Unable to open tempStaff.txt\n");
				exit(-1);
			}
			FILE* writeFile = fopen("staff.txt", "w");
			if (writeFile == NULL) {
				printf("Unable to open staff.txt\n");
				exit(-1);
			}
			while (fscanf(readFile, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {

				fprintf(writeFile, "%d|%s|%s\n", cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData);
			}
			fclose(readFile);
			fclose(writeFile);
		}
		else {
			printf("Entered password is not same\n");
			printf("Please try again\n");
			system("pause");
			system("cls");
		}
	} while ((strcmp(newPassword, confirmPassword) != 0));

	printf("Password Reset Successfully\n");
	system("pause");
	return 0;
}

void displayStaffMode(Staff* pStaffInfo) {
	int option;
	int deleteNum = 1;

	do {
		option = getStaffModeOption(); //Get Option

		switch (option) {
		case 1:
			displayStaffMenu(pStaffInfo, &deleteNum);
			break;
		case 2:
			displayAdminMenu(pStaffInfo);
			break;
		case 3:
			adminTrainMenu(pStaffInfo);
			break;
		case 4:
			editTicMenu(pStaffInfo);
			break;
		case 5:
			system("cls");
			break;
		default:
			printf("Invalid option\n");
			system("pause");
			system("cls");
		}
	} while (option != 5 && deleteNum != 0);
}

int getStaffModeOption() {
	int option;
	system("cls");

	welcome();

	printf("========== Staff Mode ==========\n");
	printf("1.Edit Staff Info\n");
	printf("2.Edit Member Info\n");
	printf("3.Edit Train Info\n");
	printf("4.Edit Ticket Info\n");
	printf("5.Logout\n");
	line();
	printf("\nSelect your mode: ");
	rewind(stdin);
	scanf("%d", &option);

	return option;
}

//Display Staff Menu
void displayStaffMenu(Staff* pStaffInfo, int* deleteNum) {
	int option;

	do {
		option = getStaffMenuOption(); //Get Option

		switch (option) {
		case 1:
			viewStaff(pStaffInfo); //View All Staff/ Admin is able to View More Information
			break;
		case 2:
			addStaff(pStaffInfo); //For Admin to Add New Staff
			break;
		case 3:
			editStaff(pStaffInfo); //Edit Info
			break;
		case 4:
			removeStaff(pStaffInfo); //Remove Staff
			break;
		case 5:
			*deleteNum = deleteAcc(pStaffInfo); //Delete Account
			break;
		case 6:
			staffLog(); //View Staff Activity Log
			break;
		case 7:
			break;
		default:
			printf("Invalid Option\n");
			system("pause");
			system("cls");
		}
	} while (option != 7 && *deleteNum != 0);
}

int getStaffMenuOption() {
	int option;
	system("cls");

	welcome();

	printf("========== Staff Menu ==========\n");
	printf("1.View all Staff\n");
	printf("2.Add Staff (Admin)\n"); //Only for Admin
	printf("3.Edit Info\n");
	printf("4.Remove Staff (Admin)\n"); //Only for Admin
	printf("5.Delete Account\n");
	printf("6.View Staff Activity Log\n");
	printf("7.Back\n");
	line();
	printf("\nSelect your mode: ");
	rewind(stdin);
	scanf("%d", &option);

	return option;
}

void viewStaff(Staff* pStaffInfo) {
	Staff staffInfo;
	Cipher cipherInfo;
	system("cls");

	welcome();

	if (strcmp(pStaffInfo->position, "Administrator") == 0) { //If Admin
		printf("No. Staff ID  Staff Name        Staff Position        Staff Password        Staff Address\n");
	}
	else {
		printf("No. Staff ID                    Staff Name                    Staff Position\n");
	}
	line2();

	//Read Staff info and Print
	FILE* readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}

	while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
		//Decrypt
		decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
		//Assign all data to Staff by using strtok
		staffInfo.num = cipherInfo.num;
		strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
		assignDataToStaff(cipherInfo.encryptData, &staffInfo);

		if (strcmp(pStaffInfo->position, "Administrator") == 0) {
			printf("%-3d %-9s %-17s %-21s %-21s No.%d, %s,\n\t\t\t\t\t\t\t\t\t    %s, %s\n", staffInfo.num, staffInfo.id, staffInfo.name, staffInfo.position, staffInfo.password,
				staffInfo.staffAddress.houseNum, staffInfo.staffAddress.streetName, staffInfo.staffAddress.residentArea, staffInfo.staffAddress.city);
		}
		else {
			printf("%-3d %-27s %-29s %s\n", staffInfo.num, staffInfo.id, staffInfo.name, staffInfo.position);
		}
	}
	printf("\n");
	line2();

	fclose(readStaff);

	system("pause");
	return;
}

void addStaff(Staff* pStaffInfo) {
	Staff staffInfo;
	Staff readStaffInfo;
	Cipher cipherInfo;
	int idNum = 0; //For Staff ID generate
	char newStaffID[MAX_IDSTAFF];
	system("cls");

	if (strcmp(pStaffInfo->position, "Administrator") != 0) { //Validate if its Admin
		printf("\nOnly Admin Have Permission To Add New Staff\n");
		system("pause");
		return;
	}

	readStaffInfo.num = 0;

	//Read File to Increase Num of Staff and Generate Staff ID
	FILE* readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}

	while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF);

	fclose(readStaff);

	//Decrypt
	decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
	//Assign all data to Staff by using strtok
	readStaffInfo.num = cipherInfo.num;
	strcpy(readStaffInfo.encryptionKey, cipherInfo.encryptionKey);
	assignDataToStaff(cipherInfo.encryptData, &readStaffInfo);

	//Generate Staff ID
	//Scanf Latest ID and Convert String into Number
	for (int i = 2; i < 6; i++) { //Loop from the 3rd Char in the String ST"0"001
		idNum += (readStaffInfo.id[i] - 48) * pow(10, 5 - i);
	}
	sprintf(newStaffID, "ST%04d", idNum + 1); //Convert Them into String
	strcpy(staffInfo.id, newStaffID);

	welcome();

	printf("======== Register Staff ========\n");
	printf("Enter \"-1\" to Exit\n");
	line();
	printf("Staff ID: %s\n", staffInfo.id);
	while (1) { //Validate
		printf("\nEnter username: ");
		rewind(stdin);
		if (scanf(" %[^\n]", staffInfo.name) != 1) { //Ensure user Input Something
			continue;
		}
		if (isContainBar(staffInfo.name)) { //Validate if Theres '|'
			getchar();
		}
		else {
			break;
		}
	}
	if (backOption(staffInfo.name) == 0) {
		return;
	}

	while (1) { //Validate if Theres Spaces
		printf("\nEnter password: ");
		rewind(stdin);
		if (scanf(" %[^\n]", staffInfo.password) != 1) {
			continue;
		}
		if (isInvalidInput(staffInfo.password)) {
			printf("No space or | is allowed in password\n");
			getchar();
		}
		else {
			break;
		}
	}
	if (backOption(staffInfo.password) == 0) {
		return;
	}

	while (1) { //Validate
		printf("\nEnter position: ");
		rewind(stdin);
		if (scanf(" %[^\n]", staffInfo.position) != 1) { //Ensure user Input Something
			continue;
		}
		if (isContainBar(staffInfo.position)) { //Validate if Theres '|'
			getchar();
		}
		else {
			break;
		}
	}
	if (backOption(staffInfo.position) == 0) {
		return;
	}

	do {
		validateNum("\nEnter house/unit number(Eg:1): ", &staffInfo.staffAddress.houseNum); //Validate if it's a Number
		if (backIntOption(staffInfo.staffAddress.houseNum) == 0) {
			return;
		}
		if (staffInfo.staffAddress.houseNum < -1) { //Validate if it's lower than 0
			printf("House Number cannot be lower than 0\n");
		}
	} while (staffInfo.staffAddress.houseNum < -1);

	while (1) { //Validate
		printf("\nEnter Street Name(Eg:Jalan Bukit Bintang): ");
		rewind(stdin);
		if (scanf(" %[^\n]", staffInfo.staffAddress.streetName) != 1) { //Ensure user Input Something
			continue;
		}
		if (isContainBar(staffInfo.staffAddress.streetName)) { //Validate if Theres '|'
			getchar();
		}
		else {
			break;
		}
	}
	if (backOption(staffInfo.staffAddress.streetName) == 0) {
		return;
	}

	while (1) { //Validate
		printf("\nEnter Residental Address(Eg:Taman Bukit Bintang): ");
		rewind(stdin);
		if (scanf(" %[^\n]", staffInfo.staffAddress.residentArea) != 1) { //Ensure user Input Something
			continue;
		}
		if (isContainBar(staffInfo.staffAddress.residentArea)) { //Validate if Theres '|'
			getchar();
		}
		else {
			break;
		}
	}
	if (backOption(staffInfo.staffAddress.residentArea) == 0) {
		return;
	}

	while (1) { //Validate
		printf("\nEnter Postal Code + City/Town(Eg:55100 Kuala Lumpur): ");
		rewind(stdin);
		if (scanf(" %[^\n]", staffInfo.staffAddress.city) != 1) { //Ensure user Input Something
			continue;
		}
		if (isContainBar(staffInfo.staffAddress.city)) { //Validate if Theres '|'
			getchar();
		}
		else {
			break;
		}
	}
	if (backOption(staffInfo.staffAddress.city) == 0) {
		return;
	}

	printf("\nFor Password Recovery(Very Important)\n");
	srand(time(0));
	int random = rand() % (3 + 1 - 1) + 1; //Choose and Printf a Random Question
	while (1) { //Validate
		if (random == 1) {
			printf("\nWhat was your childhood best friend's nickname?: ");
		}
		else if (random == 2) {
			printf("\nIn which city did your parents meet?: ");
		}
		else {
			printf("\nWhat's your neighbor's last name?: ");
		}
		rewind(stdin);
		if (scanf(" %[^\n]", staffInfo.forgotPass) != 1) { //Ensure user Input Something
			continue;
		}
		if (isContainBar(staffInfo.forgotPass)) { //Validate if Theres '|'
			getchar();
		}
		else {
			break;
		}
	}
	if (backOption(staffInfo.forgotPass) == 0) {
		return;
	}

	FILE* addStaff = fopen("staff.txt", "a");
	if (addStaff == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	generateKey(staffInfo.encryptionKey);
	sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", staffInfo.id, staffInfo.name, staffInfo.position, staffInfo.password, staffInfo.forgotPass, random, 
		staffInfo.staffAddress.houseNum, staffInfo.staffAddress.streetName, staffInfo.staffAddress.residentArea, staffInfo.staffAddress.city);
	//Encryption
	encryption(staffInfo.encryptionKey, cipherInfo.encryptData);

	fprintf(addStaff, "%d|%s|%s\n", readStaffInfo.num + 1, staffInfo.encryptionKey, cipherInfo.encryptData);

	fclose(addStaff);

	//To Update Staff Activity Into Staff Log
	logDate date;
	sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

	FILE* addLog = fopen("staffLog.txt", "a");
	if (addLog == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}

	fprintf(addLog, "%s added a new staff, %s|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, staffInfo.id, date.day, date.month, date.year, date.hour, date.minute);

	fclose(addLog);

	printf("\nSuccessfully Registered\n");
	system("pause");

	return;
}

void editStaff(Staff* pStaffInfo) {
	int option;

	do {
		option = getEditStaffOption(pStaffInfo); //Get Option

		switch (option) {
		case 1:
			editStringInfo(pStaffInfo->name, pStaffInfo, option);
			break;
		case 2:
			editStringInfo(pStaffInfo->password, pStaffInfo, option);
			break;
		case 3:
			editIntInfo(pStaffInfo->staffAddress.houseNum, pStaffInfo, option);
			break;
		case 4:
			editStringInfo(pStaffInfo->staffAddress.streetName, pStaffInfo, option);
			break;
		case 5:
			editStringInfo(pStaffInfo->staffAddress.residentArea, pStaffInfo, option);
			break;
		case 6:
			editStringInfo(pStaffInfo->staffAddress.city, pStaffInfo, option);
			break;
		case 7:
			editStringInfo(pStaffInfo->forgotPass, pStaffInfo, option);
			break;
		case 8:
			editAllInfo(pStaffInfo);
			break;
		case 9:
			break;
		default:
			printf("Invalid Option\n");
			system("pause");
			system("cls");
		}
	} while (option != 9);
}

int getEditStaffOption(Staff* pStaffInfo) {
	int option;
	system("cls");

	welcome();

	printf("====== Choose what to edit =====\n");
	printf("1.Username: %s\n", pStaffInfo->name);
	printf("2.Password: %s\n", pStaffInfo->password);
	printf("3.House/Unit number: %d\n", pStaffInfo->staffAddress.houseNum);
	printf("4.Street Name: %s\n", pStaffInfo->staffAddress.streetName);
	printf("5.Residental Address: %s\n", pStaffInfo->staffAddress.residentArea);
	printf("6.Postal Code + City/Town: %s\n", pStaffInfo->staffAddress.city);
	printf("7.Password Recovery: %s\n", pStaffInfo->forgotPass);
	printf("8.Edit All Info\n");
	printf("9.Back\n");
	line();
	printf("\nSelect your option: ");
	rewind(stdin);
	scanf("%d", &option);

	return option;
}

//To Edit String Variable
void editStringInfo(char currentStringInfo[MAX_STRINGSTAFF], Staff* pStaffInfo, int option) {
	Staff staffInfo;
	Cipher cipherInfo;
	char newStringInfo[MAX_STRINGSTAFF];
	system("cls");
	welcome();
	printf("\n======= Edit Information =======\n");
	printf("Enter \"-1\" to Exit\n");
	line();
	if (option == 7) { //Print the Question Stored
		if (pStaffInfo->forgotPassQues == 1) {
			printf("What was your childhood best friend's nickname?\n");
		}
		else if (pStaffInfo->forgotPassQues == 2) {
			printf("In which city did your parents meet?\n");
		}
		else {
			printf("What's your neighbor's last name?\n");
		}
	}
	printf("Current Information: %s\n", currentStringInfo);
	switch (option) {
	case 1:
		while (1) { //Validate
			printf("\nEnter new Username: ");
			rewind(stdin);
			if (scanf(" %[^\n]", newStringInfo) != 1) { //Ensure user Input Something
				continue;
			}
			if (isContainBar(newStringInfo)) { //Validate if Theres '|'
				getchar();
			}
			else {
				break;
			}
		}
		if (backOption(newStringInfo) == 0) {
			return;
		}
		strcpy(pStaffInfo->name, newStringInfo);
		break;
	case 2: //Password
		while (1) { //Validate if Theres Spaces
			printf("\nEnter new Password: ");
			if (scanf(" %[^\n]", newStringInfo) != 1) {
				continue;
			}
			if (isInvalidInput(newStringInfo)) {
				printf("No space or | is allowed in password\n");
				getchar();
			}
			else {
				break;
			}
		}
		if (backOption(newStringInfo) == 0) {
			return;
		}
		strcpy(pStaffInfo->password, newStringInfo);
		break;
	case 4:
		while (1) { //Validate
			printf("\nEnter new Street Name: ");
			rewind(stdin);
			if (scanf(" %[^\n]", newStringInfo) != 1) { //Ensure user Input Something
				continue;
			}
			if (isContainBar(newStringInfo)) { //Validate if Theres '|'
				getchar();
			}
			else {
				break;
			}
		}
		if (backOption(newStringInfo) == 0) {
			return;
		}
		strcpy(pStaffInfo->staffAddress.streetName, newStringInfo);
		break;
	case 5:
		while (1) { //Validate
			printf("\nEnter new Resident Address: ");
			rewind(stdin);
			if (scanf(" %[^\n]", newStringInfo) != 1) { //Ensure user Input Something
				continue;
			}
			if (isContainBar(newStringInfo)) { //Validate if Theres '|'
				getchar();
			}
			else {
				break;
			}
		}
		if (backOption(newStringInfo) == 0) {
			return;
		}
		strcpy(pStaffInfo->staffAddress.residentArea, newStringInfo);
		break;
	case 6:
		while (1) { //Validate
			printf("\nEnter new Postal Code + City/Town: ");
			rewind(stdin);
			if (scanf(" %[^\n]", newStringInfo) != 1) { //Ensure user Input Something
				continue;
			}
			if (isContainBar(newStringInfo)) { //Validate if Theres '|'
				getchar();
			}
			else {
				break;
			}
		}
		if (backOption(newStringInfo) == 0) {
			return;
		}
		strcpy(pStaffInfo->staffAddress.city, newStringInfo);
		break;
	case 7:
		while (1) { //Validate
			printf("\nEnter new Password Recovery: ");
			rewind(stdin);
			if (scanf(" %[^\n]", newStringInfo) != 1) { //Ensure user Input Something
				continue;
			}
			if (isContainBar(newStringInfo)) { //Validate if Theres '|'
				getchar();
			}
			else {
				break;
			}
		}
		if (backOption(newStringInfo) == 0) {
			return;
		}
		strcpy(pStaffInfo->forgotPass, newStringInfo);
		break;
	}

	//Update the New Info into the File
	FILE* readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	FILE* writeTemp = fopen("tempStaff.txt", "w");
	if (writeTemp == NULL) {
		printf("Unable to open tempStaff.txt\n");
		exit(-1);
	}
	while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
		//Decrypt
		decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
		//Assign all data to Staff by using strtok
		staffInfo.num = cipherInfo.num;
		strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
		assignDataToStaff(cipherInfo.encryptData, &staffInfo);

		if (strcmp(pStaffInfo->id, staffInfo.id) == 0) {
			sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", pStaffInfo->id, pStaffInfo->name, pStaffInfo->position, pStaffInfo->password, pStaffInfo->forgotPass, 
				pStaffInfo->forgotPassQues, pStaffInfo->staffAddress.houseNum, pStaffInfo->staffAddress.streetName, pStaffInfo->staffAddress.residentArea, pStaffInfo->staffAddress.city);
			//Encryption
			encryption(pStaffInfo->encryptionKey, cipherInfo.encryptData);

			fprintf(writeTemp, "%d|%s|%s\n", pStaffInfo->num, pStaffInfo->encryptionKey, cipherInfo.encryptData);
		}
		else {
			sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", staffInfo.id, staffInfo.name, staffInfo.position, staffInfo.password, staffInfo.forgotPass, 
				staffInfo.forgotPassQues, staffInfo.staffAddress.houseNum, staffInfo.staffAddress.streetName, staffInfo.staffAddress.residentArea, staffInfo.staffAddress.city);
			//Encryption
			encryption(staffInfo.encryptionKey, cipherInfo.encryptData);

			fprintf(writeTemp, "%d|%s|%s\n", staffInfo.num, staffInfo.encryptionKey, cipherInfo.encryptData);
		}
	}
	fclose(readStaff);
	fclose(writeTemp);

	FILE* readFile = fopen("tempStaff.txt", "r");
	if (readFile == NULL) {
		printf("Unable to open tempStaff.txt\n");
		exit(-1);
	}
	FILE* writeFile = fopen("staff.txt", "w");
	if (writeFile == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	while (fscanf(readFile, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {

		fprintf(writeFile, "%d|%s|%s\n", cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData);
	}
	fclose(readFile);
	fclose(writeFile);

	//To Update Staff Activity Into Staff Log
	logDate date;
	sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

	FILE* addLog = fopen("staffLog.txt", "a");
	if (addLog == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	fprintf(addLog, "%s updated his Information|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, date.day, date.month, date.year, date.hour, date.minute);

	fclose(addLog);

	printf("Edited Successfully\n");
	system("pause");
	return;
}

//To Edit Integer Variable
void editIntInfo(int currentIntInfo, Staff* pStaffInfo, int option) {
	Staff staffInfo;
	Cipher cipherInfo;
	int newIntInfo;
	system("cls");
	welcome();
	printf("======= Edit Information =======\n");
	printf("Enter \"-1\" to Exit\n");
	line();
	printf("Current Information: %d\n", currentIntInfo);
	rewind(stdin);
	switch (option) {
	case 3:
		do {
			validateNum("\nEnter house/unit number(Eg:1): ", &newIntInfo); //Validate if it's a Number
			if (backIntOption(newIntInfo) == 0) {
				return;
			}
			if (newIntInfo < -1) {
				printf("House Number cannot be lower than 0\n");
			}
		} while (newIntInfo < -1);
		pStaffInfo->staffAddress.houseNum = newIntInfo;
		break;
	}

	//Update the New Info into the File
	FILE* readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	FILE* writeTemp = fopen("tempStaff.txt", "w");
	if (writeTemp == NULL) {
		printf("Unable to open tempStaff.txt\n");
		exit(-1);
	}
	while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
		//Decrypt
		decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
		//Assign all data to Staff by using strtok
		staffInfo.num = cipherInfo.num;
		strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
		assignDataToStaff(cipherInfo.encryptData, &staffInfo);

		if (strcmp(pStaffInfo->id, staffInfo.id) == 0) {
			sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", pStaffInfo->id, pStaffInfo->name, pStaffInfo->position, pStaffInfo->password, pStaffInfo->forgotPass, 
				pStaffInfo->forgotPassQues, pStaffInfo->staffAddress.houseNum, pStaffInfo->staffAddress.streetName, pStaffInfo->staffAddress.residentArea, pStaffInfo->staffAddress.city);
			//Encryption
			encryption(pStaffInfo->encryptionKey, cipherInfo.encryptData);

			fprintf(writeTemp, "%d|%s|%s\n", pStaffInfo->num, pStaffInfo->encryptionKey, cipherInfo.encryptData);
		}
		else {
			sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", staffInfo.id, staffInfo.name, staffInfo.position, staffInfo.password, staffInfo.forgotPass,
				staffInfo.forgotPassQues, staffInfo.staffAddress.houseNum, staffInfo.staffAddress.streetName, staffInfo.staffAddress.residentArea, staffInfo.staffAddress.city);
			//Encryption
			encryption(staffInfo.encryptionKey, cipherInfo.encryptData);

			fprintf(writeTemp, "%d|%s|%s\n", staffInfo.num, staffInfo.encryptionKey, cipherInfo.encryptData);
		}
	}
	fclose(readStaff);
	fclose(writeTemp);

	FILE* readFile = fopen("tempStaff.txt", "r");
	if (readFile == NULL) {
		printf("Unable to open tempStaff.txt\n");
		exit(-1);
	}
	FILE* writeFile = fopen("staff.txt", "w");
	if (writeFile == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	while (fscanf(readFile, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {

		fprintf(writeFile, "%d|%s|%s\n", cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData);
	}
	fclose(readFile);
	fclose(writeFile);

	//To Update Staff Activity Into Staff Log
	logDate date;
	sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

	FILE* addLog = fopen("staffLog.txt", "a");
	if (addLog == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	fprintf(addLog, "%s updated his information|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, date.day, date.month, date.year, date.hour, date.minute);

	fclose(addLog);

	printf("Successfully Edited\n");
	system("pause");
	return;
}

//To Edit All Info
void editAllInfo(Staff* pStaffInfo) {
	Staff staffInfo;
	Staff newStaffInfo;
	Cipher cipherInfo;
	system("cls");

	welcome();

	printf("========= Edit All Info =========\n");
	printf("Enter \"-1\" to Exit\n");
	line();
	printf("\nStaff ID: %s\n", pStaffInfo->id);
	printf("\nCurrent username: %s\n", pStaffInfo->name);
	printf("Enter your username: ");
	rewind(stdin);
	if (scanf(" %[^\n]", newStaffInfo.name) != 1); //Ensure user Input Something
	if (backOption(newStaffInfo.name) == 0) {
		return;
	}

	printf("\nCurrent password: %s\n", pStaffInfo->password);

	while (1) { //Validate if Theres Spaces
		printf("Enter your password: ");
		rewind(stdin);
		if (scanf(" %[^\n]", newStaffInfo.password) != 1) {
			continue;
		}
		if (isInvalidInput(newStaffInfo.password)) {
			printf("No space or | is allowed in password\n\n");
			getchar();
		}
		else {
			break;
		}
	}
	if (backOption(newStaffInfo.password) == 0) {
		return;
	}

	printf("\nCurrent house/unit number: %d\n", pStaffInfo->staffAddress.houseNum);
	do {
		validateNum("Enter your house/unit number(Eg:1): ", &newStaffInfo.staffAddress.houseNum); //Validate if it's a Number
		if (backIntOption(newStaffInfo.staffAddress.houseNum) == 0) {
			return;
		}
		if (newStaffInfo.staffAddress.houseNum < -1) {
			printf("House Number cannot be lower than 0\n\n");
		}
	} while (newStaffInfo.staffAddress.houseNum < -1);

	printf("\nCurrent Street Name: %s\n", pStaffInfo->staffAddress.streetName);
	printf("Enter your Street Name(Eg:Jalan Bukit Bintang): ");
	rewind(stdin);
	if (scanf(" %[^\n]", newStaffInfo.staffAddress.streetName) != 1);
	if (backOption(newStaffInfo.staffAddress.streetName) == 0) {
		return;
	}

	printf("\nCurrent Residental Address: %s\n", pStaffInfo->staffAddress.residentArea);
	printf("Enter your Residental Address(Eg:Taman Bukit Bintang): ");
	rewind(stdin);
	if (scanf(" %[^\n]", newStaffInfo.staffAddress.residentArea) != 1);
	if (backOption(newStaffInfo.staffAddress.residentArea) == 0) {
		return;
	}

	printf("\nCurrent Postal Code: %s\n", pStaffInfo->staffAddress.city);
	printf("Enter your Postal Code + City/Town(Eg:55100 Kuala Lumpur): ");
	rewind(stdin);
	if (scanf(" %[^\n]", newStaffInfo.staffAddress.city) != 1);
	if (backOption(newStaffInfo.staffAddress.city) == 0) {
		return;
	}

	printf("\nFor Password Recovery(Very Important)\n");
	if (pStaffInfo->forgotPassQues == 1) {
		printf("What was your childhood best friend's nickname?\n");
	}
	else if (pStaffInfo->forgotPassQues == 2) {
		printf("In which city did your parents meet?\n");
	}
	else {
		printf("What's your neighbor's last name?\n");
	}
	printf("Current Answer: %s\n", pStaffInfo->forgotPass);
	printf("Enter new Answer: ");
	rewind(stdin);
	if (scanf(" %[^\n]", newStaffInfo.forgotPass) != 1);
	if (backOption(newStaffInfo.forgotPass) == 0) {
		return;
	}

	strcpy(pStaffInfo->name, newStaffInfo.name);
	strcpy(pStaffInfo->password, newStaffInfo.password);
	pStaffInfo->staffAddress.houseNum = newStaffInfo.staffAddress.houseNum;
	strcpy(pStaffInfo->staffAddress.streetName, newStaffInfo.staffAddress.streetName);
	strcpy(pStaffInfo->staffAddress.residentArea, newStaffInfo.staffAddress.residentArea);
	strcpy(pStaffInfo->staffAddress.city, newStaffInfo.staffAddress.city);
	strcpy(pStaffInfo->forgotPass, newStaffInfo.forgotPass);

	//Update the New Info into the File
	FILE* readStaff = fopen("staff.txt", "r");
	if (readStaff == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	FILE* writeTemp = fopen("tempStaff.txt", "w");
	if (writeTemp == NULL) {
		printf("Unable to open tempStaff.txt\n");
		exit(-1);
	}
	while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
		//Decrypt
		decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
		//Assign all data to Staff by using strtok
		staffInfo.num = cipherInfo.num;
		strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
		assignDataToStaff(cipherInfo.encryptData, &staffInfo);

		if (strcmp(pStaffInfo->id, staffInfo.id) == 0) {
			sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", pStaffInfo->id, pStaffInfo->name, pStaffInfo->position, pStaffInfo->password, pStaffInfo->forgotPass,
				pStaffInfo->forgotPassQues, pStaffInfo->staffAddress.houseNum, pStaffInfo->staffAddress.streetName, pStaffInfo->staffAddress.residentArea, pStaffInfo->staffAddress.city);
			//Encryption
			encryption(pStaffInfo->encryptionKey, cipherInfo.encryptData);

			fprintf(writeTemp, "%d|%s|%s\n", pStaffInfo->num, pStaffInfo->encryptionKey, cipherInfo.encryptData);
		}
		else {
			sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", staffInfo.id, staffInfo.name, staffInfo.position, staffInfo.password, staffInfo.forgotPass,
				staffInfo.forgotPassQues, staffInfo.staffAddress.houseNum, staffInfo.staffAddress.streetName, staffInfo.staffAddress.residentArea, staffInfo.staffAddress.city);
			//Encryption
			encryption(staffInfo.encryptionKey, cipherInfo.encryptData);

			fprintf(writeTemp, "%d|%s|%s\n", staffInfo.num, staffInfo.encryptionKey, cipherInfo.encryptData);
		}
	}
	fclose(readStaff);
	fclose(writeTemp);

	FILE* readFile = fopen("tempStaff.txt", "r");
	if (readFile == NULL) {
		printf("Unable to open tempStaff.txt\n");
		exit(-1);
	}
	FILE* writeFile = fopen("staff.txt", "w");
	if (writeFile == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	while (fscanf(readFile, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {

		fprintf(writeFile, "%d|%s|%s\n", cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData);
	}
	fclose(readFile);
	fclose(writeFile);

	//To Update Staff Activity Into Staff Log
	logDate date;
	sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

	FILE* addLog = fopen("staffLog.txt", "a");
	if (addLog == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}
	fprintf(addLog, "%s updated his Information|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, date.day, date.month, date.year, date.hour, date.minute);

	fclose(addLog);

	printf("\nUpdated Successfully\n");
	system("pause");
	return;
}

void removeStaff(Staff* pStaffInfo) {
	Staff staffInfo;
	Cipher cipherInfo;
	char removeID[20];
	char deleteConfirmation[20];
	int number = 1;
	system("cls");


	if (strcmp(pStaffInfo->position, "Administrator") != 0) { //Validate if its Admin
		printf("Only Admin Have Permission To Remove Staff\n");
		system("pause");
		return;
	}

	do {
		system("cls");

		welcome();

		printf("===== Remove Staff =====\n");
		printf("Enter \"-1\" to Exit\n");
		line();

		while (1) { //Validate if Theres Spaces
			printf("\nEnter the Staff ID that you want to Remove(No Space): ");
			rewind(stdin);
			if (scanf(" %[^\n]", removeID) != 1) {
				continue;
			}
			if (isInvalidInput(removeID)) {
				printf("No space or | is allowed in ID\n");
				getchar();
			}
			else {
				break;
			}
		}
		if (backOption(removeID) == 0) {
			return;
		}

		convertToUpper(removeID);

		if (strcmp(removeID, "ST0001") == 0) { //Validate if its First Admin
			printf("Not allowed to remove First Admin\n");
			system("pause");
			return;
		}
		else if (strcmp(removeID, pStaffInfo->id) == 0) {
			printf("Not allowed to remove Own Account\n");
			printf("Please go to Delete Account Options\n");
			system("pause");
			return;
		}

		FILE* readStaff = fopen("staff.txt", "r");
		if (readStaff == NULL) {
			printf("Unable to open staff.txt\n");
			exit(-1);
		}
		while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
			//Decrypt
			decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
			//Assign all data to Staff by using strtok
			staffInfo.num = cipherInfo.num;
			strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
			assignDataToStaff(cipherInfo.encryptData, &staffInfo);

			if (strcmp(removeID, staffInfo.id) == 0) { //Validate if ID exist
				printf("======== \x1b[31mWARNING\x1b[0m ========\n");
				printf("Confirm to \x1b[31mREMOVE\x1b[0m?\n");
				printf("If YES enter \"\x1b[31mCONFIRM\x1b[0m\"\n");
				printf("If NO enter \"-1\"\n");
				line();

				do {
					printf("Enter your option: ");
					scanf("%s", deleteConfirmation);
					if (strcmp(deleteConfirmation, "-1") == 0) { //If input == -1
						printf("\nAccount is safe <3\n");
						system("pause");
						return; //Back
					}
					else {
						if (strcmp(deleteConfirmation, "CONFIRM") == 0) {

							//Remove the Staff from the File
							FILE* readStaff = fopen("staff.txt", "r");
							if (readStaff == NULL) {
								printf("Unable to open staff.txt\n");
								exit(-1);
							}
							FILE* writeTemp = fopen("tempStaff.txt", "w");
							if (writeTemp == NULL) {
								printf("Unable to open tempStaff.txt\n");
								exit(-1);
							}
							while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
								//Decrypt
								decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
								//Assign all data to Staff by using strtok
								staffInfo.num = cipherInfo.num;
								strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
								assignDataToStaff(cipherInfo.encryptData, &staffInfo);

								if (strcmp(removeID, staffInfo.id) != 0) {
									sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", staffInfo.id, staffInfo.name, staffInfo.position, staffInfo.password, staffInfo.forgotPass,
										staffInfo.forgotPassQues, staffInfo.staffAddress.houseNum, staffInfo.staffAddress.streetName, staffInfo.staffAddress.residentArea, staffInfo.staffAddress.city);
									//Encryption
									encryption(staffInfo.encryptionKey, cipherInfo.encryptData);

									fprintf(writeTemp, "%d|%s|%s\n", number++, staffInfo.encryptionKey, cipherInfo.encryptData);
								}
							}
							fclose(readStaff);
							fclose(writeTemp);

							FILE* readFile = fopen("tempStaff.txt", "r");
							if (readFile == NULL) {
								printf("Unable to open tempStaff.txt\n");
								exit(-1);
							}
							FILE* writeFile = fopen("staff.txt", "w");
							if (writeFile == NULL) {
								printf("Unable to open staff.txt\n");
								exit(-1);
							}
							while (fscanf(readFile, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {

								fprintf(writeFile, "%d|%s|%s\n", cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData);
							}
							fclose(readFile);
							fclose(writeFile);

							//To Update Staff Activity Into Staff Log
							logDate date;
							sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

							FILE* addLog = fopen("staffLog.txt", "a");
							if (addLog == NULL) {
								printf("Unable to open staff.txt\n");
								exit(-1);
							}
							fprintf(addLog, "%s deleted %s account|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, removeID, date.day, date.month, date.year, date.hour, date.minute);

							fclose(addLog);

							printf("\nAccount Removed\n");
							system("pause");
							return;
						}
						else {
							printf("\nPlease enter \"\x1b[31mCONFIRM\x1b[0m\" if you want to delete\n");
						}
					}
				} while (strcmp(deleteConfirmation, "CONFIRM") != 0);
			}
		}
		if (strcmp(removeID, staffInfo.id) != 0) {// If Invalid ID
			printf("Invalid ID\n");
			system("pause");
			system("cls");
		}
	} while (strcmp(removeID, staffInfo.id) != 0);

	return;
}

int deleteAcc(Staff* pStaffInfo) {
	Staff staffInfo;
	Cipher cipherInfo;
	int number = 1;
	char deleteConfirmation[20];

	system("cls");

	welcome();
	printf("======== Delete Account ========\n");
	printf("Enter \"-1\" to Exit\n");
	line();

	if (strcmp(pStaffInfo->id, "ST0001") == 0) { //Validate if its First Admin
		printf("First Admin is not allowed to Delete Account\n");
		system("pause");
		return 1;
	}

	printf("\n============ \x1b[31mWARNING\x1b[0m ============\n");
	printf("Confirm to \x1b[31mDELETE\x1b[0m?\n");
	printf("If YES enter \"\x1b[31mCONFIRM\x1b[0m\"\n");
	printf("If NO enter \"-1\"\n");
	line();
	do {
		printf("Enter your option: ");
		scanf("%s", deleteConfirmation);

		if (strcmp(deleteConfirmation, "-1") == 0) { //If input == -1
			printf("\nYour account is safe <3\n");
			system("pause");
			return 1; //Back
		}
		else if (strcmp(deleteConfirmation, "CONFIRM") == 0) {
			//Remove the Staff from the File
			FILE* readStaff = fopen("staff.txt", "r");
			if (readStaff == NULL) {
				printf("Unable to open staff.txt\n");
				exit(-1);
			}
			FILE* writeTemp = fopen("tempStaff.txt", "w");
			if (writeTemp == NULL) {
				printf("Unable to open tempStaff.txt\n");
				exit(-1);
			}
			while (fscanf(readStaff, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {
				//Decrypt
				decryption(cipherInfo.encryptionKey, cipherInfo.encryptData);
				//Assign all data to Staff by using strtok
				staffInfo.num = cipherInfo.num;
				strcpy(staffInfo.encryptionKey, cipherInfo.encryptionKey);
				assignDataToStaff(cipherInfo.encryptData, &staffInfo);

				if (strcmp(pStaffInfo->id, staffInfo.id) != 0) {
					sprintf(cipherInfo.encryptData, "%s|%s|%s|%s|%s|%d|%d|%s|%s|%s", staffInfo.id, staffInfo.name, staffInfo.position, staffInfo.password, staffInfo.forgotPass,
						staffInfo.forgotPassQues, staffInfo.staffAddress.houseNum, staffInfo.staffAddress.streetName, staffInfo.staffAddress.residentArea, staffInfo.staffAddress.city);
					//Encryption
					encryption(staffInfo.encryptionKey, cipherInfo.encryptData);

					fprintf(writeTemp, "%d|%s|%s\n", number++, staffInfo.encryptionKey, cipherInfo.encryptData);
				}
			}
			fclose(readStaff);
			fclose(writeTemp);

			FILE* readFile = fopen("tempStaff.txt", "r");
			if (readFile == NULL) {
				printf("Unable to open tempStaff.txt\n");
				exit(-1);
			}
			FILE* writeFile = fopen("staff.txt", "w");
			if (writeFile == NULL) {
				printf("Unable to open staff.txt\n");
				exit(-1);
			}
			while (fscanf(readFile, "%d|%[^\|]|%[^\n]\n", &cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData) != EOF) {

				fprintf(writeFile, "%d|%s|%s\n", cipherInfo.num, cipherInfo.encryptionKey, cipherInfo.encryptData);
			}
			fclose(readFile);
			fclose(writeFile);

			//To Update Staff Activity Into Staff Log
			logDate date;
			sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

			FILE* addLog = fopen("staffLog.txt", "a");
			if (addLog == NULL) {
				printf("Unable to open staff.txt\n");
				exit(-1);
			}
			fprintf(addLog, "%s deleted his account|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, date.day, date.month, date.year, date.hour, date.minute);

			fclose(addLog);

			printf("\nAccount Deleted\n");
			system("pause");
			system("cls");
			return 0;
		}
		else {
			printf("\nPlease enter \"\x1b[31mCONFIRM\x1b[0m\" if you want to DELETE\n");
		}
	} while (strcmp(deleteConfirmation, "CONFIRM") != 0);
}

//To View Staff Activity Log
void staffLog() {
	char description[100];
	char date[30];
	system("cls");

	FILE* readLog = fopen("staffLog.txt", "r");
	if (readLog == NULL) {
		printf("Unable to open staff.txt\n");
		exit(-1);
	}

	printf("%-100s", "Description");
	printf("Date\n");
	line2();
	while (fscanf(readLog, "%[^\|]|%[^\n]\n", description, date) != EOF) {
		printf("%-100s%s\n", description, date);
	}
	line2();

	fclose(readLog);
	system("pause");
	return;
}

int backOption(char input[MAX_STRINGSTAFF]) { //For Back Option
	if (strcmp(input, "-1") == 0) {
		system("cls");
		return 0;
	}
	return 1;
}

int backIntOption(int input) { //Back Option for Integer
	if (input == -1) {
		system("cls");
		return 0;
	}
	return 1;
}

int resetPassOption(char input[MAX_STRINGSTAFF]) { //For Password Recovery option
	if (strcmp(input, "0") == 0) {
		system("cls");
		return 0;
	}
	return 1;
}

void convertToUpper(char* str) { //Convert ID into uppercase
	int i = 0;
	while (str[i]) {
		str[i] = toupper(str[i]);
		i++;
	}
}

int isInvalidInput(const char* input) { //Check if Theres Space
	int length = strlen(input);
	for (int i = 0; i < length; i++) {
		if (isspace(input[i])) {
			return 1; //Invalid Input if space found
		}
		else if ((input[i]) == '|') { //Invalid Input if | is found too
			return 1;
		}
	}
	return 0; //Valid Input
}

int isContainBar(const char* input) { //Check if Theres Space
	int length = strlen(input);
	for (int i = 0; i < length; i++) {
		if ((input[i]) == '|') { //Invalid Input if | is found
			printf("No '|' allowed\n");
			return 1;
		}
	}
	return 0; //Valid Input
}

int validateNum(const char* text, int* num) { //Validate if it's a Number
	int validateNum;
	printf("%s", text); //Print the text

	validateNum = scanf("%d", num); //Read the input
	while (validateNum != 1) { //Validate if it's a number
		printf("That's not a number.\n\n");
		scanf("%*[^\n]"); //Clear the input buffer
		printf("%s", text);
		validateNum = scanf("%d", num); //Read again
	}
	return *num; //Return the number
}

void generateKey(char* key) {
	char ch;
	srand(time(0));

	for (int i = 0; i < 10; i++) {
		ch = 33 + (rand() % 91);
		key[i] = ch;
	}
	key[9] = '\0';
}

//Generate a random character
char random_char() {
	return 33 + (rand() % 91);
}

//Find combination
int findCombination(char encryptionKey[MAX_KEYSTAFF]) {
	int combination = 0;
	for (int i = 0; i < strlen(encryptionKey); i++) {
		int temp = encryptionKey[i];
		if (temp >= 10) {
			addAll(&temp);
		}
		combination += temp * pow(10, i);
	}
	return combination;
}

//Add digits of a number
void addAll(int* temp) {
	int answer = 0;
	int original = *temp;
	answer += original / 100;
	original %= 100;
	answer += original / 10;
	original %= 10;
	answer += original;

	*temp = answer % 10;
}

//Encrypt data
void encryption(char encryptionKey[MAX_KEYSTAFF], char* data) {
	int stringlen = strlen(data);
	int array[200];
	int i;
	int combination = findCombination(encryptionKey);

	for (int i = 0; i < stringlen; i++) {
		array[i] = (int)data[i];
	}

	for (int digit = 8; digit >= 0; digit--) {
		int method = combination / pow(10, digit);
		method %= 4;

		if (method == 0) {
			int j = 0;
			//XOR
			for (i = 0; i < stringlen; i++) {
				array[i] ^= encryptionKey[j];
				j = i % 9;
			}
		}
		else if (method == 1) {
			int asciiData;
			//Caeser Cipher
			for (int i = 0; i < stringlen; i++) {
				array[i] = (array[i] + 3) % 128;
			}
		}
		else if (method == 2) {
			//Reverse Whole String
			for (int i = 0, k = stringlen - 1; i < k; i++, k--) {
				char temp = array[i];
				array[i] = array[k];
				array[k] = temp;
			}
		}
		else if (method == 3) {
			//Reverse String By Part
			int part = (stringlen / 4); //part=5,len=20

			for (int j = 0; j < 4; j++) {
				for (int i = j * part, k = ((j + 1) * part) - 1; i < k; i++, k--) {
					char temp = array[i];
					array[i] = array[k];
					array[k] = temp;
				}
			}
		}
		combination -= method * pow(10, digit);
	}
	dec_to_hexa_conversion(array, stringlen, data);
}

void decryption(char encryptionKey[MAX_KEYSTAFF], char* hexChar) {
	int combination = findCombination(encryptionKey);
	int stringlen = strlen(hexChar) / 2;
	int array[MAX_CIPHERSTAFF];
	hex_to_dec_conversion(array, hexChar);
	char decryptedString[200] = "";

	//Loop until the combination becomes zero
	while (combination > 0) {
		int method = combination % 10;
		combination /= 10; //Remove the last digit
		method %= 4;

		if (method == 0) {
			int j = 0;
			//XOR
			for (int i = 0; i < stringlen; i++) {
				array[i] ^= encryptionKey[j];
				j = i % 9;
			}
		}
		else if (method == 1) {
			int asciiData;
			//Caeser Cipher
			for (int i = 0; i < stringlen; i++) {
				array[i] = (array[i] - 3) % 128;
				if (array[i] < 0) {
					array[i] += 128;
				}
			}
		}
		else if (method == 2) {
			//Reverse Whole String
			for (int i = 0, k = stringlen - 1; i < k; i++, k--) {
				char temp = array[i];
				array[i] = array[k];
				array[k] = temp;
			}
		}
		else if (method == 3) {
			//Reverse String By Part
			int part = (stringlen / 4); //part=5,len=20

			for (int j = 0; j < 4; j++) {
				for (int i = j * part, k = ((j + 1) * part) - 1; i < k; i++, k--) {
					char temp = array[i];
					array[i] = array[k];
					array[k] = temp;
				}
			}
		}
	}
	for (int i = 0; i < stringlen; i++) {
		char ch = array[i];
		decryptedString[i] = ch;
	}
	strcpy(hexChar, decryptedString);
}

//Convert decimal to hexadecimal
void dec_to_hexa_conversion(int array[200], int stringlen, char* data) {
	char tempHexChar[3];
	char hexChar[MAX_CIPHERSTAFF] = "";

	for (int i = 0; i < stringlen; i++) {
		sprintf(tempHexChar, "%02X", array[i]);
		strcat(hexChar, tempHexChar);
	}
	strcpy(data, hexChar);
}

//Convert hexadecimal to decimal
void hex_to_dec_conversion(int array[MAX_CIPHERSTAFF], char* hexChar) {
	int dataLen = strlen(hexChar);
	char tempData[MAX_CIPHERSTAFF];
	char convertData[3] = "";
	char tempDecChar[3];

	strcpy(tempData, hexChar);
	int i = 0;

	for (int loop = 0; loop < dataLen; loop += 2) {
		strncpy(convertData, tempData + loop, 2); //Extract two characters
		int decnum = strtol(convertData, NULL, 16); //Convert hexadecimal to decimal
		array[i] = decnum;
		i++;
	}
}

void assignDataToStaff(const char* data, Staff* pStaffInfo) {
	char* token = strtok(data, "|");

	if (token != NULL) {
		strcpy(pStaffInfo->id, token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		strcpy(pStaffInfo->name, token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		strcpy(pStaffInfo->position, token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		strcpy(pStaffInfo->password, token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		strcpy(pStaffInfo->forgotPass, token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		pStaffInfo->forgotPassQues = atoi(token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		pStaffInfo->staffAddress.houseNum = atoi(token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		strcpy(pStaffInfo->staffAddress.streetName, token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		strcpy(pStaffInfo->staffAddress.residentArea, token);
		token = strtok(NULL, "|");
	}

	if (token != NULL) {
		strcpy(pStaffInfo->staffAddress.city, token);
	}
}

//Get Current Time
void sGetCurrentTime(int* day, int* month, int* year, int* hour, int* minute) {
	time_t currentTime = time(NULL);
	struct tm date = *localtime(&currentTime);

	*day = date.tm_mday;
	*month = date.tm_mon + 1;
	*year = date.tm_year + 1900;
	*hour = date.tm_hour;
	*minute = date.tm_min;
}

void line() {
	printf("\n================================\n");
}

void line2() {
	printf("====================================================================================================================\n");
}