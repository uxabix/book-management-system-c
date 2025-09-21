# Book Management System

This project was developed as part of the **Podstawy Programowania (Programming Fundamentals)** course during the first semester of studies at **Politechnika Białostocka (Białystok University of Technology)** in January 2024.

## Project Description

This application is a simple book management system implemented in C. It reads book data from a `books.txt` file, stores it in a linked list, and performs various operations based on the data, outputting results to separate text files (`a.txt`, `b.txt`, `c.txt`, `d.txt`, `e.txt`).

The system handles operations such as listing all books, finding the oldest book(s) and their prices, searching for books by a specific author, listing books grouped by publication year in descending order, and listing all unique authors along with their respective books.

## Features

*   **Load Data:** Reads book information from `books.txt`.
*   **Store Data:** Uses a linked list to store `Book` structures in memory.
*   **Task A (a.txt):** Lists all books with their full details.
*   **Task B (b.txt):** Identifies the oldest book(s) and calculates the arithmetic mean of their prices if there are multiple oldest books, or displays the price if there's only one.
*   **Task C (c.txt):** Prompts the user for an author's name and lists all titles written by that author.
*   **Task D (d.txt):** Lists all books, grouped by their publication year in descending order. For each year, it shows the book title and the first author.
*   **Task E (e.txt):** Lists all unique authors found in the `books.txt` file, and for each author, lists the titles of books they have written.

## Installation and Usage

### Requirements
- A C compiler (e.g., **GCC**).

### Steps to Run

1.  **Clone the repository (or download the files):**
    ```bash
    git clone https://github.com/uxabix/book-management-system-c.git
    ```

2.  **Ensure `books.txt` exists:**
    Make sure the `books.txt` file is present in the same directory as `main.c`. An example `books.txt` is provided:
    ```
    1BookName 1111 1000 1BookAuthor1
    2BookName 2222 20 2BookAuthor1 2BookAuthor2
    3BookName 3333 3000 3BookAuthor1 3BookAuthor2
    4BookName 4444 400 4BookAuthor1 1BookAuthor1 4BookAuthor3
    ...
    ```
    Each line represents a book and should contain: `Title Year Price Author1 Author2 Author3` (Authors are optional, up to 3).

3.  **Compile the source code:**
    ```bash
    gcc main.c -o book_manager
    ```

4.  **Run the executable:**
    ```bash
    ./book_manager
    ```

    The program will create `a.txt`, `b.txt`, `c.txt`, `d.txt`, and `e.txt` files in the same directory, containing the results of the respective tasks. For Task C, it will prompt you to enter an author's name in the console.

## Documentation

The original documentation is provided in Polish in the file [Sprawozdanie.pdf](./Sprawozdanie.pdf). English translation ... .

## Application Overview

This console-based application uses a singly linked list to store book information. Each Book structure includes:
- title (string)
- year (unsigned int)
- price (unsigned int)
- up to 3 authors (array of strings)

Core functionality:
1. Data Input: Read records from books.txt.
2. Parsing: Extract fields and populate Book structures.
3. Linked List Operations:
 - getLast: Retrieve the last node.
 - pushBack: Insert a book at the end.
 - printLinkedList: Write list content to file.
4. Tasks (A–E): Generate output files with structured results.

Basic error handling is implemented (e.g., file opening checks).

## Author

Kiryl Alishkevich

## Academic Context

This project was carried out as part of the Podstawy Programowania course during the first semester of studies at Politechnika Białostocka, completed in January 2024.

License

This project is licensed under the MIT License. See [LICENSE](./LICENSE)
.

---


