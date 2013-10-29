/**
@FileName: startServer.cpp
@Description: This file is used for starting the "virtualmem" server.
@Author: Ankur Upadhyay, Research Assistant, Computer Science, University at Buffalo
*/
#include<iostream>
#include "Server.cpp"
#include<time.h>
using namespace std;

/**
main function for starting the server. This function will compute
*the number of page faults for the user specified replacement policy 
*and compare with the number of page faults for Optimal page replacement
*algorithm.
*This function generates the ouput on the current terminal window.
*/
int main(int argc,char* argv[])
{
	Server server;
	server.startServer(argc, argv);
	string initialreplacementpolicy = replacementpolicy;
	server.readPageReferneceSequence();
	server.allocatePage();
	int pageFaults1 = numberOfPageFaults;
	int seconds1 = elapsedTime;
	replacementpolicy = "OPTIMAL";
	server.allocatePage();
	int pageFaults2 = numberOfPageFaults;
	int seconds2 = elapsedTime;
	double pageFaultsPenalty = ((pageFaults1 - pageFaults2)*100)/pageFaults1;
	cout<<"# page faults with "<<initialreplacementpolicy<<"		:"<<pageFaults1<<endl;
	cout<<"# page faults with "<<replacementpolicy<<"		:"<<pageFaults2<<endl;
	cout<<"% page replacement penalty using "<<initialreplacementpolicy<<"		:"<<pageFaultsPenalty<<endl;
	cout<<endl;
	cout<<"Total time to run "<<initialreplacementpolicy<<" algorithm 		:"<<seconds1<<endl;
	cout<<"Total time to run "<<replacementpolicy<<" algorithm 		:"<<seconds2<<endl;
	if(seconds1 > seconds2){
		double performance = ((seconds1 - seconds2)/seconds1)*100;
		cout<<replacementpolicy<<" is "<<performance<<" % better than "<<initialreplacementpolicy<<endl;
	}else if(seconds2 > seconds1){
		double performance = ((seconds2 - seconds1)*100)/seconds2;
		cout<<initialreplacementpolicy<<" is "<<performance<<"% better than "<<replacementpolicy<<endl;
	}else{
		cout<<"Both algorithms run in approximately same time"<<endl;
	}
	return 0;
}
