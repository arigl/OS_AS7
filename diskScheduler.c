/*
Name: Alex Rigl
ID: 2300146
Assignment: Programming Assignment #7
Due Date: 5/4/2020
Worked with:
- Filip Augustkowski
- Frank Entriken

Project Description:
The objective of this assignment consists of writing a C/C++ program that simulates the various disk-scheduling algorithms

Run with:
./diskScheduler <initial cylinder position> <cylinder request file>

Resources:
- https://www.geeksforgeeks.org/program-for-sstf-disk-scheduling-algorithm/
  - Foundation for the structure

*/

//----VARIABLES AND PACKAGES-----------------------------------------------
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
int reqArgs = 3;
int queueSize = 1000;
int initCylinderPos;
FILE *cylinderFile;
void FCFS();
void OtherAlgorithms();

typedef struct{
  int value;
  int accessed;
}Node;


int main(int argc, char** argv)
{
    /* From check input, we check input and open files */
    if (!CheckInput(argc, argv)) {
      printf("ERROR: Incorrect input ");
      exit(EXIT_FAILURE);
    }

    // Execute main functions
    FCFS();
    OtherAlgorithms();
    fclose(cylinderFile);

    // End program
    return 1;
}

/* Moves to each request as they come in */
void FCFS()
{
    char fileLine[6];
    int first = 1;
    int currNum;
    int lastNum;
    int headMovement = 0;
    // Scan each line for info, then calculate the headmovement
    while (fgets(fileLine, 6, cylinderFile) != NULL) {
      sscanf(fileLine, "%d", &currNum);
      if (first) {
        headMovement = headMovement + abs(initCylinderPos - currNum);
        first = -1;
      }
      else {
        headMovement = headMovement + abs(lastNum - currNum);
      }
      lastNum = currNum;
    }
    printf("Total Head Movement for FCFS: %d\n", headMovement);
}

/* Takes in the queue, then checks what the closest node request is to the current head position */
void ReadSSTF(int _queue[])
{
    /* Setup strucuts */
    Node queue[queueSize];
    int headPos = initCylinderPos;
    int headMovement = 0;
    int shortestDistance = 9999;
    int shortestDistanceIndex = -1;
    int i
    int j;

    /* Setup struct array */
    for (i = 0; i < queueSize; i++)
      queue[i].value = _queue[i]
      queue[i].accessed = 0;
    }

    /* For each value with our queue, Find the shortest distance from head and mark that request as visisted */
    /* Once a request is visited, head movement is calculated, shortestDistance is maxed, and we find the next request */
    for (i = 0; i < queueSize; i++) {
      for (j = 0; j < queueSize; j++) {
        int temp = abs(headPos - queue[j].value);
        if (temp < shortestDistance && queue[j].accessed == 0 && temp != 0) {
          shortestDistance = temp;
          shortestDistanceIndex = j;
        }
      }
      queue[shortestDistanceIndex].accessed = 1;
      headMovement = headMovement + abs(headPos - queue[shortestDistanceIndex].value);
      headPos = queue[shortestDistanceIndex].value;
      shortestDistance = 9999;
    }
    printf("Total Head Movement for SSTF: %d\n", headMovement);
}

