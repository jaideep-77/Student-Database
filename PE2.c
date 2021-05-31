// Jaideep Kular

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 30
#define fieldLength 200

#define diskFile "diskFile.dat"
#define courseFile "course.txt"

struct db_type
{
  char name[fieldLength];
  int age;
  char course1[fieldLength];
  char course2[fieldLength];
  char status[fieldLength];
};

struct courseInfo
{
  char code[20]; // e.g., EECS2030
  char title[fieldLength];
  char date[20];
  char time_start[20];
  char time_end[20];
  char location[20];
};

struct courseInfo courseArr[SIZE]; // global variable  array of struc

char prompt_menu(void);
void init_list(struct db_type *pArr[]);
void clearDB(struct db_type *pArr[]);
void init_courseArr(void);

void writeDisk(struct db_type *pArr[]);
void emptyDisk(void);
void loadDisk(struct db_type *pArr[]);

void displayCourses(void);
void enterNew(struct db_type *pArr[SIZE]);
void displayDB(struct db_type *pArr[]);
void removeRecord(struct db_type *pArr[]);
void swap(struct db_type *pArr[]);
void sort(struct db_type *pArr[]);

int main(int argc, char *argv[])
{

  struct db_type *db_pArr[SIZE]; // main db storage

  init_list(db_pArr); // set to NULL

  init_courseArr(); // load course from diskfile

  char choice;
  for (;;)
  {
    choice = prompt_menu();
    switch (choice)
    {
    case 'n':
      enterNew(db_pArr);
      break;
    case 'd':
      displayDB(db_pArr);
      break;
    case 'w':
      writeDisk(db_pArr);
      break;
    case 'l':
      loadDisk(db_pArr);
      break;
    case 's':
      sort(db_pArr);
      break;

    case 'c':
      clearDB(db_pArr);
      break;
    case 'e':
      emptyDisk();
      break;

    case 'v':
      displayCourses();
      break;
    case 'p':
      swap(db_pArr);
      break;
    case 'r':
      removeRecord(db_pArr);
      break;

    case 'q':
      exit(1); // terminate the whole program
    }
  }
  return 0;
}

void init_list(struct db_type *pArr[])
{
  int t;
  for (t = 0; t < SIZE; t++)
  {
    pArr[t] = NULL;
  }
}

void clearDB(struct db_type *pArr[])
{
  char c3[3];
  printf("are you sure to clear db? (y) or (n)? ");

  fgets(c3, 3, stdin);

  if (!strcmp(c3, "y\n"))
    init_list(pArr);
}

char prompt_menu(void)
{
  char s[80];
  while (1)
  {
    printf("\n-----------------------------------------------------------------\n");
    printf("|    %-20s", "(N)ew record");
    printf("%-20s", "(R)emove record");
    printf("Swa(p) records\t|\n");
    printf("|    %-20s", "(S)ort database");
    printf("%-20s", "(C)lear database");
    printf("(D)isplay db\t|\n");

    printf("|    %-20s", "(L)oad disk");
    printf("%-20s", "(W)rite disk");
    printf("(E)mpty disk\t|\n");

    printf("|    %-20s", "(V)iew courses"); //|\tSwa(p) record\t(Q)uit\t\t\t\t|\n");
    printf("%-20s", "(Q)uit");
    printf("*Case Insensitive*\t|\n");
    printf("-----------------------------------------------------------------\n");
    printf("choose one: ");

    fgets(s, 50, stdin); // \n added

    if (strlen(s) == 2 && strchr("edlsuqrcwnvpr", tolower(*s)))
      return tolower(*s); // s[0], return the first character of s
    //else
    printf("not a valid input!\n");
  }
}

