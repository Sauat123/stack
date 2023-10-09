#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

const int ROWS = 3;
const int MAX_SYMBOLS = 100;

enum MID_VAL {MID_LEFT = 1,
              MID_RIGHT = 2};

int get_file_size(FILE * fp);
int get_num_of_rows (void * arr, size_t file_size);
int FindLongestLine(const char * data, int rows, int file_size);

int MySort (void * data, int num, int element_size, int (*Compare)(const void * element1, const void * element2));
int Partition (size_t left, size_t right, void * data, int (*Compare)(const void * element1, const void * element2));
int qSort(size_t left, size_t right, void * data, int (*Compare)(const void * element1, const void * element2));

const char * FirstLettersInit(void * data, int file_size, int rows);

int Switching(char ** value1, char ** value2, char * data);

int Compare(const void * element1, const void * element2);

int readTextFromFile(const char* filename, char*** array, int size);

int main()

{
    //const char * ptr[11] = {"BIMBIM", "BANANA", "U=IR", "KENYA", "GOAL", "OVAL", "BAMBAM", "PuTiN", "BUMBUM", "KARKAR", "MEOWMEOW'"};

    FILE * fp = fopen("text1.txt", "rb");

    if(fp == NULL)
    {
        perror("Can't find this file");
    }

    else
    {

        int file_size = get_file_size(fp);
        printf("file size: %d \n", file_size);

        void * data = calloc(file_size, sizeof(char));
        assert(data != NULL);

        fread(data, sizeof(char), file_size, fp);

        int rows = get_num_of_rows(data, file_size);
        printf("amount of rows: %d \n", rows);

        char ** arr = (char**)calloc(sizeof(char), sizeof(char*));

        int lineCount = readTextFromFile("texttext.txt", &arr, rows);

        MySort(arr, 6, sizeof(char), Compare);

        //char * data_ptr = (char*)data;

        for(int i = 0; i < lineCount; i++)
        {
            printf("%s \n", arr[i]);
        }

         /*
        for(int i = 0; i < 11; i++)
        {
            printf("%s \n", ptr[i]);
        }   */


        /*printf("\nSorted array: \n");
        for (int i = 0; i < file_size; i++)
            printf("%d ", data[i]);*/

        fclose(fp);
        /*
        int data[6] = {2, 2, 5, 7, 0, 55};

        int N = sizeof(data) / sizeof(data[0]);
        printf("%d", N);

        printf("Unsorted array: \n");
        for (int i = 0; i < N; i++)
            printf("%d ", data[i]);

        for (int i = 0; i < file_size; i++)
        {
            printf ("strs[%d] = %c \n", i, strs[i]);
        }*/
    }
}

int get_file_size(FILE * fp)
{
    fseek(fp, 0L, SEEK_END);
    int file_size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);
    return file_size;
}

int get_num_of_rows (void * arr, size_t file_size)
{
    int counter = 0;

    char * arr1 = (char*)arr;

    char* current_line = arr1;

    for (size_t i = 0; i < file_size; i++)
    {
        if (arr1[i] == '\n')
        {
            counter++;
        }

        current_line = &arr1[i+1];
    }

    if(current_line < &arr1[file_size])
    {
        counter++;
    }
    return counter;
}

int MySort (void * data, int num, int element_size, int (*Compare)(const void * element1, const void * element2))
{
    int left = 0;
    int right = num / element_size;

    int mid = Partition(left, right - 1, data, Compare);

    if(mid > 0)
    {
        qSort(left, mid, data, Compare);
        qSort(mid + 1, right, data, Compare);
    }

    return 1;
}

int Partition (size_t left, size_t right, void * data1, int (*Compare)(const void * element1, const void * element2))
{
    size_t mid = (left+right)/2;
    size_t left_step = left;
    size_t right_step = right;

    printf("i = %d ", left);
    printf("j = %d ", right);
    printf("mid = %d \n", mid);

    char ** data = (char**)data1;

    printf("data[mid] = %s \n", data[mid]);

    while(left_step < right_step)
    {
            for(; left_step < right_step; left_step++)
            {
                if(Compare(data[left_step], data[mid]) >= 0)
                {
                    printf("FIND TRIG:  %s  >  %s \n", data[left_step], data[mid]);
                    break;
                }
            }

            for(; right_step > left_step; right_step--)
            {
                if(Compare(data[right_step], data[mid]) <= 0)
                {
                    printf("FIND TRIG: data[%d] = %s < data[%d] = %s \n", left_step, data[right_step], mid, data[mid]);
                    break;
                }
            }

            if(Compare(data[left_step], data[right_step]) == 0)
            {
                left_step++;
                continue;
            }

            int switch_value = Switching(&data[left_step], &data[right_step], data[mid]);

            switch(switch_value){

            case 0:
                break;
            case 1:
                mid = left_step;
                break;
            case 2:
                mid = right_step;
                break;
            default:
                printf("error in switch");
            }
            printf("MID = %d", mid);

    }

    return right_step;

}

