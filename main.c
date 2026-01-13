#include <stdio.h>
#include <stdlib.h>


/* Data Structures */
struct TextInfo {
	int character_freqs[ 256 ];
	unsigned char* text;
	int chars_found;
	int text_len;
};

struct BinaryTreeNode {
	char val;
	int count;
	struct BinaryTreeNode* left;
	struct BinaryTreeNode* right;
};

struct LinkedListNode {
	struct BinaryTreeNode* bt_node;
	struct LinkedListNode* next;
};

/* Text Info */
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

/* Binary tree operations */
void print_tree( struct BinaryTreeNode* root ) {
	if ( root == NULL ) {
		return;
	}

	print_tree( root->left );
	printf( "[ %c: %d] ", root->val, root->count );
	print_tree( root->right );

}

struct BinaryTreeNode* create_tree_node( int icount, char ival ) {
	struct BinaryTreeNode* tmp = ( struct BinaryTreeNode* ) malloc( sizeof( struct BinaryTreeNode ) );
	if ( !tmp ) {
		return NULL;
	}

	tmp->count = icount;
	tmp->val = ival;
	tmp->left = NULL;
	tmp->right = NULL;

	return tmp;
}

struct BinaryTreeNode* join_tree_nodes( struct BinaryTreeNode* new_left, struct BinaryTreeNode* new_right ) {
	// Create a new node who's count value is the sum of left and right node counts. No char is needed for internal nodes
	struct BinaryTreeNode* new_node = create_tree_node( ( new_left->count + new_right->count ), ' ' );
	if ( !new_node ) {
		printf( "ERROR in join_tree_nodes: could not create new node" );
		return NULL;
	}

	// Add given nodes to new tree
	new_node->left = new_left;
	new_node->right = new_right;

	return new_node;
}

void delete_binray_tree( struct BinaryTreeNode* root ) {
	// Do not continue if root has moved to a null space
	if ( root == NULL ) {
		return;
	}

	// Delete all nodes to the left, followed by all nodes to the right
	delete_binray_tree( root->left );
	delete_binray_tree( root->right );

	// If the left and right paths lead to NULL, you are on a leaf node, delete it
	free( root );
}

/* Linked lists operations */

void delete_linked_list( struct LinkedListNode* head ) {
	// Stop when current node is end of list
	if ( head == NULL ) {
		printf("return\r\n");
		return;
	}

	// Move to next node
	delete_linked_list( head->next );

	// Free node
	if ( head->bt_node != NULL ) {
		delete_binray_tree( head->bt_node );
	}
	free( head );
}

struct LinkedListNode* create_list_node( int icount, unsigned char ival ) {
	struct LinkedListNode* tmp = ( struct LinkedListNode* ) malloc( sizeof( struct LinkedListNode ) );
	if ( !tmp ) {
		return NULL;
	}

	// Create tree node for list
	tmp->bt_node = create_tree_node( icount, ival );

	// Initialize new node
	tmp->bt_node->count = icount;
	tmp->bt_node->val = ival;
	tmp->next = NULL;

	return tmp;
}

struct LinkedListNode* append_existing_node_sorted( struct LinkedListNode* head, struct LinkedListNode* to_add ) {
	if ( head == NULL ) {
		return to_add;
	}

	if ( head->bt_node->count > to_add->bt_node->count ) {
		to_add->next = head;
		return to_add;
	}

	// Use this pointer to search list for where to put new node
	struct LinkedListNode* mov = head;
	while ( 1 ) {
		// If mov is at end of list, head will be added to end
		if ( mov->next == NULL ) {
			break;
		}

		// If mov->next exists, check to see if it's value is greater than the new value
		if ( mov->next->bt_node->count > to_add->bt_node->count ) {
			break;
		}

		// Advance mov
		mov = mov->next;
	}

	if ( mov->next == NULL ) {
		mov->next = to_add;
	} else {
		to_add->next = mov->next;
		mov->next = to_add;
	}

	return head;
}

struct LinkedListNode* remove_first_element( struct LinkedListNode* head ) {
	if ( head == NULL ) {
		return NULL;
	}

	if ( head->next == NULL  ) {
		free( head );
		return NULL;
	}

	struct LinkedListNode* tmp = head;
	head = head->next;

	tmp->next = NULL;
	free( tmp );

	return head;
}

struct LinkedListNode* append_new_node_sorted( struct LinkedListNode* head, int c, unsigned char v ) {
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
	if ( head->bt_node->count > new_node->bt_node->count ) {
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
		if ( mov->next->bt_node->count > new_node->bt_node->count ) {
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
		if ( ( tmp->bt_node->val >= 32 ) && ( tmp->bt_node->val <= 126 ) ) {
			printf( "[   %c : %4d ]\r\n", tmp->bt_node->val, tmp->bt_node->count );
		} else {
			printf( "[ %3d : %4d ]\r\n", tmp->bt_node->val, tmp->bt_node->count );
		}
		tmp = tmp->next;
	}
	printf( "[    NULL    ]\r\n" );
}

struct LinkedListNode* convert_list_to_tree( struct LinkedListNode* head ) {
	// If list is empty, return NULL
	if ( head == NULL ) {
		return NULL;
	}

	// If list has one element, make pointer to tree node, and disconect it from the list. Return it
	if ( head->next == NULL ) {
		return head;
	}

	// Loop until only one node is left
	while ( head->next != NULL ) {
		struct LinkedListNode* created_node = ( struct LinkedListNode* ) malloc( sizeof( struct LinkedListNode ) );
		if ( !created_node ) {
			break;
		}
		created_node->next = NULL;

		created_node->bt_node = join_tree_nodes( head->bt_node, head->next->bt_node );

		head = remove_first_element( head );
		head = remove_first_element( head );

		head = append_existing_node_sorted( head, created_node );
	}

	// The list should now be a single node. Return it's tree
	return head;
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

	struct BinaryTreeNode* root = create_tree_node( 0, '0');
	root->left = create_tree_node( 0, '1' );
	root->left->left = create_tree_node( 0, '3' );
	root->left->right = create_tree_node( 0, '4' );
	root->right = create_tree_node( 0, '2' );
	root->right->left = create_tree_node( 0, '5' );

	print_tree(root);
	return 0;
	// Set file to encode
	const char* filepath = "./text_files/test3.txt";

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
			head = append_new_node_sorted( head, to_encode.character_freqs[ i ], i );
		}
	}

	printf( "\r\n**********\r\n" );
	printf( "Before:\r\n" );
	print_list( head );
	printf( "**********\r\n" );

	head = convert_list_to_tree( head );

	printf( "\r\n**********\r\n" );
	printf( "after:\r\n" );
	print_list( head );
	printf( "**********\r\n" );

	print_tree( head->bt_node );

	// Free memory
	free( to_encode.text );
	delete_linked_list( head );
	head = NULL;

	return 0;
}