// display all or specified course
void displayCourses(void)
{
  // the provide PE2.out uses "%s\t%-40s%-5s %s-%s   %s\n" as formatting string for printing each course info
  printf("course code (or 'a')? ");
  char code[10];
  fgets(code, 10, stdin);

  // if user enters 'a', prints info for all courses
  if (code[0] == 'a')
  {
    printf("%s\n", "=================================================================================");
    for (int i = 0; i < SIZE; i++)
    {
      if (strcmp(courseArr[i].code, "EECS0000") == 0)
        break;
      printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[i].code, courseArr[i].title, courseArr[i].date, courseArr[i].time_start, courseArr[i].time_end, courseArr[i].location);
    }
    printf("%s\n", "=================================================================================");
  }
  else
  {
    // code can be EECSXXXX where XXXX is the code for required course or simply XXXX
    if (code[0] == 'E')
      code[8] = '\0';
    else
    {
      // convert the XXXX to EECSXXXX
      code[4] = code[0];
      code[5] = code[1];
      code[6] = code[2];
      code[7] = code[3];
      code[0] = 'E';
      code[1] = 'E';
      code[2] = 'C';
      code[3] = 'S';
      code[8] = '\0';
    }

    // Counter variable keeps track of the course being compared
    // While loop goes through each of the courses until the course with valid code is found or EECS0000 is reached.

    int counter = 0;
    while (counter < SIZE)
    {
      if (strcmp(courseArr[counter].code, "EECS0000") == 0)
      {
        printf("%s\n", "error! course does not exist");
        break;
      }

      if (strcmp(courseArr[counter].code, code) == 0)
      {
        // prints the information about this course in specified format
        printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[counter].code, courseArr[counter].title, courseArr[counter].date, courseArr[counter].time_start, courseArr[counter].time_end, courseArr[counter].location);
        break;
      }
      counter++;
    }
  }
}

