#pragma once
/************************************************************************************************************
Name: Alina Tutuianu                Z#:Z23440498
Course: Computer Operating Systems (COP4610)
Professor: Tami Sorgente
Due Date: October 29, 2018         Due Time: 11:59
Total Points: 14
Description:
This is the implementation of the FCFS cpu scheduler.
*************************************************************************************************************/

#include<iostream>
#include<list>
#include<string>
using namespace std;
struct process
{
	string name;
	int cpu[10], io[10];	//arrays to hold cpu and io time
	int cpu_position, io_position;	//array positions of next cpu and io time
	int cpu_burst;		//current cpu burst value
	int io_burst;		//current io burst value
	int RT;				//variables for response time, waiting time, turnaround time
	int WT;
	int TT;
}
p1 = { "P1",{ 5, 3, 5, 4, 6, 4, 3, 4 },
{ 27, 31, 43, 18, 22, 26, 24 }, 0, 0, 0, 0, -1, 0, 0 },
p2 = { "P2",{ 4, 5, 7, 12, 9, 4, 9, 7, 8 },
{ 48, 44, 42, 37, 76, 41, 31, 43 }, 0, 0, 0, 0, -1, 0, 0 },
p3 = { "P3",{ 8, 12, 18, 14, 4, 15, 14, 5, 6 },
{ 33, 41, 65, 21, 61, 18, 26, 31 }, 0, 0, 0, 0, -1, 0, 0 },
p4 = { "P4",{ 3, 4, 5, 3, 4, 5, 6, 5, 3 },
{ 35, 41, 45, 51, 61, 54, 82, 77 }, 0, 0, 0, 0, -1, 0, 0 },
p5 = { "P5",{ 16, 17, 5, 16, 7, 13, 11, 6, 3, 4 },
{ 24, 21, 36, 26, 31, 28, 21, 13,11 }, 0, 0, 0, 0, -1, 0, 0 },
p6 = { "P6",{ 11, 4, 5, 6, 7, 9, 12, 15, 8 },
{ 22, 8, 10, 12, 14, 18, 24, 30 },0, 0,  0, 0, -1, 0, 0 },
p7 = { "P7",{ 14, 17, 11, 15, 4, 7, 16, 10 },
{ 46, 41, 42, 21, 32, 19, 33 }, 0, 0, 0, 0, -1, 0, 0 },
p8 = { "P8",{ 4, 5, 6, 14, 16, 6 },
{ 14, 33, 51, 73, 87 }, 0, 0, 0, 0, -1, 0, 0 };

//Function declarations
//function to initialize ready queue at time 0
void initialize_ready(list<process> &ready);

//function to get the next cpu burst of a process
int get_cpu_burst(list<process>::iterator p, int position);

//function to get the next io burst of a process
int get_io_burst(process p, int position);

//boolean function to compare two processes based on their name; used to sort the list 
bool compareName(const process& p1, const process& p2);

//function to increase the waiting time for the processes waiting in the ready queue
void increase_WT(list<process> &ready);

//Printint the ready queue
void print_ready(list<process> &ready);

//Printing the io queue
void print_io(list<process> &io);

//printing the completed processes
void print_complete(list<process> &complete);

