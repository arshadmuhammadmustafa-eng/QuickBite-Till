/*
 * File:            quickbite_till.c
 * Name:            [Mustafa]
 * Registration No: [ 1025037]
 * Date:            19/11/2026
 * Purpose:         QuickBite Till - calculates a customer bill for
 *                  QuickBite Kitchen (menu, VAT, discount, receipt).
 */

#include <stdio.h>

#define NUM_ITEMS           5       /* number of items on the menu          */
#define FINISH_CHOICE       0       /* menu choice that ends the order      */
#define MIN_QUANTITY        1       /* smallest quantity allowed            */
#define MAX_QUANTITY        10      /* largest quantity allowed             */
#define VAT_RATE            0.05f   /* 5% VAT                               */
#define DISCOUNT_RATE       0.10f   /* 10% discount                         */
#define DISCOUNT_THRESHOLD  60.00f  /* subtotal (AED) needed for discount   */

/* Menu data: item number 1 is stored at index 0, and so on. */
const char *item_names[NUM_ITEMS]  = {"Chicken Shawarma", "Beef Burger",
                                      "Falafel Wrap", "French Fries",
                                      "Fresh Juice"};
const float item_prices[NUM_ITEMS] = {18.00f, 22.00f, 12.00f, 8.00f, 10.00f};

/* Throws away any unread characters left on the current input line. */
void clear_input_buffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

/* Displays the numbered menu with prices in AED (R1). */
void show_menu(void)
{
    int i;

    printf("========== QUICKBITE KITCHEN MENU ==========\n");
    for (i = 0; i < NUM_ITEMS; i++)
    {
        printf("  %d. %-18s AED %6.2f\n", i + 1, item_names[i], item_prices[i]);
    }
    printf("  %d. Finish order and print bill\n", FINISH_CHOICE);
    printf("============================================\n");
}

/* Asks for a menu choice until a valid one is entered (R5). */
int get_choice(void)
{
    int choice = -1;
    int valid = 0;

    while (!valid)
    {
        printf("Enter item number (1-%d) or %d to finish: ", NUM_ITEMS, FINISH_CHOICE);
        if (scanf("%d", &choice) != 1)
        {
            if (feof(stdin))
            {
                return FINISH_CHOICE;
            }
            printf("Invalid input. Please enter a number from the menu.\n");
        }
        else if (choice < FINISH_CHOICE || choice > NUM_ITEMS)
        {
            printf("Invalid menu choice. Please choose 0-%d.\n", NUM_ITEMS);
        }
        else
        {
            valid = 1;
        }
        clear_input_buffer();
    }
    return choice;
}

/* Asks for a quantity until a value from 1 to 10 is entered (R5). */
int get_quantity(void)
{
    int quantity = 0;
    int valid = 0;

    while (!valid)
    {
        printf("Enter quantity (%d-%d): ", MIN_QUANTITY, MAX_QUANTITY);
        if (scanf("%d", &quantity) != 1)
        {
            if (feof(stdin))
            {
                return MIN_QUANTITY;
            }
            printf("Invalid input. Quantity must be a number.\n");
        }
        else if (quantity < MIN_QUANTITY || quantity > MAX_QUANTITY)
        {
            printf("Invalid quantity. Please enter %d to %d.\n", MIN_QUANTITY, MAX_QUANTITY);
        }
        else
        {
            valid = 1;
        }
        clear_input_buffer();
    }
    return quantity;
}

/* Returns the price of the menu item chosen (choice is 1 to NUM_ITEMS). */
float get_item_price(int choice)
{
    return item_prices[choice - 1];
}

/* Returns the 10% discount if the subtotal is AED 60 or more, else 0 (R4). */
float calc_discount(float subtotal)
{
    if (subtotal >= DISCOUNT_THRESHOLD)
    {
        return subtotal * DISCOUNT_RATE;
    }
    return 0.0f;
}

/* Returns the 5% VAT on the given amount (R3). */
float calc_vat(float amount)
{
    return amount * VAT_RATE;
}

/* Returns the final total: discounted amount plus VAT (R3, R4). */
float calc_total(float discounted_amount, float vat)
{
    return discounted_amount + vat;
}

/* Prints the final bill with every amount to two decimal places. */
void print_receipt(float subtotal, float discount, float discounted_amount,
                   float vat, float total)
{
    printf("\n============== QUICKBITE BILL ==============\n");
    printf("Subtotal:                  AED %8.2f\n", subtotal);
    printf("Discount (10%%):            AED %8.2f\n", discount);
    printf("Discounted amount:         AED %8.2f\n", discounted_amount);
    printf("VAT (5%%):                  AED %8.2f\n", vat);
    printf("--------------------------------------------\n");
    printf("TOTAL TO PAY:              AED %8.2f\n", total);
    printf("============================================\n");
    printf("Thank you for choosing QuickBite Kitchen!\n");
}

/* Main program: takes the order, then calculates and prints the bill. */
int main(void)
{
    int choice;
    int quantity;
    float subtotal = 0.0f;
    float discount;
    float discounted_amount;
    float vat;
    float total;

    show_menu();
    choice = get_choice();

    while (choice != FINISH_CHOICE)
    {
        quantity = get_quantity();
        subtotal = subtotal + get_item_price(choice) * quantity;
        printf("Added: %d x %s\n", quantity, item_names[choice - 1]);
        printf("Running subtotal: AED %.2f\n\n", subtotal);
        choice = get_choice();
    }

    if (subtotal <= 0.0f)
    {
        printf("No items were ordered. No bill to print.\n");
        return 0;
    }

    discount = calc_discount(subtotal);
    discounted_amount = subtotal - discount;
    vat = calc_vat(discounted_amount);
    total = calc_total(discounted_amount, vat);
    print_receipt(subtotal, discount, discounted_amount, vat, total);

    return 0;
}
