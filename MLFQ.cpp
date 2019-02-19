#pragma once
/************************************************************************************************************
Name: Alina Tutuianu                Z#:Z23440498
Course: Computer Operating Systems (COP4610)
Professor: Tami Sorgente
Due Date: October 29, 2018         Due Time: 11:59
Total Points: 14
Description:
This is the implementation of the MLFQ cpu scheduler.
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
	int priority;		//can be 1, 2 or 3; a lower number has a higher priority
	int arrival_time;	//arrival time in the queues
}
p1 = { "P1",{ 4, 3, 2, 4, 4, 4, 3, 4 },
{ 27, 31, 43, 18, 22, 26, 24 }, 0, 0, 0, 0, -1, 0, 0, 1, 0 },
p2 = { "P2",{ 16, 17, 5, 16, 7, 13, 11, 6, 3, 4 },
{ 24, 21, 36, 26, 31, 28, 21, 13, 11 }, 0, 0, 0, 0, -1, 0, 0, 1, 0 },
p3 = { "P3",{ 8, 12, 18, 14, 4, 15, 14, 5, 6},
{ 33, 41, 65, 21, 61, 18, 26, 31 }, 0, 0, 0, 0, -1, 0, 0, 1, 0 },
p4 = { "P4",{ 3, 4, 4, 3, 4, 3, 6, 5, 3 },
{ 35, 41, 45, 51, 61, 55, 82, 77 }, 0, 0, 0, 0, -1, 0, 0, 1, 0 },
p5 = { "P5",{ 4, 5, 7, 12, 9, 4, 9, 7, 8 },
{ 48, 44, 42, 47, 46, 41, 31, 43 }, 0, 0, 0, 0, -1, 0, 0, 1, 0 },
p6 = { "P6",{ 11, 4, 5, 6, 7, 9, 12, 15, 8 },
{22, 8, 10, 12, 14, 18, 24, 30},0, 0,  0, 0, -1, 0, 0, 1, 0 },
p7 = { "P7",{14, 17, 11, 15, 4, 7, 16, 10 },
{ 46, 41, 42, 21, 32, 19, 33 }, 0, 0, 0, 0, -1, 0, 0, 1, 0 },
p8 = { "P8",{ 4, 5, 6, 14, 16, 6, 7 },
{ 14, 33, 51, 63, 87, 74 }, 0, 0, 0, 0, -1, 0, 0, 1, 0 },
p9 = { "P9", {3, 9, 8, 4, 7, 5, 4, 5, 16 },
{ 37, 41, 30, 29, 33, 22, 24, 29 }, 0, 0, 0, 0, -1, 0, 0, 1, 0 };


//Function declarations
//function to initialize ready queue at time 0
void initialize_queue_one(list<process> &queue);

//function to get the next cpu burst of a process
int get_cpu_burst(list<process>::iterator p, int position);

//function to get the next io burst of a process
int get_io_burst(process p, int position);

//boolean function to compare two processes based on their name; used to sort the list 
bool compareName(const process& p1, const process& p2);

//function to increase the waiting time for the processes waiting in the ready queue
void increase_WT(list<process> &queue);

//Print a queue
void print_queue(list<process> &queue);

//Printing the io queue
void print_io(list<process> &io);

//printing the completed processes
void print_complete(list<process> &complete);

int main()
{
	//Variable declaration and initialization
	list<process> queue_one;	//Q1, Q2, Q3
	list<process> queue_two;
	list<process> queue_three;
	list<process> io;		//i/o list
	list<process> complete;	//complete list
	process on_cpu;			//which process is on cpu
	int time = 0;			//time
	int priority = 0;		//variable to change the priority of a process that exceeded its cpu time
	int TQ1 = 4;		//time quantum for first queue
	int TQ2 = 9;		//time quantum for second queue
	int counter = 0;			//current time quantum
	int j = 0;		//counter for current cpu burst
	int number_of_processes = 9; //total number of processses 
	bool erased = true;			//flag to help in erasing multiple elements from the io queue that are done with io time
	bool idle = false;			//flag for when the cpu is idle
	int total = 0;	//variable to calculate average RT, WT, TT
	int cpu_idle = 0;	//counter to add the times while the cpu is idle in order to calculate cpu utilization
	bool preempted_one = false;		//flag for preempted process
	bool preempted_two = false;
	int remaining_cpu = 0;		//variable to keep track if process didn't finish all of its burst time

	//put the processes in the ready queue at time 0
	initialize_queue_one(queue_one);		

	//get the first cpu bursts for all the processes in the ready queue
	for (list<process>::iterator i = queue_one.begin(); i != queue_one.end(); ++i)
	{
		i->cpu_burst = get_cpu_burst(i, i->cpu_position);
	}

	//main loop
	while (!queue_one.empty() || !queue_two.empty() || !queue_three.empty() || !io.empty())//loop until all the loops are empty
	{
			 if (queue_one.empty() && queue_two.empty()  && queue_three.empty())//no more processes to move into the cpu, cpu will get idle
			{
				if (!idle) //if the cpu is for the first time idle, print a context switch
				{
					cout << "Current Time: " << time << endl << endl;
					cout << "Next Process on the CPU: " << " [IDLE]" << endl;
					cout << "................................................" << endl << endl;
					cout << "List of processes in Queue1: " << endl << endl;
					print_queue(queue_one);
					cout << "List of processes in Queue2: " << endl << endl;
					print_queue(queue_two);
					cout << "List of processes in Queue3: " << endl << endl;
					print_queue(queue_three);
					print_io(io);
					print_complete(complete);
					cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl;
				}

				idle = true;	//change idle flag to true
				time++;	//increase the time
				cpu_idle++;	//increase counter for cpu idle time for calculation purposes
				if (!queue_one.empty())
				{
					//if there are processes in  queue 1, change the flag to false
					idle = false;
					//increase waiting time for the processes that are waiting
					increase_WT(queue_one);
				}
				if (!queue_two.empty())
				{
					//if there are processes in queue 2, change the flag to false
					idle = false;
					//increase waiting time for the processes that are waiting
					increase_WT(queue_two);
				}
				if (!queue_three.empty())
				{
					//if there are processes in queue 3, change the flag to false
					idle = false;
					//increase waiting time for the processes that are waiting
					increase_WT(queue_three);
				}
				
				//traverse the io queue and decrease their burst
				if (!io.empty()) 
				{
					list<process>::iterator i = io.begin(); //iterator at the begining of io queue
					while (i != io.end() || erased == true) // loop until the end or loop further if a process was erased
					{
						if (i->io_burst > 1)//there is io time, decrease io burst
						{
							i->io_burst--;
							erased = false; //erased flag is false, there were no processes removed from io
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
							i->arrival_time = time;
							//push back into queue based on priority
							if (i->priority == 1)
							{
								queue_one.push_back(*i);
							}
							else if (i->priority == 2)
							{
								queue_two.push_back(*i);
							}
							else
							{
								queue_three.push_back(*i);
							}
							i = io.erase(i);
							if (i != io.end())
							{
								erased = true;
							}
						}
					}
				}
			}
			else //there are processes on the cpu and on some of the three queues
			{
				idle = false; //change idle flag to false
				
				//conditions to find the process to be moved on cpu next
				//if there was a preemption, the counter will take the value of the time quantum of the queue from where the preempting process comes
				//in the case of preemption on_cpu was already initialized in the preemption process
				if (preempted_one) 
				{
					counter = TQ1;
				}
				else if (preempted_two)
				{
					counter = TQ2;
				}
				else//there was no preemption, on_cpu is taken from the queues based on priority
				{
					if (!queue_one.empty())
					{
						on_cpu = queue_one.front(); //process on cpu is from Q1
						counter = TQ1;				//counter is time quantum 1
						queue_one.pop_front();		//remove the process from its queue
					}
					else if (!queue_two.empty())
					{
						on_cpu = queue_two.front(); //process on cpu is from Q2
						counter = TQ2;				//counter is time quantum 2
						queue_two.pop_front();		//remove the process from its queue
					}
					else
					{
						on_cpu = queue_three.front(); //process on cpu is from Q3
						queue_three.pop_front();	//remove the process from its queue
					}	
				}
				
				if (on_cpu.RT < 0)
				{
					on_cpu.RT = time;	//get the response time

				}


				//print context switch
				cout << "Current Time: " << time << endl << endl;
				cout << "Next Process on the CPU: " << on_cpu.name <<"\tCPU burst: "<<on_cpu.cpu_burst<< endl;
				cout << "................................................" << endl << endl;
				cout << "List of processes in Queue1: " << endl << endl;
				print_queue(queue_one);
				cout << "List of processes in Queue2: " << endl << endl;
				print_queue(queue_two);
				cout << "List of processes in Queue3: " << endl << endl;
				print_queue(queue_three);
				print_io(io);
				print_complete(complete);
				cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl;

				//variable holds current cpu burst
				j = on_cpu.cpu_burst;

				//decrease cpu burst based on queue
				if (on_cpu.priority < 3) //first two queues
				{
					//compare j and counter and calculate if there is more cpu time than the allocated time quantum
					if (j < counter)	//cpu burst is less than the time quantum
					{
						counter = j;	//counter takes the value of the cpu burst
						remaining_cpu = 0;	
					}
					else
					{
						remaining_cpu = j - counter; //there is a remaining cpu
					}

					//for loop to decrease current cpu time
					for (counter; counter > 0; counter--)
					{
						preempted_one = false; //set preemption flags to false
						preempted_two = false;
						
						//preempting if a priority 1 process arrives in Q1
						if ((!queue_one.empty()) && (on_cpu.priority == 2) && (queue_one.front().arrival_time = time))
						{
							//update next cpu burst of process being preempted
							on_cpu.cpu_burst = counter+remaining_cpu;

							//update arrival time
							on_cpu.arrival_time = time;

							//move to corresponding queue
							queue_two.push_back(on_cpu);

							//get higher priority process on_cpu and remove from its queue
							list<process>::iterator i = queue_one.begin();
							on_cpu = *i;
							//cout << endl << "cpu burst of preempting process" << on_cpu.cpu_burst << endl;
							queue_one.pop_front();

							//mark flag for preemption from queue 1
							preempted_one = true;
						}
						//break if higher priority process comes from i/o
						if (preempted_one)
						{
							break;
						}
						//increase the time
						time++;
						//update arrival time
						on_cpu.arrival_time = time;
						//decrease the cpu time of the current process on cpu
						on_cpu.cpu_burst--;
						//increase waiting time for processes in queues
						if (!queue_one.empty())
						{
							//increase waiting time for the processes that are waiting  queue 1
							increase_WT(queue_one);
						}
						if (!queue_two.empty())
						{
							//increase waiting time for the processes that are waiting in queue 2
							increase_WT(queue_two);
						}
						if (!queue_three.empty())
						{
							//increase waiting time for the processes that are waiting in queue 3
							increase_WT(queue_three);
						}

						//decrease io time for processes in io queue; 
						//if io time is 0, move back to a queue based on priority
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
									i->arrival_time = time;
									//push back into queue based on priority
									if (i->priority == 1)
									{
										queue_one.push_back(*i);
									}
									else if (i->priority == 2)
									{
										queue_two.push_back(*i);
									}
									else
									{
										queue_three.push_back(*i);
									}
									i = io.erase(i);
									if (i != io.end())
									{
										erased = true;
									}
								}
							}

						}
					}
				}
				else //third queue
				{
					//on cpu is a process from queue 3
					//FCFS algorithm
					//for loop to decrease current cpu time
					counter = j;
					for (counter; counter > 0; counter--)
					{
						preempted_one = false;
						preempted_two = false;

						//preempting if a higher priority process arrives 
						if ((!queue_one.empty()) && (queue_one.front().arrival_time = time))	
						{
							//update next cpu burst of process being preempted
							on_cpu.cpu_burst = counter;
							//cout << endl << "cpu burst of process being preemted" << on_cpu.cpu_burst << endl;
							on_cpu.arrival_time = time;

							//move to corresponding queue
							queue_three.push_back(on_cpu);

							//get higher priority process on_cpu and remove from its queue
							list<process>::iterator i = queue_one.begin();
							on_cpu = *i;
							//cout << endl << "cpu burst of preempting process" << on_cpu.cpu_burst << endl;
							queue_one.pop_front();

							//mark flag for preemption from queue 1
							preempted_one = true;
						}
						else if ((!queue_two.empty()) && (queue_two.front().arrival_time = time))
						{
							//update next cpu burst of process being preempted
							on_cpu.cpu_burst = counter;
							//cout << endl << "cpu burst of process being preemted" << on_cpu.cpu_burst << endl;
							on_cpu.arrival_time = time;

							//move to corresponding queue
							queue_three.push_back(on_cpu);

							//get higher priority process on_cpu and remove from its queue
							list<process>::iterator i = queue_two.begin();
							on_cpu = *i;
							//cout << endl << "cpu burst of preempting process" << on_cpu.cpu_burst << endl;
							queue_two.pop_front();

							//mark flag for preemption from queue 1
							preempted_two = true;
						}
						//break if higher priority process comes from i/o
						if (preempted_one||preempted_two)
						{
							break;
						}
						//increase the time
						time++;

						//update arrival time
						on_cpu.arrival_time = time;

						//decrease the cpu time of the current process on cpu
						on_cpu.cpu_burst--;

						//increase waiting time for processes in queues
						if (!queue_one.empty())
						{
							//increase waiting time for the processes that are waiting in queue 1
							increase_WT(queue_one);
						}
						if (!queue_two.empty())
						{
							//increase waiting time for the processes that are waiting in  queue 2
							increase_WT(queue_two);
						}
						if (!queue_three.empty())
						{
							//increase waiting time for the processes that are waiting in queue 3
							increase_WT(queue_three);
						}

						//decrease io time for processes in io queue; 
						//if io time is 0, move back to a queue based on priority
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
									i->arrival_time = time;
									//push back into queue based on priority
									if (i->priority == 1)
									{
										queue_one.push_back(*i);
									}
									else if (i->priority == 2)
									{
										queue_two.push_back(*i);
									}
									else
									{
										queue_three.push_back(*i);
									}
									i = io.erase(i);
									if (i != io.end())
									{
										erased = true;
									}
								}
							}

						}
					}
				}
				//if there was no preemption check the value of cpu_burst
				//if there is a remaining cpu, increase priority and move to queue
				//if cpu burst is 0, get next io time and move to io. if no more io time, move to complete queue
				if ((!preempted_one) && (!preempted_two))
				{
					if (on_cpu.cpu_burst > 0)	//process didn't finish its cpu burst
					{
						on_cpu.priority++;
						//move current process in a queue based on priority
						on_cpu.arrival_time = time;
						if (on_cpu.priority == 1)
						{
							queue_one.push_back(on_cpu);
						}
						else if (on_cpu.priority == 2)
						{
							queue_two.push_back(on_cpu);
						}
						else
						{
							queue_three.push_back(on_cpu);
						}
					}
					else  //process finished its cpu burst
					{
						//get next io burst
						//preempted = false;
						on_cpu.io_burst = get_io_burst(on_cpu, on_cpu.io_position);
						on_cpu.io_position++;
						on_cpu.arrival_time = time;
						if (on_cpu.io_burst == 0) //if there is no more io time, the process is completed
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
							on_cpu.arrival_time = time;
							io.push_back(on_cpu);
						}

					}
				}
			}

	}
	//Print last context switch
	cout << "Current Time: " << time << endl << endl;
	cout << "Next Process on the CPU: " << "[IDLE]" << endl;
	cout << "................................................" << endl << endl;
	cout << "List of processes in Queue1: " << endl << endl;
	print_queue(queue_one);
	cout << "List of processes in Queue2: " << endl << endl;
	print_queue(queue_two);
	cout << "List of processes in Queue3: " << endl << endl;
	print_queue(queue_three);
	print_io(io);
	print_complete(complete);
	cout << "::::::::::::::::::::::::::::::::::::::::::::::::" << endl << endl;

	//Display final results of RT, WT, TT
	cout << "Finished\n\n";
	cout << "Total time: " << time << endl << endl;
	cout << "CPU utilization: " << 100 * (time - cpu_idle) / (float)time <<" %"<<endl<< endl;
	
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


void initialize_queue_one(list<process> &queue)
{
	queue.push_front(p9);

	queue.push_front(p8);
	queue.push_front(p7);
	queue.push_front(p6);
	queue.push_front(p5);
	queue.push_front(p4);
	queue.push_front(p3);
	queue.push_front(p2);
	queue.push_front(p1);
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

void increase_WT(list<process> &queue)
{
	for (list<process>::iterator i = queue.begin(); i != queue.end(); ++i)
	{
		i->WT++;	//increase waiting time for the processes that are waiting in the ready queue
	}
}
void print_queue(list<process> &queue)
{
	cout << "\tProcess\t\tBurst" << endl;
	if (queue.empty())
	{
		cout << "\t[empty]" << endl;
	}
	else
	{
		for (list<process>::iterator i = queue.begin(); i != queue.end(); ++i)
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
		cout << endl<<endl;
	}
	else
	{
		cout <<  "No completed processes"<<endl<<endl;
	}
}