/* Similar to SCAN but arm only goes as far as last request and reverses */
void ReadLOOK(int _queue[])
{
    /* Setup strucuts */
    Node queue[queueSize];
    int headPos = initCylinderPos;
    int headMovement = 0;
    int i;
    int j;
    /* Setup struct array */
    for (i = 0; i < queueSize; i++) {
      queue[i].value = _queue[i];
      queue[i].accessed = 0;
    }
    /* Choose a direction */
    if (abs(initCylinderPos - queueSize) > (queueSize/2)-1) {
      /* Go left */
      for (i = initCylinderPos; i > 0; i--) {
        for (j = 0; j < queueSize; j++){
          if (i == queue[j].value && queue[j].accessed == 0)
          {
            queue[j].accessed = 1;
            headMovement = headMovement + abs(headPos - queue[j].value);
            headPos = queue[j].value;
          }
        }
      }
      /* Now go right from our last position without hitting the end of the disk */
      for (i = initCylinderPos; i < queueSize; i++) {
        for (j = 0; j < queueSize; j++){
          if (i == queue[j].value && queue[j].accessed == 0)
          {
            queue[j].accessed = 1;
            headMovement = headMovement + abs(headPos - queue[j].value);
            headPos = queue[j].value;
          }
        }
      }
    }
    else {
      /* Go Right */
      for (i = initCylinderPos; i < queueSize; i++) {
        for (j = 0; j < queueSize; j++){
          if (i == queue[j].value && queue[j].accessed == 0)
          {
            queue[j].accessed = 1;
            headMovement = headMovement + abs(headPos - queue[j].value);
            headPos = queue[j].value;
          }
        }
      }
      /* Now go left */
      for (i = initCylinderPos; i > 0; i--) {
        for (j = 0; j < queueSize; j++){
          if (i == queue[j].value && queue[j].accessed == 0)
          {
            queue[j].accessed = 1;
            headMovement = headMovement + abs(headPos - queue[j].value);
            headPos = queue[j].value;
          }
        }
      }
    }
    printf("Total Head Movement for LOOK: %d\n", headMovement);
}

/* Similar to LOOK in that it only goes as far as last request */
/* However the reversal forces this to start at the last request on the other end of the disk */
void ReadCLOOK(int _queue[])
{
    /* Setup strucuts */
    Node queue[queueSize];
    int headPos = initCylinderPos;
    int headMovement = 0;
    int i;
    int j;
    /* Setup struct array */
    for (i = 0; i < queueSize; i++) {
      queue[i].value = _queue[i];
      queue[i].accessed = 0;
    }
    /* Choose a direction */
    if (abs(initCylinderPos - queueSize) > (queueSize/2)-1) {
      /* Go left */
      for (i = initCylinderPos; i > 0; i--) {
        for (j = 0; j < queueSize; j++){
          if (i == queue[j].value && queue[j].accessed == 0)
          {
            queue[j].accessed = 1;
            headMovement = headMovement + abs(headPos - queue[j].value);
            headPos = queue[j].value;
          }
        }
      }
      /* Now start from right most request without hitting the end of the disk, this is the only difference in LOOK and CLOOK */
      for(i = queue[queueSize-1].value; i > 0; i--) {
        for(j = 0; j < queueSize; j++){
          if(i == queue[j].value && queue[j].accessed == 0)
          {
            queue[j].accessed = 1;
            headMovement = headMovement + abs(headPos - queue[j].value);
            headPos = queue[j].value;
          }
        }
      }
    }
    else {
      /* Go Right */
      for (i = initCylinderPos; i < queueSize; i++) {
        for (j = 0; j < queueSize; j++){
          if (i == queue[j].value && queue[j].accessed == 0)
          {
            queue[j].accessed = 1;
            headMovement = headMovement + abs(headPos - queue[j].value);
            headPos = queue[j].value;
          }
        }
      }
      /* Now start from left most request without hitting the end of the disk, this is the only difference in LOOK and CLOOK */
      for (i = queue[0].value; i < queueSize; i++) {
        for (j = 0; j < queueSize; j++){
          if (i == queue[j].value && queue[j].accessed == 0)
          {
            queue[j].accessed = 1;
            headMovement = headMovement + abs(headPos - queue[j].value);
            headPos = queue[j].value;
          }
        }
      }
    }
    printf("Total Head Movement for CLOOK: %d\n", headMovement);
}

