#include "userLogin.h"
#include "code_decode.h"

bool login_admin(unsigned int seed) {
    //Administra el inicio de sesión del admin

    unsigned long long int len;
    string password, real_password;
    fflush(stdin);
    cout << endl << "Enter the admin password: "; getline(cin, password);
    if (get_text("sudo.dat", real_password, len)) {

        decode(real_password, len, seed);
        if (password == real_password) return true;
        else {
            cout << endl << "Sorry, that password is incorrect" << endl << endl;
            system("pause");
            return false;
        }
    }
    else cout << "Sorry, the sudo file could not be opened" << endl << endl;
    system("pause");
    return false;
}

bool create_user(user *&us, unsigned seed) {
    //Crea un usuario nuevo y lo guarda en el archivo users.dat.
    //Libera la memoria dinámica de us.

    string text, str_aux;
    unsigned long long int len;
    if (get_text("users.dat", text, len)) {

        if (len != 0) decode(text, len, seed);
        else text = "";

        //Ensamblamos el formato en que se guardan los datos de los usuarios en users.dat
        //CEDULA:CONTRASEÑA;SALDO\r\n
        str_aux = to_string(us->get_id());
        text.append(str_aux);
        text.push_back(':');
        len += str_aux.length() + 1;

        str_aux = us->get_password();
        text.append(str_aux);
        text.push_back(';');
        len += str_aux.length() + 1;

        //str_aux = to_string(us->get_balance());
        text.append(str_aux);
        text.push_back('\r');
        text.push_back('\n');
        len += str_aux.length() + 2;
        delete us;

        //Nota: Gracias al formato en que se guardan los datos de los usuarios
        //users.dat siempre debe terminar con \r\n

        code(text, len, seed);

        save_text("users.dat", text, len);
        return true;
    }
    else {
        cout << "Sorry, the users file could not be opened" << endl << endl;
        delete us;
        return false;
    }
}

bool search_id(string text, unsigned long long int len, unsigned long long int id, unsigned long long int &index, unsigned long long int &end_index) {
    //Busca si una cédula aparece en el string text tomado del archivo users.dat

    string str_id = to_string(id);
    while (index != len) {
        end_index = text.find(':', index);
        if (text.substr(index, (end_index - index)) == str_id) return true;
        else {
            index = text.find('\n', end_index);
            index++;
        }
    }
    return false;
}

bool register_user(unsigned seed) {
    //Solicita los datos y registra a un usuario nuevo en users.dat

    string password, text;
    unsigned long long int id, len, index = 0, end_index = 0;

    cout << "Enter the ID of the user: ";
//    if (!get_valid_input(id)) {
//        cout << endl << "Sorry, that is not a valid ID" << endl << endl;
//        return false;
//    }

    if (!get_text("users.dat", text, len)) {
        cout << endl << "Sorry, the users file could not be opened" << endl << endl;
        return false;
    }

    decode(text, len, seed);
//    if (search_id(text, len, id, index, end_index)) {
//        cout << endl << "Sorry, that ID is already registered" << endl << endl;
//        return false;
//    }

    fflush(stdin);
    cout << "Enter the password of the user: "; getline(cin, password);

    cout << "Enter the initial balance of the user: ";
//    if (!get_valid_input(balance)) {
//        cout << endl << "Sorry, that is not a valid balance" << endl << endl;
//        return false;
//    }

    user *us = new user(id, password);
    return create_user(us, seed);
}

bool update_user(user *&us, bool method, unsigned seed) {
    //Actualizamos el saldo del usuario al que apunta us,
    //dentro del archivo users.dat. Libera la memoria
    //dinámica de us

    string text, up_text;
    unsigned long long int len, index = 0, end_index = 0;
    if (get_text("users.dat", text, len)) {

        decode(text, len, seed);

        //Cortamos el texto en donde comienza el saldo y donde termina
        search_id(text, len, us->get_id(), index, end_index);
        index = text.find(';', index);
        end_index = text.find('\r', index);
        //Creamos el nuevo texto juntando los dos pedazos de los extremos y
        //colocando en medio el saldo actual del usuario
        //up_text = text.substr(0, (index + 1)) + to_string(us->get_balance()) + text.substr(end_index);
        delete us;

        len = up_text.length();
        code(up_text, len, seed);

        save_text("users.dat", up_text, len);
        return true;
    }
    else {
        cout << endl << "Sorry, the users file could not be opened" << endl << endl;
        delete us;
        system("pause");
        return false;
    }
}

