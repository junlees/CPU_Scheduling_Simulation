#include <stdio.h>

//입력 데이터
typedef struct _inputProcess {
	int type; 
	int process_id;
	int priority; //우선
	int computing_time;
}inputProcess;

typedef struct _outputProcess {
	int process_id;
	int queue_id;
	int priority;
	int computing_time;
	int turnaround_time;
}outputProcess;

#define QUANTUMTIME = 20

int main() {
	inputProcess input[14] = {	 0, 1, 25, 80,
								 0, 2, 15, 40,
								 0, 3, 8, 30,
								 1, 0, 0, 0,
								 0, 4, 12, 10,
								 0, 5, 22, 30,
								 0, 6, 28, 50,
								 1, 0, 0, 0,
								 0, 7, 5, 20,
								 0, 8, 3, 40,
								 0, 9, 13, 60,
								 1, 0, 0, 0,
								 0, 10, 24, 45,
								 -1, 0, 0, 0 };



	return 0;
}


