// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

//создаем новую структуру дерева
typedef struct node
{
    bool is_word;
    struct node *children[27];
} node;

// создаем экземпляры структур
node *root;
node *cursor;
int counter = 0;
int index;



///////////////////////////////////////////// Returns true if word is in dictionary else false

int index_char(char c)
{
    if (c == '\'')
    {
        return 26;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        return c - 65;
    }
    else
    {
        return c - 97;
    }
}


bool check(const char *word)
{
    cursor = root;
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        index = index_char(word[i]);
        if (cursor -> children[index] == NULL)
        {
            return false;
        }
        cursor = cursor -> children[index];
    }

    return cursor -> is_word;
}
// // объявляем новый экземпляр структуры node и устанавливаем его как начальный

//     cursor = root;
//     for(int i = 0, len = strlen(word); i < len; i++)
//     {
// // получаем индекс
//         if( word[i] == '\'') index = 26;
//         else index = tolower(word[i]) % 'a';

// // Если слово не существует
//         if (cursor->children[index] == NULL)
//         {
//             return false;
//         }
// // если есть значение, переходим к следующему символу
//         else cursor = cursor->children[index];
//     }
//     return true;
// }

//////////////////////////////////////////// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
// выделяем память для рут
    root = malloc(sizeof(node));

// открываем файл словаря
    FILE *in = fopen(dictionary, "r");
    if (in == NULL)
    {
        printf("Печалька с словарем %s\n", dictionary);
        fclose(in);
        return false;
    }
// курсор в начало
    cursor = root;
// устанавливаем для is_word значение false
    cursor->is_word = false;
//заполнить массив детей NULL
    for (int j = 0; j < 27; j++)
    {
        cursor->children[j] = NULL;
    }
// перебираем каждый символ

    for (int i = fgetc(in); i != EOF; i = fgetc(in))
    {
        if (i == '\n')
        {
// значит конец слова
            cursor->is_word = true;
//считаем найденные слова
            counter++;
// возвращаемся к корню
            cursor = root;
        }
        else
        {
// получаем индекс буквы
            if (i == '\'')
            {
                index = 26;
            }
            else
            {
                index = tolower(i) % 'a';
            }
            // проверяем выделялась ли под него память
            if (cursor->children[index] == NULL)
            {
                // выделяем память под новый символ
                cursor->children[index] = malloc(sizeof(node));
                if (cursor->children[index] == NULL)
                {
                    unload();
                    return false;

                }
                cursor->children[index]->is_word = false;
                for (int j = 0; j < 27; j++)
                {
                    cursor->children[index]->children[j] = NULL;
                }
            }
            // переходим в следующую структуру  и Если ссылка существует - перейти по ссылке
            cursor = cursor->children[index];
        }
    }
// закрываем словарь
    fclose(in);
    return true;
}

///////////////////////////////////// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size()
{
// нужно вернуть количество слов которое считалось в лоад
    return counter;
}

/////////////////////////////////////// Unloads dictionary from memory, returning true if successful else false
void recursion(node *current)
{
    for (int j = 0; j < 27; j++)
    {
        if (current->children[j] != NULL)
        {
            //current = current->children[j];
            recursion(current->children[j]);
        }
    }
    free(current);
}


bool unload(void)
{
    //free(cursor->children[j]);
    cursor = root;
    recursion(cursor);

    // for(int j =0; j < 27; j++ )
    // {
    //     if(cursor->children[j] != NULL)
    //     {
    //         cursor = cursor->children[j];
    //         recursion(cursor)
    //     }
    //         free(cursor->children[j]);
    //         return true;
    //     }

    // }
    return true;
}
