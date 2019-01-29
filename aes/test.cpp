#include "aes.hpp"
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace bigo::ml::bvt;
using namespace std;

int main()
{
    AESCipher cipher = AESCipher();

    auto plain1 = "As you can see, when constructors and destructors are used together, your classes can initialize and clean up after themselves without the programmer having to do any special work! This reduces the probability of making an error, and makes classes easier to use.";
    std::vector<unsigned char> plain{plain1, plain1 + 263};
    auto sz = 263;
  
    // 输出待加密的明文  
    cout << endl << "待加密的明文：" << endl;  
    for(int i=0; i<sz; ++i)  
    {  
        cout << plain[i];  
        if((i+1) % 50 == 0)  
            cout << endl;  
    }  
    cout << endl;  
        
    
    int sz_16 = sz & 0xfffffff0;
    std::vector<unsigned char> outData(16);
    std::vector<unsigned char> secret(263);
    int i;
    for ( i = 0; i < sz_16; i += 16)
    {
        cipher.cipher((unsigned char *)&plain[i], &outData[0]);
        std::copy(outData.begin(), outData.end(), secret.begin() + i);
    }
    std::copy(plain.begin() + i, plain.end(), secret.begin() + i);
  
    // 加密，输出密文  
    cout << "加密后的密文："<<endl;  
    for(int i=0; i<263; ++i)  
    {  
        cout << secret[i];  
        if((i+1)%50 == 0)  
            cout << endl;  
    }  
    cout << endl;  


    std::vector<unsigned char> out(263);
    for (int i = 0; i < sz_16; i += 16)
    {
        cipher.invCipher((unsigned char *)&secret[i], &outData[0]);
        std::copy(outData.begin(), outData.end(), out.begin() + i);
    }
    std::copy(secret.begin() + i, secret.end(), out.begin() + i);
    cout << "解密后的明文："<<endl;  
    for(int i=0; i<263; ++i)  
    {  
        cout << out[i];  
        if((i+1)%50 == 0)  
            cout << endl;  
    }  
    cout << endl;  
    return 0;  
}