/* Similar to SCAN however here we choose a direction, and service requests until hitting the other end */
/* Upon hitting the other end of disk, the disk head moves to the opposite end of the disk instead of reversing */
void ReadCSCAN(int _queue[])
{
    /* Setup strucuts */
    Node queue[queueSize];
    int headPos = initCylinderPos;
    int headMovement = 0;
    int shortestDistance = 9999;
    int shortestDistanceIndex = -1;
    int i;
    int j;
    /* Setup struct array */
    for (i = 0; i < queueSize; i++) {
      queue[i].value = _queue[i];
      queue[i].accessed = 0;
    }
    /* Go right */
    for (i = initCylinderPos; i < queueSize; i++) {
      for (j = 0; j < queueSize; j++) {
        if (i == queue[j].value && queue[j].accessed == 0) {
          queue[j].accessed = 1;
          headMovement = headMovement + abs(headPos - queue[j].value);
          headPos = queue[j].value;
        }
      }
    }
    /* Start at left */
    for (i = 0; i < queueSize; i++) {
      for (j = 0; j < queueSize; j++) {
        if (i == queue[j].value && queue[j].accessed == 0) {
          queue[j].accessed = 1;
          headMovement = headMovement + abs(headPos - queue[j].value);
          headPos = queue[j].value;
        }
      }
    }
    printf("Total Head Movement for CSCAN: %d\n", headMovement);
}
/* SCAN goes in one direction and services requests along the way */
/* After hitting the end of the disk, it the head reverses and goes right */
void ReadSCAN(int _queue[])
{
    /* Setup strucuts */
    Node queue[queueSize];
    int headPos = initCylinderPos;
    int headMovement = 0;
    int shortestDistance = 9999;
    int shortestDistanceIndex = -1;
    int i;
    int j;
    /* Setup struct array */
    for(i = 0; i < queueSize; i++) {
      queue[i].value = _queue[i];
      queue[i].accessed = 0;
    }
    /* Start by going left */
    for (i = initCylinderPos; i > 0; i--) {
      for (j = 0; j < queueSize; j++) {
        if (queue[j].value == i && queue[j].accessed == 0) {
          queue[j].accessed = 1;
          headMovement = headMovement + abs(headPos - queue[j].value);
          headPos = queue[j].value;
        }
      }
    }
    headMovement = headMovement + 2 *(headPos); /* Since we go to the end of the disk and reverse, we need to account for that head movement */
    /* Go from the last place we were, when hitting left side of disk. And head to right from */
    for (i = headPos; i < queueSize; i++) {
      for (j = 0; j < queueSize; j++) {
        if (queue[j].value == i && queue[j].accessed == 0) {
          queue[j].accessed = 1;
          headMovement = headMovement + abs(headPos - queue[j].value);
          headPos = queue[j].value;
        }
      }
    }
    printf("Total Head Movement for SCAN: %d\n", headMovement);
}
/* Function to call all algorithms besides FCFS because it is inherintley different from the others */
void OtherAlgorithms()
{
    char fileLine[6];
    int iteration = 0;
    int currNum;
    int queue[queueSize];
    /* Get all 1000 numbers */
    while (fgets(fileLine, 6, cylinderFile) != NULL) {
      sscanf(fileLine, "%d", &currNum); /* scan each line for info */
      queue[iteration] = currNum;
      iteration++;
    }
    ReadSSTF(queue);
    ReadSCAN(queue);
    ReadCSCAN(queue);
    ReadLOOK(queue);
    ReadCLOOK(queue);
}

//----METHODS----------------------------------------------------------------
/* Method that checks input from command line */

int CheckInput(int argc, char** argv)
{
    /* Fetch file names and try to open them */
    if (argc == reqArgs) { /*Check number of args */
      initCylinderPos = atoi(argv[1]);
      cylinderFile = fopen(argv[2], "r");
       /* Check if files are opened */
      if (initCylinderPos < 0 && initCylinderPos > 4999) {
        printf("ERROR: Cylinder position must be greater than or equal to 0 and must be less than 5000 \n");
        exit(EXIT_FAILURE);
      }
      else if (cylinderFile == NULL) {
        printf("ERROR: Cylinders file cannot open \n");
        exit(EXIT_FAILURE);
      }
      else { /* Input is correct */
       return 1;
     }
    }
    else {
      printf("ERROR: Not enough arguments \n");
      exit(EXIT_FAILURE);
    }
}
