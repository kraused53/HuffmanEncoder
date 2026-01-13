#include <stdio.h>
#include <stdlib.h>

const char* filepath = "./text_files/test1.txt";

/* Text information struct */
typedef struct {
	int character_freqs[ 256 ];
	unsigned char* text;
	int text_len;
} TextInfo;

void init_TextInfo( TextInfo* ti ) {
	// Clear character frequencies
	for ( int i = 0; i < 256; i++ ) {
		ti->character_freqs[ i ] = 0;
	}

	ti->text = NULL;
	ti->text_len = 0;
}

void calculate_char_freqs( TextInfo* ti ) {
	for ( int i = 0; i < 10; i++ ) {
		char c = ti->text[ i ];
		ti->character_freqs[ c ]++;
	}
}

/* File Operations */
int parse_text_file( TextInfo* ti ) {
	// Attempt to open the file "filepath"
	FILE* fp = NULL;
	fopen_s( &fp, filepath, "r" );
	if ( !fp ) {
		printf( "Could not open file: %s\r\n", filepath );
		return 1;
	}

	// Get length of file
	fseek( fp, 0, SEEK_END );
	ti->text_len = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	// Attempt to allocate buffer for file contents
	ti->text = calloc( ti->text_len, sizeof( unsigned char ) );
	if ( !ti->text ) {
		printf( "Could not allocate %d bytes of memory\r\n", ti->text_len );
		fclose( fp );
		return 1;
	}

	// Copy file to buffer
	for ( int i = 0; i < ti->text_len; i++ ) {
		ti->text[ i ] = fgetc( fp );
	}

	// Close file
	fclose( fp );

	// Calculate character frequencies
	for ( int i = 0; i < ti->text_len; i++ ) {
		ti->character_freqs[ ti->text[ i ] ]++;
	}
	printf( "\r\n" );

	// Return pointer to buffer
	return 0;
}



int main( void ) {

	// Initialize text info
	TextInfo text_to_encode;
	init_TextInfo( &text_to_encode );

	// Copy text file info into text info struct
	parse_text_file( &text_to_encode );
	if ( !text_to_encode.text ) {
		return 1;
	}

	// Print char freqs to double check that it was read without error
	for ( int i = 0; i < 256; i++ ) {
		printf( "%3d: %d\r\n", i, text_to_encode.character_freqs[ i ] );
	}

	// Free contents
	free( text_to_encode.text );

	return 0;
}