#include "code_decode.h"

//Estas son todas las implementaciones de las funciones para codificar y decodificar, texto y archivos.

//El método de códificación utilizado es el método 1 expuesto en la práctica 3 del laboratorio;
//como sabemos en este tipo de codificación hay que pasar el texto a binario, éste binario
//será almacenado en el arreglo de bools llamdado bin[].

void char2bin(char chr, bool bin[], unsigned long long int pos) {

    //Convierte el carácter chr a binario y lo almacena en 8 posiciones
    //contiguas del arreglo bin[] comenzando en el indice pos.

    for (unsigned int i = 0; i < 8; i++) {
        bin[(7 - i) + pos] = ((chr >> i) & 0x01);
    }
}

void not_bin(bool bin[], unsigned long long int pos, unsigned int pass, unsigned int n) {

    //Niega un binario de n bits cada pass de digitos. El binario corresponde
    //a las n posiciones contiguas de bin[] comenzando en el indice pos.

    for (unsigned int i = 0; i < n; i++) {
        if ((i + 1)%pass == 0) bin[pos + i] = !bin[pos + i];
    }
}

void count_bin(bool bin[], unsigned long long int pos, unsigned int n, unsigned int &zeros, unsigned int &ones) {

    //Contamos los ceros y unos del binario de n bits que se almacena en
    //las n posiciones contiguas de bin[] comenzando en el indice pos.
    //Los ceros y unos contados se modifican por referencia.

    for (unsigned int i = 0; i < n; i++) {
        if (bin[i + pos]) ones++;
        else zeros++;
    }
}

void code_method1(bool bin[], unsigned long long int len, unsigned int n) {

    //Ejecuta el método de codificación 1 sobre el arreglo de bools bin[].
    //La varibale len es la longitud de bin[] y n.

    unsigned long long int pos = n;
    unsigned int zeros = 0, ones = 0, pass;
    //Si no se puede particionar el arreglo en grupos de n, n es mayor o igual a len
    if (n >= len) not_bin(bin, 0, 1, unsigned(len));
    else {
        //Contamos los ceros y unos del grupo de n bits de la posición actual
        count_bin(bin, 0, n, zeros, ones);
        //Los negamos todos
        not_bin(bin, 0, 1, n);
        while(pos <= (len - n)) {

            //Decidimos primero que hacer con el grupo de n bits de la posición actual
            if (zeros > ones) pass = 2;
            else if (zeros < ones) pass = 3;
            else pass = 1;

            zeros = 0;
            ones = 0;

            //Contamos los ceros y unos del grupo de n bits de la posición actual
            count_bin(bin, pos, n, zeros, ones);
            //Modificamos el grupo de n bits según el valor de pass
            not_bin(bin, pos, pass, n);
            pos += n;
        }
        //Si la cantidad de bits no es multiplo de n, cuando no queden más grupos
        //de n bits, pos no será igual a len, por lo cual procesamos el último
        //grupo de bits de longitud menor a n
        if (pos != len) {
            if (zeros > ones) pass = 2;
            else if (zeros < ones) pass = 3;
            else pass = 1;
            not_bin(bin, pos, pass, unsigned(len - pos));
        }
    }
}

bool get_text(string file_name, string &text, unsigned long long int &len) {

    fstream file(file_name, ios::in | ios::ate | ios::binary);
    if (file.is_open()) {
        char chr;
        text = "";
        len = unsigned(file.tellg());

        file.seekg(0);
        while (file.get(chr)) text.push_back(chr);

        file.close();
        return true;
    }
    else return false;
}

void text2bin(string text, unsigned long long int len, bool *&bin) {

    bin = new bool[8*len];
    unsigned long long int pos = 0;
    for (unsigned long long int i = 0; i < len; i++) {
        char2bin(text[i], bin, pos);
        pos += 8;
    }
}

short int bin2dec(bool bin[], unsigned long long pos) {
    //Retorna el entero correspondiente al binario almacenado en
    //la posición pos del arreglo bin

    short int num = 0, two_pow = 1;
    for (unsigned int i = 0; i < 8; i++) {
        if (bin[(7 - i) + pos]) num += two_pow;
        two_pow *= 2;
    }
    return num;
}

void bin2text(bool bin[], unsigned long long len, string &text) {

    unsigned long long pos = 0;
    for (unsigned long long i = 0; i < len; i += 8) {
        text[pos] = char(bin2dec(bin, i));
        pos++;
    }
}

bool save_text(string file_name, string text, unsigned long long int len) {

    fstream file(file_name, ios::out | ios::binary);
    if (file.is_open()) {
        for (unsigned long long int i = 0; i < len; i++) file << text[i];
        file.close();
        return true;
    }
    else return false;
}

void decode_method1(bool bin[], unsigned long long len, unsigned int n) {
    //Ejecuta el método de decodificación 1 sobre el arreglo de bools bin.

    unsigned long long int pos = n;
    unsigned int zeros = 0, ones = 0, pass;
    if (n >= len) not_bin(bin, 0, 1, unsigned(len));
    else {
        not_bin(bin, 0, 1, n);
        count_bin(bin, 0, n, zeros, ones);
        while(pos <= (len - n)) {

            //El proceso es similar, solo que ahora se cuentan los ceros y unos del
            //grupo de n bits actual LUEGO de decodificar
            if (zeros > ones) pass = 2;
            else if (zeros < ones) pass = 3;
            else pass = 1;

            zeros = 0;
            ones = 0;

            not_bin(bin, pos, pass, n);
            count_bin(bin, pos, n, zeros, ones);
            pos += n;
        }
        if (pos != len) {
            if (zeros > ones) pass = 2;
            else if (zeros < ones) pass = 3;
            else pass = 1;
            not_bin(bin, pos, pass, unsigned(len - pos));
        }
    }
}

void code(string &text, unsigned long long int len, unsigned seed) {

    bool *bin = nullptr;
    text2bin(text, len, bin);
    code_method1(bin, 8*len, seed);
    bin2text(bin, 8*len, text);
    delete[] bin;
}

void decode(string &text, unsigned long long int len, unsigned seed) {

    bool *bin = nullptr;
    text2bin(text, len, bin);
    decode_method1(bin, 8*len, seed);
    bin2text(bin, 8*len, text);
    delete[] bin;
}

bool code_file(string file_nat, string file_code, unsigned int seed) {

    string text;
    unsigned long long int len;
    if (get_text(file_nat, text, len)) {

        code(text, len, seed);

        if (!save_text(file_code, text, len)) {
            cout << "Sorry, the code file could not be opened" << endl << endl;
            return false;
        }
        return true;
    }
    else {
        cout << "Sorry, the native file could not be opened" << endl << endl;
        return false;
    }
}

bool decode_file(string file_code, string file_nat, unsigned int seed) {

    string text;
    unsigned long long int len;
    if (get_text(file_code, text, len)) {

        decode(text, len, seed);

        if (!save_text(file_nat, text, len)) {
            cout << "Sorry, the native file could not be opened" << endl << endl;
            return false;
        }
        return true;
    }
    else {
        cout << "Sorry, the code file could not be opened" << endl << endl;
        return false;
    }
}
