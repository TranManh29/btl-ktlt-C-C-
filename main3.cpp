
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_PRODUCTS 100



FILE* readfile;
FILE* writefile;


bool startMenu = false;

typedef struct {
    char name1[50];
    char name2[50];
    char name3[50];
    char name4[50];
} Identity;

void Identity_init(Identity* identity, const char* name1, const char* name2, const char* name3, const char* name4) {
    
    printf("\n=================================================\n");
    printf("|\tRESTAURANT INFORMATION SYSTEM\t\t|\n");
    printf("|\t\t\t\t\t\t|\n|\tBY %s (20224443)\t\t|\n", name1);
    printf("|\t\t\t\t\t\t|\n|\tAND %s (20224429)\t\t|\n", name2);
    printf("|\t\t\t\t\t\t|\n|\tAND %s (20224408)\t\t|\n", name3);
    printf("|\t\t\t\t\t\t|\n|\tAND %s (20224455)\t\t|\n", name4);
}

void Identity_init_group10() {
    printf("|\t\t\t\t\t\t|\n|\tGROUP 10\t\t\t\t|\n");
    printf("=================================================\n");
}

struct Option {
    char productName[100];
    char choice[100];
    long int productPrice;
    long int totalPrice;
    int qt;
};

typedef struct Food {
    char name[100];
    long int price;
    struct Food *next;
    struct Food *head ;
    struct Food *tail;
    struct Food *temp;
    struct Food *now;
    struct Food *del;
    struct Food *show;
    struct Option choose[MAX_PRODUCTS];
} Food;

void initializeFood(Food *food) {
    food->head = NULL;
    food->tail = NULL;
}
void addFood(Food *food, const char *name, long int price, bool notNew) {
    Food *temp = (Food *)malloc(sizeof(Food));
    strcpy(temp->name, name);
    temp->price = price;
    temp->next = NULL;

    if (!notNew) {
        FILE *writefile = fopen("product.txt", "a");
        if (writefile == NULL) {
            perror("Failed to open file");
            free(temp);
            return;
        }
        fprintf(writefile, "%s, %ld\n", temp->name, temp->price);
        fclose(writefile);
    }

    if (food->head == NULL) {
        food->head = temp;
        food->tail = temp;
    } else {
        food->tail->next = temp;
        food->tail = temp;
    }
}

void begin(Food *food) {
    char line[256];
    FILE *readfile = fopen("product.txt", "r");
    if (readfile == NULL) {
        perror("Failed to open file");
        return;
    }
    while (fgets(line, sizeof(line), readfile)) {
        char name[100];
        long int price;
        sscanf(line, "%[^,], %ld", name, &price);
        addFood(food, name, price, true);
    }
    fclose(readfile);
}




void renewFood(Food *food) {
    food->temp = food->head;
    remove("product.txt");
    FILE *writefile = fopen("product.txt", "a");
    if (writefile == NULL) {
        perror("Failed to open file");
        return;
    }
    while (food->temp != NULL) {
        fprintf(writefile, "%s, %ld\n", food->temp->name, food->temp->price);
        food->temp = food->temp->next;
    }
    fclose(writefile);
}

void updatePriceFood(Food *food, const char *name) {
    long int newPrice;
    food->temp = food->head;
    bool isUpdate = false;
    while (food->temp != NULL) {
        if (strcmp(food->temp->name, name) == 0) {
            printf("\n[INFO] Price %s previously was: %ld\n", food->temp->name, food->temp->price);
            printf("\n[UPDATE PRODUCT] Input new price: ");
            scanf("%ld", &newPrice);
            food->temp->price = newPrice;
            isUpdate = true;
        }
        food->temp = food->temp->next;
    }
    if (isUpdate) {
        renewFood(food);
    }
}

void deleteFood(Food *food, const char *name) {
    int index = 0;
    food->now = food->head;
    while (food->now != NULL) {
        if (index == 0 && strcmp(food->now->name, name) == 0) {
            food->del = food->head;
            food->head = food->head->next;
            free(food->del);
        } else if (index != 0 && strcmp(food->now->name, name) == 0) {
            food->temp->next = food->now->next;
            free(food->now);
            break;
        } else {
            food->temp = food->now;
        }
        food->now = food->now->next;
        index++;
    }
    renewFood(food);
}

