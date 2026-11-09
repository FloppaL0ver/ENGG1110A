/**
 * ENGG1110 Problem Solving by Programming 
 *
 * I declare that the assignment here submitted is original
 * except for source material explicitly acknowledged,
 * and that the same or closely related material has not been
 * previously submitted for another course.
 * I also acknowledge that I am aware of University policy and
 * regulations on honesty in academic work, and of the disciplinary
 * guidelines and procedures applicable to breaches of such
 * policy and regulations, as contained in the website.
 *
 * University Guideline on Academic Honesty:
 *   http://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name : Paisal, Pavat Zhang
 * Student ID   : 1155280121
 * Class/Section: A
 * Date         : 9/11/2026
 */
 
 // Start coding here
#include <stdio.h>

int main(void) {
    int start, end;
    printf("Start? ");
    scanf("%d", &start);
    printf("End? ");
    scanf("%d", &end);

    FILE *fptr = fopen("numbers.txt", "r");
    if (!fptr) {
        return 1;
    }

    int times, num;
    for (int i = 1; i < start; i++) {
        fscanf(fptr, "%d%d", &times, &num);
    }
    for (int i = start; i <= end; i++) {
        fscanf(fptr, "%d%d", &times, &num);
        printf("%d", num);
        for (int j = 1; j < times; j++) {
            printf(" %d", num);
        }
        putchar('\n');
    }

    fclose(fptr);
    return 0;
}