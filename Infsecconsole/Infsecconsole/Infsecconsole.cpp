#include <iostream>
#include <math.h>
#include <map>
using namespace std;

#define SUBKEY(i) (subkeys+(i*5))
#define MALLOC(n, type) ((type *) malloc (n * sizeof(type)))

int n = 5;
int l = 8;
int p = 11;
int lk = 4;

void aes_linear_enc(int*, int*, int*);
void aes_linear_dec(int*, int*, int*);
void aes_nearlinear_enc(int*, int*, int*);
void aes_nearlinear_dec(int*, int*, int*);
void substitution(int*);
void transposition(int*);
void linear_function(int*);
void keysum(int*, int*);
void substitution_inv(int*);
void transposition_inv(int*);
void linear_function_inv(int*);
void keysum_inv(int*, int*);
void printarray(int*, int);
void swap(int*, int*);
void keygen(int*, int*);
void substitution2(int*);
void substitution2_inv(int*);

int main()
{
    int plaintext[8] = { 1,0,0,0,0,0,0,0 };
    int* key = MALLOC(9 , int);
    int* subkeys = MALLOC(6 * 5, int); //member i, j is subkeys[i*5+j]
    keygen(key, subkeys);
    cout << "key: ";
    printarray(key, 9);
    cout << "plaintext: ";
    printarray(plaintext, 8);
    aes_nearlinear_enc(plaintext, key, subkeys);
    cout << "encrypted plaintext: ";
    printarray(plaintext, 8);
    //cout << "decryption" << endl;
    aes_nearlinear_dec(plaintext, key, subkeys);
    cout << "decrypted plaintext: ";
    printarray(plaintext, 8);
    free(key);
    free(subkeys);
    return 0;
}

void keygen(int* key, int* subkeys)
{
    //key generation
    double kk;
    int k;
    srand(time(NULL));

    for (int i = 1; i <9; i++)
    {
        kk = (rand() / (double)RAND_MAX)*(p);
        k = (int) kk;
        key[i] = k;
    }
    key[0]=0;
    /*
    key[0] = 0; //dummy value, just 0 padding to have array 1 indexed
    key[1] = 1;
    key[2] = 0;
    key[3] = 0;
    key[4] = 0;
    key[5] = 0;
    key[6] = 0;
    key[7] = 0;
    key[8] = 0;*/
    
    //subkeys
    subkeys[0] = 0;
    subkeys[1] = key[1];
    subkeys[2] = key[3];
    subkeys[3] = key[5];
    subkeys[4] = key[7];
    subkeys[5] = 0;
    subkeys[6] = key[1];
    subkeys[7] = key[2];
    subkeys[8] = key[3];
    subkeys[9] = key[4];
    subkeys[10] = 0;
    subkeys[11] = key[1];
    subkeys[12] = key[4];
    subkeys[13] = key[5];
    subkeys[14] = key[8];
    subkeys[15] = 0;
    subkeys[16] = key[1];
    subkeys[17] = key[4];
    subkeys[18] = key[6];
    subkeys[19] = key[7];
    subkeys[20] = 0;
    subkeys[21] = key[1];
    subkeys[22] = key[3];
    subkeys[23] = key[6];
    subkeys[24] = key[8];
    subkeys[25] = 0;
    subkeys[26] = key[3];
    subkeys[27] = key[4];
    subkeys[28] = key[5];
    subkeys[29] = key[6];
}

void aes_linear_enc(int* plaintext, int* key, int* subkeys)
{
    for (int i = 0; i < n - 1; i++)
    {
        keysum(plaintext, subkeys+(i*5));
        //printarray(plaintext, 8);
        substitution(plaintext);
        //printarray(plaintext, 8);
        transposition(plaintext);
        //printarray(plaintext, 8);
        linear_function(plaintext);
        //printarray(plaintext, 8);
    }
    keysum(plaintext, subkeys+((n - 1)*5));
    //printarray(plaintext, 8);
    substitution(plaintext);
    //printarray(plaintext, 8);
    transposition(plaintext);
    //printarray(plaintext, 8);
    keysum(plaintext, subkeys+(n*5));
    //printarray(plaintext, 8);
}

void aes_linear_dec(int* cyphertext, int* key, int* subkeys)
{
    keysum_inv(cyphertext, subkeys + (n * 5));
    //printarray(cyphertext, 8);
    transposition_inv(cyphertext);
    //printarray(cyphertext, 8);
    substitution_inv(cyphertext);
    //printarray(cyphertext, 8);
    keysum_inv(cyphertext, subkeys + ((n - 1) * 5));
    //printarray(cyphertext, 8);
    for (int i = n - 2; i >= 0; i--)
    {
        linear_function_inv(cyphertext);
        //printarray(cyphertext, 8);
        transposition_inv(cyphertext);
        //printarray(cyphertext, 8);
        substitution_inv(cyphertext);
        //printarray(cyphertext, 8);
        keysum_inv(cyphertext, subkeys + (i * 5));
        //printarray(cyphertext, 8);
    }
}