int showFood(Food *food) {
    int i = 0;
    food->temp = food->head;
    printf("\n-------------------------------------------------\n");
    printf("|\t\t  PRODUCT LIST  \t\t|\n");
    printf("-------------------------------------------------\n");
    printf("|   NO \t|  MENU\t\t\t|  PRICE\t|\n");
    printf("-------------------------------------------------\n");
    while (food->temp != NULL) {
        printf("|   %d \t|   %s \t\t|  %ld \t|\n", i + 1, food->temp->name, food->temp->price);
        i++;
        food->temp = food->temp->next;
    }
    printf("-------------------------------------------------\n");
    return 0;
}
// Định nghĩa cấu trúc Counts
typedef struct {
    int value;
} Counts;

// Khởi tạo hàm tăng giá trị
    void increment(Counts *counts) {
        counts->value++;
    }   

// Khởi tạo hàm hiển thị
    void display(const Counts *counts) {
        printf("\n===================================================\n");
        printf("|\t You have logged in incorrectly : %d\t  |\n", counts->value);
        printf("===================================================\n");
    }

typedef struct Menu {
    Food *head;
    Food *tail;
    Food *next, *temp, *now, *del, *show;
    int option;
    int length;
    char nameProduct[100];
    char choiceUser[100];
    long int total;
    long int price;
    long int cash;
    struct Option choose[MAX_PRODUCTS];
    bool isStop;
} Menu;

void initializeFood(Menu *menu) {
    menu->head = NULL;
    menu->tail = NULL;
}

void begin(Menu *menu) {
    char line[256];
    FILE *readfile = fopen("product.txt", "r");
    if (readfile == NULL) {
        perror("Failed to open file");
        return;
    }
    while (fgets(line, sizeof(line), readfile)) {
        char name[100];
        long int price;
        sscanf(line, "%[^,], %ld", name, &price);
        Food *temp = (Food *)malloc(sizeof(Food));
        strcpy(temp->name, name);
        temp->price = price;
        temp->next = NULL;

        if (menu->head == NULL) {
            menu->head = temp;
            menu->tail = temp;
        } else {
            menu->tail->next = temp;
            menu->tail = temp;
        }
    }
    fclose(readfile);
}

void addFood(Menu *menu, const char *name, long int price, bool notNew) {
    Food *temp = (Food *)malloc(sizeof(Food));
    strcpy(temp->name, name);
    temp->price = price;
    temp->next = NULL;

    if (!notNew) {
        FILE *writefile = fopen("product.txt", "a");
        if (writefile == NULL) {
            perror("Failed to open file");
            free(temp);
            return;
        }
        fprintf(writefile, "%s, %ld\n", temp->name, temp->price);
        fclose(writefile);
    }

    if (menu->head == NULL) {
        menu->head = temp;
        menu->tail = temp;
    } else {
        menu->tail->next = temp;
        menu->tail = temp;
    }
}

void renewFood(Menu *menu) {
    Food *temp = menu->head;
    remove("product.txt");
    FILE *writefile = fopen("product.txt", "a");
    if (writefile == NULL) {
        perror("Failed to open file");
        return;
    }
    while (temp != NULL) {
        fprintf(writefile, "%s, %ld\n", temp->name, temp->price);
        temp = temp->next;
    }
    fclose(writefile);
}

void updatePriceFood(Menu *menu, const char *name) {
    long int newPrice;
    Food *temp = menu->head;
    bool isUpdate = false;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("\n[INFO] Price %s previously was: %ld\n", temp->name, temp->price);
            printf("\n[UPDATE PRODUCT] Input new price: ");
            scanf("%ld", &newPrice);
            temp->price = newPrice;
            isUpdate = true;
        }
        temp = temp->next;
    }
    if (isUpdate) {
        renewFood(menu);
    }
}

void deleteFood(Menu *menu, const char *name) {
    Food *temp = menu->head;
    Food *prev = NULL;
    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        return;
    }
    if (prev == NULL) {
        menu->head = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
    renewFood(menu);
}