int main()
{
	list<process> ready;	//ready list
	list<process> io;		//i/o list
	list<process> complete;	//complete list
	process on_cpu;					//which process is on cpu
	int time = 0;					//time
	int j = 0;		//counter for current cpu burst
	int number_of_processes = 8; //total number of processses 
	bool erased = true;			//flag to help in erasing multiple elements from the io queue that are done with io time
	bool idle = false;			//flag for when the cpu is idle
	int total = 0;	//variable to calculate average RT, WT, TT
	int cpu_idle = 0;	//counter to add the times while the cpu is idle in order to calculate cpu utilization

						//put the processes in the ready queue at time 0
	initialize_ready(ready);

	//get the first cpu bursts for all the processes in the ready queue
	for (list<process>::iterator i = ready.begin(); i != ready.end(); ++i)
	{
		i->cpu_burst = get_cpu_burst(i, i->cpu_position);
	}

	//main loop
	while (!ready.empty() || !io.empty())//loop until both ready and io are empty
	{
		if (idle == true) //the cpu was idle
		{
			time++;	//increase the time
			cpu_idle++;	//increase counter for cpu idle time for calculation purposes
			if (!ready.empty())
			{
				//if there are processes in the ready queue, change the flag to false
				idle = false;
				//increase waiting time for the processes that are waiting in the ready queue
				increase_WT(ready);
			}
			if (!io.empty()) //there are processes in io; we have to decrease their burst even when cpu is idle
			{
				//traverse all the processes in the io queue and decrease their io burst
				list<process>::iterator i = io.begin();
				while (i != io.end() || erased == true)
				{
					if (i->io_burst > 1)//there is io time
					{
						i->io_burst--;	//decrease burst
						erased = false;
						++i;
					}
					else //process has completed io time or has a single time unit of io
					{
						if (i->io_burst == 1)
						{
							i->io_burst--;
						}
						//get next cpu-burst
						i->cpu_position++;
						i->cpu_burst = get_cpu_burst(i, i->cpu_position);
						//put back to ready queue
						ready.push_back(*i);
						idle = false;
						i = io.erase(i);
						if (i != io.end())
						{
							erased = true;
						}
					}
				}
			}
		}
		else //cpu is not currently idle
		{
			if (ready.empty())//no more processes to move into the cpu, cpu will get idle
			{
				idle = true;
				cout << "Current Time: " << time << endl << endl;
				cout << "Next Process on the CPU: " << " [IDLE]" << endl;
				cout << "................................................" << endl << endl;
				//print context switch
				print_ready(ready);
				print_io(io);
				print_complete(complete);
				cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl;
			}
			else //there are processes on the cpu and on the ready queue
			{
				idle = false;
				cout << "Current Time: " << time << endl << endl;

				//move the first process in the ready queue to cpu
				on_cpu = ready.front();
				if (on_cpu.RT < 0)
				{
					on_cpu.RT = time;	//get the response time

				}
				//initialize counter for cpu burst
				j = on_cpu.cpu_burst;

				//remove first process from ready queue
				ready.pop_front();

				//print context switch
				cout << "Next Process on the CPU: " << on_cpu.name << endl;
				cout << "................................................" << endl << endl;
				print_ready(ready);
				print_io(io);
				print_complete(complete);
				cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl;

				//for loop to decrease current cpu time
				for (j; j > 0; j--)
				{
					//decrease the cpu time of the current process on cpu
					on_cpu.cpu_burst--;

					//increase the time
					time++;

					//increase waiting time for processes in ready queue
					increase_WT(ready);

					//decrease io time for processes in io queue; if io time is 0, move back to ready queue
					if (!io.empty()) //there are processes in io
					{
						//traverse all the processes in the io queue and decrease their io burst
						list<process>::iterator i = io.begin();
						while (i != io.end() || erased == true)
						{
							if (i->io_burst > 1)//there is io time, decrease io burst
							{
								i->io_burst--;
								erased = false;
								++i;
							}
							else //process has completed io time or has a single time unit of io
							{
								if (i->io_burst == 1)
								{
									i->io_burst--;
								}
								//get next cpu-burst
								i->cpu_position++;
								i->cpu_burst = get_cpu_burst(i, i->cpu_position);
								//push back into ready queue
								ready.push_back(*i);
								i = io.erase(i);
								if (i != io.end())
								{
									erased = true;
								}
							}
						}

					}

				}
				//when current cpu time is decreased to 0, if there is no io left, 
				//the process is moved to complete list
				if (on_cpu.cpu_burst == 0)
				{
					//get next io burst
					on_cpu.io_burst = get_io_burst(on_cpu, on_cpu.io_position);
					on_cpu.io_position++;
					if (on_cpu.io_burst == 0)
					{
						//move process to complete queue
						if (complete.empty()) //first process completed
						{
							on_cpu.TT = time; //calculate turnaround time
							complete.push_front(on_cpu);
						}
						else  //there are other processes in complete list; sort processes
						{
							on_cpu.TT = time;
							complete.push_back(on_cpu);
							//sort complete list after each insertion based on name
							complete.sort(compareName);
						}

					}
					else
					{
						//there is a new io burst for the current process; move to io
						io.push_back(on_cpu);
					}

				}
			}
		}
	}
	//Print last context switch
	cout << "Current Time: " << time << endl << endl;
	cout << "Next Process on the CPU: " << " [IDLE]" << endl;
	idle = true;
	cout << "................................................" << endl << endl;
	print_ready(ready);
	print_io(io);
	print_complete(complete);
	cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl;

	//Display final results of RT, WT, TT
	cout << "Finished\n\n";
	cout << "Total time: " << time << endl << endl;
	cout << "CPU utilization: " << 100 * (time - cpu_idle) / (float)time << endl;

	//Calculating response time
	cout << "Response time:" << endl << endl;
	for (list<process>::iterator i = complete.begin(); i != complete.end(); ++i)
	{
		cout << i->name << "\t";
	}
	cout << endl;
	for (list<process>::iterator i = complete.begin(); i != complete.end(); ++i)
	{
		cout << i->RT << "\t";
		total += i->RT;
	}
	cout << endl;
	cout << "Average response time:\t" << (float)total / (float)number_of_processes << endl << endl;

	//Calculating waiting time
	total = 0;
	cout << "Waiting time:" << endl << endl;
	for (list<process>::iterator i = complete.begin(); i != complete.end(); ++i)
	{
		cout << i->name << "\t";
	}
	cout << endl;
	for (list<process>::iterator i = complete.begin(); i != complete.end(); ++i)
	{
		cout << i->WT << "\t";
		total += i->WT;
	}
	cout << endl;
	cout << "Average waiting time:\t" << (float)total / (float)number_of_processes << endl << endl;

	//Calculating turnaround time
	total = 0;
	cout << "Turnaround time:" << endl << endl;
	for (list<process>::iterator i = complete.begin(); i != complete.end(); ++i)
	{
		cout << i->name << "\t";
	}
	cout << endl;
	for (list<process>::iterator i = complete.begin(); i != complete.end(); ++i)
	{
		cout << i->TT << "\t";
		total += i->TT;
	}
	cout << endl;
	cout << "Average turnaround time:\t" << (float)total / (float)number_of_processes << endl << endl;

	return 0;
}


