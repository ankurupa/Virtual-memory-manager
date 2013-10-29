/**
@FileName: startServer.cpp
@Description: This file is used to perform various page operations when stored
*	       in physical memory frames. It implements the circular doubly linked list.
@Author: Ankur Upadhyay, Research Assistant, Computer Science Department, Unviersity at Buffalo.
*/
#include<iostream>
#include<math.h>
using namespace std;

/**
*struct definition representing the page when stored in the frame.
*/
struct Page
{
	Page *nextPage;
	Page *previousPage;
	char pageNumber;
	//pageFrequency attribute is used for LFU replacement algorithm
	int pageFrequency;
	//clockBit attribute is used for LRU-CLOCK replacement algorithm
	int clockBit;
	//referenceBits and equivalenceDecimalValue is used for LRU-REF8 replacement algorithm
	int referenceBits[8];
	int equivalentDecimalValue;
};

/**
*Global variables declaration
*/
//variable for keeping track of Head Page.
Page *HeadPage;
//variable for keeping track of Last Page.
Page *LastPage;
//variable for keeping track of clock hand. This is
//used for LRU-CLOCK algorithm
Page *clockIterator;

class PageList
{
	public:
		void addFirstPageToList(char);
		void addPageToList(char);
		bool findPageInList(char);
		void replacePageByFIFO(char);
		void replacePage(char, char);
		char findLFUPage();
		char findOptimalFirstPage(char[], int);
		void printList();
		void moveElementToLast(char);
		void findLRUClockVictim();
		void replacePageByLRUClock(char);
		void referenceBitsRightShift(char);
		char findLRUReferenceVictim();
};

/**
*This function is used for printing thr page sequence as they are stored in
*physical memory frames. This used as test function for testing the correctness
*of various page replacement algorithms implementation.
*/
void PageList::printList(){
	Page *temp;
	temp = new Page;
	temp = HeadPage;
	do{
		cout<<temp->pageNumber<<endl;
		temp = temp->nextPage;
	}while(temp != HeadPage);
}

/**
*This function is used for Optimal page replacement algorithm. The method is called
*to find the victim which has been allocated in the memory frame and will not be
*referenced again in the input page sequence. This function selects the first entered
*victim.
*/
char PageList::findOptimalFirstPage(char PageInList[], int numberOfPages){
	Page *temp;
	temp = new Page;
	temp = HeadPage;
	int found = 0;
	do{
		for(int i = 0; i < numberOfPages; i++){
			if(temp->pageNumber == PageInList[i]){
				found = 1;
			}
		}
		if(found == 0)
			break;
		temp = temp->nextPage;
		found = 0;
	}while(temp != HeadPage);
	return temp->pageNumber;
}

/**
*This function is being used by all the page replacement algorithms for adding the first
*page to the physical memory frame.
*/
void PageList::addFirstPageToList(char pageNumber)
{
	HeadPage = new Page;
	LastPage = new Page;
	clockIterator = new Page;
	HeadPage->pageNumber = pageNumber;
	HeadPage->nextPage = HeadPage;
	HeadPage->previousPage = HeadPage;
	HeadPage->pageFrequency = 1;
	HeadPage->clockBit = 1;
	LastPage = HeadPage;
	clockIterator = HeadPage;
	for(int i=1; i<8; i++){
		HeadPage->referenceBits[i] = 0;
	}
}

/**
*This function is being used by all page replacement algorithms for adding the page
*to the physical memory frame if one is not been occupied.
*/
void PageList::addPageToList(char pageNumber)
{
	Page *newPage;
	newPage = new Page;
	newPage->pageNumber = pageNumber;
	newPage->nextPage = HeadPage;
	HeadPage->previousPage = newPage;
	newPage->previousPage = LastPage;
	newPage->pageFrequency = 1;
	newPage->clockBit = 1;
	LastPage->nextPage = newPage;
	LastPage = newPage;
	clockIterator = LastPage;
	for(int i=0; i<8; i++)
	{
		LastPage->referenceBits[i] = 0;
	}
}

/**
*This function is used for LRU-REF8 page replacement algorithm to perform right-shift of the
*reference bits of each allocated page in the memory frame. It also calculated the equivalent
*decimal value from the reference bits.
*/
void PageList::referenceBitsRightShift(char referencedPage)
{
	Page *pageIterator;
	pageIterator = new Page;
	pageIterator = HeadPage;
	do{
		for(int i=7; i>0; i--){
			pageIterator->referenceBits[i] = pageIterator->referenceBits[i-1];
		}
		pageIterator = pageIterator->nextPage;
	}while(pageIterator != HeadPage);
	pageIterator = HeadPage;
	do{
		if(pageIterator->pageNumber == referencedPage)
			pageIterator->referenceBits[0] = 1;
		else
			pageIterator->referenceBits[0] = 0;
		//logic for calculating the equivalent decimal value for the the pages in frames
		pageIterator->equivalentDecimalValue = 0;
		for(int i=0; i<8; i++){
			if(pageIterator->referenceBits[i] == 1){
				int k = i+1;
				pageIterator->equivalentDecimalValue = pageIterator->equivalentDecimalValue + pow(2,(8-k));
			}
		}
		pageIterator = pageIterator->nextPage;
	}while(pageIterator != HeadPage);
}

