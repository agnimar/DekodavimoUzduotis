#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <cstdlib>

using namespace std;

struct Data{
    char eilesNr;
    char konst;
};

int main()
{
    Data decryptor[16];
    char prog_mem[256];
    int size = 0;

    stringstream ss;

    ifstream dec_file("decryptor.bin", ios::in | ios::binary);
    if (dec_file.is_open()) {
        Data byte;

        while(dec_file.read((char*)&byte, sizeof(Data)))
        {
            decryptor[size].eilesNr = byte.eilesNr;
            decryptor[size].konst = byte.konst;
            //ss << setfill ('0') << setw(2) << hex << (int)byte.eilesNr << '\t' << hex << (int)byte.konst << endl;
            size++;
        }
        //cout << ss.str() << '\n';
        dec_file.close();
    }
    else {
        cerr << "Unable to open file\n";
    }

    int i_reg_1;
    int i_reg_2;
    unsigned char registrai[16];
    ifstream file("q1_encr.txt");

    char baitas;
    int zeroflag = 0, ieof_flag = 0;
    int x, y;

    ofstream fr("rezultatai.txt");
    fr.close();

    // MAIN LOGIC
    int i = 0;
    while(1)
    {
        i_reg_1 = decryptor[i].eilesNr;
        i_reg_2 = decryptor[i].konst;

        switch (i_reg_1)
        {
            case 0x01:
                registrai[i_reg_2]++;
                break;
            case 0x02:
                registrai[i_reg_2]--;
                break;
            case 0x03:
                x = i_reg_2 & 0b1111;
                i_reg_2 = i_reg_2 >> 4;
                y = i_reg_2 & 0b1111;
                registrai[x] = registrai[y];
                if(registrai[x] == 0)
                    zeroflag = 1;
                else zeroflag = 0;
                break;
            case 0x04:
                registrai[0] = i_reg_2;

                break;
            case 0x05:
                registrai[i_reg_2] = registrai[i_reg_2] << 1;

                break;
            case 0x06:
                registrai[i_reg_2] = registrai[i_reg_2] >> 1;
                break;
            case 0x07:
                i+= i_reg_2/2-1;
                break;
            case 0x08:
                if(zeroflag == 1)
                    i+= i_reg_2*0.5 -1;
                break;
            case 0x09:
                if(zeroflag == 0)
                    i+= i_reg_2*0.5 -1;
                break;
            case 0x0A:
                if(ieof_flag == 1)
                    i+= i_reg_2/2 - 1;
                break;
            case 0x0B:
                return 0;
                break;
            case 0x0C:
                x = i_reg_2 & 0b1111;
                i_reg_2 = i_reg_2 >> 4;
                y = i_reg_2 & 0b1111;
                registrai[x] = registrai[x] + registrai[y];
                if(registrai[x] == 0)
                    zeroflag = 1;
                else zeroflag = 0;
                break;
            case 0x0D:
                x = i_reg_2 & 0b1111;
                i_reg_2 = i_reg_2 >> 4;
                y = i_reg_2 & 0b1111;
                registrai[x] = registrai[x] - registrai[y];
                if(registrai[x] == 0)
                    zeroflag = 1;
                else zeroflag = 0;
                break;
            case 0x0E:
                x = i_reg_2 & 0b1111;
                i_reg_2 = i_reg_2 >> 4;
                y = i_reg_2 & 0b1111;
                registrai[x] = registrai[x] ^ registrai[y];
                if(registrai[x] == 0)
                    zeroflag = 1;
                else zeroflag = 0;
                break;
            case 0x0F:
                x = i_reg_2 & 0b1111;
                i_reg_2 = i_reg_2 >> 4;
                y = i_reg_2 & 0b1111;
                registrai[x] = registrai[x] | registrai[y];
                if(registrai[x] == 0)
                    zeroflag = 1;
                else zeroflag = 0;
                break;
            case 0x10:
                x = i_reg_2 & 0b1111;
                if(!file.eof())
                {
                    file>>registrai[x];
                }
                if(file.eof())
                {
                    ieof_flag = 1;
                }
                if(registrai[x] == 0)
                    zeroflag = 1;
                else zeroflag = 0;
                break;
            case 0x11:
                ofstream fr("rezultatai.txt", ios::app);
                x = i_reg_2 & 0b1111;
                fr<< registrai[x];
                fr.close();
                break;
            }
        i++;
    }
    file.close();
}
