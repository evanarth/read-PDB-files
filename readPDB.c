/* This file is designed to read in a PDB and output it using C-formats
 * Notice that it only occupies a user-def memory block at a time, making 
 * it a lightweight program (see line 11).
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// RAM block size used for reading in data
#define RAMusage 95

int main ( int argc, char *argv[] ) {
	
	printf ("Opening PDB file \"%s\".\n", argv[1]);
	
	// array must be (num. characters + 1) length for end-of-line byte!
	// strangely enough, declaring character arrays without defining them creates errors
	
	//char molType[7], atmNumTmp[6],  atomName[5], altLoc[2], resName[4];
	//char resNumTmp[5], Xtmp[9], Ytmp[9], Ztmp[9], chainID[2], occupTmp[7];
	//char betaTmp[7], comments[15];
	
	// declare and fill all output character arrays to eliminate errors
	char molType[7] = { ' ', ' ', ' ', ' ', ' ', ' ' };
	char atmNumTmp[6] = { ' ', ' ', ' ', ' ', ' ' };
	char atomName[5] = { ' ', ' ', ' ', ' ' };
	char altLoc[2] = { ' ' };
	char resName[4] = { ' ', ' ', ' ' };
	char resNumTmp[5] = { ' ', ' ', ' ', ' ' };
	char Xtmp[9] = { ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ' };
	char Ytmp[9] = { ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ' };
	char Ztmp[9] = { ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ' };
	char chainID[2] = { ' ' };
	char occupTmp[7] = { ' ', ' ', ' ', ' ', ' ', ' ' };
	char betaTmp[7] = { ' ', ' ', ' ', ' ', ' ', ' ' };
	char comments[15] = { ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	
	// logical and input variables
	char fileBuffer[RAMusage]; // input character array from file
	int atomNum, resNum; // output integers
	double X, Y, Z, occup, beta; // output floats
	int i, j, endOfLine, endOfComment, size; // logical integers
	
	// open file, check file valid, check line of text exists
	FILE *fileID;
	fileID = fopen( argv[1], "r" );
	if ( fileID != NULL ) {
		
		while ( fgets( fileBuffer, sizeof (fileBuffer), fileID ) != NULL ) {
			
			// check if text contains valid data
			j = 0; for ( i =  0; i <  6; i++ ) { molType[j]   = fileBuffer[i]; j++; }
			for ( i = 0; fileBuffer[i] != '\0' ; i++ ) {} endOfLine = i - 1;// find end of line
			
			if ( memcmp( molType, "ATOM" , 4 ) == 0 && endOfLine > 66 ) {
				
				// split line into various data types
				j = 0; for ( i =  6; i < 11; i++ ) { atmNumTmp[j] = fileBuffer[i]; j++; }
				j = 0; for ( i = 12; i < 16; i++ ) { atomName[j]  = fileBuffer[i]; j++; }
				j = 0;       i = 16;                 altLoc[j]    = fileBuffer[i];
				j = 0; for ( i = 17; i < 20; i++ ) { resName[j]   = fileBuffer[i]; j++; }
				j = 0;       i = 21;                 chainID[j]   = fileBuffer[i];
				j = 0; for ( i = 22; i < 26; i++ ) { resNumTmp[j] = fileBuffer[i]; j++; }
				j = 0; for ( i = 30; i < 38; i++ ) { Xtmp[j]      = fileBuffer[i]; j++; }
				j = 0; for ( i = 38; i < 46; i++ ) { Ytmp[j]      = fileBuffer[i]; j++; }
				j = 0; for ( i = 46; i < 54; i++ ) { Ztmp[j]      = fileBuffer[i]; j++; }
				j = 0; for ( i = 54; i < 60; i++ ) { occupTmp[j]  = fileBuffer[i]; j++; }
				j = 0; for ( i = 60; i < 66; i++ ) { betaTmp[j]   = fileBuffer[i]; j++; }
				
				// deal with unknown length of comment section
				endOfComment = endOfLine; 
				if ( endOfLine > ( sizeof( comments ) + 65 ) ) 
					{ endOfComment = ( sizeof( comments ) + 65 ); }
				j = 0; for ( i = 66; i < endOfComment; i++ ) { comments[j]  = fileBuffer[i]; j++; }
				
				// convert text to numbers where appropriate
				atomNum      = atof( atmNumTmp );
				resNum       = atof( resNumTmp );
				X     = atof( Xtmp );
				Y     = atof( Ytmp );
				Z     = atof( Ztmp );
				occup = atof( occupTmp );
				beta  = atof( betaTmp );
				
				// print out data of each line displaying interpreted format
				printf("%-5s%5d %4s%1s%3s %1s%4d    %8.3lf%8.3lf%8.3lf%6.2lf%6.2lf%s\n", 
					   molType, atomNum, atomName, altLoc, resName, chainID, 
					   resNum, X, Y, Z, occup, beta, comments);
				
			} else { printf( "%s", fileBuffer ); } // don't interpret non-recognized lines
		}
	} else { printf("File could not be opened\n"); } // if file opening failed
	
	fclose( fileID );
	
	return 0;
}

