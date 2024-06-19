    #include <fstream>
    #include <iostream>
    #include <regex>

using namespace std;

ifstream readfile;
ofstream writefile;

regex re{R"(([^,]*), ([^,]*))"};
smatch m;
bool startMenu = false;

class Identity {
   public:
    Identity(string name1, string name2, string name3 , string name4) {
        system("cls");
        cout << "\n=================================================\n";
        cout << "|\tRESTAURANT INFORMATION SYSTEM\t\t|\n";
        cout << "|\t\t\t\t\t\t|\n|\tBY " << name1 << " (20224443)\t\t|\n";
        cout << "|\t\t\t\t\t\t|\n|\tAND " << name2 << " (20224429)\t\t|\n";
        cout << "|\t\t\t\t\t\t|\n|\tAND " << name3 << " (20224408)\t\t|\n";
        cout << "|\t\t\t\t\t\t|\n|\tAND " << name4 << " (20224455)\t\t|\n";

    }

    Identity(int kel) {
        cout << "|\t\t\t\t\t\t|\n|\tGROUP 10"  << "\t\t\t\t|\n";
        cout << "=================================================\n";
    }
};

class food {
   public:
    string name, line;
    long int price;
    struct option {
        string productName, choice;
        long int productPrice, totalPrice;
        int qt;
    };
    struct option choose[100];

    food *next, *head, *tail, *temp, *now, *del, *show;
    food() {
        head = NULL;
        tail = NULL;
    }

    void begin() {
        int i = 0;
        readfile.open("product.txt");
        while (getline(readfile, line)) {
            regex_match(line, m, re);
            string name{m[1]};
            long int price{stoi(m[2])};  // stoi => String TO Integer
            this->addFood(name, price, true);
            i++;
        }
        readfile.close();
    }

    void addFood(string name, long int price, bool notNew) {
        temp = new food();
        temp->name = name;
        temp->price = price;
        temp->next = NULL;

        if (notNew == false) {
            writefile.open("product.txt", ios::app);
            writefile << temp->name << ", " << temp->price << "\n";
            writefile.close();
        }

        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = tail->next;
        }
    }

    void updatePriceFood(string name) {
        long int newPrice ;
        temp = head;
        bool isUpdate = false;
        while (temp != NULL) {
            if (temp->name == name) {
                cout << "\n[INFO] price " + name + " Previously it was : " << temp-> price<< endl;
                cout << "\n[UPDATE PRODUCT] Input Price New : ";
                cin >> newPrice;
                temp->price = newPrice ;
                isUpdate = true;
            }
            temp = temp->next;
        }
        if (isUpdate) {
            renewFood();
        }
    }

    void renewFood() {
        temp = head;
        remove("product.txt");
        writefile.open("product.txt", ios::app);
        while (temp != NULL) {
            writefile << temp->name + ", " << temp->price << "\n";
            temp = temp->next;
        }
        writefile.close();
    }

    void deleteFood(string name) {
        int index = 0;
        now = head;
        while (now != NULL) {
            if (index == 0 && now->name == name) {
                del = head;
                head = head->next;
                delete del;
            } else if (index != 0 && now->name == name) {
                temp->next = now->next;
            } else {
                temp = now;
            }
            now = now->next;
            index++;
        }
        renewFood();
    }

    int showFood() {
        int i = 0;
        readfile.open("product.txt");
        show = head;
        cout << "\n-------------------------------------------------\n";
        cout << "|\t\t  PRODUCT LIST  \t\t|\n";
        cout << "-------------------------------------------------\n";
        cout << "|   NO \t|  MENU\t\t\t|  PRICE\t|\n";
        cout << "-------------------------------------------------\n";
        while (show != NULL) {
            cout << "|   " << i + 1 << " \t|   " + show->name + " \t\t|  " << show-> price << " \t|\n";
            i++;
            show = show->next;
        }
        cout << "-------------------------------------------------\n";
        readfile.close();
        return i;
    }
};

class Counts {
   public:
    int value = 0;

    void operator++() {
        ++value;
    }

    void display() {
        cout << "\n===================================================\n";
        cout << "|\t You have logged in incorrectly : " << value << "\t  |";
        cout << "\n===================================================\n";
    }
};

class Menu : public food {
   public:
    int option, length = 0;
    string nameProduct, choiceUser;
    long int total = 0, price, cash;
    food mk;
    bool isStop;

