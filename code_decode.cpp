#include "code_decode.h"
#include <iostream>
#include <fstream>

using namespace std;

//Estas son todas las implementaciones de las funciones para codificar y decodificar texto.

//El método de códificación utilizado es el método 1 expuesto en la práctica 3 del laboratorio;
//como sabemos en este tipo de codificación hay que pasar el texto a binario, éste binario
//será almacenado en el arreglo de bools llamdado bin[].

//NOTA IMPORTANTE: La carpeta 'data' que contiene todos los .txt que sirven
//como bases de datos para el programa, debe estar dentro de la carpeta
//del proyecto que lleva el nombre de 'DesafioEvaluativoV2'. De lo contrario
//el programa no podrá encontrar esas bases de datos.

void char2bin(char chr, bool bin[], unsigned long long int pos) {

    //Convertimos el carácter chr a binario y lo almacena en 8 posiciones
    //contiguas del arreglo bin[] comenzando en el indice pos.

    for (unsigned int i = 0; i < 8; i++) {
        bin[(7 - i) + pos] = ((chr >> i) & 0x01);
    }
}

void not_bin(bool bin[], unsigned long long int pos, unsigned int pass, unsigned int n) {

    //Negamos un binario de n bits cada pass de digitos. El binario corresponde
    //a las n posiciones contiguas de bin[] comenzando en el indice pos.

    for (unsigned int i = 0; i < n; i++) {
        if ((i + 1)%pass == 0) bin[pos + i] = !bin[pos + i];
    }
}

void count_bin(bool bin[], unsigned long long int pos, unsigned int n, unsigned int &zeros, unsigned int &ones) {

    //Contamos los ceros y unos del binario de n bits que se almacena en
    //las n posiciones contiguas de bin[] comenzando en el indice pos.
    //Los ceros y unos contados son almacenados en las variables zeros
    //y ones, que son recibidas por referencia.

    for (unsigned int i = 0; i < n; i++) {
        if (bin[i + pos]) ones++;
        else zeros++;
    }
}

void code_method(bool bin[], unsigned long long int len, unsigned int n) {

    //Ejecutamos el método de codificación 1 sobre el arreglo de bools bin[].
    //La varibale len es la longitud de bin[] y n es la cantidad bits
    //que tendrá cada bloque de bits según el método de codificación 1.

    unsigned long long int pos = n;
    unsigned int zeros = 0, ones = 0, pass;

    //En el caso en que n sea mayor o igual a len, simplemente negamos los bits.

    if (n >= len) not_bin(bin, 0, 1, len);
    else {

        //En caso contrario procedemos a recorrer los grupos de n bits,
        //pero primero tenemos que negar los primeros n bits según dicta el método 1,
        //aunque antes de hacerlo, contamos la cantidad de ceros y unos presentes
        //para saber cada cuanto negar los bits del grupo de n bits posterior.

        //Contamos los ceros y unos.
        count_bin(bin, 0, n, zeros, ones);

        //Negamos los bits.
        not_bin(bin, 0, 1, n);

        while(pos <= (len - n)) {

            //Decidimos cada cuanto debemos negar los bits del grupo de n bits actual
            //segun la cantidad de ceros y unos del grupo de n bits previo.
            if (zeros > ones) pass = 2;
            else if (zeros < ones) pass = 3;
            else pass = 1;

            //Reiniciamos las variables ya que son utilizadas por referencia.
            zeros = 0;
            ones = 0;

            //Contamos los ceros y unos del grupo de n bits de la posición actual.
            count_bin(bin, pos, n, zeros, ones);

            //Modificamos el grupo de n bits según el valor de pass.
            not_bin(bin, pos, pass, n);
            pos += n;
        }

        //Al termianr el ciclo anterior, si la cantidad de bits no es multiplo de n,
        //pos será diferente de len, por lo cual procesamos el último grupo de bits
        //de longitud menor a len - pos.

        if (pos != len) {
            if (zeros > ones) pass = 2;
            else if (zeros < ones) pass = 3;
            else pass = 1;
            not_bin(bin, pos, pass, len - pos);
        }
    }
}

bool get_text(string path, string &text, unsigned long long int &len) {

    //Leemos el archivo .txt de nombre file_name y almacena por referencia
    //el texto contenido en él en el string text, además almacenamos su
    //longitud en la variable len recibida por referencia.
    //Retornamos true si el archivo pudo ser abirto, o false en caso contrario
    //y no modificamos ni text ni len.

    //El archivo file_name es abirto con ios::binary para leer los binarios
    //almacenados sin que sean interpretados por el método get().

    //NOTA IMPORTANTE: La carpeta 'data' que contiene todos los .txt que sirven
    //como bases de datos para el programa, debe estar dentro de la carpeta
    //del proyecto que lleva el nombre de 'DesafioEvaluativoV2'. De lo contrario
    //el programa no podrá encontrar esas bases de datos.

    fstream file(path, ios::in | ios::ate | ios::binary);

    if (file.is_open()) {
        char chr;
        text = "";

        //Recuperamos la longitud del texto.
        len = file.tellg();

        file.seekg(0);
        while (file.get(chr)) text.push_back(chr);

        file.close();
        return true;
    }
    else return false;
}