int showFood(Menu *menu) {
    FILE *file = fopen("product.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Food *newNode = (Food *)malloc(sizeof(Food));
        if (newNode == NULL) {
            perror("Error allocating memory");
            fclose(file);
            return -1;
        }

        sscanf(line, "%[^,], %ld", newNode->name, &newNode->price);
        newNode->next = NULL;

        if (menu->head == NULL) {
            menu->head = newNode;
        } else {
            Food *last = menu->head;
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = newNode;
        }
    }

    fclose(file);

    // Hiển thị danh sách sản phẩm
    int i = 0;
    Food *show = menu->head;
    printf("\n-------------------------------------------------\n");
    printf("|\t\t  PRODUCT LIST  \t\t|\n");
    printf("-------------------------------------------------\n");
    printf("|   NO \t|  MENU\t\t\t|  PRICE\t|\n");
    printf("-------------------------------------------------\n");
    while (show != NULL) {
        printf("|   %d \t|   %s \t\t|  %ld \t|\n", i + 1, show->name, show->price);
        i++;
        show = show->next;
    }
    printf("-------------------------------------------------\n");

    return i;
}


void invoice(Menu *menu) {
    printf("\n=================================================\n");
    printf("|\t    [INFO] Displays Bills    \t|");
    printf("\n=================================================\n");
    printf("\n=================================================\n");
    printf("|\t\t    INVOICE\t\t\t|");
    printf("\n=================================================\n");
    printf("\n-------------------------------------------------\n");
    printf("|   NO \t|  MENU\t\t\t|  PRICE\t|\n");
    printf("-------------------------------------------------\n");
    for (int a = 0; a < menu->length; a++) {
        menu->choose[a].totalPrice = menu->choose[a].productPrice * menu->choose[a].qt;
        menu->total += menu->choose[a].totalPrice;
        printf("|   %d \t|   %s \t\t|  %ld \t|\n", a + 1, menu->choose[a].productName, menu->choose[a].totalPrice);
    }
    printf("-------------------------------------------------\n");
    printf("|\t  TOTAL AMOUNT  \t|  %ld  \t|\n", menu->total);
    printf("-------------------------------------------------\n");
}

void choiceProduct(Menu *menu) {
    menu->isStop = false;
    while (menu->isStop == false) {
        printf("\n[ORDER] Write down the desired food/drink (ex: ga_ran): ");
        scanf("%s", menu->choose[menu->length].choice);
        printf("[ORDER] Enter Portion/Quantity (ex: 2): ");
        scanf("%d", &menu->choose[menu->length].qt);

        printf("\n[ORDER] Want to Order Other Food/Drinks [y/n]? : ");
        scanf("%s", menu->choiceUser);

        FILE *readfile = fopen("product.txt", "r");
        if (readfile == NULL) {
            perror("Failed to open file");
            return;
        }
        char line[256];
        while (fgets(line, sizeof(line), readfile)) {
            char name[100];
            long int prices;
            sscanf(line, "%[^,], %ld", name, &prices);

            if (strcmp(menu->choose[menu->length].choice, name) == 0) {
                strcpy(menu->choose[menu->length].productName, name);
                menu->choose[menu->length].productPrice = prices;
            }
        }
        fclose(readfile);

        if (strcmp(menu->choiceUser, "y") == 0 || strcmp(menu->choiceUser, "Y") == 0) {
            menu->isStop = false;
        } else {
            menu->isStop = true;
        }
        menu->length++;
    }
    invoice(menu);
}

