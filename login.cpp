#include "login.h"
#include "code_decode.h"

bool login_admin(unsigned int seed) {

    //Administra el inicio de sesión del admin

    unsigned long long int len;
    string password, real_password;

    fflush(stdin);
    cout << endl << "Enter the admin password: "; getline(cin, password);

    if (get_text("sudo.txt", real_password, len)) {

        decode(real_password, len, seed);
        if (password == real_password) return true;
        else {
            cout << endl << "Sorry, that password is incorrect" << endl << endl;
            system("pause");
            return false;
        }
    }
    else cout << "Sorry, the sudo.txt file could not be opened" << endl << endl;
    system("pause");
    return false;
}

bool create_user(User *&user, unsigned seed) {

    //Almacena los datos del usuario user en el archivo users.txt, el cual está codificado,
    //en el formato que se especifica a continuación. La variable seed corresponde a la
    //semilla de codificación. Retorna true si el archivo users.txt puedo ser abierto,
    //o false en caso contrario. En cualquier caso esta función libera la memoria
    //dinámica reservada por user.

    string text, str_aux;
    unsigned long long int len;

    //Recuperamos el texto codificado de users.txt y lo almacenamos en text.

    if (get_text("users.txt", text, len)) {

        //Si había texto en users.txt, lo decodificamos.

        if (len != 0) decode(text, len, seed);

        //El formato en que se guardarán los usuarios en users.txt será el siguiente:

        //CEDULA CONTRASEÑA\r\n

        //A continuación contruimos un string con los datos de user en este formato
        //y lo guardamos en el archivo users.txt para posteriormente codificarlo
        //de nuevo.

        //Nota: El \r es porque cuando se modifican los archivos .txt desde el bloc de notas,
        //cuando se escribe un salto de línea y se guarda el archivo, al ser leído aparece
        //el carácter \r precediendo a todos los \n, por lo cual, para mantener consistencia
        //entre cuando el archivo .txt es modificado por el programa o por el programador
        //desde el bloc de notas, se necesita añadir ese \r extra.

        //Agregamos la cedula de user y un espacio en blanco a text.

        str_aux = to_string(user->get_id());
        text.append(str_aux);
        text.push_back(' ');
        len += str_aux.length() + 1;

        //Agregamos la contraseña de user y el \r seguido del \n a text.

        str_aux = user->get_password();
        text.append(str_aux);
        text.push_back('\r');
        text.push_back('\n');
        len += str_aux.length() + 2;

        //Liberamos la memoria dinámica reservada por user.

        delete user;

        //Codificamos de nuevo el texto de text y lo guardamos en users.txt.

        code(text, len, seed);
        save_text("users.txt", text, len);

        return true;
    }
    else {
        cout << "Sorry, the users.txt file could not be opened" << endl << endl;

        //Liberamos de todas formas la memoria dinámica reservada por user.

        delete user;
        return false;
    }
}

bool search_id(string text, unsigned long long int len, unsigned long long int id, unsigned long long int &index, unsigned long long int &end_index) {

    //Retorna true si la cédula correspondiente a id aparece registrada en text,
    //en index almacena el índice del primer dígito de la cédula y en end_index
    //el índice del espacio en blanco inmediatamente posterior al último dígito
    //de la cédula; o retorna false en caso contrario. La variable len es la
    //longitud del string text.

    index = 0;
    string str_id = to_string(id);

    while (index != len) {
        end_index = text.find(' ', index);
        if (text.substr(index, (end_index - index)) == str_id) return true;
        else {
            index = text.find('\n', end_index);
            index++;
        }
    }
    return false;
}

bool valid_password(string password) {

    int len = password.length();
    if ((len < 4) or (16 < len)) {
        cout << endl << "Sorry, your password must have between 4 and 16 characters" << endl << endl;
        return false;
    }
    else {
        for (short int i = 0; i < len; i++) {
            if (((password[i] < 48) or (57 < password[i])) and ((password[i] < 65) or (90 < password[i])) and ((password[i] < 97) or (122 < password[i]))) {
                cout << endl << "Sorry, your password must have only numbers or letters" << endl;
                cout << "different from: " << char(165) << ", " << char(164) << " or letters with accent" << endl << endl;
                return false;
            }
        }
        return true;
    }
}

bool register_user(unsigned long long int id, unsigned seed) {

    //Solicita al usuario una constraseña de entre 4 y 16 carácteres de números
    //y letras a excepción de la Ñ, ñ o letras con tilde, con el propósito
    //de resigtrarlo en el sistema del cine, es decir, en users.txt,
    //el cual es un archivo que está codificado con la semilla seed.
    //La variable id almacena la cédula del usuario.

    string password;

    fflush(stdin);
    cout << "Enter what will be the password for you account, it must have" << endl;
    cout << "between 4 and 16 characters (only letters and numbers): "; getline(cin, password);

    while (!valid_password(password)) {
        system("pause");
        system("cls");
        cout << "Enter what will be the password for you account, it must have" << endl;
        cout << "between 4 and 16 characters (only letters and numbers): "; getline(cin, password);
    }

    User *user = new User(id, password);
    return create_user(user, seed);
}

bool login_user(unsigned int seed, User *&us) {
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