int Switching(char ** value1, char ** value2, char * data)
{
    printf("BEFORE SWOP: Value 1 = %s  Value 2 = %s \n", *value1, *value2);
    char * temp = *value1;

    *value1 = *value2;
    *value2 = temp;
    printf("AFTER SWOP: Value 1 = %s  Value 2 = %s \n", *value1, *value2);

    if(strcmp(data, *value2) == 0)
    {
        return MID_RIGHT;
    }
    if(strcmp(data, *value1) == 0)
    {
        return MID_LEFT;
    }

    return 0;
}

int Compare(const void * element1, const void * element2)
{
    const char * first = (const char*)element1;
    const char * second = (const char*)element2;

    int check = strcmp(first, second);

    printf("\n\nIN COMPARE - First = %s || Second = %s\n", first, second);

    printf("STRCMP value = %d \n", check);

    return strcmp(first, second);
}

int qSort(size_t left, size_t right, void * data, int (*Compare)(const void * element1, const void * element2))
{

    if (left == right)
    {
        return 0;
    }

    int mid = Partition(left, right - 1, data, Compare);

    if(mid > 0)
    {
        qSort(left, mid, data, Compare);
        qSort(mid + 1, right, data, Compare);
    }

    return 1;
}
/*
const char * FirstLettersInit(void * data, int file_size, int rows)
{
    const char * first_letter_ptr = (const char **)first_letter;
    const char * data_ptr = (const char *)data;

    int longest_line = FindLongestLine(data_ptr, rows, file_size);

    for(int i = 0; i < rows; i++)
    {
        first_letter_ptr[i] = (char*)calloc(longest_line, sizeof(char));
    }

    int index = 0;
    int check = 0;

    for(int i = 0; i < file_size; i++)
    {

        if(data_ptr[i] == '\n')
        {
            while((check = isalpha(data_ptr[i])) < 1)
            {
                i++;
                check = isalpha(data_ptr[i]);
            }

            first_letter_ptr = &data_ptr[i];
            //printf("first letter[%d] = %c \n", index, *first_letter_ptr[index]);
            index++;
        }
        continue;
    }

    return first_letter_ptr;
}
*/
int FindLongestLine(const char * data, int rows, int file_size)
{
    int longest_line = 0;
    int counter = 0;
    for(int j = 0; j < rows; j++)
    {
        for(int i = 0; i < file_size; i++)
        {
            if(data[i] == '\n')
            {
                break;
            }
            counter++;
        }

        if(counter > longest_line)
        {
            longest_line = counter;
        }
    }
    return longest_line;
}
/*
char ** ReadTextFromFile (FILE * fp)
{
    if(fp == NULL)
    {
        fprintf(stderr, "can't find this file");
    }

    char ** data = calloc(sizeof(char), sizeof(short));

    if(data == NULL)
    {
        fprintf(stderr, "data pointer is NULL");
    }

    int num_lines = f();

    for(int i = 0; i < num_lines; i++)
    {
        data[i] = (char*)calloc(max_length, sizeof(char));
    }


}  */

int readTextFromFile(const char* filename, char*** array, int size)
{
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(EXIT_FAILURE);
    }


    *array = (char**) calloc(size, sizeof(char*));

    if (*array == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    int lineCount = 0;  // Variable to keep track of the number of lines read.

    // Read lines from the file until the end is reached or the maximum size is reached.
    while (lineCount < size && !feof(file)) {
        (*array)[lineCount] = (char*)calloc(256, sizeof(char));  // Assuming a maximum line length of 255 characters.

        if ((*array)[lineCount] == NULL)
        {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        fgets((*array)[lineCount], 256, file);  // Read a line from the file.

        // Remove the newline character at the end of the line, if present.
        int length = strlen((*array)[lineCount]);
        if ((*array)[lineCount][length - 1] == '\n') {
            (*array)[lineCount][length - 1] = '\0';
        }

        lineCount++;
    }

    fclose(file);  // Close the file.

    return lineCount;  // Return the number of lines read.
}

