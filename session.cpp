#include "session.h"
#include "code_decode.h"
#include "dialog.h"
#include "login.h"
#include <array>

//Esta libreria contiene todas las implementaciones de las funciones que administran
//las sesiones de usuario y de administrador.

void user_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned long long int &user_id, const unsigned int &seed) {

    //Esta función es muy pequeña, pero es para no sobrecargar mucho el main y mantenerlo ordenado.

    short int id;
    bool reserve = false;
    while (!reserve and get_show_id(shows, id, false)) {
        shows.at(id).reserve_seat(shows.size(), sales, total, user_id, reserve, seed);
        system("cls");
    }
}

bool update_user(const unsigned long long int &user_id, const short int &hour, const short int &room,
                 const short int &row, const short int &column, const short int &combo, const unsigned int &seed) {

    //Esta función actualiza la información de la cuenta del usuario almacenada en users.txt,
    //para el momento en que el usuario confirma la reserva pagándola.

    string text, f_text, aux = "";
    unsigned long long int len, index, end_index;

    if (get_text("../DesafioEvaluativoV2/data/users.txt", text, len)) {

        //Decodificamos la información de users.txt, en este punto siempre
        //habrá información pues el usuario se tiene que haber registrado
        //previamente para llegar aquí.

        decode(text, len, seed);

        //Buscamos la cédula en text y procedemos a agregar toda la información de la reserva.
        //guardando toda la información de users.txt posterior a index (inicializado dentro
        //de search_id()) en f_text, y la estrictamente previa a index en text, para agregar
        //los datos extras a text y luego volverlo a juntar con f_text, codificar y guardar
        //la información de nuevo en users.txt.

        search_id(text, len, user_id, index, end_index);

        index = text.find('\r', end_index);
        f_text = text.substr(index);
        text = text.substr(0, index);

        text.push_back(' ');

        //Si la hora está entre 0 y 9 inclusive, le colocamos un '0' antes.

        if (hour < 10) {
            aux.push_back('0');
            aux.push_back(char(hour + 48));
        }
        else aux.append(to_string(hour));

        text.append(aux);
        text.push_back('-');

        text.push_back(char(room + 48));
        text.push_back('-');

        //Recordemos que las columnas de las salas son 10, por lo cual
        //dentro del programa van desde 0 hasta 9, es decir, siempre
        //se pueden guardar como un único carácter al igual de la fila.

        text.push_back(char(row + 48));
        text.push_back(char(column + 48));
        text.push_back('-');

        text.push_back(char(combo + 48));
        text.push_back(':');

        len += 11;
        text.append(f_text);

        code(text, len, seed);
        save_text("../DesafioEvaluativoV2/data/users.txt", text, len);

        return true;
    }
    return false;
}

void display_admin_menu() {

    //Imprimimos en pantalla las opciones que conforman el menú
    //de la adminitración.

    //Nota: Esta fución al ser para imprimir algo en pantalla debería ir en la libreria dialog,
    //sin embargo me parece más ilustrativo colocarla aquí para poder tener presente cuales
    //son las funcionalidades del admin.

    display_title(true);

    cout << endl << endl << "  What you want to do?" << endl;
    cout << endl << "  1. Schedule new show" << endl;
    cout << "  2. Offer seats at scheduled shows" << endl;
    cout << "  3. Make sales report of today" << endl;
    cout << "  4. Reset sales record" << endl;
    cout << "  5. Encode file" << endl;
    cout << "  6. Decode file" << endl;
    cout << "  7. Turn off the application" << endl;
    cout << "  8. Exit" << endl;
}

void admin_session(vector<Show> &shows, array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned int &code_seed, bool &on) {

    //Esta función administra la sesión del administrador permitiéndole
    //acceder a todas sus funcionalidades.

    //Los nombres de las funciones y métodos se explican solos.

    short int opt = 1, id;
    while (opt != 8) {

        display_admin_menu();
        opt = get_int_input("What you want to do?", "Sorry, that is not a valid option", short(1), short(8));

        //En lo personal prefiero el if y else if en lugar del switch.

        if (opt == 1) {
            system("cls");
            schedule_show(shows);
        }
        else if (opt == 2) {

            system("cls");

            bool offer = false;
            while (!offer and get_show_id(shows, id, true)) {
                shows.at(id).modify_offers(shows.size(), offer);
                system("cls");
            }
        }
        else if (opt == 3) display_sales(sales, total);

        //En esta parte coloqué de nuevo la validación de la cuanta de administrador
        //porque es la funcionalidad más peligrosa del administrador, la de poder
        //codificar o decodificar información, pues permite cambiar la contraseña
        //de administrador codificando un archivo y llamándolo sudo.txt, o decodificar
        //la información de users.txt, donde están las constraseñas, cédulas y registro
        //de reservas de asientos. La opción 4 también sigue siendo peligrosa pues
        //resetea el archivo del registro de ventas del día.

        //Nota: No es un requisito, pero para no llenar la carpeta data de muchos archivos .txt,
        //todas las codificaciones o decodificaciones de archivos se sugieren hacer en el
        //desde el archivo nat.txt a un archivo codificado como users.txt, sudo.txt o
        //sales_record.txt, o desde uno de los codificados a nat.txt.

        else if ((opt == 4) and login_admin(code_seed)) reset_sales_record(sales, total, code_seed);
        else if ((opt == 5) and login_admin(code_seed)) {

            system("cls");

            string file_nat, file_code;
            get_files_names(file_nat, file_code, true);
            code_file("../DesafioEvaluativoV2/data/" + file_nat, "../DesafioEvaluativoV2/data/" + file_code, code_seed);
        }
        else if ((opt == 6)  and login_admin(code_seed)) {

            system("cls");

            string file_nat, file_code;
            get_files_names(file_code, file_nat, false);
            decode_file("../DesafioEvaluativoV2/data/" + file_code, "../DesafioEvaluativoV2/data/" + file_nat, code_seed);
        }
        else if (opt == 7) {
            on = false;
            opt = 8;
        }
    }
}