bool login_user(unsigned int seed, user *&us) {
    //Administra el inicio de sesión de los usuarios

    string text, password, real_password;
    unsigned long long int len;
    if (get_text("users.dat", text, len)) {

        decode(text, len, seed);

        unsigned long long int index = 0, end_index = 0, id;

        cout << endl << "Remember that logging into account to check the balance or withdraw money costs $1000" << endl;

        cout << "Enter your ID: ";
        if (!get_valid_input(id)) {
            cout << endl << "Sorry, that is not a valid ID" << endl << endl;
            system("pause");
            return false;
        }

        if (!search_id(text, len, id, index, end_index)) {
            cout << endl << "Sorry, that ID is not registered yet." << endl;
            cout << "Talk to the admin to be registered." << endl << endl;
            system("pause");
            return false;
        }

        //Extraemos la contraseña del string text tomado de users.dat
        index = text.find(':', index);
        end_index = text.find(';', index);
        real_password = text.substr((index + 1), (end_index - index - 1));

        fflush(stdin);
        cout << "Enter your password: "; getline(cin, password);

        if (real_password != password) {
            cout << endl << "Sorry, that password is incorrect." << endl << endl;
            system("pause");
            return false;
        }

        //Extraemos el saldo del string text tomado de users.dat
        unsigned long long int balance;
        index = end_index;
        end_index = text.find('\r', index);
        str2int(text.substr((index + 1), (end_index - index - 1)), balance);

        //Verificamos que el usuario pueda pagar la cuota de $ 1000 por entrar a su cuenta
        if (balance < 1000) {
            cout << endl << "Sorry, you dont have at least $1000 in your account" << endl << endl;
            system("pause");
            return false;
        }

        us = new user(id, real_password, (balance - 1000));
        cout << endl << "Login successful. Welcome!" << endl << endl;
        system("pause");
        return true;
    }
    else {
        cout << "Sorry, the users file could not be opened" << endl << endl;
        system("pause");
        return false;
    }
}

void user_session(user *&us) {
    //Administra la sesión de usuario

    unsigned int opt = display_user_menu();
    while (opt != 3) {
        if (opt == 1) cout << endl << "Your current balance is $" << us->get_balance() << endl << endl;
        else if (opt == 2) {

            unsigned long long int money;
            cout << endl << "Enter the amount of money you want to withdraw: ";
            while (!get_valid_input(money)) {
                cout << "Sorry, that is not a valid number" << endl << endl;
                cout << "Enter the amount of money you want to withdraw: ";
            }

            if (money > us->get_balance()) {
                cout << endl << "Your current balance is $" << us->get_balance() << "." << endl;
                cout << "You cant withdraw $" << money << "!" << endl << endl;
            }
            else {
                us->decrease_balance(money);
                cout << endl << "Transaction completed successfully!" << endl << endl;
            }
        }
        system("pause");
        system("cls");
        opt = display_user_menu();
    }
}

void ATM_application() {
    //Administra la aplicación del cajero automático (ATM)

    system("cls");
    bool method;
    unsigned seed;

    //Preguntamos cual método y semilla se usara en la codificación
    //y descoficiación de los archivos sudo.dat y users.dat
    string *options = new string[2]{"Method 1", "Method 2"};
    if (ask("\nWhich method do you want to use?", options, 2) == 1) method = 0;
    else method = 1;
    seed = get_seed();

    user *us = nullptr;
    unsigned int opt = display_main_menu();
    while (opt != 3) {
        if ((opt == 1) and login_admin(method, seed)) {

            system("cls");
            cout << endl << "Login as admin successful. Welcome admin!" << endl << endl;
            cout << "-----------REGISTER A NEW USER-----------" << endl << endl;

            if (register_user(method, seed)) cout << endl << "The user was registered successfully!" << endl << endl;
            else cout << "The user was NOT registered successfully" << endl << endl;

            system("pause");
        }
        else if ((opt == 2) and login_user(method, seed, us)) {

            system("cls");
            user_session(us);
            update_user(us, method, seed);
        }
        opt = display_main_menu();
    }
    system("cls");
}
