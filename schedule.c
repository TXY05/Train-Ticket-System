#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>
#include<math.h>
#pragma warning(disable:4996)

#define WHITE(string) "\x1b[37m" string "\x1b[0m"
#define PAX_LIMIT 6
#define MAX_STATION 15
#define MAX_COACH 10
#define MAX_SEAT 20

#define MAX_STREETNAMESTAFF 30
#define MAX_RESIDENTAREASTAFF 30
#define MAX_CITYSTAFF 20
#define MAX_IDSTAFF 7
#define MAX_NAMESTAFF 25
#define MAX_POSITIONSTAFF 20
#define MAX_PASSWORDSTAFF 30
#define MAX_STRINGSTAFF 30
#define MAX_FORGOTPASSSTAFF 30
#define MAX_KEYSTAFF 10

#define MAX_ID_LENGTH 6
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20
#define MAX_GENDER_LENGTH 7
#define MAX_IC_LENGTH 15
#define MAX_CONTACT_LENGTH 15
#define MAX_SECURITY_QUESTION_LENGTH 100
#define MAX_SECURITY_ANSWER_LENGTH 100
#define MAX_CHOICE_LENGTH 6
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

/*************************/
typedef struct {
	int day;
	int month;
	char status[12];
}Date;

typedef struct {
	Date date[7];
	int seatNumber;
}Seat;

typedef struct {
	char coachNumber;
	Seat seat[MAX_SEAT];
}Coach;

typedef struct {
	Coach coach[MAX_COACH];
	char name[30];
	int arrivalHour, arrivalMinute;
}Station;

typedef struct {
	Station station[MAX_STATION];
	int trainNumber;
}Train;




// *******************************************//

typedef struct {
	char paxName[20];
	char paxIC[13];
}PaxDetail;

typedef struct {
	char paymentMethod[20];
	double amount;
	double totalAmount;
}Payment;

typedef struct {
	char departureStation[30];
	char arrivalStation[30];
	int departureMonth;
	int departureDay;
	int departureHour;
	int departureMinute;
	int arrivalHour;
	int arrivalMinute;
}StationDetail;

typedef struct {
	char coach;
	int seatNumber;
}SeatDetail;

typedef struct {
	int trainID;
	SeatDetail seatDetail[PAX_LIMIT];
}TrainDetail;

typedef struct {
	int bYear;
	int bMonth;
	int bDay;
}BookingDate;

typedef struct {
	char ticketID[13];
	char memberID[13];
	int pax;
	PaxDetail pDetail[PAX_LIMIT];
	Payment payment;
	StationDetail sDetail;
	TrainDetail tDetail;
	BookingDate bDate;
	char ticketStatus[20];
}Ticket;

Ticket ticket;

struct Member {
	char id[MAX_ID_LENGTH];
	char name[MAX_NAME_LENGTH];
	char password[MAX_PASSWORD_LENGTH];
	char passwordRecovery[MAX_PASSWORD_LENGTH];
	char gender[MAX_GENDER_LENGTH];
	char IC[MAX_IC_LENGTH];
	char contact[MAX_CONTACT_LENGTH];
	char securityQuestion1[MAX_SECURITY_QUESTION_LENGTH];
	char securityAnswer1[MAX_SECURITY_ANSWER_LENGTH];
};

void printSchedule();
void showAllSchedule(int i, int option);
int menu();
void adminTrainMenu(Staff* pStaffInfo);
void editTrainId(Staff* pStaffInfo);
void resetTrainSchedule(Staff* pStaffInfo);
void addTrain(Staff* pStaffInfo);
void editStationName(Staff* pStaffInfo);
int validateTime(int hour, int minute);
int getCurrentTime(int hour, int minute);
int availableTrain(int option);
void updateSeat();
int checkTrainId(Ticket ticket);
void checkStation(int* departure, int* arrival, Ticket* ptrTicket);
int checkCoach(int currentPax, Ticket ticket);
void editTicketTime(Train* ptrTrain);
void editTicketTrainId(int newId, int oldId);
void editTicketStationName(char oldStation[30], char newStation[30]);
int getDepartureTime(Train* ptrTrain);
void resetSeat();
void deleteTrain();

