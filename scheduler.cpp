#include <stdio.h> 
#include <stdlib.h> 
#include<time.h> 
#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>

using namespace std;



enum PriorityType {NP_EDF, P_EDF, NP_RM, P_RM};

class Task
{
	protected:
		double period;
		double execcost;
		double relativedeadline;
		int tid;
	public:	
		double nexttaskat;
		Task(const Task &ntask)
		{
			nexttaskat = 0;
			period = ntask.period;
			execcost = ntask.execcost;
			relativedeadline = ntask.relativedeadline;
			tid = ntask.tid;
		}
		int gettaskid()
		{
			return tid;
		}
		Task (double nperiod, double nexeccost, double nrelativedeadline, int ntid)
		{
			period = nperiod;
			execcost = nexeccost;
			relativedeadline = nrelativedeadline;
			tid = ntid;
		}
		double getPeriod()
		{
			return period;
		}
};

class Job: public Task
{	
	protected:
		double executedtime;
		double remainingexectime;
		double absolutedeadline;
		int deadlinemiss;
		double cur_priority, original_priority;
		
		int waiting, waitingonid;
 
		int jobid, job_in_taskid;
		
		double begin;

		void initjob (double inittime)
		{
			waiting = 0;
			waitingonid = -1;			

			deadlinemiss = 0;
			executedtime = 0;

			remainingexectime = execcost;
			absolutedeadline = inittime + relativedeadline;
			
			begin = inittime;

		}
	public:
		
		

		Job (Task ntask, double inittime, PriorityType ptype, int njobid, int njob_in_taskid): Task(ntask)
		{
			//Setup IDs
			jobid = njobid;		
			job_in_taskid = njob_in_taskid;

			initjob(inittime);
			setinitpriority(ptype);
		}

		int getjobid()
		{
			return jobid;
		}

		void setbegin(double b)
		{
			begin = b;
		}

		void setinitpriority(PriorityType ptype)
		{
			switch(ptype)
			{
				case NP_EDF:
				case P_EDF: cur_priority = absolutedeadline;
					    break;
				case NP_RM:
				case P_RM: cur_priority = period;
					   break;
			}
			original_priority = cur_priority;
		}
		double getpriority()
		{
			return cur_priority;
		}

		int exec(double timeslice, double absolutetime) //1 means done, 0 means not done
		{

			executedtime += timeslice;
			remainingexectime -= timeslice;
			
			cerr << "Remaining Time : "<< remainingexectime << endl;			

			if (absolutetime > absolutedeadline)
				deadlinemiss=1;

			if (remainingexectime < 0.00001) 
				return 1;
			return 0;
		}
		void print(double now, int pno)
		{
			cout << begin << "\t" << now << "\t"  << pno << "\t" << tid << "\t" << jobid << endl;
		}
		
};



class Job_Queue{
	protected:
		vector <Job *> jobs;
		void swap (int i,int j) //swap elements i and j
		{
			iter_swap(jobs.begin() + i, jobs.begin() + j);	
		}
	public:
		Job_Queue ()
		{
			
		}	
	
		int size()
		{
			return jobs.size();
		}
	
		Job * getTopJob()
		{
			return jobs.at(0);
		}
		void removeTopJob()
		{
			jobs.erase(jobs.begin());
		}
		
		void addJob(Job  * j)
		{
			jobs.push_back(j);
		}
		void sort_queue() //selection sort
		{
			for(int i = 0; i < jobs.size()-1; i++)
			{
				double min = jobs.at(i)->getpriority();
				int min_at = i;
				for (int j = i+1; j < jobs.size(); j++ )
				{
					if( jobs.at(j)->getpriority() < min)
					{
						min = jobs.at(j)->getpriority();
						min_at = j;
					}
				}
				if (min_at != i)
				{
					swap(i, min_at);
				}
			}
		}

};


class Processor{
	protected:
		double granularity, contextswitchtime;
		double hp;
		vector <Task> taskList;
		vector <int> taskListIDcount;
		Job_Queue queue;
		int ProcNumber;
		int nextnewjobID;
		double time;

		PriorityType ptype;

