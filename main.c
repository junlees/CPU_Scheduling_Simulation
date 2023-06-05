#include "func.h"

int main() {

	//inputData input[14] = { 0, 1, 25, 80,
	//						0, 2, 24, 40,
	//						0, 3, 8, 30,
	//						1, 0, 0, 0,
	//						0, 4, 12, 10,
	//						0, 5, 22, 30,
	//						0, 6, -3, 50,
	//						1, 0, 0, 0,
	//						0, 7, -5, 20,
	//						0, 8, 3, 40,
	//						0, 9, 13, 60,
	//						1, 0, 0, 0,
	//						0, 10, 24, 45,
	//						-1, 0, 0, 0 };
	//int size = 14;
	
	int n = 0;
	int data = 0;
	fp = fopen(FILENAME, "r");
	if (fp == NULL) {
		printf("file open fail\n");
		exit(0);
	}

	while (fscanf(fp, "%d", &data) != EOF) {
		//printf("%d ", data);
		n++;
	}
	fclose(fp);
	int size = n / 4 + 1;

	fp = fopen(FILENAME, "r");
	inputData* input = (inputData*)malloc(sizeof(inputData) * (size));

	for (int i = 0; i < size; i++) {
		fscanf(fp, "%d", &input[i].type);
		if (input[i].type == -1) {
			break;
		}
		fscanf(fp, "%d", &input[i].process_id);
		fscanf(fp, "%d", &input[i].priority);
		fscanf(fp, "%d", &input[i].computing_time);
	}
	fclose(fp);
	int quantomTime = 0;
	printf("Input Quantom Time : ");
	scanf("%d", &quantomTime);
	printf("Quantom Time : %d\n", quantomTime);


	//RealTime Queue
	queue priorityRTQueueHead = NULL;
	queue priorityRTQueueTail = NULL;
	//Priority Queue
	queue priorityQueueHead = NULL;
	queue priorityQueueTail = NULL;
	//Last Priority Queue
	queue priorityLastQueueHead = NULL;
	queue priorityLastQueueTail = NULL;

	//TurnAround Time
	int taTime = 0;
	

	int count = 0;
	while (input[count].type != -1) {

		//type : 0 -> input data
		if (input[count].type == 0) {
			if (input[count].priority < 0) {
				priorityRTQueueHead = insertNode4Priority(priorityRTQueueHead, input[count]);
				priorityRTQueueTail = findTail(priorityRTQueueHead);
			}
			else if (input[count].priority >= MAXPRIORITY) {
				input[count].priority = MAXPRIORITY;
				priorityLastQueueHead = insertNode4ComputingTime(priorityLastQueueHead, input[count]);
				priorityLastQueueTail = findTail(priorityLastQueueHead);
			}
			else {
				priorityQueueHead = insertNode4Priority(priorityQueueHead, input[count]);
				priorityQueueTail = findTail(priorityQueueHead);
			}
		}

		//type : 1 -> scheduling
		else if (input[count].type == 1) {
			//priorityQueueHead = findHead(priorityQueueHead);
			//print head only
			//printf("Head : %d\n", priorityQueueHead->data.process_id);

			int qTime = quantomTime;

			while (qTime != 0) {
				if (priorityRTQueueHead != NULL) {
					priorityRTQueueHead = priorityRTQueueScheduling(priorityRTQueueHead, input, &qTime, &taTime, size);
					priorityLastQueueHead = priorityQueueToLastQueue(priorityQueueHead, priorityLastQueueHead);
					priorityQueueHead = deleteMaxPriority(priorityQueueHead);
				}
				else if (priorityQueueHead != NULL) {
					priorityQueueHead = priorityQueueScheduling(priorityQueueHead, input, &qTime, &taTime, size);

				}
				else if (priorityLastQueueHead != NULL) {
					priorityLastQueueHead = priorityLastQueueScheduling(priorityLastQueueHead, priorityLastQueueTail, input, &qTime, &taTime, size);
				}

				//printf("taTime : %d\n", taTime);
				
			}
			
		}
		count++;
	}
	
	//debug
	//printf("RTQueue\n");
	//queueHeadToTailPrint(priorityRTQueueHead);
	//printf("Queue\n");
	//queueHeadToTailPrint(priorityQueueHead);
	//printf("LQueue\n");
	//queueHeadToTailPrint(priorityLastQueueHead);
	//printf("Tail To Head Queue\n");
	//queueTailToHeadPrint(priorityQueueTail);

	//printf("taTime : %d\n", taTime);

	
	//scheduling
	int qTime = quantomTime;
	while (1) {
		if (priorityRTQueueHead != NULL) {
			priorityRTQueueHead = priorityRTQueueScheduling(priorityRTQueueHead, input, &qTime, &taTime, size);
			//printf("taTime : %d\n", taTime);
		}
		else if (priorityQueueHead != NULL) {
			priorityQueueHead = priorityQueueScheduling(priorityQueueHead, input, &qTime, &taTime, size);
			priorityLastQueueHead = priorityQueueToLastQueue(priorityQueueHead, priorityLastQueueHead);
			priorityQueueHead = deleteMaxPriority(priorityQueueHead);
			//printf("taTime : %d\n", taTime);
		}
		else if (priorityLastQueueHead != NULL) {
			priorityLastQueueTail = findTail(priorityLastQueueHead);
			priorityLastQueueHead = priorityLastQueueScheduling(priorityLastQueueHead, priorityLastQueueTail, input, &qTime, &taTime, size);
			//printf("taTime : %d\n", taTime);
		}
		//printf("LQueue\n");
		//queueHeadToTailPrint(priorityLastQueueHead);
		if (qTime == 0) {
			qTime = quantomTime;
		}
		if (priorityRTQueueHead == NULL && priorityQueueHead == NULL && priorityLastQueueHead == NULL) {
			break;
		}
	}

	
	
	return 0;
}

