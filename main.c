#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Structure representing a book with up to 3 authors
 */
struct Book {
    char authors[3][100];    // Array of up to 3 author names
    char title[100];         // Book title
    unsigned int price;      // Book price
    unsigned int year;       // Publication year
};

/**
 * Node structure for the linked list implementation
 */
struct Node {
    struct Book value;       // Book data stored in this node
    struct Node *next;       // Pointer to the next node in the list
};

/**
 * Returns the last node in the linked list
 * @param head Pointer to the first node of the list
 * @return Pointer to the last node, or NULL if list is empty
 */
struct Node *getLast(struct Node *head) {
    if (head == NULL) {
        return NULL;
    }
    // Traverse to the end of the list
    while (head->next) {
        head = head->next;
    }
    return head;
}

/**
 * Adds a new book to the end of the linked list
 * @param head Pointer to the pointer of the first node (allows modification of head)
 * @param data Book data to be added
 */
void pushBack(struct Node **head, struct Book data) {
    // Allocate memory for new node
    struct Node *tmp = malloc(sizeof(struct Node));
    if (!tmp) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    // Initialize the new node
    tmp->value = data;
    tmp->next = NULL;

    // If list is empty, make this node the head
    if (*head == NULL) {
        *head = tmp;
        return;
    }

    // Otherwise, add to the end of the list
    struct Node *last = getLast(*head);
    last->next = tmp;
}

/**
 * Prints all books in the linked list to a file
 * @param file Output file pointer
 * @param head Pointer to the first node of the list
 */
void printLinkedList(FILE *file, struct Node *head) {
    while (head) {
        // Print book details: title, year, price, and all authors
        fprintf(file, "%s %u %u %s %s %s\n",
                head->value.title,
                head->value.year,
                head->value.price,
                head->value.authors[0],
                head->value.authors[1],
                head->value.authors[2]);
        head = head->next;
    }
}

/**
 * Parses a string into an array of tokens (words)
 * @param input Input string to be parsed (will be modified by strtok)
 * @param out_count Pointer to store the number of tokens found
 * @return Array of strings containing the parsed tokens
 */
char **parseStringToArr(char input[], int *out_count) {
    const int MAX_TOKENS = 6;      // Maximum number of expected tokens
    const int MAX_TOKEN_LEN = 100; // Maximum length of each token

    // Allocate memory for array of string pointers
    char **res = malloc(sizeof(char*) * MAX_TOKENS);
    if (!res) { perror("malloc"); exit(EXIT_FAILURE); }
    
    // Allocate memory for each string and initialize
    for (int i = 0; i < MAX_TOKENS; ++i) {
        res[i] = malloc(MAX_TOKEN_LEN);
        if (!res[i]) { 
            // Clean up already allocated memory on failure
            for (int j = 0; j < i; ++j) {
                free(res[j]);
            }
            free(res);
            perror("malloc"); 
            exit(EXIT_FAILURE); 
        }
        res[i][0] = '\0'; // Initialize as empty string
    }

    // Parse the input string using strtok
    int i = 0;
    char *token = strtok(input, " \t\r\n"); // Split on whitespace and newlines
    while (token != NULL && i < MAX_TOKENS) {
        // Copy token to result array with bounds checking
        strncpy(res[i], token, MAX_TOKEN_LEN - 1);
        res[i][MAX_TOKEN_LEN - 1] = '\0'; // Ensure null termination
        i++;
        token = strtok(NULL, " \t\r\n");
    }

    *out_count = i; // Return the actual number of tokens found
    return res;
}

/**
 * Converts an array of strings to a Book structure
 * @param bookString Array of strings containing book data
 * @param tokens Number of tokens in the array
 * @param book Pointer to Book structure to be filled
 */