void menuOrder(Menu *menu) {
begin:
    system("clear"); // For Unix/Linux systems, use "cls" for Windows
    printf("\n=================================================\n");
    printf("|\t\tMAKE AN ORDER\t\t\t|");
    printf("\n=================================================\n");
    printf("\n=================================================\n");
    printf("|\tSHOWING AVAILABLE PRODUCTS...\t|");
    printf("\n=================================================\n");
    int many = showFood(menu);

    if (many == 0) {
        printf("\n=====================================================================\n");
        printf("|  THERE IS NO FOOD MENU! PLEASE ADD FOOD TO THE ADMIN MENU  |");
        printf("\n=====================================================================\n");
        return;
    } else {
        choiceProduct(menu);
    pay:
        printf("\n=================================================\n");
        printf("|\t\tPAYMENT SUMMARY\t\t\t|");
        printf("\n=================================================\n");
        printf("[PAYMENT] Enter the CASH that you have \t: ");
        scanf("%ld", &menu->cash);

        if (menu->total == 0) {
            printf("\n=========================================================\n");
            printf("[PAYMENT] You are not currently ordering any food/drink menu!\n");
            printf("[PAYMENT] Please select MAKE ORDER on the Admin Menu");
            printf("\n=========================================================\n");
            return;
        } else if (menu->cash < menu->total) {
            printf("\n=========================================================================\n");
            printf("|\t\t[INFO] CASH is not enough to pay for the order!\t\t|");
            printf("\n=========================================================================\n");

            printf("\n=========================================================================\n");
            printf("|\t[INFO] CASH input is more than or equal to the total price!\t|");
            printf("\n=========================================================================\n");
            printf("\n[PAYMENT] Do You Want to Cancel the Order [Yes (y)/No (n)]? : ");
            scanf("%s", menu->choiceUser);

            if (strcmp(menu->choiceUser, "y") == 0 || strcmp(menu->choiceUser, "Y") == 0) {
                system("clear"); // For Unix/Linux systems, use "cls" for Windows
                printf("\n=======================================================\n");
                printf("| [INFO] Order Canceled! Return to MAIN MENU  |");
                printf("\n=======================================================\n");
                menu->total = 0;
                menu->length = 0;
                return;
            } else {
                menu->total = 0;
                invoice(menu);
                goto pay;
            }
        } else if (menu->cash >= menu->total) {
            printf("[PAYMENT] Cashback \t\t\t: %ld", menu->cash - menu->total);
            printf("\n\n[PAYMENT] PAYMENT SUCCESSFUL!\n");
            printf("\n=========================================================\n");
            printf("|\t\t\tTHANK YOU\t\t\t|");
            printf("\n=========================================================\n");
            menu->total = 0;
            menu->length = 0;
            printf("\n[PAYMENT] Do You Want to Go Back to the MAIN MENU[y/n]? : ");
            scanf("%s", menu->choiceUser);

            if (strcmp(menu->choiceUser, "y") == 0 || strcmp(menu->choiceUser, "Y") == 0) {
                system("clear"); // For Unix/Linux systems, use "cls" for Windows
                printf("\n=======================================================\n");
                printf("|  [INFO] Order Canceled! Return to MAIN MENU   |");
                printf("\n=======================================================\n");
                return;
            } else {
                printf("\n=========================================================\n");
                printf("\n\nEXIT PROGRAM...");
                exit(0);
            }
        }
    }
}

