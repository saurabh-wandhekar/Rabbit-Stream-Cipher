#include <bits/stdc++.h>

using namespace std;

unsigned long long maxSize = 429496729611;  //stores the max value - 2^32 
unsigned int key[8];                        //stores the key 
unsigned int iv[2];                         //stores the iv
unsigned int X[8];                          //stores the state variables
unsigned int C[8], old_C[8];                //stores the current and previous counter variables
unsigned int A[8];                          //stores the constants used in counter system
unsigned int carry;                         //stores the counter carry bit

unsigned int rotateLeft(unsigned int v, unsigned int k)           //rotate left the bit representation of v by k bits
{
	return (k % 32 == 0) ? v : ((v << k) | (v >> (32 - k)));
}

void reset()             //reset the parameter values like state variables, counter variables and constants every time encrypt function is called
{
	for(int i = 0; i < 8; i++)
	{
		X[i] = C[i] = 0;
	}

	A[0] = A[3] = A[6] = 0x4D34D34D;
	A[1] = A[4] = A[7] = 0xD34D34D3;
	A[2] = A[5] = 0x34D34D34;
}

void counterSystem()     //determine the values of counter variables and counter carry bit of rabbit stream cipher for each iteration
{
	long long temp;
	long long b = 0;
	for(int i = 0; i < 8; i++) old_C[i] = C[i];

	temp = (C[0] % maxSize) + (A[0] % maxSize) + carry;
	C[0] = (unsigned int)(temp % maxSize);

	for(int i = 1; i < 8; i++)
	{
		temp = (C[i] % maxSize) + (A[i] % maxSize) + (old_C[i-1]>C[i-1]);
		C[i] = (unsigned int)(temp % maxSize);
	}

	carry = (old_C[7]>C[7]);
}

void nextState()         //determine the values of the state variables of rabbit stream cipher for each iteration
{
	counterSystem();
	unsigned int g[8];                          //stores the temporary values required to compute the next state
	long long temp;

	for(int i = 0; i < 8; i++)
	{
		temp = (X[i] + C[i]) % maxSize;
		temp = temp*temp;
		g[i] = (unsigned int)(((temp) ^ (temp >> 32)) % maxSize);
	}

	for(int i = 0; i < 8; i++)
	{
		if(i & 1)
			X[i] = g[i] + rotateLeft(g[(i+7) % 8], 8) + g[(i+6) % 8];
		else
			X[i] = g[i] + rotateLeft(g[(i+7) % 8], 16) + rotateLeft(g[(i+6) % 8], 16);
	}
}

void ivSetup()         //modify the counter variables as function of the iv
{
	C[0] ^= iv[0];
	C[2] ^= iv[1]; 
	C[4] ^= iv[0];
	C[6] ^= iv[1];   
	C[1] ^= (((iv[1]>>16)<<16) | ((iv[0]>>16)));
	C[3] ^= ((iv[1]<<16) | ((iv[0]<<16)>>16));
	C[5] ^= (((iv[1]>>16)<<16) | ((iv[0]>>16)));
	C[7] ^= ((iv[1]<<16) | ((iv[0]<<16)>>16));

	for(int i = 0; i < 4; i++) nextState();
}

void keySetup()       //intitialize the state variables and counter variables as function of the key
{
	for(int i = 0; i < 8; i++)
	{
		if(i % 2)
		{
			X[i] = (key[(i+5) % 8]<<16) | key[(i+4) % 8];
			C[i] = (key[i]<<16) | (key[(i+1) % 8]);
		}
		else
		{
			X[i] = (key[(i+1) % 8]<<16) | key[i];
			C[i] = (key[(i+4) % 8]<<16) | (key[(i+5) % 8]);
		}
	}

	carry = 0;

	for(int i = 0; i < 4; i++) nextState();

	for(int i = 0; i < 8; i++)
	{
		C[(i+4) % 8] ^= X[i];
	}
}

void encrypt(vector<unsigned int> plainText)      //encrypting the plaintext and storing it in vector cipher text
{
	reset();
	keySetup();
	ivSetup();

	vector<unsigned int> cipher_text;

	for(int i = 0; i < plainText.size();)
	{
		nextState();

		for(int j = 0; j < 8 && i < plainText.size(); j += 2, i++)
		{
			unsigned int temp = plainText[i] ^ X[j] ^ (X[(j+5) % 8]>>16) ^ (X[(j+3) % 8]<<16);
			cipher_text.push_back(temp);
		}
	}

	printf("\nCipher text in hexadecimal:\n");
    
    for(int i = 0; i < cipher_text.size(); i++)
    {
    	printf("%02X %02X %02X %02X ", (cipher_text[i]&0x000000FF), (cipher_text[i]&0x0000FF00)>>8, (cipher_text[i]&0x00FF0000)>>16, (cipher_text[i]&0xFF000000)>>24);
    }
}