void parseArrToBook(char **bookString, int tokens, struct Book *book) {
    // Need at least 3 tokens: title, year, price
    if (tokens < 3) {
        // Initialize with empty/default values
        book->title[0] = '\0';
        book->year = 0;
        book->price = 0;
        for (int k = 0; k < 3; ++k) book->authors[k][0] = '\0';
        return;
    }

    // Parse title, year, and price
    strncpy(book->title, bookString[0], sizeof(book->title)-1);
    book->title[sizeof(book->title)-1] = '\0';
    book->year = (unsigned int) atoi(bookString[1]);
    book->price = (unsigned int) atoi(bookString[2]);

    // Parse up to 3 authors (optional)
    for (int i = 0; i < 3; ++i) {
        if (3 + i < tokens) {
            strncpy(book->authors[i], bookString[3 + i], sizeof(book->authors[i]) - 1);
            book->authors[i][sizeof(book->authors[i]) - 1] = '\0';
        } else {
            book->authors[i][0] = '\0'; // Empty author slot
        }
    }
}

/**
 * Frees all nodes in the linked list
 * @param head Pointer to the first node of the list
 */
void freeList(struct Node *head) {
    while (head) {
        struct Node *tmp = head;
        head = head->next;
        free(tmp);
    }
}

/**
 * Frees the memory allocated for parsed string array
 * @param arr Array of strings to be freed
 * @param count Number of elements (not used, always frees MAX_TOKENS=6)
 */
void freeParsedArray(char **arr, int count) {
    for (int i = 0; i < 6; ++i) { // parseStringToArr always allocates 6 elements
        free(arr[i]);
    }
    free(arr);
}

/**
 * Task A: Write all books to a.txt file
 * @param head Pointer to the first node of the book list
 * @return 0 on success, 1 on failure
 */
int aTask(struct Node *head) {
    FILE *aFile = fopen("a.txt", "wt");

    if (aFile == NULL) {
        perror("fopen a.txt");
        return 1;
    }

    // Print all books to file
    printLinkedList(aFile, head);

    fclose(aFile);
    return 0;
}

/**
 * Task B: Find oldest book(s) and calculate price statistics
 * @param head Pointer to the first node of the book list
 * @return 0 on success, 1 on failure
 */
int bTask(struct Node *head) {
    FILE *bFile = fopen("b.txt", "wt");

    if (bFile == NULL) {
        perror("fopen b.txt");
        return 1;
    }

    // Find the oldest books and calculate price statistics
    unsigned int lowestYear = head->value.year;
    unsigned int oldestBookPrice = head->value.price;
    unsigned int priceArithmeticMean = head->value.price;
    int count = 1;
    
    // Traverse the list to find all books with the lowest year
    while (head->next) {
        head = head->next;
        if (head->value.year < lowestYear) {
            // Found an older book, reset counters
            lowestYear = head->value.year;
            priceArithmeticMean = head->value.price;
            oldestBookPrice = head->value.price;
            count = 1;
        } else if (head->value.year == lowestYear) {
            // Found another book from the same oldest year
            count++;
            priceArithmeticMean += head->value.price;
        }
    }

    // Write results based on whether there are multiple oldest books
    if (count > 1) {
        fprintf(bFile, "There were several oldest books. Price arithmetic mean: %d", priceArithmeticMean / count);
    } else {
        fprintf(bFile, "There was only 1 oldest book. The price is: %d", oldestBookPrice);
    }

    fclose(bFile);
    return 0;
}

/**
 * Task C: Find books by a specific author
 * @param head Pointer to the first node of the book list
 * @return 0 on success, 1 on failure
 */
int cTask(struct Node *head) {
    FILE *cFile = fopen("c.txt", "wt");
    if (cFile == NULL) { perror("fopen c.txt"); return 1; }

    // Get author name from user input
    printf("Enter author name (f.e. 1BookAuthor1): \n");
    char author[100];
    if (fgets(author, sizeof(author), stdin) == NULL) {
        author[0] = '\0';
    } else {
        // Remove newline character from input
        author[strcspn(author, "\r\n")] = '\0';
    }

    // Search through all books for the specified author
    struct Node *cur = head;
    while (cur) {
        // Check all three author slots for each book
        for (int i = 0; i < 3; ++i) {
            if (author[0] != '\0' && strcmp(cur->value.authors[i], author) == 0) {
                fprintf(cFile, "%s\n", cur->value.title);
                break; // Found author, no need to check other slots
            }
        }
        cur = cur->next;
    }

    fclose(cFile);
    return 0;
}

