#include "aes.hpp"

#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

using namespace bigo::ml::bvt;
using namespace std;

constexpr unsigned char AESCipher::AES_SBOX[16 * 16];
constexpr unsigned char AESCipher::AES_INV_SBOX[16 * 16];
constexpr unsigned char AESCipher::AES_RCON[11 * 4];


void AESCipher::initCipher(unsigned char *key, int keySize)
{
    setKey(keySize);
    memcpy(&m_key[0], key, keySize);
    keyExpansion();
}


// aes encrypt
void AESCipher::cipher(unsigned char *input, unsigned char *output)
{
    int i;
    memset(&m_state[0][0], 0, 16);
    for (i = 0; i < 4 * m_Nb; i++)
    {
        m_state[i % 4][i / 4] = input[i];
    }
    addRoundKey(0);

    for (int round = 1; round <= (m_Nr - 1); ++round)
    {
        subBytes();
        shiftRows();
        mixColumns();
        addRoundKey(round);
    }
    subBytes();
    shiftRows();
    addRoundKey(m_Nr);
    for (int i = 0; i < (4 * m_Nb); ++i)
    {
        output[i] = m_state[i % 4][i / 4];
    }
}


void AESCipher::invCipher(unsigned char *input, unsigned char *output)
{
    int i;
    memset(&m_state[0][0], 0, 16);
    for (i = 0; i < (4 * m_Nb); i++)
    {
        m_state[i % 4][i / 4] = input[i];
    }
    addRoundKey(m_Nr);

    for (int round = m_Nr - 1; round >= 1; round--)  // main round loop
    {
        invShiftRows();
        invSubBytes();
        addRoundKey(round);
        invMixColumns();
    }  // end main round loop for InvCipher

    invShiftRows();
    invSubBytes();
    addRoundKey(0);

    // output = state
    for (i = 0; i < (4 * m_Nb); i++)
    {
        output[i] = m_state[i % 4][i / 4];
    }
}


void AESCipher::setKey(int keySize)
{
    if (keySize == 16)
    {
        m_Nk = 4;    // 4*4 unsigned char，128 bits key，10 rounds of encryption
        m_Nr = 10;
    }
    else if (keySize == 24)
    {
        m_Nk = 6;    // 6*4 unsigned char，192 bits key，12 rounds of encryption
        m_Nr = 12;
    }
    else if (keySize == 32)
    {
        m_Nk = 8;    // 8*4 unsigned char，256 bits key，14 rounds of encryption
        m_Nr = 14;
    }
    else
    {
        std::cout << "aaa";
    }
}


void AESCipher::keyExpansion()
{
    int row;
    memset(&m_expandKey[0], 0, 16 * 15);
    // copy seed key
    for (row = 0; row < m_Nk; row++)
    {
        m_expandKey[4 * row + 0] = m_key[4 * row];
        m_expandKey[4 * row + 1] = m_key[4 * row + 1];
        m_expandKey[4 * row + 2] = m_key[4 * row + 2];
        m_expandKey[4 * row + 3] = m_key[4 * row + 3];
    }
    auto *temp = new unsigned char[4];
    for (row = m_Nk; row < 4 * (m_Nr + 1); row++)
    {
        temp[0] = m_expandKey[4 * row - 4];
        temp[1] = m_expandKey[4 * row - 3];
        temp[2] = m_expandKey[4 * row - 2];
        temp[3] = m_expandKey[4 * row - 1];
        if (row % m_Nk == 0)
        {
            rotWord(temp);
            subWord(temp);
            temp[0] = (unsigned char) ((int) temp[0] ^ (int) AES_RCON[4 * (row / m_Nk) + 0]);
            temp[1] = (unsigned char) ((int) temp[1] ^ (int) AES_RCON[4 * (row / m_Nk) + 1]);
            temp[2] = (unsigned char) ((int) temp[2] ^ (int) AES_RCON[4 * (row / m_Nk) + 2]);
            temp[3] = (unsigned char) ((int) temp[3] ^ (int) AES_RCON[4 * (row / m_Nk) + 3]);
        }
        else if (m_Nk > 6 && (row % m_Nk == 4))
        {
            subWord(temp);
        }

        // m_expandKey[row] = m_expandKey[row-Nk] xor temp
        m_expandKey[4 * row + 0] = (unsigned char) ((int) m_expandKey[4 * (row - m_Nk) + 0] ^ (int) temp[0]);
        m_expandKey[4 * row + 1] = (unsigned char) ((int) m_expandKey[4 * (row - m_Nk) + 1] ^ (int) temp[1]);
        m_expandKey[4 * row + 2] = (unsigned char) ((int) m_expandKey[4 * (row - m_Nk) + 2] ^ (int) temp[2]);
        m_expandKey[4 * row + 3] = (unsigned char) ((int) m_expandKey[4 * (row - m_Nk) + 3] ^ (int) temp[3]);
    }  // for loop

    delete[] temp;
}


