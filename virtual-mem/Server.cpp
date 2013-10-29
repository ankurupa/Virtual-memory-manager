/**
@FileName: Server.cpp
@Description: This file is used for implementing various page replacement algorithms.
@Author: Ankur Upadhyay, Research Assistant, Computer Science Department, Univeristy at Buffalo.
*/
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/time.h>
#include "PageList.cpp"
using namespace std;

/**
*Global variables declaration
*/
//integer variable to store number of avaliable memory frames
int numberOfAvaliableFrames = 5; //default number of avaliable frames.
//string variable to store page replacement policy
string replacementpolicy = "FIFO";	//default replacement policy
//string variable to store the input file containing the page reference sequence
string inputFile;
char file[100];
//character array to store the page sequence read from the input file
char pageReferenceSequence[100];
//int counter to record the allocated intial number of frames
int numberOfAllocatedFrames = 5;
//int counter variable to keep track of the number of free frames
int avaliableFreeFrames;
//int counter variable to record initial number of pages in the file
int numberOfInitialPages;
//int counter variable to keep track of the number of pages already allocated
int numberOfAllocatedPages;
//int counter variablle to keep track of the number of page faults
int numberOfPageFaults;
//page to be replaced by Replacement Policy
char pageToBeReplaced;
//variable for tracking time
int startTime;
int endTime;
int elapsedTime;
struct timeval tv;

class Server{
	public:
		void startServer(int, char*[]);
		void readPageReferneceSequence();
		void allocatePage();
		void FIFOPageRepalacemntPolicy(PageList, int);
		void OptimalReplacementPolicy(PageList, int);
		void LFUReplacementPolicy(PageList, int);
		void LRUStackReplacementPolicy(PageList, int);
		void LRUClockReplacementPolicy(PageList, int);
		void LRUReferenceBitsReplacementPolicy(PageList, int);
};

/**
*function used for starting the server. The function accepts the command line arguments
*and sets the various global parameters.
*/
void Server::startServer(int argc, char* argv[])
{
	int params;
	while ((params = getopt(argc, argv, "hf:r:i:")) != -1){

		switch (params) {
			case 'h':
				cout<<"\n -h\t\t : Print a usage summary with all options and exit.\n";
				cout<<"\n -f\t\t : Set the number of available frames. By default it will be 5.\n";
				cout<<"\n -r\t\t : Set the page replacement policy. The options can be FIFO, LFU, LRU-STACK, LRU-CLOCK, LRU-REF8. The default will be FIFO\n";
				cout<<"\n -i\t\t : Read the page reference sequence from the input file\n";
				exit(0);
  				break;
			case 'f':
				numberOfAllocatedFrames = atoi(optarg);
				numberOfAvaliableFrames = atoi(optarg);
				break;
			case 'r':
				replacementpolicy = optarg;
				break;
			case 'i':
				inputFile = optarg;
				strcpy(file, inputFile.c_str());
				break;
			default:
				abort();
				break;
		}
	}
}

/**
*funtion for reading the page sequence from the user specified input file.
*/

void Server::readPageReferneceSequence()
{
	int k = 0;
	numberOfInitialPages = 0;
	char *page;
	char *endPtr;
	FILE *userFile = fopen(file, "r");
	if(file != NULL)
	{
		char line[256];
		while(fgets(line, sizeof(line), userFile) != NULL)
		{
			page = strtok(line, " ");
			while(page != NULL)
			{
				pageReferenceSequence[k] = *page;
				k++;
				numberOfInitialPages++;
				page = strtok(NULL, " ");
			}
		}
		fclose(userFile);
	}else{
		cout<<"Error opening file"<<endl;
	}
}

