#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define QUANTUMTIME 20
#define MAXPRIORITY 31
#define INCREASEPRIORITY 10
#define FILENAME "input.txt"


typedef struct _inputData {
	int type;
	int process_id;
	int priority; 
	int computing_time;
}inputData;


typedef struct _process {
	int type;
	int process_id;
	int queue_id;
	int priority;
	int computing_time;
	int turnaround_time;
}process;


typedef struct _qNode {
	process data;
	struct _qNode* next;
	struct _qNode* prev;
}qNode;


typedef qNode* queue;



FILE* fp;

queue createNodeByInput(inputData);
queue createNodeByProcess(queue);
queue insertNode4Priority(queue, inputData);
queue sortNode4Priority(queue);
queue insertNode4ComputingTime(queue, inputData);
queue sortNode4ComputingTime(queue);
queue defineQeuID(queue);
queue findTail(queue);
queue findHead(queue);
queue priorityQueueScheduling(queue, inputData[], int*, int*, int);
queue priorityRTQueueScheduling(queue, inputData[], int*, int*, int);
queue priorityLastQueueScheduling(queue, queue, inputData[], int*, int*, int);
queue priorityQueueToLastQueue(queue, queue);
queue deleteMaxPriority(queue);
queue addToQueue(queue, queue);
//queue getLastNode(queue);
void resultProcessed(queue, int, int);
void queueHeadToTailPrint(queue);
void queueTailToHeadPrint(queue);
