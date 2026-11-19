/**
 * ENGG1110 Problem Solving by Programming
 * Lab 11 Exercise 5 - Inventory Deliveries
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
 * Date         : 19/11/2026
 */

#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTS 20
#define CODE_LENGTH 16

typedef struct {
    char code[CODE_LENGTH];
    int stock;
} Product;

int findProduct(const Product products[], int n, const char target[])
{
    int i;

    for (i = 0; i < n; i++) {
        if (strcmp(products[i].code, target) == 0) {
            return i;
        }
    }

    return -1;
}

int applyDelivery(Product products[], int n, const char target[], int amount)
{
    /* TODO: Find target, increase its stock by amount, and return its new
       stock. Return -1 without changing the array if target is not found. */
    int i = findProduct(products, n, target);
    if (i != -1) {
        products[i].stock += amount;

        return products[i].stock;
    }

    return -1;
}

int calculateTotalStock(const Product products[], int n)
{
    /* TODO: Return the sum of the stock counts of the first n products. */
    int res = 0;
    for (int i = 0; i < n; i++) {
        res += products[i].stock;
    }
    return res;

}

/* DO NOT modify the given main() function. */
int main(void)
{
    Product products[MAX_PRODUCTS];
    char target[CODE_LENGTH];
    int n, m, i, amount, newStock;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%15s %d", products[i].code, &products[i].stock);
    }

    scanf("%d", &m);
    for (i = 0; i < m; i++) {
        scanf("%15s %d", target, &amount);
        newStock = applyDelivery(products, n, target, amount);

        if (newStock == -1) {
            printf("%s: not found\n", target);
        } else {
            printf("%s: %d\n", target, newStock);
        }
    }

    printf("Final inventory:\n");
    for (i = 0; i < n; i++) {
        printf("%s %d\n", products[i].code, products[i].stock);
    }
    printf("Total stock: %d\n", calculateTotalStock(products, n));

    return 0;
}
