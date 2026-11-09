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
 * http://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name : Paisal, Pavat Zhang
 * Student ID   : 1155280121
 * Class/Section: A
 * Date         : 9/11/2026
 */

#include <stdio.h>

int checkSymmetricMatrix( int matrix[16][16],  int size ){
  int isSymmetric = 1; // assume iSymmetric is True first
  // check whether the matrix is symmetric
  // matrix is symmetric if the transpose of it is the same as itself (transpose is swapping rows and columns)
  int transpose[16][16];
  for (int i = 0; i < size; i++) {
    for (int j =0; j< size; j++) {
      transpose[i][j] = matrix[j][i];
    }
  }

  for (int i = 0; i < size; i++) {
    for (int j =0; j< size; j++) {
      if (transpose[i][j] == matrix[i][j])
        continue;
      return 0;
    }
  }
  return 1;
  // return 1 if it is symmetric; otherwise return 0

  return isSymmetric;
}

int main(void){
  int n;
  int matrixRead[16][16];
  // read matrix size and elements from user input via nested for loop
  scanf("%d", &n);
  for (int i = 0; i< n; i++) {
    for (int j =0; j <n; j++) {
      scanf("%d", &matrixRead[i][j]);
    }
  }

  // call function - checkSymmetricMatrix
  int res = checkSymmetricMatrix(matrixRead, n);

  // print your checking result
  if (res)
    printf("It is a symmetric matrix!");
  else
    printf("It is not a symmetric matrix!");

  return 0;
}