void initialize_ready(list<process> &ready)
{
	ready.push_front(p8);
	ready.push_front(p7);
	ready.push_front(p6);
	ready.push_front(p5);
	ready.push_front(p4);
	ready.push_front(p3);
	ready.push_front(p2);
	ready.push_front(p1);
}

int get_cpu_burst(list<process>::iterator p, int position)
{
	p->cpu_burst = p->cpu[position];
	position++;
	return p->cpu_burst;
}

int get_io_burst(process p, int position)
{
	p.io_burst = p.io[position];
	position++;
	return p.io_burst;
}


bool compareName(const process& p1, const process& p2)
{
	if (p1.name != p2.name) return p1.name < p2.name;
}

void increase_WT(list<process> &ready)
{
	for (list<process>::iterator i = ready.begin(); i != ready.end(); ++i)
	{
		i->WT++;	//increase waiting time for the processes that are waiting in the ready queue
	}
}
void print_ready(list<process> &ready)
{
	cout << "List of processes in the ready queue: " << endl << endl;
	cout << "\tProcess\t\tBurst" << endl;
	if (ready.empty())
	{
		cout << "\t[empty]" << endl;
	}
	else
	{
		for (list<process>::iterator i = ready.begin(); i != ready.end(); ++i)
		{
			cout << "\t" << i->name;
			cout << "\t\t" << i->cpu_burst;
			cout << endl;
		}
	}
	cout << "................................................" << endl << endl;
}

void print_io(list<process> &io)
{
	cout << "List of processes in I/O: " << endl << endl;
	cout << "\tProcess\t\tRemaining I/O time" << endl;
	if (io.empty())
	{
		cout << "\t[empty]" << endl;
	}
	else
	{
		for (list<process>::iterator i = io.begin(); i != io.end(); ++i)
		{
			cout << "\t" << i->name;
			cout << "\t\t" << i->io_burst;
			cout << endl;
		}
	}
	cout << "................................................" << endl << endl;
}

void print_complete(list<process> &complete)
{
	if (!complete.empty())
	{
		cout << "Completed: ";
		for (list<process>::iterator i = complete.begin(); i != complete.end(); ++i)
		{
			cout << i->name << ", ";
		}
		cout << endl << endl;
	}
	else
	{
		cout << "No completed processes" << endl << endl;
	}
}