void menuAdmin(Menu *menu) {
begin:
    printf("\n=========================================\n");
    printf("|\t\tADMIN MENU\t\t|");
    printf("\n=========================================\n");
    printf("| 1. | \t\tShow Product\t\t|\n| 2. | \t\tAdd Product\t\t|\n| 3. | \t\tUpdate price\t\t|\n| 4. | \t\tDelete Product\t\t|\n| 5. | \t\tExit Program\t\t|");
    printf("\n=========================================\n");
    printf("\nYour choice: \n> ");
    scanf("%d", &menu->option);

    switch (menu->option) {
        case 1:
            system("clear"); // For Unix/Linux systems, use "cls" for Windows
            printf("\n=================================================================\n");
            printf("|\t[INFO] Choose Option 1! Displaying Products/Food\t|");
            printf("\n=================================================================\n");
            showFood(menu);
            goto begin;
            break;

        case 2:
            system("clear"); // For Unix/Linux systems, use "cls" for Windows
            printf("\n=================================================================\n");
            printf("|\t[INFO] Choose Option 2! Displaying Products/Food\t|");
            printf("\n=================================================================\n");
            showFood(menu);
            printf("\n[ADD PRODUCT] Input New Food/Drink Name (ex: ga ran): ");
            scanf("%s", menu->nameProduct);
            printf("\n[ADD PRODUCT]Input New Food/Drink Prices (ex: 3000) : ");
            scanf("%ld", &menu->price);
            addFood(menu, menu->nameProduct, menu->price, false);
            printf("\n[ADD PRODUCT]Displaying the Latest Food/Drink List..\n");
            showFood(menu);
            goto begin;
            break;

        case 3:
            system("clear"); // For Unix/Linux systems, use "cls" for Windows
            printf("\n=================================================================\n");
            printf("|\t[INFO] Choose Option 3! Displaying Products/Food\t|");
            printf("\n=================================================================\n");
            showFood(menu);
            printf("\n[UPDATE PRODUCT]Input the name of the food/drink whose price will be updated (ex: ga ran) : ");
            scanf("%s", menu->nameProduct);
            updatePriceFood(menu, menu->nameProduct);
            printf("\n[UPDATE PRODUCT] Displaying the Latest Food/Drink List..\n");
            showFood(menu);
            goto begin;
            break;

        case 4:
            system("clear"); // For Unix/Linux systems, use "cls" for Windows
            printf("\n=================================================================\n");
            printf("|\t[INFO]Choose Option 4! Displaying Products/Food\t|");
            printf("\n=================================================================\n");
            showFood(menu);
            printf("\n[DELETE PRODUCT] Input the name of the food/drink to be deleted (ex: ga ran): ");
            scanf("%s", menu->nameProduct);
            deleteFood(menu, menu->nameProduct);
            printf("\n[DELETE PRODUCT] Displaying the Latest Food/Drink List..\n");
            showFood(menu);
            goto begin;
            break;

        case 5:
            system("clear"); // For Unix/Linux systems, use "cls" for Windows
            printf("\n=================================================================\n");
            printf("|\t[INFO] Choose Option 5! Displays the exit program \t\t|");
            printf("\n=================================================================\n");
            return;
            break;

        default:
            printf("\nchoice %d There isn't any! Please Select/Type 1, 2, 3, 4, 5, or 6!\n", menu->option);
            printf("\n======================================================\n");
            goto begin;
            break;
    }
}

void mainMenu(Menu *menu) {
begin:
    printf("\n=========================================\n");
    printf("|\t\tMAIN MENU\t\t|");
    printf("\n=========================================\n");
    printf("|  1. | \t Order\t\t\t|\n|  2. | \tAdmin Mode\t\t|\n|  3. | \tExit Program\t\t|");
    printf("\n=========================================\n");
    printf("\nYour  Choice \n> ");
    scanf("%d", &menu->option);

    switch (menu->option) {
        case 1:
            menuOrder(menu);
            break;

        case 2:
            system("clear"); // For Unix/Linux systems, use "cls" for Windows
            printf("\n=================================================================\n");
            printf("|\t[INFO] You Choose 2! Directed to Menu Admin\t\t|");
            printf("\n=================================================================\n");
            menuAdmin(menu);
            break;

        case 3:
            printf("\n\nEXIT PROGRAM...");
            exit(0);
            break;

        default:
            printf("\nChoice %d There isn't any! Please Select/Type 1, 2, 3, 4, 5, or 6!\n", menu->option);
            printf("\n======================================================\n");
            goto begin;
            break;
    }
}

struct User {
    char name[100];
    char pass[100];
    char line[256];
    int result; // Biến thành viên để lưu kết quả đăng nhập

    // Khai báo hàm void registers và int login
    void registers();
    int login(const char *username, const char *password); // Hàm login sử dụng thông tin người dùng từ tham số
};

// Định nghĩa hàm registers
void User::registers() {
    int isRegistered = 0;

    FILE *readfile = fopen("dataUser.txt", "r");
    while (fgets(line, sizeof(line), readfile)) {
        char username[100], password[100];
        sscanf(line, "%[^,], %[^\n]", username, password);

        if (strcmp(username, name) == 0 && strcmp(password, pass) == 0) {
            printf("\n===================================================\n");
            printf("|\t\t\t\t\t\t  |\n|   [WARN!] Username has been registered previously!  |\n");
            printf("|\t\t\t\t\t\t  |\n===================================================\n");
            isRegistered = 1;
            break;
        }
    }
    fclose(readfile);

    if (!isRegistered) {
        FILE *writefile = fopen("dataUser.txt", "a");
        fprintf(writefile, "%s, %s\n", name, pass);
        fclose(writefile);
        printf("\n==========================================================================\n");
        printf("|\t\t\t\t\t\t\t\t\t |\n|   [INFO] Successful REGISTER New User! Please Continue Login (Choose 2)!   |\n");
        printf("|\t\t\t\t\t\t\t\t\t |\n==========================================================================\n");
    }
}