void text2bin(string text, unsigned long long int len, bool *&bin) {

    //Convertimos el texto del string text en binario, reservamos mediante
    //memoria dinámica 8 veces la longitud del texto para almacenar
    //el binario. Almacenamos la dirección de éste arreglo de bools
    //en el puntero bin ingresado por referencia. La variable len
    //es la longitud del string text.

    bin = new bool[8*len];
    unsigned long long int pos = 0;

    for (unsigned long long int i = 0; i < len; i++) {
        char2bin(text[i], bin, pos);
        pos += 8;
    }
}

short int bin2dec(bool bin[], unsigned long long pos) {

    //Retornamos el entero correspondiente al binario almacenado en
    //las 8 posiciones contiguas de bin[] comenzando en el indice pos.

    short int num = 0, two_pow = 1;
    for (unsigned int i = 0; i < 8; i++) {
        if (bin[(7 - i) + pos]) num += two_pow;
        two_pow *= 2;
    }
    return num;
}

void bin2text(bool bin[], unsigned long long len, string &text) {

    //Convertimos los binarios de 8 bits almacenados en bin[] en carácteres
    //y los concatena dentro del string text ingresado por referencia.
    //La variable len es la londitud de text.

    unsigned long long pos = 0;
    for (unsigned long long i = 0; i < len; i += 8) {
        text[pos] = char(bin2dec(bin, i));
        pos++;
    }
}

void save_text(string path, string text, unsigned long long int len) {

    //Guardamos el texto del string text en el archivo de nombre file_name.
    //La varibale len es la longitud de text.

    //El archivo file_name es abirto con ios::binary para guardar los carácteres
    //de text sin que sean interpretados.

    //NOTA IMPORTANTE: La carpeta 'data' que contiene todos los .txt que sirven
    //como bases de datos para el programa, debe estar dentro de la carpeta
    //del proyecto que lleva el nombre de 'DesafioEvaluativoV2'. De lo contrario
    //el programa no podrá encontrar esas bases de datos.

    fstream file(path, ios::out | ios::binary);
    for (unsigned long long int i = 0; i < len; i++) file << text[i];
    file.close();
}

void decode_method(bool bin[], unsigned long long len, unsigned int n) {

    //Ejecutamos el método de decodificación 1 sobre el arreglo de bools bin[].
    //La varibale len es la longitud de bin[] y n es la cantidad bits
    //que tendrá cada bloque de bits según el método de decodificación 1.

    unsigned long long int pos = n;
    unsigned int zeros = 0, ones = 0, pass;

    //En el caso en que n sea mayor o igual a len, simplemente negamos los bits.

    if (n >= len) not_bin(bin, 0, 1, len);
    else {

        //El proceso es similar a la codificación, sólo que ahora se cuentan
        //los ceros y unos del grupo de n bits actual LUEGO de decodificarlos.

        not_bin(bin, 0, 1, n);
        count_bin(bin, 0, n, zeros, ones);

        while(pos <= (len - n)) {

            //Decidimos cada cuanto debemos negar los bits del grupo de n bits actual
            //segun la cantidad de ceros y unos del grupo de n bits previo después de
            //haber sido descodificado.
            if (zeros > ones) pass = 2;
            else if (zeros < ones) pass = 3;
            else pass = 1;

            //Reiniciamos las variables ya que son utilizadas por referencia.
            zeros = 0;
            ones = 0;

            //Modificamos el grupo de n bits según el valor de pass.
            not_bin(bin, pos, pass, n);

            //Contamos los ceros y unos del grupo de n bits de la posición actual
            //luego de ser decodificado.
            count_bin(bin, pos, n, zeros, ones);
            pos += n;
        }

        //De nuevo, si len no era multiplo de n, pos será diferente de len,
        //por lo cual procesamos el último bloque de len - pos bits.

        if (pos != len) {
            if (zeros > ones) pass = 2;
            else if (zeros < ones) pass = 3;
            else pass = 1;
            not_bin(bin, pos, pass, len - pos);
        }
    }
}

void code(string &text, unsigned long long int len, unsigned seed) {

    //Codificamos el texto del string text recibido por referencia.
    //La variable seed corresponde a la semilla de condificación,
    //es decir, a el valor de n en el métodod 1. La variable len
    //es la longitud de text.

    bool *bin = nullptr;
    text2bin(text, len, bin);
    code_method(bin, 8*len, seed);
    bin2text(bin, 8*len, text);
    delete[] bin;
}

void decode(string &text, unsigned long long int len, unsigned seed) {

    //Decodificamos el texto del string text recibido por referencia.
    //La variable seed corresponde a la semilla de decondificación,
    //es decir, a el valor de n en el métodod 1. La variable len
    //es la longitud de text.

    bool *bin = nullptr;
    text2bin(text, len, bin);
    decode_method(bin, 8*len, seed);
    bin2text(bin, 8*len, text);
    delete[] bin;
}

bool code_file(string path_nat, string path_code, unsigned int seed) {

    string text;
    unsigned long long int len;
    if (get_text(path_nat, text, len)) {

        code(text, len, seed);

        save_text(path_code, text, len);
        return true;
    }
    else {
        cout << "  Sorry, the native file could not be opened" << endl << endl;
        return false;
    }
}

bool decode_file(string path_code, string path_nat, unsigned int seed) {

    string text;
    unsigned long long int len;
    if (get_text(path_code, text, len)) {

        decode(text, len, seed);

        save_text(path_nat, text, len);
        return true;
    }
    else {
        cout << "  Sorry, the code file could not be opened" << endl << endl;
        return false;
    }
}