/**
 * Comparison function for qsort (integers)
 * @param a Pointer to first integer
 * @param b Pointer to second integer
 * @return -1 if a < b, 0 if a == b, 1 if a > b
 */
int compare(const void *a, const void *b) {
    int int_a = *((int *) a);
    int int_b = *((int *) b);

    if (int_a == int_b) return 0;
    else if (int_a < int_b) return -1;
    else return 1;
}

/**
 * Reverses an array in place
 * @param arr Array to be reversed
 * @param start Starting index
 * @param end Ending index
 */
void reverseArray(int arr[], int start, int end) {
    int temp;
    while (start < end) {
        // Swap elements from both ends moving towards center
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

/**
 * Task D: Group books by publication year (descending order)
 * @param head Pointer to the first node of the book list
 * @return 0 on success, 1 on failure
 */
int dTask(struct Node *head) {
    struct Node *headCopy = head; // Keep reference to original head

    FILE *dFile = fopen("d.txt", "wt");
    if (dFile == NULL) {
        perror("fopen d.txt");
        return 1;
    }

    // Collect all unique years from books
    int *years;
    int size = 1;
    years = calloc(size, sizeof(int));
    if (!years) {
        perror("calloc");
        fclose(dFile);
        return 1;
    }

    int isThere = 0;
    years[0] = head->value.year; // Start with first book's year
    
    // Traverse list to find all unique years
    while (head->next) {
        head = head->next;
        
        // Check if this year is already in our array
        for (int i = 0; i < size; i++) {
            if (years[i] == head->value.year) {
                isThere = 1;
                break;
            }
        }
        
        // If year not found, add it to array
        if (!isThere) {
            size++;
            int *tmp = realloc(years, size * sizeof(int));
            if (!tmp) {
                perror("realloc");
                free(years);
                fclose(dFile);
                return 1;
            }
            years = tmp;
            years[size - 1] = head->value.year;
        }
        isThere = 0;
    }

    // Sort years and reverse to get descending order
    qsort(years, size, sizeof(int), compare);
    reverseArray(years, 0, size - 1);

    // Print books grouped by year
    for (int i = 0; i < size; i++) {
        fprintf(dFile, "%d: \n", years[i]);
        head = headCopy; // Reset to beginning of list
        
        // Find all books from this year
        while (head) {
            if (head->value.year == years[i]) {
                fprintf(dFile, "\t%s %s\n", head->value.title, head->value.authors[0]);
            }
            head = head->next;
        }
    }

    free(years);
    fclose(dFile);
    return 0;
}

/**
 * Task E: List all unique authors and their books
 * @param head Pointer to the first node of the book list
 * @return 0 on success, 1 on failure
 */
int eTask(struct Node *head) {
    struct Node *headCopy = head; // Keep reference to original head

    FILE *eFile = fopen("e.txt", "wt");
    if (eFile == NULL) {
        perror("fopen e.txt");
        return 1;
    }

    // Initialize array to store unique authors
    int size = 1;
    char **authors;
    authors = calloc(1, sizeof(char *));
    if (!authors) {
        perror("calloc");
        fclose(eFile);
        return 1;
    }
    
    // Allocate memory for first author
    authors[0] = malloc(100 * sizeof(char));
    if (!authors[0]) {
        perror("malloc");
        free(authors);
        fclose(eFile);
        return 1;
    }
    strcpy(authors[size - 1], head->value.authors[0]); // Start with first author

    int isThere;
    // Traverse list to collect all unique authors
    while (head->next) {
        head = head->next;
        
        // Check all three author slots for current book
        for (int i = 0; i < 3; i++) {
            if (head->value.authors[i][0] == '\0') continue; // Skip empty author slots
            
            isThere = 0;
            // Check if this author is already in our list
            for (int j = 0; j < size; j++) {
                if (strcmp(head->value.authors[i], authors[j]) == 0) {
                    isThere = 1;
                    break;
                }
            }
            
            // If author not found, add to list
            if (!isThere) {
                size++;
                char **tmp = realloc(authors, size * sizeof(char *));
                if (tmp == NULL) {
                    perror("realloc");
                    // Clean up all allocated memory
                    for (int k = 0; k < size - 1; k++) {
                        free(authors[k]);
                    }
                    free(authors);
                    fclose(eFile);
                    return 1;
                }
                authors = tmp;
                
                // Allocate memory for new author name
                authors[size - 1] = malloc(100 * sizeof(char));
                if (!authors[size - 1]) {
                    perror("malloc");
                    // Clean up all allocated memory
                    for (int k = 0; k < size - 1; k++) {
                        free(authors[k]);
                    }
                    free(authors);
                    fclose(eFile);
                    return 1;
                }
                strcpy(authors[size - 1], head->value.authors[i]);
            }
        }
    }

    // Print each author and their books
    for (int i = 0; i < size; i++) {
        fprintf(eFile, "%s: \n", authors[i]);
        head = headCopy; // Reset to beginning of list

        // Find all books by this author
        while (head) {
            for (int j = 0; j < 3; j++) {
                if (strcmp(head->value.authors[j], authors[i]) == 0) {
                    fprintf(eFile, "\t%s\n", head->value.title);
                    break; // Found author, no need to check other slots
                }
            }
            head = head->next;
        }
    }

    // Clean up allocated memory
    for (int i = 0; i < size; i++) {
        free(authors[i]);
    }
    free(authors);
    fclose(eFile);
    return 0;
}

/**
 * Main function - entry point of the program
 * Reads book data from books.txt and performs various tasks (A-E)
 */
int main() {
    // Open input file
    FILE *booksFile = fopen("books.txt", "rt");
    if (booksFile == NULL) {
        perror("fopen books.txt");
        return 1;
    }

    char s[1000];           // Buffer for reading lines
    struct Book book;       // Temporary book structure
    struct Node *head = NULL; // Head of the linked list

    // Read first valid line to initialize the list
    while (fgets(s, sizeof s, booksFile)) {
        if (s[0] == '\n' || s[0] == '\0') continue; // Skip empty lines
        
        int tokens = 0;
        char *buff = strdup(s); // Create copy because parseStringToArr uses strtok (modifies buffer)
        char **parsed = parseStringToArr(buff, &tokens);
        parseArrToBook(parsed, tokens, &book);
        free(buff);
        
        // Free parsed array
        freeParsedArray(parsed, 6);
        
        pushBack(&head, book);
        break; // Break after first line (we used this to init head)
    }

    // Read remaining lines and add books to list
    while (fgets(s, sizeof s, booksFile) != NULL) {
        int tokens = 0;
        char *buff = strdup(s);
        char **parsed = parseStringToArr(buff, &tokens);
        parseArrToBook(parsed, tokens, &book);
        
        // Free parsed array and buffer
        freeParsedArray(parsed, 6);
        free(buff);

        pushBack(&head, book);
    }

    // Execute all tasks and check for errors
    if (aTask(head)) {
        freeList(head);
        fclose(booksFile);
        return 2;
    }
    if (bTask(head)) {
        freeList(head);
        fclose(booksFile);
        return 2;
    }
    if (cTask(head)) {
        freeList(head);
        fclose(booksFile);
        return 2;
    }
    if (dTask(head)) {
        freeList(head);
        fclose(booksFile);
        return 2;
    }
    if (eTask(head)) {
        freeList(head);
        fclose(booksFile);
        return 2;
    }

    // Clean up resources
    fclose(booksFile);
    freeList(head);

    return 0;
}