		int justpushnewjobs()
		{
			int ret = 0;
			cerr << time << endl;
			for(int i = 0; i < taskList.size(); i++)
			{
				if((taskList.at(i).nexttaskat - time) < 0.0001)
				{
					cerr << "New Job!!" << endl;
					//add job
					//Job (Task ntask, double inittime, PriorityType ptype, int njobid, int njob_in_taskid): Task(ntask)
					Job * j = new Job(taskList.at(i), time, ptype, getnewjobID(), taskListIDcount.at(i));
					taskListIDcount.at(i)++;
					queue.addJob(j);
					taskList.at(i).nexttaskat += taskList.at(i).getPeriod();
					
					cerr << "next New Job at"<<  taskList.at(i).nexttaskat<< endl;
					ret = 1;					
				}
			}
			return ret;
		}

		void getNewJobs()
		{

			int wasempty = queue.size(); //to avoid context switch on idle
	
			int njob = justpushnewjobs();



			if((ptype == P_EDF || ptype == P_RM) && njob == 1)
			{
				Job * f = queue.getTopJob();
				queue.sort_queue();
				if(f !=  queue.getTopJob() && time != 0 && wasempty != 0)
				{
					f->print(time, ProcNumber);
					cout << time << "\t" << time + contextswitchtime << "\t" << "-1" << "\t" << f->gettaskid() << "\t" << f->getjobid() << endl;	
								
					int nj = 0;
		
					for(double k = 0; k < contextswitchtime; k+=granularity)
					{
						time+= granularity;
						nj += justpushnewjobs();	
					}

					if(nj > 0)
						queue.sort_queue();					

					queue.getTopJob()->setbegin(time);
				}
			}



		}	

	public:
		Processor(int number, PriorityType p, double grn, double ctxt, double nhp)
		{
			hp = nhp;
			granularity = grn;
			contextswitchtime = ctxt;
			ptype = p;
			time = 0;
			ProcNumber = number;
			nextnewjobID = number*1000+1;
		}		
		int getnewjobID()
		{
			nextnewjobID++;
			return nextnewjobID-1;
		}
		void addTask (Task t)
		{
			taskList.push_back(t);
			taskListIDcount.push_back(0);
		}
		int exec(double slice)
		{
			if(time >= hp)return -1;

			getNewJobs();

			
			if(queue.size() == 0)
			{
					time+=slice;
					return time;
			}
			if(time == 0)
				queue.sort_queue();
			

			//int exec(double timeslice, double absolutetime) //1 means done, 0 means not done
			Job * j =  queue.getTopJob();
			int retval = j->exec(slice,time);
			time+=slice;
			cerr << "exec with retval " << retval << endl;

			if(retval == 1)
			{
				//done with job, print and remove
				j->print(time, ProcNumber);
				
				cout << time << "\t" << time + contextswitchtime << "\t" << "-1" << "\t" << j->gettaskid() << "\t" << j->getjobid() << endl;	

				queue.removeTopJob();
							
				int nj = 0;

				for(double i = 0; i < contextswitchtime; i+=slice)
				{
					nj += justpushnewjobs();	
					time+= slice;
				}

				if(queue.size() == 0)
				{
					return time;
				}	

				if(ptype == NP_EDF || ptype == NP_RM || nj > 0)
				{
					queue.sort_queue();
				}
				queue.getTopJob()->setbegin(time);
			}
			return time;

		}
		void finishup()
		{
			if(queue.size() == 0)
				return;
			Job * j =  queue.getTopJob();
			j->print(time-granularity, ProcNumber);
		}

};


int main(void) 
{ 
	 double granularity = 0.1;
	 double contextswitchtime = 0.1;

	int n, hp, pcnt;
	cin >> n;
	cin >> hp;	
	cin >> pcnt;	

	Processor **P;
	P = new Processor *[pcnt];

 	for(int i = 0; i < pcnt; i++)
	{
		P[i] = new Processor(i, P_RM, granularity, contextswitchtime, hp);
	}

	cout << hp << "\t" << n <<endl;

	for(int i = 0; i < n;i++)
	{
		double compute, period;
		int proc;
		cin >> compute;
		cin >> period;
		cin >> proc;

		Task t(period, compute, period, i);

		P[proc]->addTask(t);
	}

	for(double time = 0; time <= hp; time+=granularity)
	{
		for(int j = 0; j < pcnt; j++)
		{
			P[j]->exec(granularity);
		}
	}
 	for(int i = 0; i < pcnt; i++)
	{
		P[i]->finishup();
	}

	cout << "-1";

	return 0; 
} 
