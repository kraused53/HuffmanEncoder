#include <stdio.h>
#include <stdlib.h>

/* Text Info */
struct TextInfo {
	int character_freqs[ 256 ];
	unsigned char* text;
	int text_len;
};

void init_TextInfo( struct TextInfo* ti ) {
	for ( int i = 0; i < 256; i++ ) {
		ti->character_freqs[ i ] = 0;
	}
	ti->text = NULL;
	ti->text_len = 0;
}

void calc_char_freqs( struct TextInfo* ti ) {
	// calculate character frequencies
	for ( int i = 0; i < ti->text_len; i++ ) {
		if ( ti->text[ i ] != '\0' ) {
			ti->character_freqs[ ( unsigned char ) ti->text[ i ] ]++;
		}
	}
}

/* Binary Tree */

/* Linked Lists */
struct LinkedListNode {
	int count;
	char val;
	struct LinkedListNode* next;
};

struct LinkedListNode* create_list_node( int icount, unsigned char ival ) {
	struct LinkedListNode* tmp = ( struct LinkedListNode* ) malloc( sizeof( struct LinkedListNode ) );
	if ( !tmp ) {
		return NULL;
	}

	// Initialize new node
	tmp->count = icount;
	tmp->val = ival;
	tmp->next = NULL;

	return tmp;
}

struct LinkedListNode* append_node_sorted( struct LinkedListNode* head, int c, unsigned char v ) {
	// Create new node
	struct LinkedListNode* new_node = create_list_node( c, v );
	if ( !new_node ) {
		printf( "Error making new node\r\n" );
		return head;
	}

	// If given an empty list, return new node as new head
	if ( head == NULL ) {
		return new_node;
	}

	// If new node goes before head
	if ( head->count > new_node->count ) {
		new_node->next = head;
		return new_node;
	}

	// Use this pointer to search list for where to put new node
	struct LinkedListNode* mov = head;
	while ( 1 ) {
		// If mov is at end of list, head will be added to end
		if ( mov->next == NULL ) {
			break;
		}

		// If mov->next exists, check to see if it's value is greater than the new value
		if ( mov->next->count > new_node->count) {
			break;
		}

		// Advance mov
		mov = mov->next;
	}

	if ( mov->next == NULL ) {
		mov->next = new_node;
	} else {
		new_node->next = mov->next;
		mov->next = new_node;
	}

	return head;
}

void print_list( struct LinkedListNode* head ) {
	if ( head == NULL ) {
		printf( "[ NULL ]\r\n" );
		return;
	}

	struct LinkedListNode* tmp = head;
	while ( tmp != NULL ) {
		if ( ( tmp->val >= 32 ) && ( tmp->val <= 126 ) ) {
			printf( "[   %c : %4d ]\r\n", tmp->val, tmp->count );
		} else {
			printf( "[ %3d : %4d ]\r\n", tmp->val, tmp->count );
		}
		tmp = tmp->next;
	}
	printf( "[    NULL    ]\r\n" );
}

void delete_linked_list( struct LinkedListNode* head ) {
	// Stop when current node is end of list
	if ( head->next == NULL ) {
		return;
	}

	// Move to next node
	delete_linked_list( head->next );

	// Free node
	free( head );
}

/* File Operations */
int copy_file_to_text_info( const char* path, struct TextInfo* ti ) {
	// Attempt to open the file "filepath"
	FILE* fp = NULL;
	fopen_s( &fp, path, "r" );
	if ( !fp ) {
		printf( "Could not open file: %s\r\n", path );
		return 1;
	}

	// Get length of file
	fseek( fp, 0, SEEK_END );
	ti->text_len = ftell( fp );
	rewind( fp );

	// Attempt to allocate buffer for file contents
	ti->text = calloc( ti->text_len, sizeof( char ) );
	if ( !ti->text ) {
		printf( "Could not allocate %d bytes of memory\r\n", ti->text_len );
		fclose( fp );
		return 1;
	}

	// Copy file to buffer
	char c;
	for ( int i = 0; i < ti->text_len; i++ ) {
		c = fgetc( fp );
		if ( c == EOF ) {
			break;
		}
		ti->text[ i ] = c;
	}

	// Close file
	fclose( fp );

	// No Errors
	return 0;
}

int main( void ) {

	// Set file to encode
	const char* filepath = "./text_files/test1.txt";

	// Set up text info
	struct TextInfo to_encode;
	init_TextInfo( &to_encode );

	// Load text info
	if ( copy_file_to_text_info( filepath, &to_encode ) != 0 ) {
		return 1;
	}
	
	// Calculate character frequencies
	calc_char_freqs( &to_encode );

	// Linked List
	struct LinkedListNode* head = NULL;

	for ( int i = 0; i < 256; i++ ) {
		if ( to_encode.character_freqs[ i ] != 0 ) {
			head = append_node_sorted( head, to_encode.character_freqs[ i ], i );
		}
	}

	print_list( head );

	
	// Free memory
	free( to_encode.text );
	delete_linked_list( head );
	head = NULL;

	return 0;
}