#include <stdio.h>
#include <string.h>

#define MAX_HISTORY 100

// Global variables
float balance = 1000.0;
char history[MAX_HISTORY][100];
int historyCount = 0;
int PIN = 1234;

// Function to add history entry
void addHistory(char entry[]) {
    if (historyCount < MAX_HISTORY) {
        strcpy(history[historyCount], entry);
        historyCount++;
    }
}

// Function to show transaction history
void showHistory() {
    if (historyCount == 0) {
        printf("\nNo transactions yet.\n");
        return;
    }

    printf("\n----- Transaction History -----\n");
    for (int i = 0; i < historyCount; i++) {
        printf("%d. %s\n", i + 1, history[i]);
    }
    printf("--------------------------------\n");
}

// Main program
int main() {
    int enteredPin, attempts = 0, choice;
    float amount;

    // PIN Verification with 3 attempts
    while (attempts < 3) {
        printf("Enter your 4-digit PIN: ");
        scanf("%d", &enteredPin);

        if (enteredPin == PIN) {
            printf("Access Granted!\n");
            break;
        } else {
            attempts++;
            printf("Incorrect PIN! Attempts left: %d\n", 3 - attempts);
        }
    }

    if (attempts == 3) {
        printf("\nYour account is locked due to too many wrong attempts!\n");
        return 0;
    }

    // Main ATM menu
    while (1) {
        printf("\n----- MINI ATM -----\n");
        printf("1. Check Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transaction History\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("\nYour Balance: %.2f Rs\n", balance);
            addHistory("Checked Balance");
            break;

        case 2:
            printf("\nEnter amount to deposit: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount!\n");
            } else {
                balance += amount;

                char log[100];
                sprintf(log, "Deposited: %.2f Rs", amount);
                addHistory(log);

                printf("Successfully deposited %.2f Rs\n", amount);
            }
            break;

        case 3:
            printf("\nEnter amount to withdraw: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount!\n");
            }
            else if (amount > balance) {
                printf("Insufficient balance!\n");
            } 
            else {
                balance -= amount;

                char log[100];
                sprintf(log, "Withdrawn: %.2f Rs", amount);
                addHistory(log);

                printf("Successfully withdrawn %.2f Rs\n", amount);
            }
            break;

        case 4:
            showHistory();
            break;

        case 5:
            printf("\nThank you for using the ATM!\n");
            return 0;

        default:
            printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
