#include <iostream>
#include <stdlib.h>
using namespace std;
void enterMatrix(int **arr, int R, int C, char const * msg) //function to enter matrix
{
	cout<<msg<<"\n";
	cout<<"Dimensions are "<<R<<"x"<<C<<"\n";
	for(int i=0;i<R;i++)
		for(int j=0;j<C;j++)
			cin>>arr[i][j];
	cout<<"*************************************\n";
	return;
}
void showMatrix(int **arr, int R, int C, char const * msg) //function to show matrix
{
        cout<<msg<<"\n";
        cout<<"Dimensions are "<<R<<"x"<<C<<"\n";
        for(int i=0;i<R;i++)
	{
                for(int j=0;j<C;j++)
			cout<<arr[i][j]<<" ";
		cout<<"\n";
	}
        cout<<"*************************************\n";
        return;
}
int done_size=0; //the running size of the alreadyDone array
int *alreadyDone= new int[100]; //list of processes that are already done
bool isDone(int p_number) //check if a certain process is already done
{
	for(int i=0;i<done_size;i++)
	{
		if(alreadyDone[i]==p_number)
			return true;
	}
	return false;
}
int findSlot(int *avail, int **need, int R, int C) //find a process that can be done with currently available resources
{
	int flag=0;
	for(int i=0;i<R;i++)
	{
		for(int j=0;j<C;j++)
		{
			if(avail[j]<need[i][j])
			{
				flag=1;
				break;
			}
		}
		if(flag==0 && !isDone(i)) //found a process to be done
		{
			alreadyDone[done_size]=i;
			done_size++;
			return i; //return the process number
		}
		else
			flag=0;
	}
	return -1; //fail condition
}
int main()
{
	int n_res,n_proc; //number of resources and processes
	cout<<"Enter the number of resources and processes\n";
	cin>>n_res>>n_proc;
	int **max, **need, **allocated, *available;
	max = new int *[n_proc];
	need = new int *[n_proc];
	allocated= new int *[n_proc];
	available = new int [n_res];
	for(int i = 0; i <n_proc; i++)
	{
		max[i] = new int[n_res];
		need[i] = new int [n_res];
		allocated[i] = new int [n_res];
	}
	enterMatrix(max,n_proc,n_res,"Enter the maximum matrix");
	enterMatrix(allocated,n_proc,n_res,"Enter the allocated matrix");
	cout<<"Enter the available vector\n";
	for(int i=0;i<n_res;i++)
		cin>>available[i];
	for(int i=0;i<n_proc;i++)
		for(int j=0;j<n_res;j++)
			need[i][j]=max[i][j]-allocated[i][j]; //calculate the need matrix
	showMatrix(need,n_proc,n_res,"The need matrix is "); //print the need matrix
	int res=-100;
	bool success=false;
	while(!success)
	{
		res=findSlot(available,need,n_proc,n_res); //returns a process number or -1
		if(res==-1 && done_size!=n_proc) //can't find do-able process and all are not done
		{
			cout<<"\nDeadlock\n";
			exit(0);
		}
		else if(res==-1 && done_size==n_proc) // cant find doable process and all are done
		{
			success=true; //all processes are done
		}
		else //found doable process
		{
			cout<<"Process "<<res<<" completed \n";
			for(int i=0;i<n_res;i++)
			{
				available[i]=available[i]+need[res][i]; //increase the available vector
			}
		}
	}
	return 0;
}
