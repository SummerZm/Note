#include <iostream>
using namespace std;

int main()
{
	double val = 1231444.449999;
	char* pch = (char*)&val;
	int bytes = sizeof(double)/sizeof(char);
	int count = 0;
	char ch = *pch;
	for (int i=0; i<bytes; i++)
	{
		for (int j=0; j<32; j++) 
		{
			if (ch & 0x01)
			{
				count++;
			}
			ch = ch>>1;
		}
		ch = *(pch++);
	}
	cout<<"bit cout: "<<count<<endl;
	return 0;
}
