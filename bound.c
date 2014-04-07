#include <stdio.h>
#include <windows.h>

typedef struct {
	int threadID;
	int amount;
} ThreadData;

int *buffer = NULL;
int buffer_counter, BUFFER_SIZE;
HANDLE empty, full, mutex;

void init_data() {
	empty = CreateSemaphore(
		NULL,
		0,
		BUFFER_SIZE,
		NULL);
		
	full = CreateSemaphore(
		NULL,
		0,
		BUFFER_SIZE,
		NULL);
		
	mutex = CreateMutex(
		NULL,
		FALSE,
		NULL);
		
	buffer_counter = 0;
}

DWORD WINAPI producer(LPVOID Param) {
	ThreadData threadData = *(ThreadData*) Param;
	int i, counter = 0;
	
	for(; counter < threadData.amount;i++) {
		int integer = threadData.threadID * 1000000 + counter;
		
		WaitForSingleObject(empty, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		
		i = add_item(integer);
		if(i == 0) {
			printf("Producer thread %d produced: %d\n", threadData.threadID, integer);
			ReleaseMutex(mutex);
			ReleaseSemaphore(full,1,NULL);
		} else if(i == -1) {
			fprintf(stderr, "Error in Production thread %d\n", threadData.threadID);
			ReleaseMutex(mutex);
		}
		
		counter++;
	}
	
	return 0;
}
	
DWORD WINAPI consumer(LPVOID Param) {
	ThreadData threadData = *(ThreadData*) Param;
	int i, integer;
	while(TRUE) {
		WaitForSingleObject(empty, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
	
		i = remove_item(&integer);
		if(i == 0) {
			printf("Consumer thread %d consumed: %d\n", threadData.threadID, integer);
			ReleaseMutex(mutex);
			ReleaseSemaphore(empty,1,NULL);
		} else {
			fprintf(stderr, "Error in Consumer thread %d\n", threadData.threadID);
			ReleaseMutex(mutex);
		}
	}
	
	return 0;
}

int add_item(int item) {
	if(buffer_counter < BUFFER_SIZE) {
		buffer[buffer_counter] = item;
		buffer_counter++;
		return 0;
	} else {
		return -1;
	}
}

int remove_item(int *item) {
	if(buffer_counter > 0) {
		item = buffer[(buffer_counter - 1)];
		buffer_counter--;
		return 0;
	} else {
		return -1;
	}
}

int main(int argc, char** argv) {
	int producerNo, consumerNo, productionAmt, i;
	DWORD *threadID;
	HANDLE *hThread;
	ThreadData *thread_data;
	
	if(argc != 5) {
		fputs("Try bound.exe <BUF_SIZE> <PROD_NUM> <CONS_NUM> <PROD_AMT>\n", stderr);
		return 1;
	}
	for(i = 1;i < 5;i++) {
		if(atoi(argv[i]) < 1) {
			fputs("COMMAND-LINE PARAMS NEEDS TO BE GREATER THAN 0\n", stderr);
			return 1;
		}
	}
	if(atoi(argv[4]) > 1000000) {
		fputs("AMT PRODUCED NEEDS TO BE LESS THAN 1000000\n", stderr);
		return 1;
	}
	
	BUFFER_SIZE = atoi(argv[1]);
	producerNo = atoi(argv[2]);
	consumerNo = atoi(argv[3]);
	productionAmt = atoi(argv[4]);
	
	printf("BUFFER_SIZE = %d\nPRODUCER_AMT = %d\nCONSUMER_AMT = %d\nAMT_PRODUCED = %d\n",
		BUFFER_SIZE, producerNo, consumerNo, productionAmt);
	
	init_data();
	
	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	hThread = malloc(sizeof(HANDLE) * (producerNo + consumerNo));
	threadID = malloc(sizeof(DWORD) * (producerNo + consumerNo));
	thread_data = malloc(sizeof(ThreadData) * (producerNo + consumerNo));

	
/*
	for(i = 0; i < (producerNo + consumerNo); i++) {
		thread_data[i].threadID = i;
		if(i < producerNo) {
			hThread[i] = CreateThread(NULL,
						0,
						producer,
						&(thread_data[i]),
						0,
						&threadID[i]);
		} else {
			hThread[i] = CreateThread(NULL,
						0,
						consumer,
						&(thread_data[i]),
						0,
						&threadID[i]);
		}
	}
	WaitForMultipleObjects((producerNo + consumerNo), hThread, TRUE, INFINITE);

	for(i = 0; i < (producerNo + consumerNo); i++)
		CloseHandle(hThread[i]);*/
	CloseHandle(empty);
	CloseHandle(full);
	CloseHandle(mutex);
	
	free(buffer);
	free(hThread);
	free(threadID);
	free(thread_data);
	
	return 0;
}