    void mainMenu() {
    begin:
        if (startMenu) {
            this->begin();
            startMenu = false;
        }
        cout << "\n=========================================\n";
        cout << "|\t\tMAIN MENU\t\t|";
        cout << "\n=========================================\n";
        cout << "|  1. | \t Order\t\t\t|\n|  2. | \tAdmin Mode\t\t|\n|  3. | \tExit Program\t\t|";
        cout << "\n=========================================\n";
        cout << "\nYour  Choice \n> ";
        cin >> option;

        switch (option) {
            case 1:
                this->menuOrder();
                break;

            case 2:
                system("cls");
                cout << "\n=================================================================\n";
                cout << "|\t[INFO] You Choose 2! Directed to Menu Admin\t\t|";
                cout << "\n=================================================================\n";
                this->menuAdmin();
                break;

            case 3:
                cout << "\n\nEXIT PROGRAM...";
                break;

            default:
                cout << "\nChoice " << option << " There isn't any! Please Select/Type 1, 2, 3, 4, 5, or 6!\n";
                cout << "\n======================================================\n";
                goto begin;
                break;
        }
    }

    void menuOrder() {
    begin:
        system("cls");
        cout << "\n=================================================\n";
        cout << "|\t\tMAKE AN ORDER\t\t\t|";
        cout << "\n=================================================\n";
        cout << "\n=================================================\n";
        cout << "|\tSHOWING AVAILABLE PRODUCTS...\t|";
        cout << "\n=================================================\n";
        int many = this->showFood();

        if (many == 0) {
            cout << "\n=====================================================================\n";
            cout << "|  THERE IS NO FOOD MENU! PLEASE ADD FOOD TO THE ADMIN MENU  |";
            cout << "\n=====================================================================\n";
            this->mainMenu();
        } else {
            this->choiceProduct();
        pay:
            cout << "\n=================================================\n";
            cout << "|\t\tPAYMENT SUMMARY\t\t\t|";
            cout << "\n=================================================\n";
            cout << "[PAYMENT] Enter the CASH that you have \t: ";
            cin >> cash;

            if (total == 0) {
                cout << "\n=========================================================\n";
                cout << "[PAYMENT] You are not currently ordering any food/drink menu!\n";
                cout << "[PAYMENT] Please select MAKE ORDER on the Admin Menu";
                cout << "\n=========================================================\n";
                this->mainMenu();
            } else if (cash < total) {
                cout << "\n=========================================================================\n";
                cout << "|\t\t[INFO] CASH is not enough to pay for the order!\t\t|";
                cout << "\n=========================================================================\n";

                cout << "\n=========================================================================\n";
                cout << "|\t[INFO] CASH input is more than or equal to the total price!\t|";
                cout << "\n=========================================================================\n";
                cout << "\n[PAYMENT] Do You Want to Cancel the Order [Yes (y)/No (n)]? : ";
                cin >> choiceUser;

                regex regExp("^(|y|Y)(|e|E)(|s|S)$");
                smatch m;
                if (regex_search(choiceUser, m, regExp)) {
                    system("cls");
                    cout << "\n=======================================================\n";
                    cout << "| [INFO] Order Canceled! Return to MAIN MENU  |";
                    cout << "\n=======================================================\n";
                    total = 0;
                    length = 0;
                    mainMenu();
                } else {
                    total = 0;
                    invoice();
                    goto pay;
                }

            } else if (cash >= total) {
                cout << "[PAYMENT] Cashback \t\t\t: " << cash - total;
                cout << "\n\n[PAYMENT] PAYMENT SUCCESSFUL!\n";
                cout << "\n=========================================================\n";
                cout << "|\t\t\tTHANK YOU\t\t\t|";
                cout << "\n=========================================================\n";
                total = 0;
                length = 0;
                cout << "\n[PAYMENT] Do You Want to Go Back to the MAIN MENU[y/n]? : ";
                cin >> choiceUser;

                regex regExp("^(|y|Y)(|e|E)(|s|S)$");
                smatch m;
                if (regex_search(choiceUser, m, regExp)) {
                    system("cls");
                    cout << "\n=======================================================\n";
                    cout << "|  [INFO] Order Canceled! Return to MAIN MENU   |";
                    cout << "\n=======================================================\n";
                    mainMenu();
                } else {
                    cout << "\n=========================================================\n";
                    cout << "\n\nEXIT PROGRAM...";
                }
            }
        }
    }

