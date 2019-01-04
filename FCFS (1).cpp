#pragma once
/************************************************************************************************************
Name: Alina Tutuianu                Z#:Z23440498
Course: Computer Operating Systems (COP4610)
Professor: Tami Sorgente
Due Date: October 15, 2018         Due Time: 11:59
Total Points: 14
Description:
This is the header of the class process, that implements a CPU process.
*************************************************************************************************************/

#include<iostream>
#include<forward_list>
#include<string>
using namespace std;
struct process
{
	string name;
	int cpu[10], io[10];
	int cpu_position, io_position;
	int arrival_time;
	int cpu_burst;
	int io_burst;
	int RT;
	int WT;
	int TT;
}
p1 = { "P1", { 5, 3, 5, 4, 6, 4, 3, 4},
{27, 31, 43, 18, 22, 26, 24}, 0, 0, 0, 0, 0, 0, 0, 0 },
p2 = { "P2", { 4, 5, 7, 12, 9, 4, 9, 7, 8},
{48, 44, 42, 37, 76, 41, 31, 43}, 0, 0, 0, 0, 0, 0, 0, 0 },
p3 = { "P3",{ 8, 12, 18, 14, 4, 15, 14, 5, 6 },
{ 33, 41, 65, 21, 61, 18, 26, 31 }, 0, 0, 0, 0, 0, 0, 0, 0 },
p4 = { "P4",{ 3, 4, 5, 3, 4, 5, 6, 5, 3 },
{ 35, 41, 45, 51, 61, 54, 82, 77 }, 0, 0, 0, 0, 0, 0, 0, 0 },
p5 = { "P5",{ 16, 17, 5, 16, 7, 13, 11, 6, 3, 4 },
{ 24, 21, 36, 26, 31, 28, 21, 13,11 }, 0, 0, 0, 0, 0, 0, 0, 0 },
p6 = { "P6",{ 11, 4, 5, 6, 7, 9, 12, 15, 8 },
{ 22, 8, 10, 12, 14, 18, 24, 30 },0, 0,  0, 0, 0, 0, 0, 0 },
p7 = { "P7",{ 14, 17, 11, 15, 4, 7, 16, 10 },
{ 46, 41, 42, 21, 32, 19, 33}, 0, 0, 0, 0, 0, 0, 0, 0 },
p8 = { "P8",{ 4, 5, 6, 14, 16, 6 },
{ 14, 33, 51, 73, 87 }, 0, 0, 0, 0, 0, 0, 0, 0 };

void print_data(forward_list<process> &list);
void print_process(process &p);
void print_ready(forward_list<process> &list);
void initialize_ready(forward_list<process> &ready);
int get_cpu_burst(forward_list<process>::iterator p, int position);
int get_io_burst(process p, int position);
void print_io(forward_list<process> &list);

