#include <stdio.h>
#include <stdlib.h>

/* Text Info */
const char* filepath = "./text_files/test1.txt";
int character_freqs[ 256 ] = { 0 };
unsigned char* text;
int text_len;

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
			printf( "[ %c: %d ]->", tmp->val, tmp->count );
		} else {
			printf( "[ %d: %d ]->", tmp->val, tmp->count );
		}
		tmp = tmp->next;
	}
	printf( "NULL\r\n" );
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

int main( void ) {

	// Attempt to open the file "filepath"
	FILE* fp = NULL;
	fopen_s( &fp, filepath, "r" );
	if ( !fp ) {
		printf( "Could not open file: %s\r\n", filepath );
		return 1;
	}

	// Get length of file
	fseek( fp, 0, SEEK_END );
	text_len = ftell( fp );
	rewind( fp );

	// Attempt to allocate buffer for file contents
	text = calloc( text_len, sizeof( char ) );
	if ( !text ) {
		printf( "Could not allocate %d bytes of memory\r\n", text_len );
		fclose( fp );
		return 1;
	}

	// Copy file to buffer
	char c;
	for ( int i = 0; i < text_len; i++ ) {
		c = fgetc( fp );
		if ( c == EOF ) {
			break;
		}
		text[ i ] = c;
	}

	// Close file
	fclose( fp );

	// calculate character frequencies
	for ( int i = 0; i < text_len; i++ ) {
		character_freqs[ ( unsigned char )text[ i ] ]++;
	}
	
	// Linked List
	struct LinkedListNode* head = NULL;

	for ( int i = 0; i < 256; i++ ) {
		if ( character_freqs[ i ] != 0 ) {
			head = append_node_sorted( head, character_freqs[ i ], i );
		}
	}

	print_list( head );

	// Free memory
	free( text );
	delete_linked_list( head );

	return 0;
}