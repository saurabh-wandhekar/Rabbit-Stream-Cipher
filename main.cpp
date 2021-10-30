#include "rabbit_cipher.h"
#include <bits/stdc++.h>

using namespace std;

int main()
{
	cout<<"Enter the key for the cipher : \n";

	for(int i = 0; i < 8; i++)
	{
		cin>>key[i];                 //input the 128-bit key as eight 16-bit subkeys
		cout<<key[i]<<" ";
	}

	cout<<"\nEnter the IV for the cipher : \n";

	for(int i = 0; i < 2; i++)
	{
		cin>>IV[i];                  //input the 64-bit IV as two different 32-bit parts
		cout<<IV[i]<<" ";
	}

	vector<unsigned int> plainText;

	cout<<"\nEnter the plain-text to be encrypted by the cipher : \n";

	for(int i = 0; i < 4; i++)
	{
		int x;
		cin>>x;                      //input the 128-bit plaintext as 4 different 32-bit parts
		cout<<x<<" ";
		plainText.push_back(x);
	}

	encrypt(plainText);              //call the encrypt function on plaintext

	return 0;
}