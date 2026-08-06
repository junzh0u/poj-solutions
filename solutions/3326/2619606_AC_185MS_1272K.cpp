#include<iostream>
#include<deque>
using namespace std;

struct Usage
{
	int loginTime,logoutTime,logInNum;
};

typedef deque<Usage> Student;

void LogIn(Student& record,int time)
{
	if(record.empty() || record.back().logInNum==0)
	{
		Usage newTimeScale;
		newTimeScale.loginTime=newTimeScale.logoutTime=time;
		newTimeScale.logInNum=1;
		record.push_back(newTimeScale);
	}
	else
		record.back().logInNum++;
}

void LogOut(Student& record,int time)
{
	record.back().logInNum--;
	if(record.back().logInNum==0)
		record.back().logoutTime=time;
}

void ReadRecord(Student record[])
{
	int time,pc,student,operation;
	cin>>time>>pc>>student>>operation;
	const int LOGIN=1,LOGOUT=0;
	if(operation==LOGIN)
		LogIn(record[student-1],time);
	else
		LogOut(record[student-1],time);
}

int min(int left,int right)
{
	return (left<right)?left:right;
}

int max(int left,int right)
{
	return (left>right)?left:right;
}

int TotalUseTime(Student record,int startTime,int endTime)
{
	int timeSum=0;
	while(!record.empty())
	{
		if(record.front().logoutTime>startTime)
		{
			if(record.front().loginTime>=endTime)
				break;
			timeSum+=min(record.front().logoutTime,endTime)-max(record.front().loginTime,startTime);
		}
		record.pop_front();
	}
	return timeSum;
}

void GetQuery(Student record[])
{
	int startTime,endTime,student;
	cin>>startTime>>endTime>>student;
	cout<<TotalUseTime(record[student-1],startTime,endTime)<<endl;
}

void main()
{
	int pcNum,studentNum;
	const int STUDENTNUMMAX=10000;
	while(cin>>pcNum>>studentNum)
	{
		if(pcNum==0)
			break;
		int recordNum;
		cin>>recordNum;
		Student record[STUDENTNUMMAX];
		while(recordNum--)
			ReadRecord(record);
		int queryNum;
		cin>>queryNum;
		while(queryNum--)
			GetQuery(record);
	}
}