void resetSeat() {
	Train train;

	FILE* scheduleOut = fopen("schedule.dat", "rb");
	FILE* tmp = tmpfile();

	while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
		for (int j = 0; j < MAX_STATION; j++) {
			for (int k = 0; k < MAX_COACH; k++) {
				for (int l = 0; l < MAX_SEAT; l++) {
					for (int m = 0; m < 7; m++) {
						strcpy(train.station[j].coach[k].seat[l].date[m].status, "Available");
					}
				}
			}
		}
			fwrite(&train, sizeof(Train), 1, tmp);
	}
		fclose(scheduleOut);
		rewind(tmp);

		FILE* scheduleIn = fopen("schedule.dat", "wb");


		while (fread(&train, sizeof(Train), 1, tmp) != 0) {
			fwrite(&train, sizeof(Train), 1, scheduleIn);
		}
		fclose(scheduleIn);
		fclose(tmp);
	
		

}
void checkSchedule(struct Member* member) {

	updateValidDate();
	resetSeat();
	updateSeat();

	int option;

	do {
		option = menu();

		switch (option) {
		case 1:
			printSchedule(); // Print Full Schedule For All Station and Train
			break;
		case 2:
			memberBooking(member); //Select Departure and Arrival Station To Check Date and Time
			break;
		case 3:
			break;
		default:
			printf("Invalid Option!!!\n");
			Sleep(1500);
			system("cls");
		}
	} while (option != 3);
}

void printSchedule() {
	Train train;

	FILE* schedule = fopen("schedule.dat", "rb");
	fread(&train, sizeof(Train), 1, schedule);

	rewind(schedule);

	int i, j, option, valid = 0;
	const int PULAUSEBANG_TO_BATUCAVES = 1;
	const int BATUCAVES_TO_PULAUSEBANG = 2;

	do {
		system("cls");

		welcome();

		printf("Select Train Direction\n");
		printf("1. %s -> %s\n", train.station[14].name, train.station[0].name);
		printf("2. %s -> %s\n", train.station[0].name, train.station[14].name);
		printf("Select > ");
		scanf("%d", &option);

		if (option != PULAUSEBANG_TO_BATUCAVES && option != BATUCAVES_TO_PULAUSEBANG) {
			printf("\nInvalid Option!!!");
			Sleep(1500);
			system("cls");
		}

	} while (option != PULAUSEBANG_TO_BATUCAVES && option != BATUCAVES_TO_PULAUSEBANG);

	system("cls");

	welcome();

	printf("%84s", "TRAIN TIMETABLE\n\n");

	printf("%-18s", "Number of Train");

	valid = availableTrain(option);

	for (i = 1; i <= valid; i++) {
		printf("%8d", i); //"i" Represent Number Of Train Available
	}

	printf("\n%-19s", "Train ID");

	while (fread(&train, sizeof(Train), 1, schedule) != 0) {
		if (train.trainNumber % 2 == (option - 1)) {
			printf("%8d", train.trainNumber); //Print All Available Train ID
		}
	}

	printf("\n");

	if (option == PULAUSEBANG_TO_BATUCAVES) {
		for (i = 14; i >= 0; i--) {
			showAllSchedule(i, option);
		}

	}
	else {
		for (i = 0; i < MAX_STATION; i++) {
			showAllSchedule(i, option);
		}
	}

	fclose(schedule);
	printf("\n");
	system("pause");

}