// input items into the list
void enterNew(struct db_type *pArr[SIZE])
{
  // Create a new struct pointer to store the information
  struct db_type *record = (struct db_type *)malloc(sizeof(struct db_type));
  char answer[fieldLength];
  int num;

  printf("name: ");
  fgets(record->name, fieldLength, stdin);
  record->name[strlen(record->name) - 1] = '\0';

  printf("age: ");
  scanf("%d", &num);
  getchar();
  record->age = num;

  strcpy(answer, "");

  // If the course doesn't exist, ask the user to enter the code again.

  int courseExists = 0;
  printf("course-1: ");

  // While loop to keep asking user for code until a valid code is entered.
  while (courseExists == 0)
  {
    fgets(answer, fieldLength, stdin);
    answer[strlen(answer) - 1] = '\0';

    // If the code entered is only in numbers, then convert the code to EECSXXXX format.
    if (answer[0] != 'E')
    {
      answer[4] = answer[0];
      answer[5] = answer[1];
      answer[6] = answer[2];
      answer[7] = answer[3];
      answer[0] = 'E';
      answer[1] = 'E';
      answer[2] = 'C';
      answer[3] = 'S';
      answer[8] = '\0';
    }

    int counter = 0;
    // While loop to keep checking through all the courses in course db
    while (counter < SIZE)
    {
      // If no course is found with valid code, the while loop will continue as courseExists variable is still 0.
      if (strcmp(courseArr[counter].code, "EECS0000") == 0)
      {
        printf("error! course does not exist. Enter again:");
        counter = 0;
        break;
      }

      // If the course with given code is found, copy that course's info to record's course as a formatted string
      // This string has the format -> "%s\t%-40s%-5s %s-%s   %s\n"
      // used for loops to cover up any required spaces
      if (strcmp(courseArr[counter].code, answer) == 0)
      {
        strcpy(record->course1, courseArr[counter].code);
        strcat(record->course1, "\t");
        strcat(record->course1, courseArr[counter].title);
        for (int i = 0; i < 40 - strlen(courseArr[counter].title); i++)
          strcat(record->course1, " ");
        strcat(record->course1, courseArr[counter].date);
        for (int i = 0; i < 5 - strlen(courseArr[counter].date); i++)
          strcat(record->course1, " ");
        strcat(record->course1, courseArr[counter].time_start);
        strcat(record->course1, "-");
        strcat(record->course1, courseArr[counter].time_end);
        strcat(record->course1, "   ");
        strcat(record->course1, courseArr[counter].location);
        courseExists = 1;

        break;
      }
      counter++;
    }

    strcpy(answer, "");
  }

  // Now ask the user for course 2
  // While loop to keep asking user for code until a valid code is entered.
  courseExists = 0;
  printf("course-2: ");
  while (courseExists == 0)
  {
    fgets(answer, fieldLength, stdin);
    answer[strlen(answer) - 1] = '\0';

    // If the code entered is only in numbers, then convert the code to EECSXXXX format.
    if (answer[0] != 'E')
    {
      answer[4] = answer[0];
      answer[5] = answer[1];
      answer[6] = answer[2];
      answer[7] = answer[3];
      answer[0] = 'E';
      answer[1] = 'E';
      answer[2] = 'C';
      answer[3] = 'S';
      answer[8] = '\0';
    }

    int counter = 0;
    // While loop to keep checking through all the courses in course db
    while (counter < SIZE)
    {
      // If no course is found with valid code, the while loop will continue as courseExists variable is still 0.
      if (strcmp(courseArr[counter].code, "EECS0000") == 0)
      {
        printf("error! course does not exist. Enter again:");
        counter = 0;
        break;
      }

      // If the course with given code is found, copy that course's info to record's course as a formatted string
      // This string has the format -> "%s\t%-40s%-5s %s-%s   %s\n"
      // used for loops to cover up any required spaces
      if (strcmp(courseArr[counter].code, answer) == 0)
      {
        strcpy(record->course2, courseArr[counter].code);
        strcat(record->course2, "\t");
        strcat(record->course2, courseArr[counter].title);
        for (int i = 0; i < 40 - strlen(courseArr[counter].title); i++)
          strcat(record->course2, " ");
        strcat(record->course2, courseArr[counter].date);
        for (int i = 0; i < 5 - strlen(courseArr[counter].date); i++)
          strcat(record->course2, " ");
        strcat(record->course2, courseArr[counter].time_start);
        strcat(record->course2, "-");
        strcat(record->course2, courseArr[counter].time_end);
        strcat(record->course2, "   ");
        strcat(record->course2, courseArr[counter].location);
        courseExists = 1;

        // printf("%s %s\n","You have added this course for course2: ", record->course2);

        // char hour2[] = {courseArr[counter].time_start[0], courseArr[counter].time_start[1]};
        // printf("%s %s","THis is hour of the course: ",hour2);

        break;
      }
      counter++;
    }

    strcpy(answer, "");
  }

  // Now set the status of this struct
  // Check if the start time of course 1 is after the end time of course 2 OR
  // start time of course 2 is after the end of course 1

  // This part adds the record to the pArr array.
  // It checks for the empty spaces and inserts the newly created record in the first empty space.
  for (int i = 0; i < SIZE; i++)
  {
    if (pArr[i] == NULL)
    {
      pArr[i] = record;
      break;
    }
  }
}

// display records
void displayDB(struct db_type *pArr[])
{
  // If there are no records present
  if (pArr[0] == NULL)
  {
    printf("%s\n", "===============================");
    printf("%s\n", "========== 0 records ==========");
  }

  // it loops through all the elements of pArr and stops when it encounters NULL
  for (int i = 0; i < SIZE; i++)
  {

    if (pArr[i] == NULL)
      break;
    else
    {

      // Prints the record of a student
      printf("%s %s\n", "name:", pArr[i]->name);
      printf("%s %d\n", "age:", pArr[i]->age);
      printf("%s %s\n", "course1:", pArr[i]->course1);
      printf("%s %s\n", "course2:", pArr[i]->course2);
      printf("%s %s\n", "remarks:", pArr[i]->status);
      puts("");
    }
  }
}