/**
*function used for allocating pages to the frames in the memory.
*This unction calls page replacement specific functions according
*to the user input.
*/
void Server::allocatePage()
{
	PageList pageList;
	numberOfAvaliableFrames = numberOfAllocatedFrames;
	numberOfAllocatedPages = 0;
	numberOfPageFaults = 0;
	bool pageExists = false;
	gettimeofday(&tv, NULL);
	startTime = tv.tv_usec / 1000;
	//cout<<"Start time for "<<replacementpolicy<<" is"<<startTime<<endl;
	while(numberOfAllocatedPages != (numberOfInitialPages - 2)){
		pageExists = false;
		if(numberOfAllocatedPages != 0)
			pageExists = pageList.findPageInList(pageReferenceSequence[numberOfAllocatedPages]);
		if(pageExists == true){
			if(replacementpolicy.compare("LRU-STACK") == 0)
				pageList.moveElementToLast(pageReferenceSequence[numberOfAllocatedPages]);
			if(replacementpolicy.compare("LRU-REF8") == 0)
				pageList.referenceBitsRightShift(pageReferenceSequence[numberOfAllocatedPages]);
			numberOfAllocatedPages++;
		}
		else{
			if(numberOfAvaliableFrames == 0){
				if(replacementpolicy.compare("OPTIMAL") == 0)
					OptimalReplacementPolicy(pageList, numberOfAllocatedPages);
				else if(replacementpolicy.compare("FIFO") == 0)
					FIFOPageRepalacemntPolicy(pageList, numberOfAllocatedPages);
				else if(replacementpolicy.compare("LFU") == 0)
					LFUReplacementPolicy(pageList, numberOfAllocatedPages);
				else if(replacementpolicy.compare("LRU-STACK") == 0)
					LRUStackReplacementPolicy(pageList, numberOfAllocatedPages);
				else if(replacementpolicy.compare("LRU-CLOCK") == 0)
					LRUClockReplacementPolicy(pageList, numberOfAllocatedPages);
				else
					LRUReferenceBitsReplacementPolicy(pageList, numberOfAllocatedPages);
				numberOfAllocatedPages++;
				numberOfPageFaults++;
			}else{
				if(numberOfAllocatedPages == 0){
					pageList.addFirstPageToList(pageReferenceSequence[numberOfAllocatedPages]);
					if(replacementpolicy.compare("LRU-REF8") == 0)
						pageList.referenceBitsRightShift(pageReferenceSequence[numberOfAllocatedPages]);
				}else{
					pageList.addPageToList(pageReferenceSequence[numberOfAllocatedPages]);
					if(replacementpolicy.compare("LRU-REF8") == 0)
					       pageList.referenceBitsRightShift(pageReferenceSequence[numberOfAllocatedPages]);
				}
				numberOfAllocatedPages++;
				numberOfAvaliableFrames--;
				numberOfPageFaults++;
			}
		}
	}
	gettimeofday(&tv, NULL);
	endTime = tv.tv_usec / 1000;
	/*cout<<"End time for "<<replacementpolicy<<" is"<<endTime<<endl;
	elapsedTime = endTime - startTime;
	cout<<"Elapsed time for "<<replacementpolicy<<" is"<<elapsedTime<<endl;
	cout<<endl<<endl;*/
}

/**
*function used for replacing the page by FIFO replacement algorithm if no frame
*is avaliable for allocation.
*/
void Server::FIFOPageRepalacemntPolicy(PageList pageList, int numberOfAllocatedPagess)
{
	pageList.replacePageByFIFO(pageReferenceSequence[numberOfAllocatedPages]);
}

/**
*function used for replacing the page by LFU replacement algorithm if no frame
*is avaliable for allocation.
*/
void Server::LFUReplacementPolicy(PageList pageList, int numberOfAllocatedPages)
{
	char pageToBeReplaced = pageList.findLFUPage();
	pageList.replacePage(pageToBeReplaced, pageReferenceSequence[numberOfAllocatedPages]);
}

/**
*function used for replacing the page by Optimal replacement algorithm if no frame
*is avaliable for allocation.
*/
void Server::OptimalReplacementPolicy(PageList pageList, int numberOfAllocatedPages)
{
	int pageCounter = numberOfAllocatedPages + 1;
	bool optimalPageExists = false;
	char pagesInListToBeRefereced[numberOfAllocatedFrames];
	int numberOfPages = 0;
	while(pageCounter <= (numberOfInitialPages - 2)){
	     optimalPageExists = pageList.findPageInList(pageReferenceSequence[pageCounter]);
	     if(optimalPageExists == true){
		int found = 0;
		for(int i = 0; i < numberOfPages; i++){
			if(pageReferenceSequence[pageCounter] == pagesInListToBeRefereced[i]){
				found = 1;
				break;
			}
		}
		if(found == 0){
       			pagesInListToBeRefereced[numberOfPages] = pageReferenceSequence[pageCounter];
		   	numberOfPages++;
		}
		}
	     optimalPageExists = false;
	     pageCounter++;
	}
	if(numberOfPages != 0)
		if(numberOfPages == numberOfAllocatedFrames){
			pageList.replacePage(pagesInListToBeRefereced[numberOfPages - 1], pageReferenceSequence[numberOfAllocatedPages]);
		}else{
			char optimalPageToBeReplaced = pageList.findOptimalFirstPage(pagesInListToBeRefereced, numberOfPages);
			pageList.replacePage(optimalPageToBeReplaced, pageReferenceSequence[numberOfAllocatedPages]);
		}
	else
		pageList.replacePageByFIFO(pageReferenceSequence[numberOfAllocatedPages]);
}

/**
*function used for replacing the page by LRU-STACK replacement algorithm if no frame
*is avaliable for allocation.
*/
void Server::LRUStackReplacementPolicy(PageList pageList, int numberOfAllocatedPages)
{
	pageList.replacePageByFIFO(pageReferenceSequence[numberOfAllocatedPages]);
}

/**
*function used for replacing the page by LRU-CLOCK replacement algorithm if no frame
*is avaliable for allocation.
*/
void Server::LRUClockReplacementPolicy(PageList pageList, int numberOfAllocatedPages)
{
	pageList.findLRUClockVictim();
	pageList.replacePageByLRUClock(pageReferenceSequence[numberOfAllocatedPages]);
}

/**
*function used for replacing the page by LRU-REF8 (Additional) replacement algorithm if no frame
*is avaliable for allocation.
*/
void Server::LRUReferenceBitsReplacementPolicy(PageList pageList, int numberOfAllocatedPages)
{
	char LRURefVictim = pageList.findLRUReferenceVictim();
	pageList.replacePage(LRURefVictim, pageReferenceSequence[numberOfAllocatedPages]);
	pageList.referenceBitsRightShift(pageReferenceSequence[numberOfAllocatedPages]);
}





