// Định nghĩa hàm login
int User::login(const char *username, const char *password) {
    bool isLogin = false;
    FILE *readfile = fopen("dataUser.txt", "r");
    while (fgets(line, sizeof(line), readfile)) {
        char storedUsername[100], storedPassword[100];
        sscanf(line, "%[^,], %[^\n]", storedUsername, storedPassword);

        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, password) == 0) {
            printf("\n==================================================================\n");
            printf("|\t\t\t\t\t\t\t\t |\n|   [INFO] USER LOGIN Successful! Redirected to MAIN MENU Page!\t |\n");
            printf("|\t\t\t\t\t\t\t\t |\n==================================================================\n");
            isLogin = true;
            break;
        }
    }
    fclose(readfile);

    if (!isLogin) {
        printf("\n===================================================\n");
        printf("|\t\t\t\t\t\t  |\n|   [WARN!] Username & Password Not Registered!  |\n");
        printf("|\t\t\t\t\t\t  |\n===================================================\n");
    }

    this->result = isLogin == true ? 1 : 0; // Lưu kết quả vào biến thành viên result
    return this->result;
}


int main() {
    int option, resultLogin ;
    Counts cou, counts ;
    Menu menu;
    struct User user;
    Identity identity;
   
    char username[100], password[100]; 
    counts.value = 0;
    

    Identity_init(&identity, "TRAN_DUC_MANH", "TRAN_QUANG_ANH", "PHAM_VIET_HA", "VU_MINH_QUAN");
    Identity_init_group10();
    begin:
    printf("\n---------------------------------\n");
    printf("|\t  HOME MENU\t\t|\n");
    printf("---------------------------------\n");
    printf("|  1. REGISTER\t\t\t|\n|  2. LOG IN\t\t\t|\n|  3. EXIT PROGRAM\t\t|\n");
    printf("---------------------------------\n");
    printf("\nChoose the Menu Above (1, 2, or 3)!\n> ");
    scanf("%d", &option);
    getchar(); // Consume newline character after option input

    switch (option) {
        case 1:
            printf("\n=========================================\n");
            printf("|\t Choose 1 : REGISTER\t\t|\n");
            printf("=========================================\n");
            printf("\n[REGISTER] Input Username: ");
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = 0; // Remove trailing newline
            printf("[REGISTER] Input Password: ");
            fgets(user.pass, sizeof(user.pass), stdin);
            user.pass[strcspn(user.pass, "\n")] = 0; // Remove trailing newline

            user.registers();
            goto begin;
            break;

        case 2:
            printf("\n=========================================\n");
            printf("|\t choose 2 : LOGIN\t\t|\n");
            printf("=========================================\n");
            printf("\n[LOGIN] Input Username: ");
             fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0'; // Remove trailing newline
            printf("[LOGIN] Input Password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0'; // Remove trailing newline

            resultLogin = user.login(username, password);
            if (resultLogin == 1) {
                startMenu = true;
                mainMenu(&menu);
            } else {
                ++cou.value;
                display(&counts);
                if (cou.value >= 5) {
                    printf("\nTOO MANY LOGIN ATTEMPT, PROGRAM STOPS...\n");
                } else {
                    goto begin;
                }
            }
            break;

        case 3:
            printf("\n=========================================\n");
            printf("|\t Choose 3 : EXIT\t\t|\n");
            printf("=========================================\n");
            printf("\nPROGRAM STOP...\n");
            break;

        default:
            printf("\n=========================================\n");
            printf("|\t Choose %d : NOT FOUND\t\t|\n", option);
            printf("=========================================\n");
            printf("\nChoice %d There isn't any! Please Select/Type 1, 2, or 3!\n", option);
            printf("\n=======================================================\n");
            goto begin;
            break;
    }

    return 0;
}