int main()
{
	forward_list<process> ready;	//ready list
	forward_list<process> io;		//i/o list
	forward_list<process> complete;	//complete list
	process on_cpu;					//which process is on cpu
	int time = 0;					//time
	//int i = 0;		//counter for the cpu and io arrays
	int j = 0;		//counter for current cpu burst
	//int k = 0;		//counter for current io burst

	initialize_ready(ready);		//put the processes in the ready queue
	//print_data(ready);			//print ready for testing purposes
	
	//get the first cpu and io bursts for all the processes in the ready queue
	for (forward_list<process>::iterator i = ready.begin(); i != ready.end(); ++i)
	{
		i->cpu_burst = get_cpu_burst(i, i->cpu_position);
	}

	for (int z = 0; z < 7; z++)
	{
		//copy the first process in the ready queue on cpu
		on_cpu = ready.front();
		//initialize counter for cpu burst
		j = on_cpu.cpu_burst;

		//on_cpu.RT = time;

		//remove first process from ready queue
		ready.pop_front();
	
		cout << "Current Time: " << time << endl << endl;
		cout << "Next Process on the CPU: " << on_cpu.name << endl;
		cout << "................................................" << endl << endl;
		print_ready(ready);
		cout << "................................................" << endl << endl;
		print_io(io);
		cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl;

		for (j; j > 0; j--)
		{
			//decrease the cpu time of the current process on cpu
			on_cpu.cpu_burst--;
			//increase the time
			time++;
			if (!io.empty()) //there are processes in io
			{
				//traverse all the processes in the io queue and decrease their io burst
				for (forward_list<process>::iterator i = io.begin(); i != io.end(); ++i)
				{
					if (i->io_burst > 0)//there is io time left
					{
						i->io_burst--;
					}
					else //process has completed io time
					{
						//get next cpu-burst
						i->cpu_position++;
						i->cpu_burst = get_cpu_burst(i, i->cpu_position);
						if (i->cpu_burst == 0)
						{
							//no more cpu time; move to complete queue
						}
						else
						{
							//move to ready queue based on arrival time
							i->arrival_time = time;	//new arrival time based on elapsed time
							//iterate through ready list to insert the current process
							forward_list<process>::iterator p = ready.end();
							ready.end() =ready.insert_after(p, *i);
							/*if (i->arrival_time < p->arrival_time)
							{
								ready.push_front(*i);//insert at the front of ready
								io.remove()
								break;
							}
							else
							{
								while (p != ready.end())
								{
									if (i->arrival_time > p->arrival_time)
									{
										p++;
									}
									else
									{
										ready.insert_after(p, *i);
									}
								}
							}*/
							
							

						}
					}
				}
			}
			
			//on_cpu.TT++;
		}
		if (on_cpu.cpu_burst == 0)
		{
			//get next io burst
			on_cpu.io_burst = get_io_burst(on_cpu, on_cpu.io_position);
			on_cpu.io_position++;
			if (on_cpu.io_burst == 0)
			{
				//move process to complete queue
			}
			else
			{
				//move on_cpu to i/o queue
				io.push_front(on_cpu);
			}
		}
	}
	//cout << "Current time: " << time << endl;*/
	return 0;
}

void print_data(forward_list<process> &list)
{
	for (forward_list<process>::iterator i = list.begin(); i != list.end(); ++i)
	{
		cout << "Process " << distance(list.begin(), i)+1<< endl;
		print_process(*i);
		cout << endl;
	}
}

void print_process(process &p)
{
	cout << "CPU time:" << endl;
	for (int i = 0; i < 10; i++)
	{
		if (p.cpu[i] != 0)
		{
			cout << p.cpu[i] << " ";
		}
	}
	cout << endl;
	cout << "I/O time:" << endl;
	for (int i = 0; i < 10; i++)
	{
		if (p.io[i] != 0)
		{
			cout << p.io[i] << " ";
		}
	}
	cout << endl;
}

void initialize_ready(forward_list<process> &ready)
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

int get_cpu_burst(forward_list<process>::iterator p, int position)
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

void print_ready(forward_list<process> &list)
{
	cout << "List of processes in the ready queue: " << endl<<endl;
	cout << "\tProcess\t\tBurst" << endl;
	if (list.empty())
	{
		cout << "\t[empty]" << endl;
	}
	else
	{
		for (forward_list<process>::iterator i = list.begin(); i != list.end(); ++i)
		{
			cout << "\t" << i->name;
			cout << "\t\t" << i->cpu_burst;
			cout << endl;
		}
	}
}

void print_io(forward_list<process> &list)
{
	cout << "List of processes in I/O: " << endl<<endl;
	cout << "\tProcess\t\tRemaining I/O time" << endl;
	forward_list<process>::iterator i = list.begin();
	if (list.empty())
	{
		cout << "\t[empty]" << endl;
	}
	else
	{
		for (forward_list<process>::iterator i = list.begin(); i != list.end(); ++i)
		{
			cout << "\t" << i->name;
			cout << "\t\t" << i->io_burst;
			cout << endl;
		}
	}
}