void showAllSchedule(int i, int option) {
	Train train;

	FILE* schedule = fopen("schedule.dat", "rb");
	fread(&train, sizeof(Train), 1, schedule);
	printf("%-22s", train.station[i].name); //Print All Station Name

	rewind(schedule);

	while (fread(&train, sizeof(Train), 1, schedule) != 0) {
		if (train.trainNumber % 2 == (option - 1)) {
			if (train.station[i].arrivalHour == 0 && train.station[i].arrivalMinute == 0) {
				printf(WHITE("%2d:%-5.02d"), train.station[i].arrivalHour, train.station[i].arrivalMinute); //Hide Unavailable Time With White Font
			}
			else {
				printf("%2d:%-5.02d", train.station[i].arrivalHour, train.station[i].arrivalMinute);//Print All Available Time With Black Font
			}
		}

	}
	printf("\n");
	fclose(schedule);
}

int menu() {
	int admin = 1, user = 2;	//nid to change after combine
	int option;

	option = 0;

	system("cls");
	welcome();

	printf("\n======== Check Schedule ========\n");
	printf("1.Check Full Train Schedule\n");
	printf("2.Make Booking\n");
	printf("3.Exit\n");
	line();

	printf("\nChoose An Option > ");
	rewind(stdin);
	scanf("%d", &option);

	return option;
}

void adminTrainMenu(Staff* pStaffInfo) {
	int option;

	do {

		system("cls");

		welcome();

		printf("========== Edit Train ==========\n");

		printf("1. Edit Train ID\n");
		printf("2. Reset Train Schedule\n");
		printf("3. Add Train\n");
		printf("4. Edit Station Name\n");
		printf("5. Delete Train\n");
		printf("6. Check Full Train Schedule\n");
		printf("7. Main Menu\n");
		line();
		printf("Select > ");
		scanf("%d", &option);

		switch (option) {
		case 1:
			editTrainId(pStaffInfo);
			break;
		case 2:
			resetTrainSchedule(pStaffInfo);
			break;
		case 3:
			addTrain(pStaffInfo);
			break;
		case 4:
			editStationName(pStaffInfo);
			break;
		case 5:
			deleteTrain(pStaffInfo);
			break;
		case 6:
			printSchedule();
			break;
		case 7:
			break;
		default:
			printf("Invalid Option!!!\n");
			Sleep(1500);
		}

	} while (option != 7);


}

void editTrainId(Staff* pStaffInfo) {
	Train train;

	FILE* tmp = tmpfile();
	FILE* scheduleOut = fopen("schedule.dat", "rb");

	int i, newId, j, oldId;

	int id, valid;

	do {
		valid = -1;
		printf("\nEnter Train ID To Edit (-1 To Stop) > ");
		scanf("%d", &id);

		rewind(scheduleOut);
		if (id != -1) {
			while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
				if (train.trainNumber == id) {
					valid = 0;
					break;
				}
				fwrite(&train, sizeof(Train), 1, tmp);
			}
			if (valid != 0) {
				printf("Invalid Train ID!!!\n");
				Sleep(1500);
			}
		}
		else {
			printf("\n\nExiting...");
			Sleep(1500);
			return;
		}
	} while (valid != 0 && id != -1);

	oldId = train.trainNumber;

	printf("Enter New ID (-1 To Stop) > ");
	scanf("%d", &newId);

	if (newId != -1) {
		printf("\n\nEdited Successfully...\n");
		Sleep(1500);

		train.trainNumber = newId;
		fwrite(&train, sizeof(Train), 1, tmp);

		logDate date;
		sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

		FILE* addLog = fopen("staffLog.txt", "a");
		if (addLog == NULL) {
			printf("Unable to open staff.txt\n");
			exit(-1);
		}
		fprintf(addLog, "%s edited Train ID to \"%d\"|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, train.trainNumber, date.day, date.month, date.year, date.hour, date.minute);

		editTicketTrainId(train.trainNumber, oldId);
		fclose(addLog);

		while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
			fwrite(&train, sizeof(Train), 1, tmp);
		}
		fclose(scheduleOut);
		rewind(tmp);

		FILE* scheduleIn = fopen("schedule.dat", "wb");

		while (fread(&train, sizeof(Train), 1, tmp) != 0) {
			fwrite(&train, sizeof(Train), 1, scheduleIn);
		}
		fclose(scheduleIn);
		fclose(tmp);
	}
	else {
		printf("\n\nExiting...");
		Sleep(1500);
	}

}