    void menuAdmin() {
    begin:
        // this->awal();
        cout << "\n=========================================\n";
        cout << "|\t\tADMIN MENU\t\t|";
        cout << "\n=========================================\n";
        cout << "| 1. | \t\tShow Product\t\t|\n| 2. | \t\tAdd Product\t\t|\n| 3. | \t\tUpdate price\t\t|\n| 4. | \t\tDelete Product\t\t|\n| 5. | \t\tMain Menu\t\t|";
        cout << "\n=========================================\n";
        cout << "\nYour choice: \n> ";
        cin >> option;

        switch (option) {
            case 1:
                system("cls");
                cout << "\n=================================================================\n";
                cout << "|\t[INFO] Choose Option 1! Displaying Products/Food\t|";
                cout << "\n=================================================================\n";
                this->showFood();
                goto begin;
                break;

            case 2:
                system("cls");
                cout << "\n=================================================================\n";
                cout << "|\t[INFO] Choose Option 2! Displaying Products/Food\t|";
                cout << "\n=================================================================\n";
                this->showFood();
                cout << "\n[ADD PRODUCT] Input New Food/Drink Name (ex: ga ran): ";
                getline(cin >> ws, nameProduct);
                cout << "\n[ADD PRODUCT]Input New Food/Drink Prices (ex: 3000) : ";
                cin >> price;
                this->addFood(nameProduct, price, false);
                cout << "\n[ADD PRODUCT]Displaying the Latest Food/Drink List..\n";
                this->showFood();
                goto begin;
                break;

            case 3:
                system("cls");
                cout << "\n=================================================================\n";
                cout << "|\t[INFO] Choose Option 3! Displaying Products/Food\t|";
                cout << "\n=================================================================\n";
                this->showFood();
                cout << "\n[UPDATE PRODUCT]Input the name of the food/drink whose price will be updated (ex: ga ran) : ";
                getline(cin >> ws, nameProduct);
                this->updatePriceFood(nameProduct);
                cout << "\n[UPDATE PRODUCT] Displaying the Latest Food/Drink List..\n";
                this->showFood();
                goto begin;
                break;

            case 4:
                system("cls");
                cout << "\n=================================================================\n";
                cout << "|\t[INFO]Choose Option 4! Displaying Products/Food\t|";
                cout << "\n=================================================================\n";
                this->showFood();
                cout << "\n[DELETE PRODUCT] Input the name of the food/drink to be deleted (ex: ga ran): ";
                getline(cin >> ws, nameProduct);
                this->deleteFood(nameProduct);
                cout << "\n[DELETE PRODUCT] Displaying the Latest Food/Drink List..\n";
                this->showFood();
                goto begin;
                break;

            case 5:
                system("cls");
                cout << "\n=================================================================\n";
                cout << "|\t[INFO] Choose Option 5! Displays the Main Menu\t\t|";
                cout << "\n=================================================================\n";
                this->mainMenu();
                break;

            default:
                cout << "\nchoice " << option << " There isn't any! Please Select/Type 1, 2, 3, 4, 5, or 6!\n";
                cout << "\n======================================================\n";
                goto begin;
                break;
        }
    }

    void invoice() {
        system("cls");
        cout << "\n=================================================\n";
        cout << "|\t    [INFO] Displays Bills    \t|";
        cout << "\n=================================================\n";
        cout << "\n=================================================\n";
        cout << "|\t\t    INVOICE\t\t\t|";
        cout << "\n=================================================\n";
        cout << "\n-------------------------------------------------\n";
        cout << "|   NO \t|  MENU\t\t\t|  PRICE\t|\n";
        cout << "-------------------------------------------------\n";
        for (int a = 0; a < this->length; a++) {
            this->choose[a].totalPrice = this->choose[a].productPrice * this->choose[a].qt;
            total += this->choose[a].totalPrice;
            cout << "|   " << a + 1 << " \t|   " + this->choose[a].productName + " \t\t|  " << this->choose[a].totalPrice << " \t|\n";
        }
        cout << "-------------------------------------------------\n";
        cout << "|\t  TOTAL AMOUNT  \t|  " << total << "  \t|\n";
        cout << "-------------------------------------------------\n";
    }

    void choiceProduct() {
        this->isStop = false;
        while (this->isStop == false) {
            cout << "\n[ORDER] Write down the desired food/drink (ex: ga ran): ";
            getline(cin >> ws, this->choose[length].choice);
            cout << "[ORDER] Enter Portion/Quantity (ex: 2): ";
            cin >> this->choose[length].qt;

            cout << "\n[ORDER] Want to Order Other Food/Drinks [y/n] ? : ";
            cin >> choiceUser;

            readfile.open("product.txt");
            while (getline(readfile, line)) {
                regex_match(line, m, re);
                string name{m[1]};
                long int prices{stoi(m[2])};

                if (this->choose[length].choice == name) {
                    this->choose[length].productName = name;
                    this->choose[length].productPrice = prices;
                };
            }
            readfile.close();

            regex regExp("^(|y|Y)(|e|E)(|s|S)$");
            if (regex_search(choiceUser, m, regExp)) {
                this->isStop = false;
            } else {
                this->isStop = true;
            }
            length++;
        }
        system("cls");
        this->invoice();
    }
};