/**
*This function is used by LRU-REF8 page replacement algorithm for finding the page victim
*which needs to be get replaced by the new incoming page.
*/
char PageList::findLRUReferenceVictim()
{
	Page *pageIterator;
	pageIterator = new Page;
	pageIterator = HeadPage;
	int minimumDecimalValue = pageIterator->equivalentDecimalValue;
	char victim;
	victim = pageIterator->pageNumber;
	do
	{
		if(pageIterator->equivalentDecimalValue < minimumDecimalValue)
		{
			minimumDecimalValue = pageIterator->equivalentDecimalValue;
			victim = pageIterator->pageNumber;
		}
		pageIterator = pageIterator->nextPage;
	}while(pageIterator!= HeadPage);
	return victim;
}

/**
*This function is used by all page replacement algorithms for replacing the new page
*with the victim page in the memory frames.
*/
void PageList::replacePage(char pageToBeReplaced, char newPage)
{
	Page *pageIterator;
	pageIterator = new Page;
	pageIterator = HeadPage;
	do{
		if(pageIterator->pageNumber == pageToBeReplaced){
			if(pageIterator == HeadPage){
				HeadPage = pageIterator->nextPage;
				HeadPage->previousPage = LastPage;
				LastPage->nextPage = HeadPage;
			}else if(pageIterator == LastPage){
				LastPage = pageIterator->previousPage;
				LastPage->nextPage = HeadPage;
				HeadPage->previousPage = LastPage;
			}else{
				pageIterator->previousPage->nextPage = pageIterator->nextPage;
				pageIterator->nextPage->previousPage = pageIterator->previousPage;
			}
			break;
		}
		pageIterator = pageIterator->nextPage;
	}while(pageIterator != HeadPage);
	Page *newPageNode;
	newPageNode = new Page;
	newPageNode->pageNumber = newPage;
	newPageNode->pageFrequency = 1;
	newPageNode->clockBit = 1;
	newPageNode->nextPage = HeadPage;
	HeadPage->previousPage = newPageNode;
	newPageNode->previousPage = LastPage;
	LastPage->nextPage = newPageNode;
	LastPage = newPageNode;
	for(int i=0; i<8 ; i++)
	{
		LastPage->referenceBits[i] = 0;
	}
}

/**
*This function is used by all the page replacement algorithms for finding
*whether the incoming page is already been present in the memory frames or not.
*/
bool PageList::findPageInList(char pageNumber)
{
	Page *pageIterator;
	pageIterator = new Page;
	pageIterator = HeadPage;
	bool pageFound = false;
	do{
		if(pageIterator->pageNumber == pageNumber){
			pageIterator->pageFrequency++;
			pageIterator->clockBit = 1;
			pageFound = true;
			break;
		}
		pageIterator = pageIterator->nextPage;
	}while(pageIterator != HeadPage);
	return pageFound;
}

/**
*This function is used by LFU page replacement algorithm for finding the victim page
*which needs to be get replaced by the new incoming page.
*/
char PageList::findLFUPage()
{
	Page *pageIterator;
	pageIterator = new Page;
	pageIterator = HeadPage;
	char LFUPage = pageIterator->pageNumber;
	int minimumFrequency = pageIterator->pageFrequency;
	do{
		if(pageIterator->pageFrequency < minimumFrequency){
			minimumFrequency = pageIterator->pageFrequency;
			LFUPage = pageIterator->pageNumber;
		}
		pageIterator = pageIterator->nextPage;
	}while(pageIterator != HeadPage);
	return LFUPage;
}

/**
*This function is used for replacing the head page with the new incoming page.
*/
void PageList::replacePageByFIFO(char pageNumber)
{
	Page *newPage;
	newPage = new Page;
	newPage->pageNumber = pageNumber;
	LastPage->pageFrequency = 1;
	LastPage->nextPage = newPage;
	newPage->previousPage = LastPage;
	HeadPage = HeadPage->nextPage;
	newPage->nextPage = HeadPage;
	HeadPage->previousPage = newPage;
	LastPage = newPage;

}

/**
*This function is used by LRU-STACK page replacement algorithm for moving the
*page to top of list whenever one gets refrenced.
*/
void PageList::moveElementToLast(char pageNumber)
{
	Page *pageIterator;
	pageIterator = new Page;
	pageIterator = HeadPage;
	do{
		if(pageIterator->pageNumber == pageNumber){
			if(pageIterator == HeadPage){
				HeadPage = HeadPage->nextPage;
				HeadPage->previousPage = LastPage;
				LastPage->nextPage = HeadPage;
			}else{
				pageIterator->nextPage->previousPage = pageIterator->previousPage;
				pageIterator->previousPage->nextPage = pageIterator->nextPage;
			}
			break;
		}
		pageIterator = pageIterator->nextPage;
	}while(pageIterator != HeadPage);
	LastPage->nextPage = pageIterator;
	pageIterator->previousPage = LastPage;
	pageIterator->nextPage = HeadPage;
	HeadPage->previousPage = pageIterator;
	LastPage = pageIterator;
}

/**
*This function is used by LRU-CLOCK page replacement algorithm for finding out
*the victim page which needs to be get replaced by the new incoming page.
*/
void PageList::findLRUClockVictim()
{
	clockIterator = clockIterator->nextPage;
	while(clockIterator->clockBit != 0){
		clockIterator->clockBit = 0;
		clockIterator = clockIterator->nextPage;
	}
}

/**
*This function is used by LRU-CLOCK page replacement algorithm for replacing
*the victim page with the new page.
*/
void PageList::replacePageByLRUClock(char newPage)
{
	clockIterator->pageNumber = newPage;
	clockIterator->clockBit = 1;
}