// remove an existing item
void removeRecord(struct db_type *pArr[])
{
  // Gets the name to be removed
  printf("%s", "enter full name to be removed: ");
  char name[fieldLength];
  fgets(name, fieldLength, stdin);
  name[strlen(name) - 1] = '\0';

  int removed = 0;

  // Goes through every student record in the database and stops when it reaches the max size of the database or when it encounters a null value
  for (int i = 0; i < SIZE; i++)
  {

    if (pArr[i] == NULL)
      break;
    else
    {

      // compares the inputted string to name of a record and if they are equal, put all the next records 1 index behind to remove the current record.
      if (strcmp(pArr[i]->name, name) == 0)
      {
        printf("%s%s%s\n", "record [", pArr[i]->name, "] removed successfully.");

        // This while loop puts all the next records 1 index behind
        while (i < SIZE - 1 && pArr[i + 1] != NULL)
        {
          pArr[i] = pArr[i + 1];
          i++;
        }
        // this statement sets the last value of the pArr to be null.
        // Only need it when database is full because otherwise it's NULL by default
        pArr[i] = NULL;

        removed = 1;
        break;
      }
    }
  }

  // This statement checks the remove variable's value and if the value is 0, that means record was not removed.
  if (removed == 0)
    printf("%s\n", "record not found");
}

// swap records
void swap(struct db_type *pArr[])
{

  // for loop to iterate through every other record
  for (int i = 0; i < SIZE; i += 2)
  {

    // breaks if current index or next index is NULL
    if (pArr[i] == NULL || pArr[i + 1] == NULL)
      break;

    // Creates a temp variable to store the info for record at index i and then switches the records at index i and i+1
    struct db_type *temp = pArr[i];
    pArr[i] = pArr[i + 1];
    pArr[i + 1] = temp;
  }
}

// load from course.txt, store into (global) array of courses
void init_courseArr(void)
{

  FILE *fr; //file pointer
  fr = fopen(courseFile, "r");
  char arr[50];
  int count = 0;
  while ((fgets(arr, 100, fr)) != NULL)
  {
    strncpy(courseArr[count].code, arr, strlen(arr) - 1); // remove \n
    courseArr[count].code[strlen(arr) - 1] = '\0';        //to be safe

    fgets(arr, 100, fr);
    strncpy(courseArr[count].title, arr, strlen(arr) - 1);
    courseArr[count].title[strlen(arr) - 1] = '\0'; //to be safe

    fgets(arr, 100, fr);
    strncpy(courseArr[count].date, arr, strlen(arr) - 1);
    courseArr[count].date[strlen(arr) - 1] = '\0'; //to be safe

    // other ways, remove \n before
    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].time_start, arr);

    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].time_end, arr);

    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].location, arr);

    count++;
    fgets(arr, 100, fr); // read the empty line
  }

  strcpy(courseArr[count].code, "EECS0000"); // a terminator token structure

  fclose(fr);
}

// Write data to storage

void writeDisk(struct db_type *pArr[])
{
  FILE *fp;
  int i;

  if ((fp = fopen(diskFile, "ab")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  for (i = 0; i < SIZE; i++)
  {
    if (pArr[i] != NULL)
    {
      if (fwrite(pArr[i], sizeof(struct db_type), 1, fp) != 1)
      {
        printf("file write error\n");
      }
    }
  }
  fclose(fp);
}

void emptyDisk(void)
{

  FILE *fp;

  char c3[3];
  printf("are you sure to empty disk? (y) or (n)? ");
  fgets(c3, 3, stdin);
  if (strcmp(c3, "y\n"))
    return;

  //else
  if ((fp = fopen(diskFile, "w")) == NULL)
  {
    printf("no file to open\n");
    return;
  }
  fclose(fp); // open close, will empty the file
}

void loadDisk(struct db_type *pArr[])
{
  FILE *fp;
  int i;
  char c3[3];
  printf("will overwrite current records. are you sure to load disk? (y) or (n)? ");
  fgets(c3, 3, stdin);
  if (strcmp(c3, "y\n"))
    return;

  struct db_type *tmp;

  if ((fp = fopen(diskFile, "r")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  init_list(pArr);
  for (i = 0; i < SIZE; i++)
  {

    tmp = (struct db_type *)malloc(sizeof(struct db_type));

    if (fread(tmp, sizeof(struct db_type), 1, fp) != 1)
    {
      if (feof(fp))
      {
        fclose(fp);
        return;
      }
      printf("file read error\n");
    }
    else
      pArr[i] = tmp;
  }
}
