#include "login.h"
#include "dialog.h"
#include "code_decode.h"
#include <iostream>

//Esta libreria contiene todas las implementaciones de las funciones que administrarán
//el inicio de sesión tanto de los usuarios como del admin.

bool login_admin(unsigned int seed) {

    //Solicitamos al usuario la contraseña para iniciar sesión como admin.
    //La variable seed es la semilla con la que se codificó el archivo
    //sudo.txt. Retornamos true si el usuario ingresa la contraseña
    //correcta, o false en caso contrario.

    unsigned long long int len;
    string password, real_password;

    fflush(stdin);
    cout << "  Enter the admin password: "; getline(cin, password);

    //Tomamos el texto de sudo.txt y lo decodificamos.

    if (get_text("../DesafioEvaluativoV2/data/sudo.txt", real_password, len)) {
        decode(real_password, len, seed);

        //Verificamos que la contraseña ingresada sea la correcta.

        if (password == real_password) return true;
        else {
            cout << endl << "  Sorry, that password is incorrect" << endl << endl;
            cout << "  "; system("pause");
            return false;
        }
    }
    else cout << "  Sorry, the sudo.txt file could not be opened" << endl << endl;
    cout << "  "; system("pause");
    return false;
}

bool search_id(string text, unsigned long long int len, unsigned long long int id, unsigned long long int &index, unsigned long long int &end_index) {

    //Retornamos true si la cédula correspondiente a id aparece registrada en text,
    //en cuyo caso almacenamos en index el índice del primer dígito de la cédula
    //y en end_index el índice del espacio en blanco inmediatamente posterior
    //al último dígito de la cédula; o en caso de no encontrar la id en text,
    //retornamos false. La variable len es la longitud del string text.

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

bool add_user(unsigned long long int id, string password, unsigned seed) {

    //Almacenamos los datos del usuario (id y password), en el formato que se especificará
    //a continuación, dentro del archivo users.txt, el cual se encuentra codificado
    //con semilla seed. Retornamos true si el archivo users.txt pudo ser abierto,
    //o false en caso contrario.

    string text;
    unsigned long long int len;

    //Recuperamos el texto codificado de users.txt y lo almacenamos en text.

    if (get_text("../DesafioEvaluativoV2/data/users.txt", text, len)) {

        //Si había texto en users.txt, lo decodificamos.

        if (len != 0) decode(text, len, seed);

        //El formato en que se guardarán los usuarios en users.txt será el siguiente:

        //CEDULA CONTRASEÑA\r\n

        //En el caso en que se registre por primera vez y no haya realizado ninguna
        //reserva aún. En el caso de que el usuario ya haya hecho previamente
        //una reserva, el formato es el sigueiten:

        //CEDULA CONTRASEÑA HORA-SALA-FILACOLUMNA-COMBO:\r\n

        //Nótese el ':' al final de COMBO.

        //HORA es la hora de la película donde reservó el asiento (en horario militar),
        //SALA es la sala donde reservó el asiento, ROW es la fila y COLUMN es la columna
        //de la posición de su asiento en la matriz seats[][], del show correspondiente,
        //y COMBO es el número del combo que eligió en caso de reservar un asiento Gold.
        //Para los usuarios que no reservaron un asiento Gold, COMBO tomará el valor de 0.

        //Por ejemplo, la información de un usuario podría tomar esta forma, si no ha hecho
        //una reserva aún, pero sí se ha registrado:

        //9276346837 salpicon\r\n

        //O en el caso de que haya reservado por ejemplo un asiento Gold y haya seleccionado
        //el combo 3, supongamos que el asiento sea el B5 de la sala 4, para ver una película
        //programada para las 6:00 pm, se verá de la siguiente forma:

        //9276346837 salpicon 18-4-14-3:\r\n

        //Nota1: Recordemos que la fila B en nustro programa corresponde a 1, y la columna 5 a 4.
        //Nota2: Las horas que están entre 0 y 9, inclusive, se colocan precedidas por un 0.

        //El dos puntos del final lo utilizamos para saber si el usuario ingresó una
        //cédula que ya había reservado un asiento, para indicarle que no puede reservar
        //más de un asiento por cédula, si desea reservar otro debe ponerlo a nombre
        //de otra.

        //El \r es porque cuando se modifican los archivos .txt desde el bloc de notas,
        //cuando se escribe un salto de línea y se guarda el archivo, al ser leído aparece
        //el carácter \r precediendo a todos los \n, por lo cual, para mantener consistencia
        //entre cuando el archivo .txt es modificado por el programa, o por el programador
        //desde el bloc de notas, se necesita añadir ese \r extra.

        //A continuación construimos un string con los datos del usuario en el formato indicado
        //previamente para los que se registran por primera vez (ie el primer formato),
        //y lo guardamos en el archivo users.txt, para posteriormente codificarlo de nuevo.

        //Agregamos la cédula y un espacio en blanco a text.

        text.append(to_string(id));
        text.push_back(' ');
        len += to_string(id).length() + 1;

        //Agregamos la contraseña y el \r seguido del \n a text.

        text.append(password);
        text.push_back('\r');
        text.push_back('\n');
        len += password.length() + 2;

        //Codificamos nuevamente el texto de text y lo guardamos en users.txt.

        code(text, len, seed);
        save_text("../DesafioEvaluativoV2/data/users.txt", text, len);

        return true;
    }
    else {
        cout << "  Sorry, the users.txt file could not be opened" << endl << endl;
        return false;
    }
}

bool register_user(unsigned long long int id, unsigned seed) {

    //Solicitamos al usuario una contraseña para poder registrarlo en el
    //sistema, es decir, en users.txt, el cual es un archivo que está
    //codificado con la semilla seed. La contraseña debe tener entre
    //4 y 16 carácteres formados únicamente por enteros entre el 0
    //y el 9, inclusive, o letras, mayúsculas o minúsculas, pertenecientes
    //exclusivamente al alfabeto inglés (26 letras).
    //La variable id almacena la cédula del usuario.

    string password;
    system("cls");

    fflush(stdin);
    cout << endl << "  Enter what will be the password for you account, it must have" << endl;
    cout << "  between 4 and 16 characters (only letters and numbers): "; getline(cin, password);

    //Continuamos solicitando al usuario una contraseña hasta que cumpla con
    //los requerimientos mencionados.

    while (!valid_password(password)) {
        cout << "  "; system("pause");
        system("cls");

        cout << endl << "  Enter what will be the password for you account, it must have" << endl;
        cout << "  between 4 and 16 characters (only letters and numbers): "; getline(cin, password);
    }

    //Llamamos a la función add_user() para agregar al usuario al archivo users.txt.

    //Nota: En esta parte me hubiera gustado hacer algo como enviarle un correo electrónico
    //al usuario o algo por el estilo, para confiamar la cuenta creada, pues así podriamos
    //evitar que cualquier persona puediera reservar a nombre de otra con solo conocer su
    //cédula, aunque como el pago se cobra a la persona que está utilizando la aplicación,
    //no habría problema con el dinero, pero sí con la identidad.

    return add_user(id, password, seed);
}

bool valid_password(string password) {

    //Retornamos true si la contraseña almacenada en password cumple con los requerimientos
    //de estar formada por entre 4 y 16 carácteres conformados únicamente por enteros
    //desde el 0 hasta el 9, o letras, mayúsculas o minúsculas, pertenecientes
    //exclusivamente al alfabeto inglés (26 letras).

    unsigned short int len = password.length();

    //Verificamos que password posea entre 4 y 16 carácteres.

    if (is_in_range(len, 4, 16)) {

        //En caso de que sí posea entre 4 y 16 carácteres, recorremos la constraseña
        //y verificamos que esté conformada únicamente por enteros desde el 0 hasta
        //el 9, o letras, mayúsculas o minúsculas, pertenecientes exclusivamente al
        //alfabeto inglés (26 letras).

        for (unsigned short int i = 0; i < len; i++) {
            if (((password[i] < 48) or (57 < password[i])) and ((password[i] < 65) or (90 < password[i])) and ((password[i] < 97) or (122 < password[i]))) {

                cout << endl << "  Sorry, your password must have only numbers or letters" << endl;
                cout << "  differents from: " << char(165) << ", " << char(164) << " or letters with accent." << endl << endl;

                return false;
            }
        }
        return true;
    }
    else {
        cout << endl << "  Sorry, your password must have between 4 and 16 characters" << endl << endl;
        return false;
    }
}

bool login(unsigned int seed, bool &is_admin,  unsigned long long int &user_id) {

    //Solicitamos al usuario una cédula para que pueda iniciar sesión en el sistema.
    //La variable seed es la semilla con la que se codificaron los archivos
    //users.txt y sudo.txt. La variable is_admin, recibida por referencia,
    //terminará siendo true si el usuarios ingresa la palabra clave 'admin'
    //en lugar de su cédula, o false en caso de que inicie sesión o se
    //registre en el sistema.

    //Retornamos true en caso de que el usuario se registre, inicie su sesión,
    //o ingrese como administrador; false en caso contrario.

    unsigned long long int len;
    string text, password, real_password, ans;

    //Tomamos el texto de users.txt y en caso de que hayan datos
    //los decodificamos.

    if (get_text("../DesafioEvaluativoV2/data/users.txt", text, len)) {

        if (len != 0) decode(text, len, seed);
        unsigned long long int index = 0, end_index = 0;

        //Solicitamos al usuario su contraseña.

        cout << endl << "  Enter your ID for make a reservation: ";
        if (!get_id_input(user_id, is_admin)) {
            cout << endl << "  Sorry, the ID only can have numbers" << endl << endl;
            cout << "  "; system("pause");
            return false;
        }

        //No está demás recalcar el funcionamiento de get_id_input():

        //Solicitamos al usuario una cédula, las cuales están compuestas
        //únicamente por enteros desde el 0 hasta el 9. Retornamos true
        //y almacenamos la cédula ingresada en id en caso de que
        //efectivamente sólo estuviera compuesta por enteros desde
        //el 0 hasta el 9, o en el caso de que se haya ingresado
        //la palabra clave 'admin', retornamos de nuevo true pero
        //no modificamos la variable id y colocariamos is_admin en
        //true. Retornamos false si no se presenta alguno de los
        //dos casos anteriormente mencionados.

        //En caso de que is_admin haya sido colocado en true,
        //llamamos entonces a la función login_admin().

        else if (is_admin) return login_admin(seed);

        //Si is_admin es false, pero se ingresó una cédula válida, procedemos
        //a buscar dicha cédula en el string text tomado de users.txt.

        else if (!search_id(text, len, user_id, index, end_index)) {

            //Si no la encontramos, es porque el usuario no se ha registrado aún,
            //por lo cual le preguntamos si desea hacerlo.

            cout << endl << "  It looks that you don't have an account yet, do you want to create one?" << endl;
            if (yes_no_question("Enter 'Yes' for create an account, or 'No' not to create it:")) {

                //En caso afirmativo llamamos a la función register_user().

                register_user(user_id, seed);

                cout << endl << "  Your account has been created successfully!" << endl << endl;
                cout << "  "; system("pause");

                return true;
            }
            else return false;
        }
        else {

            //Si el usuario ingresó una cédula válida, is_admin es false, y la cédula fue
            //encontrada en el string text, procedemos a extraer de éste último la contraseña
            //del usuario con ayuda de las variables index y end_index (ver funcionamiento
            //de search_id()) para compararla con la que ingresará el usuario a continuación.

            index = text.find(' ', index);
            end_index = text.find('\r', index);

            //Antes de comparar, verificamos si quizá el usuario posee una reserva
            //previa comprobando si antes del \r, del formato en que se guarda la
            //información, está el ':'; en caso afirmativo es porque ya posee una
            //reserva y por tanto le indicamos que debe ingresar con otra cédula,
            //o en caso contrario procedemos a comparar la contraseña real con
            //la que ingresará el usuario.

            if (text[end_index - 1] == ':') {

                cout << endl << "  Sorry, this ID have an active reserve, if you want to make" << endl;
                cout << "  an other reserve you have to enter other ID; cinema politics." << endl << endl;
                cout << "  "; system("pause");

                return false;
            }

            real_password = text.substr((index + 1), (end_index - index - 1));

            fflush(stdin);
            cout << "  Enter your password: "; getline(cin, password);

            if (real_password != password) {
                cout << endl << "  Sorry, that password is incorrect." << endl << endl;
                cout << "  "; system("pause");
                return false;
            }
            else return true;
        }
    }
    else {
        cout << "  Sorry, the users.txt file could not be opened" << endl << endl;
        cout << "  "; system("pause");
        return false;
    }
}
