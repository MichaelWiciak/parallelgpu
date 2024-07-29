//
// Starting code for the MPI coursework.
//
// Compile with:
//
// mpicc -Wall -o cwk2 cwk2.c
//
// or use the provided makefile.
//


//
// Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


// Some extra routines for this coursework. DO NOT MODIFY OR REPLACE THESE ROUTINES,
// as this file will be replaced with a different version for assessment.
#include "cwk2_extra.h"


//
// Case is not considered (i.e. 'a' is the same as 'A'), and any non-alphabetic characters
// are ignored, so only need to count 26 different possibilities.
//
#define MAX_LETTERS 26


//
// Main
//
int main( int argc, char **argv )
{
	int i, lc;

	// Initialise MPI and get the rank and no. of processes.
	int rank, numProcs;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &numProcs );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank     );

	// Read in the text file to rank 0.
	char *fullText = NULL;
	int totalChars = 0;
	if( rank==0 )
	{
		// Try to read in the file. The pointer 'text' must be free()'d before termination. Will add spaces to the
		// end so that the total size of the text array is a multiple of numProcs. Will print an error message and
		// return NULL if the operation could not be completed for any reason.
		fullText = readText( "input.txt", &totalChars, numProcs );
		if( fullText==NULL )
		{
			MPI_Finalize();
			return EXIT_FAILURE;
		}

		printf( "Rank 0: Read in text file with %d characters (including padding).\n", totalChars );
	}

	// The final global histogram - declared for all processes but the final answer will only be on rank 0.
	int globalHist[MAX_LETTERS];
	for( i=0; i<MAX_LETTERS; i++ ) globalHist[i] = 0;		// Initialise to zero.

	// Calculate the number of characters per process. Note that only rank 0 has the correct value of totalChars
	// (and hence charsPerproc) at this point. Also, we know by this point that totalChars is a multiple of numProcs.
	int charsPerProc = totalChars / numProcs;

	// Start the timing.
	double startTime = MPI_Wtime();


	//
	// Your solution will primarily go here, although dynamic memory allocation and freeing may go elsewhere.
	//

	// Determine if the number of processes is a power of 2
	int isPowerOf2 = -1;
	if( numProcs && ((numProcs&(numProcs-1))==0) ){
		isPowerOf2 = 1;
	}
	else{
		isPowerOf2 = 0;
	}

	// printf( "Number of processes is a power of 2: %d\n", isPowerOf2 );


	// so rank 0 is the only one that read the file so it needs to first distribute the problem size to all processes using MPI_Bcast
	// this is done only if the number of processes is not a power of 2
	
	if (isPowerOf2 == 0){
		MPI_Bcast(&totalChars, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else{
		// instead use point-to-point communication in a inverse binary tree pattern
		// the number of levels is log2(p)
		int numLevels=0; while(1<<numLevels<=numProcs)numLevels++;
		// if (rank == 0){
		// 	printf("Number of levels: %d\n", numLevels);
		// }
		// the way this work is that there are levels to the inverse binary tree
		// so at the top, there is only rank 0 which will send the totalChars to the next level to multiple processes (0, 1)
		// then at the next level, each process sends the totalCharts to the next level by communicating with 2 processes (itself and itself+p/2)
		// this continues until the last level where all processes have the totalChars
		// so for example if the number of processes is 8, this will happen
		// Level 0: 0 -> 0, 1
		// Level 1: 0 -> 0, 2 and 1 -> 1, 3
		// Level 2: 0 -> 0, 4 and 1 -> 1, 5 and 2 -> 2, 6 and 3 -> 3, 7
		// so the number of processes that will be communicated with at each level is 2^level

		// so the first level is rank 0 sending the totalChars to rank 1 and rank 0

		// we know rank 0 will start, we know the number of levels, we know the number of processes
		// so we can determine the number of processes involved at each level

		// note that it is redundant to send to self if a process already has the size so won't send 0 -> 0, 1 just 0-> 1 etc

		
		for (int level=0; level<numLevels-1; level++){
			// print the level and rank 
			// if (rank == 0){
				// printf("Level %d\n", level);
			// }
			// need to determine if at this level, the rank is a sender or receiver or idle (as only a number of processes are involved at each level)

			// so at level 0, 2^0=1 process is involved, so rank 0 is the sender and rank 1 is the receiver
			// at level 1, 2^1=2 processes are involved, so rank 0 and rank 1 are the senders and rank 2 and rank 3 are the receivers
			// rank 0 sends to rank 2 and rank 1 sends to rank 3
			// at level 2, 2^2=4 processes are involved, so rank 0, rank 1, rank 2, rank 3 are the senders and rank 4, rank 5, rank 6, rank 7 are the receivers
			// rank 0 sends to rank 4, rank 1 sends to rank 5, rank 2 sends to rank 6, rank 3 sends to rank 7
			
			// implementing this below
			// determine the number of processes involved at this level
			int numProcessesAtLevel = 1<<level;
			// if (rank == 0){
			// 	printf("Number of processes at level %d: %d\n", level, numProcessesAtLevel);
			// }
			// determine if the rank is a sender or receiver or idle
			if (rank < numProcessesAtLevel){
				// printf("Rank %d is a sender\n", rank);
				// sender
				// determine the receiver
				int receiver = rank + numProcessesAtLevel;
				// printf("Rank %d sending to rank %d\n", rank, receiver);
				// send the totalChars to the receiver
				MPI_Send(&totalChars, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);
			}
			else if (rank < 2*numProcessesAtLevel){
				// printf("Rank %d is a receiver\n", rank);
				// receiver
				// determine the sender
				int sender = rank - numProcessesAtLevel;
				// printf("Rank %d receiving from rank %d\n", rank, sender);
				// receive the totalChars from the sender
				MPI_Recv(&totalChars, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}

		}

		
	}

	// only need to change the value of the charsPerProc if the rank is not 0 since rank 0 knows
	if (rank != 0){
		charsPerProc = totalChars / numProcs;
		// print what rank has what value of charsPerProc
		// printf("Rank %d: charsPerProc = %d\n", rank, charsPerProc);
	}

	// now all processes can allocate memory for their part of the text
	char *localText = (char *)malloc((charsPerProc + 1) * sizeof(char));
	if( localText==NULL )
	{
		MPI_Finalize();
		// print error message and return EXIT_FAILURE
		// printf("Error: Could not allocate memory for localText\n");
		return EXIT_FAILURE;
	}

	// now rank 0 needs to distribute the text to all processes using MPI_Scatter
	MPI_Scatter(fullText, charsPerProc, MPI_CHAR, localText, charsPerProc, MPI_CHAR, 0, MPI_COMM_WORLD);

	// now all processes can calculate their local histogram
	int localHist[MAX_LETTERS];
	for( i=0; i<MAX_LETTERS; i++ ) localHist[i] = 0;		// Initialise to zero.

	// Construct the local histogram.
	for( i=0; i<charsPerProc; i++ )
		if( (lc=letterCodeForChar(localText[i]))!= -1 )
			localHist[lc]++;
	
	// now all processes can reduce their local histograms to the global histogram using MPI_Reduce
	MPI_Reduce(localHist, globalHist, MAX_LETTERS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	
	// Complete the timing and output the result.
	if( rank==0 )
		printf( "Distribution, local calculation and reduction took a total time: %g s\n", MPI_Wtime() - startTime );

	//
	// Check against the serial calculation (rank 0 only).
	//
	if( rank==0 )
	{
		printf( "\nChecking final histogram against the serial calculation.\n" );

		// Initialise the serial check histogram.
		int serialHist[MAX_LETTERS];
		for( i=0; i<MAX_LETTERS; i++ ) serialHist[i] = 0;

		// Construct the serial histogram as per the parallel version, but over the whole text.
		for( i=0; i<totalChars; i++ )
			if( (lc=letterCodeForChar(fullText[i]))!= -1 )
				serialHist[lc]++;

		// Check for errors (i.e. differences to the serial calculation).
		int errorFound = 0;
		for( i=0; i<MAX_LETTERS; i++ )
		{
			if( globalHist[i] != serialHist[i] ) errorFound = 1;
			printf( "For letter code %2i ('%c' or '%c'): MPI count (serial check) = %4i (%4i)\n", i, 'a'+i, 'A'+i, globalHist[i], serialHist[i] );
		}

		if( errorFound )
			printf( "- at least one error found when checking against the serial calculation.\n" );
		else
			printf( " - globalHist has the same values as the serial check.\n" );
	}

	//
	// Clear up and quit.
	//
	if( rank==0 )
	{
		saveHist( globalHist, MAX_LETTERS );			// Defined in cwk2_extras.h; do not change or replace the call.
		free( fullText );
		free( localText );
	}
	else
	{
		free( localText );
	}
	
	MPI_Finalize();
	return EXIT_SUCCESS;
}