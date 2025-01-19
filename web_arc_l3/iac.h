//
// Created by toor on 22.10.2020.
//

#ifndef UNTITLED_IAC_H
#define UNTITLED_IAC_H

// Schemat funkcji dekodera IAC:

void decodeCommand(unsigned char code) {
    switch (code) {
        case 240: cout << "\nSE"; break;
        case 250: cout << "\nSB"; break;
        case 251: cout << "\nWILL"; break;
        case 252: cout << "\nWONT"; break;
        case 253: cout << "\nDO"; break;
        case 254: cout << "\nDONT"; break;
    }
}

void decodeOption(unsigned char opt) {
    switch (opt) {
        case 1: cout << "\nECHO"; break;
        case 3: cout << "\nSUP GO AHEAD"; break;
        case 5: cout << "\nSTATUS"; break;
        case 6: cout << "\nTIMING MARK"; break;
        case 24: cout << "\nTERMINAL TYPE"; break;
    }
}

void decodeIAC(unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        if (data[i] == 255) {
            cout << "\nIAC";
            i++;
            decodeCommand(data[i]);
            i++;
            decodeOption(data[i]);
        }
    }
}


#endif //UNTITLED_IAC_H
