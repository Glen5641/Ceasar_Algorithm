
/*
 * ceasar.c -- ceasar algorithm program
 *
 * <Timed>
 * Send an array of characters across the processes and shift 
 * them by k characters. Return the character array to master
 * <Timed> 
 * and shift them back while checking integrity of array.
 *
 * Input: none.
 * Output: contents of rank 0 messages
 *
 * Modified by Clayton Glenn.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

int main (int argc, char* argv[]) { /* main */

    	const int 	first_rank  = 0;       /* Ranks run 0..number_of_processes-1 */
    	const int 	master_rank = 0;       /* Rank of master process             */
    	const int 	tag         = 0;       /* Tag for messages                   */
    	MPI_Status 	recv_status;           /* Return status for receive          */
    	int        	number_of_processes;   /* Number of processes in this run    */
    	int        	my_rank;               /* Rank of this process               */
    	int        	source;                /* Rank of sender                     */
    	int        	destination;           /* Rank of receiver                   */
    	int        	mpi_error_code;        /* Error code returned by MPI call    */
	int 		size 		= pow(2.0, 10);		       /* Array Size */
	int		array_length 	= (size) * sizeof(char) + 1; /* Array Length */

	/* Initiallize MPI Communication */
    	mpi_error_code = MPI_Init(&argc, &argv);
    	mpi_error_code = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    	mpi_error_code = MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

	// Create local array of length = local size 
	int 	local_size = (size / number_of_processes) * sizeof(char);
	char 	*local_arr;

	// Create master array of length = size
   	char 	array[size];

	/* If process is not Master process */
    	if (my_rank != master_rank) {

		// Allocate memory for array and zero array out
		local_arr = malloc(local_size);
		bzero(local_arr, local_size);

		// Declare a key for the process
		int key;

		// Receive the broadcasted key
		mpi_error_code = MPI_Bcast(&key, sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);

		mpi_error_code = MPI_Recv(local_arr, local_size, MPI_CHAR, 0, tag, 
				MPI_COMM_WORLD, &recv_status);

		// Increment each local character by key
		int i = 0;
		while(i < local_size) {
			if(local_arr[i] != 0) {
				local_arr[i] = (local_arr[i] + key) % 256;
			}
			i++;
		}		

		// Send the local array back to the master process
		mpi_error_code = MPI_Send(local_arr, local_size, MPI_CHAR, 0, tag, 
					MPI_COMM_WORLD);

		// Free the local array
		free(local_arr);

	/* If process is Master process */
    	} else {

		// Declare character getters
		int c;
		int i = 0;
		int key = 0;
		char num[20];	
		
		// Print size and processes
		fprintf(stdout, "\nSend and Receive\n");
		fprintf(stdout, "Running Program with Size: %d\n", size);
		fprintf(stdout, "Running Program with %d Processes\n", number_of_processes);


		// Get the key from the first line of the file
		while((c = getchar()) != '\n' && c != EOF && i < 20) {
			num[i] = c;
			i++;
		}

		// Parse the key to an Integer	
		key = (int)strtol(num, (char**)NULL, 10);
		fprintf(stdout, "key: %d\n", key);

		// Zero out array for file read
		bzero(array, array_length);

		// Read oversized file to populate array
		i = 0;
		while ((c = getchar()) != EOF && i < size) {
			array[i] = c;
			i++;
		}

		// Start program time
		double time1 = MPI_Wtime();

		// Allocate memory for the local and send array and zero it out
		local_arr = malloc(local_size);
		bzero(local_arr, local_size);
		char *send_arr = malloc(local_size);		
		bzero(send_arr, local_size);

		// Broadcast the key to each process
		mpi_error_code = MPI_Bcast(&key, sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);

		// Populate its own local array
		i = 0;
		while(i < local_size) {
			local_arr[i] = array[i];
			i++;
		}		

		//Create a send array of local_size for p processes and send to correct process
		int j = 1;
		int k = 0;
		while(j < number_of_processes && i < size) {
			while(k < local_size && i < size) {
				send_arr[k] = array[i];
				k++;
				i++;
			}
			mpi_error_code = MPI_Send(send_arr, local_size, MPI_CHAR, j, tag, 
						MPI_COMM_WORLD);
			bzero(send_arr, local_size);
			j++;
			k = 0;
		}

		// Increment the local array's characters by key
		i = 0;
		while(i < local_size) {
			if(local_arr[i] != 0) {
				local_arr[i] = (local_arr[i] + key) % 256;
			}
			i++;
		}		

		// Zero out the master array to ensure new values
		bzero(array, size);

		i = 0;
		while(i < local_size) {
			array[i] = local_arr[i];
			i++;
		}

		// Get the local arrays from the processes and join the local arrays
		j = 1;
		k = 0;
		while(j < number_of_processes && i < size) {
			bzero(send_arr, local_size);
			mpi_error_code = MPI_Recv(send_arr, local_size, MPI_CHAR, j, tag, 
						MPI_COMM_WORLD, &recv_status);
			k = 0;	
			while(k < local_size && i < size) {
				array[i] = send_arr[k];
				k++;
				i++;
			}
			j++;
		}


		// End time and print elapsed time
		double time2 = MPI_Wtime();
		fprintf(stdout, "Elapsed time of the program is %.10f seconds\n\n", 
				(time2 - time1));

		///////////////////////////////////////////////////////////////////
		// Print contents before
		///////////////////////////////////////////////////////////////////
		/*
		// Print the characters to console to check correctness
		i = 0;
		while(i < size) {
			fprintf(stdout, "%c", array[i]);
			i++;
		}
		fprintf(stdout, "\n\n");
		*/

		// Decrement the characters by key
		i = 0;
		while(i < size) {
			if(array[i] != 0) {
				array[i] = (array[i] + 256 - key) % 256;
			}
			i++;
		}		

		///////////////////////////////////////////////////////////////////
		// Print contents after
		///////////////////////////////////////////////////////////////////
		/*// Print the new characters of the array
		i = 0;
		while(i < size) {
			fprintf(stdout, "%c", array[i]);
			i++;
		}
		fprintf(stdout, "\n");
		*/

		// Free local array
		free(local_arr);

    	}

	// Close MPI Connection
    	mpi_error_code =
        	MPI_Finalize();
}
