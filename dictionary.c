#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"


typedef struct node
{
  struct node* next[28];
  bool is_word;
}node;

node* FIRST = NULL;
node* LAST = NULL;

FILE* dic = NULL;
int counter2 = 0;

unsigned int numberwords;
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    if ((strlen(word) == 1) && ((tolower(word[0]) == 97) || (tolower(word[0]) == 105)))
    {
     return true;
    }
       
    node* checker = FIRST;
    
    for (int i = 0; word[i] != '\0'; i++)
    {
     int l = tolower(word[i]) - 97;
     if ((word[i] == '\'') && (i != 0))
      {
       l = 26;
      }  
     if (checker->next[l] == NULL)
      {
       return false;
      }
      checker = checker->next[l];
    }
    
    if (checker->is_word == true)
    {
     return true;
    }
     else
     {
      return false;
     }   
}


bool load(const char* dictionary)
{

     dic = fopen(dictionary, "r");    
     if (dic == NULL)
     {
      printf("Could not open %s.\n", dictionary);    
      return false;
     } 
  
   // THE FIRST NODE;
   FIRST = malloc(sizeof(node));
   FIRST->is_word = true;
   // NULL FIRST
   for (int i = 0; i < 28; i++)
   {
     FIRST->next[i] = NULL; 
   }
   
   LAST = FIRST;
   // CYCLE FOR THE WHOLE TEXT, WORD BY WORD    
   for (int i = 0; i < 143091; i++)
   {  
    char word[46];
    fgets(word, 46, dic);       
    // FOR THE WORD, LETTER BY LETTER    
     for (int j = 0; word[j] != word[strlen(word) - 1]; j++)
     {
      int k = tolower(word[j]) - 97; 
      if ((word[j] == '\'') && (j != 0))
      {
       k = 26;
      }     
      
      //FIRST LETTER
      if (j == 0)
      {
       if (FIRST->next[k] != NULL)
        {  
         LAST = FIRST->next[k];      
         continue;
        }    
      node* lastletter = malloc(sizeof(node));            //next
         for (int z = 0; z < 27; z++)
         {
          lastletter->next[z] = NULL; 
         }
       lastletter->next[27] = FIRST;            
       lastletter->is_word = false;
       FIRST->next[k] = lastletter;
       LAST = lastletter; 
      }
      
      //SECOND AND FURTHER
        else
         {      
           node* lastletter = LAST;
                                     
              if (lastletter->next[k] != NULL)
              {
               LAST = lastletter->next[k];
               continue;
              }
              
          //CREATE NEW NODE 
          node* lastletter1 = malloc(sizeof(node));                                              
          for (int z = 0; z < 27; z++)
           {
            lastletter1->next[z] = NULL; 
           } 
           lastletter1->is_word = false;
           lastletter->next[k] = lastletter1;
           lastletter1->next[27] = lastletter;
           LAST = lastletter1;
           
           // IF LAST LETTER
           if (j == strlen(word) - 2)
           {
            lastletter1->is_word = true;
            numberwords++;
            LAST = FIRST;
           }
         }                        
     }
    
   }    
 fclose(dic);  
  
return true;  
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return (numberwords + 4);
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
  int counter3 = 0;
  while (counter3 == 0)
  {
   node* current = LAST;
    
    int counter = 0;
    for (int i = 0; i < 27; i++)
     {
       if (current->next[i] != NULL)
        {          
          LAST = current->next[i];
          break;
        }
         else 
          {counter++;}      
     }
          
     if (counter == 27)
     {
       if (current->next[27] == NULL)
        {
         for (int i = 0; i < 28; i++)
          {
           if (FIRST->next[i] != NULL)
            {
             return false;               
            }
            else if (i == 27)
            {
             free(FIRST);
             return true;               
            }
          }
        }
      LAST = current->next[27];
      free(current);
       for (int i = 0; i < 27; i++)
       {
        if ((LAST->next[i] != NULL) && (counter2 == 0))
         {
          LAST->next[i] = NULL;
          counter2++;
         }         //valgrind ./speller texts/kjv.txt | valgrind --leak-check=full ./load slovar1.txt

       }
       counter2 = 0;         
     }
  }   
   
return true;      
}















