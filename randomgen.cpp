#include <stdio.h> 
#include <stdlib.h> 
#include<time.h> 
#include<iostream>

using namespace std;
  
int main(void) 
{ 
  
	srand(time(0)); 
 	int m = 1;
	int hp = 12 * m;

	

	int processors = 2;
	int ntasks = 2; //perprocessor
	double maxutilization = 0.8; //perprocessor
	double minutilization = 0.2; //perprocessor

	cout << ntasks*processors <<"\t" << hp << "\t" << processors << endl;
	for (int j = 0; j < processors; j++)
	for(int i = 0; i < ntasks;i++)
	{
		int p;
		do
		{
			p = rand()%hp;
		}while(p <=0);


		double exec;
		do
		{
			long int x = p * 10;
			do{
				exec = (rand()*rand()) % x;
				exec/=10;
			}while(exec <= 0.1);
		}while (!( (exec/(double)p) < maxutilization/(double)ntasks && (exec/(double)p) > minutilization/(double)ntasks));
		exec *= m;
		p *= m;
		cout << exec << "\t" << p << "\t" << j << endl;

	}

 
  
    return 0; 
} 