void resetTrainSchedule(Staff* pStaffInfo) {
	Train train;

	FILE* tmp = tmpfile();
	FILE* scheduleOut = fopen("schedule.dat", "rb");

	int trainArray, i, j;
	int id, valid;

	do {
		valid = -1;
		printf("\nEnter Train ID To Edit (-1 To Stop) > ");
		scanf("%d", &id);

		rewind(scheduleOut);
		if (id != -1) {
			while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
				if (train.trainNumber == id) {
					valid = 0;
					break;
				}
				fwrite(&train, sizeof(Train), 1, tmp);
			}
			if (valid != 0) {
				printf("Invalid Train ID!!!\n");
				Sleep(1500);
			}
		}
		else {
			printf("\n\nExiting...");
			Sleep(1500);
			return;
		}
	} while (valid != 0 && id != -1);

	printf("\nEditing Train ID : %d\n", train.trainNumber);
	if (getDepartureTime(&train) == 0) {

		fwrite(&train, sizeof(Train), 1, tmp);


		logDate date;
		sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

		FILE* addLog = fopen("staffLog.txt", "a");
		if (addLog == NULL) {
			printf("Unable to open staff.txt\n");
			exit(-1);
		}
		fprintf(addLog, "%s reset schedule for Train ID \"%d\"|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, train.trainNumber, date.day, date.month, date.year, date.hour, date.minute);

		fclose(addLog);

		editTicketTime(&train);

		while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
			fwrite(&train, sizeof(Train), 1, tmp);
		}
		fclose(scheduleOut);
		rewind(tmp);

		FILE* scheduleIn = fopen("schedule.dat", "wb");

		while (fread(&train, sizeof(Train), 1, tmp) != 0) {
			fwrite(&train, sizeof(Train), 1, scheduleIn);
		}
		fclose(scheduleIn);
		fclose(tmp);
	}
}

void editTicketTime(Train* ptrTrain) {
	Ticket ticket;

	FILE* bookingOut = fopen("ticket.dat", "rb");
	FILE* tmp = tmpfile();

	while (fread(&ticket, sizeof(Ticket), 1, bookingOut) != 0) {
		if (ptrTrain->trainNumber == ticket.tDetail.trainID) {
			for (int j = 0; j < MAX_STATION; j++) {
				if (strcmp(ticket.sDetail.departureStation, ptrTrain->station[j].name) == 0) {
					ticket.sDetail.departureHour = ptrTrain->station[j].arrivalHour;
					ticket.sDetail.departureMinute = ptrTrain->station[j].arrivalMinute;
				}
				else if (strcmp(ticket.sDetail.arrivalStation, ptrTrain->station[j].name) == 0) {
					ticket.sDetail.arrivalHour = ptrTrain->station[j].arrivalHour;
					ticket.sDetail.arrivalMinute = ptrTrain->station[j].arrivalMinute;
				}
			}
		}
		fwrite(&ticket, sizeof(Ticket), 1, tmp);
	}
	fclose(bookingOut);
	rewind(tmp);

	FILE* bookingIn = fopen("ticket.dat", "wb");

	while (fread(&ticket, sizeof(Ticket), 1, tmp) != 0) {
		fwrite(&ticket, sizeof(Ticket), 1, bookingIn);
	}
	fclose(bookingIn);
	fclose(tmp);
}