void aes_nearlinear_enc(int* plaintext, int* key, int* subkeys)
{
    for (int i = 0; i < n - 1; i++)
    {
        keysum(plaintext, subkeys + (i * 5));
        //printarray(plaintext, 8);
        substitution2(plaintext);
        //printarray(plaintext, 8);
        transposition(plaintext);
        //printarray(plaintext, 8);
        linear_function(plaintext);
        //printarray(plaintext, 8);
    }
    keysum(plaintext, subkeys + ((n - 1) * 5));
    //printarray(plaintext, 8);
    substitution2(plaintext);
    //printarray(plaintext, 8);
    transposition(plaintext);
    //printarray(plaintext, 8);
    keysum(plaintext, subkeys + (n * 5));
    //printarray(plaintext, 8);
}

void aes_nearlinear_dec(int* cyphertext, int* key, int* subkeys)
{
    keysum_inv(cyphertext, subkeys + (n * 5));
    //printarray(cyphertext, 8);
    transposition_inv(cyphertext);
    //printarray(cyphertext, 8);
    substitution2_inv(cyphertext);
    //printarray(cyphertext, 8);
    keysum_inv(cyphertext, subkeys + ((n - 1) * 5));
    //printarray(cyphertext, 8);
    for (int i = n - 2; i >= 0; i--)
    {
        linear_function_inv(cyphertext);
        //printarray(cyphertext, 8);
        transposition_inv(cyphertext);
        //printarray(cyphertext, 8);
        substitution2_inv(cyphertext);
        //printarray(cyphertext, 8);
        keysum_inv(cyphertext, subkeys + (i * 5));
        //printarray(cyphertext, 8);
    }
}

void linear_function(int* text)
{
    int textcopy[8];
    for (int i = 0; i < 8; i++)
    {
        textcopy[i] = text[i];
    }
    text[0] = 2 * textcopy[0] + 5 * textcopy[4];
    text[1] = 2 * textcopy[1] + 5 * textcopy[5];
    text[2] = 2 * textcopy[2] + 5 * textcopy[6];
    text[3] = 2 * textcopy[3] + 5 * textcopy[7];
    text[4] = 1 * textcopy[0] + 7 * textcopy[4];
    text[5] = 1 * textcopy[1] + 7 * textcopy[5];
    text[6] = 1 * textcopy[2] + 7 * textcopy[6];
    text[7] = 1 * textcopy[3] + 7 * textcopy[7];

    for (int i = 0; i < 8; i++)
    {
        text[i] = text[i] % p;
    }
}

void linear_function_inv(int* text)
{
    double textcopy[8];
    for (int i = 0; i < 8; i++)
    {
        /*if (text[i] % 2 != 0)
        {
            text[i] += p;
        }*/
        textcopy[i] = text[i];
    }
    text[0] = 2 * textcopy[0] + 8 * textcopy[4];
    text[1] = 2 * textcopy[1] + 8 * textcopy[5];
    text[2] = 2 * textcopy[2] + 8 * textcopy[6];
    text[3] = 2 * textcopy[3] + 8 * textcopy[7];
    text[4] = 6 * textcopy[0] + 10 * textcopy[4];
    text[5] = 6 * textcopy[1] + 10 * textcopy[5];
    text[6] = 6 * textcopy[2] + 10 * textcopy[6];
    text[7] = 6 * textcopy[3] + 10 * textcopy[7];

    for (int i = 0; i < 8; i++)
    {
        text[i] = text[i] % p;
    }
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void transposition(int* text)
{
    swap(text + 4, text + 7);
    swap(text + 5, text + 6);
}

void transposition_inv(int* text)
{
    transposition(text);
}

void substitution(int* text)
{
    for (int i = 0; i < 8; i++)
    {
        text[i] = (2 * text[i]) % p;
    }
}

void substitution_inv(int* text)
{
    for (int i = 0; i < 8; i++)
    {
        if (text[i] % 2 != 0)
        {
            text[i] += p;
        }
        text[i] = (text[i] / 2) % p;
    }
}

void keysum(int* text, int* subkey)
{
    for (int i = 0; i < 8; i++)
    {
        //cout << "summing index " << (i % 4 + 1)<< endl;
        //cout << "summing with " << subkey[(i % 4 + 1)] << endl;
        text[i] = (text[i] + subkey[(i % 4 + 1)]) % p;
    }
}

void keysum_inv(int* text, int* subkey)
{
    for (int i = 0; i < 8; i++)
    {
        text[i] = (text[i] - subkey[(i % 4 + 1)] + 11) % p;
    }
}

void printarray(int* a, int len)
{
    for (int i = 0; i < len; i++)
        cout << a[i];
    cout << endl;
}

void substitution2(int* text)
{
    map<int, int> m;
    m[0] = 0;
    m[1] = 2;
    m[2] = 4;
    m[3] = 8;
    m[4] = 6;
    m[5] = 10;
    m[6] = 1;
    m[7] = 3;
    m[8] = 5;
    m[9] = 7;
    m[10] = 9;

    for (int i = 0; i < 8; i++)
    {
        text[i] = m[text[i]];
    }
}

void substitution2_inv(int* text)
{
    map<int, int> m;
    m[0] = 0;
    m[1] = 6;
    m[2] = 1;
    m[3] = 7;
    m[4] = 2;
    m[5] = 8;
    m[6] = 4;
    m[7] = 9;
    m[8] = 3;
    m[9] = 10;
    m[10] = 5;

    for (int i = 0; i < 8; i++)
    {
        text[i] = m[text[i]];
    }
}