void schedule_show(vector<Show> &shows) {

    //Permite al administrador programar un nuevo show y agregarlo al vector Shows.

    bool is_3D, ask = true;
    string movie_name, genre, ans;
    short int clasi, hour, finish_hour, duration, room, show_hour, show_finish_hour;

    //Mostramos los shows actules para que sepa cuales están siendo ofrecidos.

    display_shows(shows);

    //Comenzamos a solicitar toda la información necesaria para crear el show,
    //haciendo validaciones naturalmente.

    movie_name = get_non_empty_line("Enter the name of the movie:");
    is_3D = yes_no_question("The movie is in 3D? (Enter 'Yes' if it is, or 'No' otherwise)");
    genre = get_non_empty_line("Enter the genre of the movie:");

    //Técnicamente en todos las warnings siguintes debería decir '...must be a positive integer...', pero
    //considero que eso no sería amigable con el usuario, por lo cual lo dejamos en '...must be a number...'.

    clasi = get_int_input("Enter the classification of the movie:", "Sorry, the classification must be a number.", 0);
    hour = get_int_input("At what time will the movie be shown? (In 24 hours format)", "Sorry, the hour must be a number between 0 and 23.", 0, 23);
    duration = get_int_input("Enter the duration of the movie: (In minutes)", "Sorry, the duration must be a number.", 1);
    finish_hour = (hour + ceiling(float(duration)/60.0f))%24;
    room = get_int_input("In which of the four rooms will the movie be shown? (Enter the room number)", "Sorry, the room number must be a number between 1 and 4.", 1, 4);

    //Verificamos que la sala indicada esté disponible en la hora ingresada por el admin,
    //para más información sobre este proceso ver la documentación de is_room_available().

    while (ask and !is_room_available(shows, room, hour, finish_hour, show_hour, show_finish_hour)) {

        system("cls");

        display_shows(shows);

        cout << endl << "  Sorry, room " << room << " is reserved from " << show_hour << ":00 to " << show_finish_hour << ":00, and you need to reserved" << endl;
        cout << "  the room " << finish_hour - hour << " hour(s), because your movie lasts " << duration << " minutes." << endl << endl;

        cout << "  Do you want to change the room or the hour?";
        ask = yes_no_question("Enter 'Yes' for change they, or 'No' for cancel the show)");

        if (ask) {

            system("cls");

            display_shows(shows);
            hour = get_int_input("At what time will the movie be shown? (In 24 hours format)", "Sorry, the hour must be a number between 0 and 23.", 0, 23);
            finish_hour = (hour + ceiling(float(duration)/60.0f))%24;
            room = get_int_input("In which of the four rooms will the movie be shown? (Enter the room number)", "Sorry, the room number must be a number between 1 and 4.", 1, 4);
        }
    }

    //Si salimos del ciclo while anterior y ask continua siedno true, es porque is_room_available()
    //retorno true, por lo cual procedemos a crear el show ingresado y almacenarlo en el vector de
    //shows, o simplemente no hacemos nada en caso de que ask sea false, pues significa que la
    //sala indicada estaba ocupada a la hora ingresada y el admin decidió cancelar la función
    //en lugar de reprogramarla.

    if (ask) {
        Show show(shows.size(), movie_name, is_3D, genre, clasi, hour, finish_hour, duration, room, 58);
        shows.push_back(show);
    }
}