void editTicketTrainId(int newId, int oldId) {
	Ticket ticket;

	FILE* bookingOut = fopen("ticket.dat", "rb");
	FILE* tmp = tmpfile();

	while (fread(&ticket, sizeof(Ticket), 1, bookingOut) != 0) {
		if (oldId == ticket.tDetail.trainID) {
			ticket.tDetail.trainID = newId;
		}
		fwrite(&ticket, sizeof(Ticket), 1, tmp);
	}
	fclose(bookingOut);
	rewind(tmp);

	FILE* bookingIn = fopen("ticket.dat", "wb");

	while (fread(&ticket, sizeof(Ticket), 1, tmp) != 0) {
		fwrite(&ticket, sizeof(Ticket), 1, bookingIn);
	}
	fclose(bookingIn);
	fclose(tmp);
}

void addTrain(Staff* pStaffInfo) {

	Train train;

	FILE* scheduleOut = fopen("schedule.dat", "rb");

	int newId, valid;

	do {
		valid = 0;

		printf("\nEnter New Train ID (-1 To Stop) > ");
		scanf("%d", &newId);

		rewind(scheduleOut);
		if (newId == -1) {
			printf("\n\nExiting...");
			Sleep(1500);
			valid = -1;
			break;
		}
		else {
			while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
				if (newId == train.trainNumber) {
					printf("Repeated Train ID!!!\n");
					Sleep(1500);
					valid = -1;
				}
			}
		}
	} while (valid == -1 && newId != -1);
	rewind(scheduleOut);
	fread(&train, sizeof(Train), 1, scheduleOut);
	fclose(scheduleOut);

	if (valid == 0) {
		FILE* scheduleIn = fopen("schedule.dat", "ab");

		if (getDepartureTime(&train) == 0) {
			train.trainNumber = newId;

			fwrite(&train, sizeof(Train), 1, scheduleIn);

			logDate date;
			sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

			FILE* addLog = fopen("staffLog.txt", "a");
			if (addLog == NULL) {
				printf("Unable to open staff.txt\n");
				exit(-1);
			}
			fprintf(addLog, "%s added new Train ID \"%d\"|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, newId, date.day, date.month, date.year, date.hour, date.minute);
			fclose(scheduleIn);
			fclose(addLog);

		}
	}


}
void editStationName(Staff* pStaffInfo) {
	Train train;

	FILE* scheduleOut = fopen("schedule.dat", "rb");
	FILE* tmp = tmpfile();

	fread(&train, sizeof(Train), 1, scheduleOut);

	char editStation[30], newStation[30];
	int i, valid, temp;

	do {
		valid = -1;

		printf("\nEnter Station Name To Edit (-1 to Stop) > ");
		rewind(stdin);
		scanf("%[^\n]", editStation);

		if (strcmp(editStation, "-1") != 0) {
			for (i = 0; i < MAX_STATION; i++) {
				if (strcmp(train.station[i].name, editStation) == 0) {
					valid = 0;
					temp = i;
					break;
				}
			}
			if (valid != 0) {
				printf("Invalid Station!!!\n");
				Sleep(1500);
			}
		}
		else {
			printf("\n\nExiting...");
			Sleep(1500);
			return;
		}
	} while (valid != 0);

	rewind(scheduleOut);

	if (valid == 0) {

		printf("Enter New Station Name (-1 to stop) > ");
		rewind(stdin);
		scanf("%[^\n]", newStation);

		if (strcmp(newStation, "-1") == 0) {
			printf("Edit Cancelled...");
			Sleep(1000);
			return;
		}

		printf("\n\nEdit Succesful");
		Sleep(1000);

		while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
			strcpy(train.station[temp].name, newStation);
			fwrite(&train, sizeof(Train), 1, tmp);
		}
		fclose(scheduleOut);
		rewind(tmp);

		FILE* scheduleIn = fopen("schedule.dat", "wb");

		while (fread(&train, sizeof(Train), 1, tmp) != 0) {
			fwrite(&train, sizeof(Train), 1, scheduleIn);
		}
		fclose(scheduleIn);
		fclose(tmp);

		logDate date;
		sGetCurrentTime(&date.day, &date.month, &date.year, &date.hour, &date.minute);

		FILE* addLog = fopen("staffLog.txt", "a");
		if (addLog == NULL) {
			printf("Unable to open staff.txt\n");
			exit(-1);
		}
		fprintf(addLog, "%s edited station name to \"%s\"|%02d-%02d-%d %02d:%02d\n", pStaffInfo->id, newStation, date.day, date.month, date.year, date.hour, date.minute);

		fclose(addLog);

		editTicketStationName(editStation, newStation);
	}
}