void AESCipher::rotWord(unsigned char *word)
{
    m_tempRotWord[0] = word[1];
    m_tempRotWord[1] = word[2];
    m_tempRotWord[2] = word[3];
    m_tempRotWord[3] = word[0];
    memcpy(word, &m_tempRotWord[0], 4);
}


void AESCipher::subWord(unsigned char *word)
{
    for (int j = 0; j < 4; j++)
    {
        m_tempSubWord[j] = AES_SBOX[16 * (word[j] >> 4) + (word[j] & 0x0f)];
    }
    memcpy(word, &m_tempSubWord[0], 4);
}


void AESCipher::addRoundKey(int round)
{
    int i, j;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 4; i++)
        {
            m_state[i][j] = (unsigned char) ((int) m_state[i][j] ^ (int) m_expandKey[4 * ((round * 4) + j) + i]);
        }
    }
}


void AESCipher::subBytes()
{
    int i, j;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 4; i++)
        {
            m_state[i][j] = AES_SBOX[m_state[i][j]];
        }
    }
}


void AESCipher::shiftRows()
{
    unsigned char temp[4 * 4];
    int i, j;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 4; i++)
        {
            temp[4 * i + j] = m_state[i][j];
        }
    }
    for (i = 1; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (i == 1)
            {
                m_state[i][j] = temp[4 * i + (j + 1) % 4];
            }
            else if (i == 2)
            {
                m_state[i][j] = temp[4 * i + (j + 2) % 4];
            }
            else if (i == 3)
            {
                m_state[i][j] = temp[4 * i + (j + 3) % 4];
            }
        }
    }
}


void AESCipher::mixColumns()
{
    unsigned char temp[4 * 4];
    int i, j;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 4; i++)
        {
            temp[4 * i + j] = m_state[i][j];
        }
    }
    for (j = 0; j < 4; j++)
    {
        m_state[0][j] = (unsigned char) ((int) gfmultby02(temp[0 + j]) ^ (int) gfmultby03(temp[4 * 1 + j]) ^
                                         (int) gfmultby01(temp[4 * 2 + j]) ^ (int) gfmultby01(temp[4 * 3 + j]));
        m_state[1][j] = (unsigned char) ((int) gfmultby01(temp[0 + j]) ^ (int) gfmultby02(temp[4 * 1 + j]) ^
                                         (int) gfmultby03(temp[4 * 2 + j]) ^ (int) gfmultby01(temp[4 * 3 + j]));
        m_state[2][j] = (unsigned char) ((int) gfmultby01(temp[0 + j]) ^ (int) gfmultby01(temp[4 * 1 + j]) ^
                                         (int) gfmultby02(temp[4 * 2 + j]) ^ (int) gfmultby03(temp[4 * 3 + j]));
        m_state[3][j] = (unsigned char) ((int) gfmultby03(temp[0 + j]) ^ (int) gfmultby01(temp[4 * 1 + j]) ^
                                         (int) gfmultby01(temp[4 * 2 + j]) ^ (int) gfmultby02(temp[4 * 3 + j]));
    }
}


void AESCipher::invShiftRows()
{
    unsigned char temp[4 * 4];
    int i, j;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 4; i++)
        {
            temp[4 * i + j] = m_state[i][j];
        }
    }
    for (i = 1; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (i == 1)
            {
                m_state[i][j] = temp[4 * i + (j + 3) % 4];
            }
            else if (i == 2)
            {
                m_state[i][j] = temp[4 * i + (j + 2) % 4];
            }
            else if (i == 3)
            {
                m_state[i][j] = temp[4 * i + (j + 1) % 4];
            }
        }
    }
}


void AESCipher::invSubBytes()
{
    int i, j;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 4; i++)
        {
            m_state[i][j] = AES_INV_SBOX[m_state[i][j]];
        }
    }
}

void AESCipher::invMixColumns()
{
    unsigned char temp[4 * 4];
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            temp[4 * i + j] = m_state[i][j];
        }
    }

    for (j = 0; j < 4; j++)
    {
        m_state[0][j] = (unsigned char) ((int) gfmultby0e(temp[j]) ^ (int) gfmultby0b(temp[4 + j]) ^
                                         (int) gfmultby0d(temp[4 * 2 + j]) ^ (int) gfmultby09(temp[4 * 3 + j]));
        m_state[1][j] = (unsigned char) ((int) gfmultby09(temp[j]) ^ (int) gfmultby0e(temp[4 + j]) ^
                                         (int) gfmultby0b(temp[4 * 2 + j]) ^ (int) gfmultby0d(temp[4 * 3 + j]));
        m_state[2][j] = (unsigned char) ((int) gfmultby0d(temp[j]) ^ (int) gfmultby09(temp[4 + j]) ^
                                         (int) gfmultby0e(temp[4 * 2 + j]) ^ (int) gfmultby0b(temp[4 * 3 + j]));
        m_state[3][j] = (unsigned char) ((int) gfmultby0b(temp[j]) ^ (int) gfmultby0d(temp[4 + j]) ^
                                         (int) gfmultby09(temp[4 * 2 + j]) ^ (int) gfmultby0e(temp[4 * 3 + j]));
    }
}
