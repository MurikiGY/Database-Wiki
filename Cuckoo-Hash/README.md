# Cuckoo-Hash
A more simple implementation of the Cuckoo-Hash algorithm.

## Data structure:
- Hash_t is a struct that has 2 integer variables, one represents the value stored and the other represents the status (empty, stored or excluded).
- 2 vectors of Hash_t representing tables.

## Algorithm:
- The position of the data inside the tables are calculed by two functions, named as hash1 and hash2.

## Insertions:
- Are always in the table1.
- If already exists an value in its position, then the insertion is in table2.
- Collisions in table2 are not considered.

## Search:
- Returning by parameter T = 0 and pos = -1 if the value doesn't exists, first it is tested if the key in table1 is empty. after that it is tested if the key in the table1 is full AND is equal to it's value. Finally it is tested if the key in the table2 is full AND is equal to it's value.

## Remove:
- There are two implementations of the remove function. One is a normal remove function and the other uses the search function.