void editTicketStationName(char oldStation[30], char newStation[30]) {
	Ticket ticket;

	FILE* bookingOut = fopen("ticket.dat", "rb");
	FILE* tmp = tmpfile();

	while (fread(&ticket, sizeof(Ticket), 1, bookingOut) != 0) {
		for (int j = 0; j < MAX_STATION; j++) {
			if (strcmp(ticket.sDetail.departureStation, oldStation) == 0) {
				strcpy(ticket.sDetail.departureStation, newStation);
			}
			else if (strcmp(ticket.sDetail.arrivalStation, oldStation) == 0) {
				strcpy(ticket.sDetail.arrivalStation, newStation);
			}
		}
		fwrite(&ticket, sizeof(Ticket), 1, tmp);
	}
	fclose(bookingOut);
	rewind(tmp);

	FILE* bookingIn = fopen("ticket.dat", "wb");

	while (fread(&ticket, sizeof(Ticket), 1, tmp) != 0) {
		fwrite(&ticket, sizeof(Ticket), 1, bookingIn);
	}
	fclose(bookingIn);
	fclose(tmp);

}

int availableTrain(int option) {
	int i;
	int valid = 0;

	Train train;

	FILE* schedule = fopen("schedule.dat", "rb");

	while (fread(&train, sizeof(Train), 1, schedule) != 0) {
		if (train.trainNumber % 2 == (option - 1)) {
			valid++;
		}
	}

	fclose(schedule);
	return valid;
}

void updateSeat() {

	FILE* booking = fopen("ticket.dat", "rb");
	FILE* scheduleOut = fopen("schedule.dat", "rb");
	FILE* tmp = tmpfile();

	Train train;
	Ticket ticket;

	int j;
	int departureStation, arrivalStation;

	while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
		while (fread(&ticket, sizeof(Ticket), 1, booking) != 0) {
			for (int i = 0; i < 7; i++) {
				if (ticket.sDetail.departureMonth == train.station[0].coach[0].seat[0].date[i].month && ticket.sDetail.departureDay == train.station[0].coach[0].seat[0].date[i].day
					&& strcmp(ticket.ticketStatus, "Valid") == 0 && train.trainNumber == ticket.tDetail.trainID) {
					for (j = 0; j < ticket.pax; j++) {
						checkStation(&departureStation, &arrivalStation, &ticket);
						if (departureStation < arrivalStation) {
							while (departureStation < arrivalStation) {
								strcpy(train.station[departureStation].coach[checkCoach(j, ticket)].seat[ticket.tDetail.seatDetail[j].seatNumber - 1].date[i].status, "Booked");

								departureStation++;
							}
						}
						else {
							while (departureStation > arrivalStation) {
								strcpy(train.station[departureStation].coach[checkCoach(j, ticket)].seat[ticket.tDetail.seatDetail[j].seatNumber - 1].date[i].status, "Booked");
								departureStation--;
							}
						}
					}
				}
			}
		}
		rewind(booking);
		fwrite(&train, sizeof(Train), 1, tmp);
	}
	fclose(booking);
	fclose(scheduleOut);
	rewind(tmp);

	FILE* scheduleIn = fopen("schedule.dat", "wb");

	while (fread(&train, sizeof(Train), 1, tmp) != 0) {
		fwrite(&train, sizeof(Train), 1, scheduleIn);
	}
	fclose(scheduleIn);
	fclose(tmp);

}

