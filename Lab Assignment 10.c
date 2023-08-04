#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{
    struct Trie* children[26];
    int count;
};

// Initializes a trie structure
struct Trie* createTrie()
{
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if (pTrie)
    {
        for (int i = 0; i < 26; ++i)
            pTrie->children[i] = NULL;
        pTrie->count = 0;
    }
    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word)
{
    struct Trie* current = pTrie;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            current->children[index] = createTrie(); // Initialize child node
        }
        current = current->children[index];
    }
    current->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie* pTrie, char* word)
{
    struct Trie* current = pTrie;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
            return 0;
        current = current->children[index];
    }
    return current->count;
}

// Deallocates the trie structure
struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie)
    {
        for (int i = 0; i < 26; ++i)
            pTrie->children[i] = deallocateTrie(pTrie->children[i]);
        free(pTrie);
    }
    return NULL;
}

// Reads the dictionary file and returns the number of words
int readDictionary(char* filename, char** pInWords)
{
     FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numWords;
    if (fscanf(file, "%d\n", &numWords) != 1)
    {
        perror("Error reading number of words");
        exit(EXIT_FAILURE);
    }

    char line[256];
    for (int i = 0; i < numWords; ++i)
    {
        if (!fgets(line, sizeof(line), file))
        {
            perror("Error reading word");
            exit(EXIT_FAILURE);
        }
        line[strlen(line) - 1] = '\0'; // Removing newline character
        pInWords[i] = (char*)malloc(strlen(line) + 1);
        if (!pInWords[i])
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        strcpy(pInWords[i], line);
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word to the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}