bool get_sales_record(array<unsigned int, 6> &sales, unsigned long long int &total, const unsigned int &seed) {

    //Carga la información del archivo sales_record.txt en el vector sales que se menciona
    //en el vídeo de Youtube. Retorna true si el archivo pudo ser abierto o false en caso
    //constraio.

    //Nota: La variable sales pudo haber sido tranquilamente un array convencional, es decir, int sales[6],
    //sin embargo, como mencionó el profesor Augusto durante una de las asesorías, es bueno mostrar
    //que uno es capaz de resolver los mismos problemas, en este caso almacenar exactamente 6 datos
    //enteros, de varias formas, por lo cual decidí utilizar un array de la STL para éste propósito
    //pues serán siempre 6 datos enteros.

    string text;
    unsigned long long int len, index = 62, end_index, aux;

    if (get_text("../DesafioEvaluativoV2/data/sales_record.txt", text, len)) {

        //Decodificamos la información almacenada en sales_record.txt

        decode(text, len, seed);

        //El registro de ventas tiene un formato simple y está hecho en parte para
        //que pueda ser facilmente visualizado al decodificar el archivo y abrirlo
        //con un programa como bloc de notas. El formato es el siguiente:

        //TODAY'S SALES RECORD:
        //----------------------------3D
        //General: GE3D --- $VAL_GE3D
        //VibroSound: VI3D --- $VAL_VI3D
        //Gold: GO3D --- $VAL_GO3D
        //----------------------------2D
        //General: GE2D --- $VAL_GE2D
        //VibroSound: VI2D --- $VAL_VI2D
        //Gold: GO2D --- $VAL_GO2D
        //----------------------Total: $TOTAL
        //

        //Donde GE corresponde a General, VI corresponde a VibroSound y GO corresponde a Gold,
        //GE3D corresponde a la cantidad de asientos vendidos en General para películas
        //3D, VI3D, a la cantidad de asientos vendido en VibroSound para películas 3D,
        //y así análogamente; mientras que VAl_GE3D corresponde al valor equivalente
        //de haber vendido GE3D asientos en General para películas 3D, y así análogamente.
        //TOTAL corresponde a el valor total de las ventas.

        //Nota: Al final siempre se debe dejar un salto de línea.

        //Ahora, explicado el formato, no cuesta mucho entender el código
        //utilizado para tomar los datos del archivo.

        for (short int i = 0; i < 6; i++) {
            end_index = text.find(' ', index);
            str2int(text.substr(index, end_index - index), aux);
            sales.at(i) = aux;
            index = text.find('\n', end_index);
            index = text.find(' ', index) + 1;
        }

        index++;
        end_index = text.find('\n', index);
        str2int(text.substr(index, end_index - index), aux);
        total = aux;

        return true;
    }
    else {
        cout << "  Sorry, the sales_record.txt file could not be opened" << endl << endl;
        return false;
    }
}

void sales_record_line(string &text, string sale_type, unsigned int price, unsigned long long int num) {

    //Esta función añade al string text, recibido por referencia, una de las líneas
    //del archivo sales_record.txt. Es sólo con el propósito de reutilizar el código
    //pues es usado varias veces.

    text.append(sale_type);
    text.append(": ");
    text.append(to_string(num));
    text.append(" --- $");
    text.append(to_string(num*price));
    text.push_back('\n');
}

void save_sales(const array<unsigned int, 6> &sales, const unsigned long long int &total, const unsigned int &seed) {

    //Esta función almacena en el string llamado text todo el contenido del archivo sales_record.txt
    //con ayuda de la función sales_record_line(), para luego poder guardar el contenido,
    //previametne codificado, de text en el archivo sales_record.txt.

    //Las tarifas son las siguientes:

    //------------------------3D
    //General --- $11700
    //VibroSound --- $13900
    //Gold --- $22900

    //------------------------2D
    //General --- $8700
    //VibroSound --- $10900
    //Gold --- $19900

    //Nótese que el 3D cuesta $3000 pesos más.

    string text;

    text = "TODAY'S SALES RECORD:\n----------------------------3D\n";
    sales_record_line(text, "General", 11700, sales.at(0));
    sales_record_line(text, "VibroSound", 13900, sales.at(1));
    sales_record_line(text, "Gold", 22900, sales.at(2));

    text.append("----------------------------2D\n");
    sales_record_line(text, "General", 8700, sales.at(3));
    sales_record_line(text, "VibroSound", 10900, sales.at(4));
    sales_record_line(text, "Gold", 19900, sales.at(5));

    text.append("----------------------Total: $");
    text.append(to_string(total));
    text.push_back('\n');

    code(text, text.length(), seed);
    save_text("../DesafioEvaluativoV2/data/sales_record.txt", text, text.length());
}

void reset_sales_record(array<unsigned int, 6> &sales, unsigned long long int &total, unsigned int seed) {

    //Esta función simplemente resetea todo el registro de ventas almacenado en
    //el array sales, es decir, los coloca en 0 junto con el valor de la variable,
    //recibida por referencia, total; luego procede a actualizar ésta información
    //en el archivo sales_record.txt.

    total = 0;
    for (short int i = 0; i < 6; i++) sales.at(i) = 0;
    save_sales(sales, total, seed);
}