int checkTrainId(Ticket ticket) {
	int i = 0;

	Train train;

	FILE* schedule = fopen("schedule.dat", "rb");

	while (fread(&train, sizeof(Train), 1, schedule) != 0) {
		if (ticket.tDetail.trainID == train.trainNumber) {
			return i;
		}
		i++;
	}

	fclose(schedule);
}

void checkStation(int* departure, int* arrival, Ticket* ptrTicket) {
	Train train;

	FILE* schedule = fopen("schedule.dat", "rb");

	fread(&train, sizeof(Train), 1, schedule);

	for (int i = 0; i < MAX_STATION; i++) {
		if (strcmp(ptrTicket->sDetail.departureStation, train.station[i].name) == 0) {
			*departure = i;
		}

		if (strcmp(ptrTicket->sDetail.arrivalStation, train.station[i].name) == 0) {
			*arrival = i;
		}

	}


	fclose(schedule);
}

int checkCoach(int currentPax, Ticket ticket) {
	return ticket.tDetail.seatDetail[currentPax].coach - 65;
}

int validateTime(int hour, int minute) {
	if (hour == -1 || minute == -1) {
		return 0;
	}

	if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
		return 1;
	}

	return 0;
}

int getDepartureTime(Train* ptrTrain) {
	int i;

	Train tempTrain;

	FILE* schedule = fopen("schedule.dat", "rb");
	fread(&tempTrain, sizeof(Train), 1, schedule);

	printf("Enter In Format HH MM (-1 To Stop)\n\n");
	for (i = 0; i < MAX_STATION; i++) {

		do {
			printf("%-22s", tempTrain.station[i].name);
			scanf("%d %d", &tempTrain.station[i].arrivalHour, &tempTrain.station[i].arrivalMinute);

			if (validateTime(tempTrain.station[i].arrivalHour, tempTrain.station[i].arrivalMinute) == 1) {
				printf("Time Invalid ! Please Enter Again !\n\n");
			}

		} while (validateTime(tempTrain.station[i].arrivalHour, tempTrain.station[i].arrivalMinute) == 1);

		if (tempTrain.station[i].arrivalHour == -1 || tempTrain.station[i].arrivalMinute == -1) {
			printf("\n\nEdit Cancelled...");
			Sleep(1000);
			return 1;
		}

	}

	for (i = 0; i < MAX_STATION; i++) {
		ptrTrain->station[i].arrivalHour = tempTrain.station[i].arrivalHour;
		ptrTrain->station[i].arrivalMinute = tempTrain.station[i].arrivalMinute;
	}

	fclose(schedule);

	return 0;
}


void deleteTrain() {
	int id, valid;

	Train train;

	FILE* scheduleOut = fopen("schedule.dat", "rb");
	FILE* tmp = tmpfile();

	do {
		valid = -1;
		printf("\nEnter Train ID To Delete (-1 To Stop) > ");
		scanf("%d", &id);

		rewind(scheduleOut);
		if (id != -1) {
			while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
				if (train.trainNumber == id) {
					valid = 0;
					printf("Train Deleted");
					Sleep(1000);
					break;
				}
			}
			if (valid != 0) {
				printf("Invalid Train ID!!!\n");
				Sleep(1500);
			}
		}
		else {
			printf("\n\nExiting...");
			Sleep(1500);
			return;
		}
	} while (valid != 0 && id != -1);

	rewind(scheduleOut);

	if (valid == 0) {
		while (fread(&train, sizeof(Train), 1, scheduleOut) != 0) {
			if (train.trainNumber != id) {
				fwrite(&train, sizeof(Train), 1, tmp);
			}
		}

		fclose(scheduleOut);
		rewind(tmp);

		FILE* scheduleIn = fopen("schedule.dat", "wb");

		while (fread(&train, sizeof(Train), 1, tmp) != 0) {
			fwrite(&train, sizeof(Train), 1, scheduleIn);
		}
		fclose(scheduleIn);
		fclose(tmp);
	}
}