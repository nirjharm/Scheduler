#include <iostream>
#include <string>
#include <math.h>


using namespace std;

#define H 85
#define W 90

class framework
{
        double hyperperiod;
        int ntasks;
        int * tops;
        int taskheight;

        void settops(int n)
        {
                tops = new int[n];
                //divide 90 into s=n slices of size taskheight+2
                // s * (taskheight + 2) = H
                //(taskheight+2) = H/s = H /n

                taskheight = ((double)(H / n));
                for(int i = 0; i < n; i++)
                {
                        tops[i] = (taskheight * i) + (1.5 * ceil(((double)taskheight) * 0.15)) + ((100-H)/2)+1;
                }
        }

        public:

        framework(double new_hyperperiod, int new_ntasks)
        {
                hyperperiod = new_hyperperiod;
                ntasks = new_ntasks;
                settops(ntasks);
        }
        int gettop(int i) //0 to n-1 tasks
        {
                if(i >= ntasks) return -1;
                return tops[i];
        }
	int getmid(int i) //0 to n-1 tasks
        {
                if(i >= ntasks) return -1;
                return tops[i] + (gettaskheight()/2);
        }
        int gettaskheight()
        {
                return ((double)taskheight)* 0.7;
        }
};

double calc_width(double h, double w) //hyperperiod, endtime - starttime
{
        double perc = ((w*W)/h);
        return perc; //in term of vw on css
}

double calc_left(double h, double st) //hyperperiod, starttime
{
        double perc = ((st*W)/h);
        return (5 + perc);
}

int main()
{
        framework * f;
        double hyp;
        int tasks;
	cin >> hyp;
	cin >> tasks;
        f = new framework(hyp, tasks);



	while(1)
	{
		double start, stop;
		int proc;
		int task, jobid;
		cin >> start;
		if(start == -1)break;		
		cin >> stop; 
		cin >> proc;
		cin >> task;
		cin >> jobid;

		string i_a = "Job " + to_string(jobid) + " of Task ";
		string i_b = to_string(task+1);
		string i_c = "<br>Job Start Time : ";
		string i_d = to_string(start);
		string i_e = "<br>Job Completion : ";
		string i_f = to_string(stop);
		string i_g = "<br>On Processor : ";
		string i_h = to_string(proc+1);

		if(proc == -1)
		{
			i_a = "Context Switch from Task";
			i_c = "<br>Context Switch Started at : ";
			i_e = "<br>Context Switch Ended at : ";
			i_g = "";
			i_h = "";
		}

		string info = i_a + i_b + i_c + i_d + i_e +i_f+i_g+i_h;

		cout << "<div class=\"div" << proc <<"\" id=\"rectangle\"  style=\"position:absolute; top:" << f->gettop(task) << "vh; left:" << calc_left(hyp, start) << 
													"vw; width:" << calc_width(hyp, stop-start) << "vw; height:"<< f->gettaskheight() <<
													"vh\"><span class=\"tooltiptext\">" << info <<"</span></div>\n";
	}

	cout << "<div style=\"position:absolute; top:95vh; left:94.5vw;\">"<<hyp <<"</div>\n";


	for(int i = 0; i < tasks;i ++)
	{
		cout << "<div style=\"position:absolute; top:"<<   f->getmid(i) <<"vh; left:1vw;\">Task "<<i+1<<"</div>\n";
	}

        return 0;
}

















