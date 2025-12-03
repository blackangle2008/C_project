#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 10
#define NAME_LEN 50
#define SUBJECT_NAME_LEN 30

typedef struct {
    int roll;
    char name[NAME_LEN];
    float marks[MAX_SUBJECTS];
    float total;
    float average;
    char grade;
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

char subjects[MAX_SUBJECTS][SUBJECT_NAME_LEN];
int subjectCount = 0;

// --------- Function Declarations ----------
void setupSubjects();
void addStudent();
void calculateTotalsAndGrade(Student *s);
char getGradeFromAverage(float avg);
void viewAllStudents();
void searchByRoll();
void showClassStats();
void clearInputBuffer();

// -----------------------------------------

int main() {
    int choice;

    printf("===== STUDENT GRADE ANALYZER =====\n\n");

    setupSubjects();  // Ask user for subjects at the start

    do {
        printf("\n---------- MENU ----------\n");
        printf("1. Add new student\n");
        printf("2. View all students\n");
        printf("3. Search student by roll number\n");
        printf("4. Show class statistics\n");
        printf("5. Exit\n");
        printf("--------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewAllStudents();
                break;
            case 3:
                searchByRoll();
                break;
            case 4:
                showClassStats();
                break;
            case 5:
                printf("Exiting... Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    return 0;
}

// Ask user to define subjects
void setupSubjects() {
    int i;
    printf("Enter number of subjects (max %d): ", MAX_SUBJECTS);
    while (1) {
        if (scanf("%d", &subjectCount) != 1 || subjectCount <= 0 || subjectCount > MAX_SUBJECTS) {
            printf("Invalid number. Enter between 1 and %d: ", MAX_SUBJECTS);
            clearInputBuffer();
        } else {
            break;
        }
    }

    clearInputBuffer(); // clear newline before fgets

    printf("Enter subject names:\n");
    for (i = 0; i < subjectCount; i++) {
        printf("Subject %d: ", i + 1);
        fgets(subjects[i], SUBJECT_NAME_LEN, stdin);
        // Remove newline from fgets
        size_t len = strlen(subjects[i]);
        if (len > 0 && subjects[i][len - 1] == '\n') {
            subjects[i][len - 1] = '\0';
        }
    }
}

// Add a new student
void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Cannot add more students. Limit reached.\n");
        return;
    }

    Student s;
    int i;

    printf("\n--- Add New Student ---\n");
    printf("Enter roll number: ");
    while (scanf("%d", &s.roll) != 1) {
        printf("Invalid input. Enter an integer roll number: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Enter name: ");
    fgets(s.name, NAME_LEN, stdin);
    size_t len = strlen(s.name);
    if (len > 0 && s.name[len - 1] == '\n') {
        s.name[len - 1] = '\0';
    }

    printf("Enter marks for %d subjects (0 - 100):\n", subjectCount);
    for (i = 0; i < subjectCount; i++) {
        float mark;
        while (1) {
            printf("%s: ", subjects[i]);
            if (scanf("%f", &mark) != 1 || mark < 0 || mark > 100) {
                printf("Invalid marks. Enter a number between 0 and 100.\n");
                clearInputBuffer();
            } else {
                s.marks[i] = mark;
                break;
            }
        }
    }

    calculateTotalsAndGrade(&s);

    // Store in global array
    students[studentCount] = s;
    studentCount++;

    printf("Student added successfully!\n");
}

// Calculate total, average and grade
void calculateTotalsAndGrade(Student *s) {
    float sum = 0.0f;
    for (int i = 0; i < subjectCount; i++) {
        sum += s->marks[i];
    }
    s->total = sum;
    s->average = sum / subjectCount;
    s->grade = getGradeFromAverage(s->average);
}

// Decide grade from average marks
char getGradeFromAverage(float avg) {
    if (avg >= 90) return 'S';   // Super / A+ (you can change symbols)
    else if (avg >= 80) return 'A';
    else if (avg >= 70) return 'B';
    else if (avg >= 60) return 'C';
    else if (avg >= 50) return 'D';
    else return 'F';
}

// View all students
void viewAllStudents() {
    if (studentCount == 0) {
        printf("No students added yet.\n");
        return;
    }

    printf("\n---------------- ALL STUDENTS ----------------\n");
    printf("%-5s %-20s %-10s %-10s %-5s\n", "Roll", "Name", "Total", "Average", "Grade");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < studentCount; i++) {
        printf("%-5d %-20s %-10.2f %-10.2f %-5c\n",
               students[i].roll,
               students[i].name,
               students[i].total,
               students[i].average,
               students[i].grade);
    }
}

// Search student by roll number
void searchByRoll() {
    if (studentCount == 0) {
        printf("No students added yet.\n");
        return;
    }

    int roll;
    printf("Enter roll number to search: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }

    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll) {
            printf("\nStudent found:\n");
            printf("Roll   : %d\n", students[i].roll);
            printf("Name   : %s\n", students[i].name);
            for (int j = 0; j < subjectCount; j++) {
                printf("%s : %.2f\n", subjects[j], students[i].marks[j]);
            }
            printf("Total  : %.2f\n", students[i].total);
            printf("Average: %.2f\n", students[i].average);
            printf("Grade  : %c\n", students[i].grade);
            return;
        }
    }

    printf("Student with roll number %d not found.\n", roll);
}

// Show class statistics
void showClassStats() {
    if (studentCount == 0) {
        printf("No students added yet.\n");
        return;
    }

    float classTotalAvg = 0.0f;
    int topperIndex = 0;

    // For subject-wise average
    float subjectSums[MAX_SUBJECTS] = {0};

    for (int i = 0; i < studentCount; i++) {
        classTotalAvg += students[i].average;

        if (students[i].average > students[topperIndex].average) {
            topperIndex = i;
        }

        for (int j = 0; j < subjectCount; j++) {
            subjectSums[j] += students[i].marks[j];
        }
    }

    classTotalAvg /= studentCount;

    printf("\n---------- CLASS STATISTICS ----------\n");
    printf("Number of students: %d\n", studentCount);
    printf("Class average (overall): %.2f\n", classTotalAvg);

    printf("\nTopper:\n");
    printf("Roll   : %d\n", students[topperIndex].roll);
    printf("Name   : %s\n", students[topperIndex].name);
    printf("Average: %.2f\n", students[topperIndex].average);
    printf("Grade  : %c\n", students[topperIndex].grade);

    printf("\nSubject-wise average:\n");
    for (int j = 0; j < subjectCount; j++) {
        float subAvg = subjectSums[j] / studentCount;
        printf("%s : %.2f\n", subjects[j], subAvg);
    }
}

// Clear input buffer after invalid scanf
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // discard
    }
}