class User {
   public:
    string name, pass, choiceUser, line;
    int result, length = 0, total = 0;
    bool isStop;

    void registers() {
        bool isRegistered = false;

        readfile.open("dataUser.txt");
        while (getline(readfile, line)) {
            regex_match(line, m, re);
            string username{m[1]}, password{m[2]};
            if (username == name && password == pass) {
                system("cls");
                cout << "\n===================================================\n";
                cout << "|\t\t\t\t\t\t  |\n|   [WARN!] Username has been registered previously!  |\n";
                cout << "|\t\t\t\t\t\t  |\n===================================================\n";
                isRegistered = true;
                break;
            }
        }
        readfile.close();

        if (!isRegistered) {
            writefile.open("dataUser.txt", ios::app);
            writefile << name + ", " + pass + "\n";
            system("cls");
            cout << "\n==========================================================================\n";
            cout << "|\t\t\t\t\t\t\t\t\t |";
            cout << "\n|   [INFO] Successful REGISTER New User! Please Continue Login (Choose 2)!   |\n";
            cout << "|\t\t\t\t\t\t\t\t\t |";
            cout << "\n==========================================================================\n";
            writefile.close();
        }
    }

    int login() {
        bool isLogin = false;
        readfile.open("dataUser.txt");
        while (getline(readfile, line)) {
            regex_match(line, m, re);
            string username{m[1]}, password{m[2]};
            if (username == name && pass == password) {
                system("cls");
                cout << "\n==================================================================\n";
                cout << "|\t\t\t\t\t\t\t\t |";
                cout << "\n|   [INFO] USER LOGIN Successful! Redirected to MAIN MENU Page!\t |\n";
                cout << "|\t\t\t\t\t\t\t\t |";
                cout << "\n==================================================================\n";
                isLogin = true;
                break;
            }
        }
        readfile.close();
        if (!isLogin) {
            system("cls");
            cout << "\n===================================================\n";
            cout << "|\t\t\t\t\t\t  |\n|   [WARN!] Username & Password Not Registered!  |\n";
            cout << "|\t\t\t\t\t\t  |\n===================================================\n";
        }
        this->result = isLogin == true ? 1 : 0;
        return this->result;
    }
};

int main() {
    int option, resultLogin;
    User user;
    Menu menu;
    Counts cou;
    


    Identity id = Identity ("TRAN_DUC_MANH", "TRAN_QUANG_ANH","PHAM_VIET_HA","VU_MINH_QUAN");
    Identity id2 = Identity (1);

begin:
    cout << "\n---------------------------------\n";
    cout << "|\t  HOME MENU\t\t|\n";
    cout << "---------------------------------\n";
    cout << "|  1. REGISTER\t\t\t|\n|  2. LOG IN\t\t\t|\n|  3. EXIT PROGRAM\t\t|\n";
    cout << "---------------------------------\n";
    cout << "\nChoose the Menu Above (1, 2, or 3)!\n> ";
    cin >> option;

    switch (option) {
        case 1:
            cout << "\n=========================================\n";
            cout << "|\t Choose 1 : REGISTER\t\t|\n";
            cout << "=========================================\n";
            cout << "\n[REGISTER] Input Username : ";
            cin >> user.name;
            cout << "[REGISTER] Input Password : ";
            cin >> user.pass;

            user.registers();
            goto begin;
            break;

        case 2:
            cout << "\n=========================================\n";
            cout << "|\t choose 2 : LOGIN\t\t|\n";
            cout << "=========================================\n";
            cout << "\n[LOGIN] Input Username : ";
            cin >> user.name;
            cout << "[LOGIN] Input Password : ";
            cin >> user.pass;

            resultLogin = user.login();
            if (resultLogin == 1) {
                startMenu = true;
                menu.mainMenu();
            } else {
                ++cou;
                cou.display();
                if (cou.value >= 5) {
                    cout << "\nTOO MANY LOGIN ATTEMPT, PROGRAM STOPS...";
                } else {
                    goto begin;
                }
            }
            break;

        case 3:
            cout << "\n=========================================\n";
            cout << "|\t Choose 3 : EXIT\t\t|\n";
            cout << "=========================================\n";
            cout << "\nPROGRAM STOP...";
            break;

        default:
            system("cls");
            cout << "\n=========================================\n";
            cout << "|\t Choose " << option << " : NOT FOUND\t\t|\n";
            cout << "=========================================\n";
            cout << "\nChoice " << option << " There isn't any! Please Select/Type 1, 2, or 3!\n";
            cout << "\n=======================================================\n";
            goto begin;
            break;
    }
}