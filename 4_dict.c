#Операції над множинами. Реалізувати операції над множинами, що задані у вигляді масивів. 
#Операції – об’єднання, перетин, доповнення, різниця, симетрична різниця.
#Об’єднання множин

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 1024

typedef struct TMap
{
    char key[MAX_LEN];
    char value[MAX_LEN];
} Map;

int doubleMemoryMap(Map** a, int* max_length)
{
    Map* temp = (Map*) malloc(*max_length*2*sizeof(Map));
    if (temp == NULL)
    {
        return 0;
    }

    for (int i = 0; i < *max_length; i++)
        temp[i] = (*a)[i];
    free(*a);
    *a = temp;
    *max_length <<=1;
    return 1;
}

char* get(Map* map, int work_length, char* search_key, int *exists)
{
    *exists = 1;
    for(int i = 0; i < work_length; i++)
    {
        if(!strcmp(map[i].key, search_key))
        {
            return map[i].value;
        }
    }
    *exists = 0;
    return "Not exists";
}

int add(Map** map, int *work_length, int *max_length, char* key_to_add, char* value_to_add)
{
    //0 if dublicate, 1 if succeed, -1 if not
    int is_key_dublicate = 0;
    get(*map, *work_length, key_to_add, &is_key_dublicate);
    if(is_key_dublicate)
        return 0;

    if(*work_length < *max_length)
    {
        strcpy((*map)[*work_length].key, key_to_add);
        strcpy((*map)[*work_length].value, value_to_add);
        *work_length += 1;
        return 1;
    }
    else if (*work_length == *max_length)
            //trying to double max_length
        if(doubleMemoryMap(map, max_length))
        {
            strcpy((*map)[*work_length].key, key_to_add);
            strcpy((*map)[*work_length].value, value_to_add);
            *work_length += 1;
            return 1;
        }

    return -1;
}

int del(Map* map, int *work_length, char* key_to_remove)
{
    //1 if succeed, 0 if not
    for (int i = 0; i < *work_length; i++)
    {
        if(!strcmp(map[i].key, key_to_remove))
        {
            Map temp = map[i];
            map[i] = map[*work_length - 1];
            map[*work_length - 1] = temp;
            *work_length -= 1;
            return 1;
        }
    }
    return 0;
}

Map* createMap(int length)
{
    Map* map = (Map*) malloc(length*sizeof(Map));
    return map;
}

void printMap(Map* map, int work_length)
{
    printf("%-16s %-16s\n","Key", "Value");

    for (int i = 0; i < work_length; i++)
    {
        printf("%-16s: ", map[i].key);
        printf("%-16s\n", map[i].value);
    }

    printf("\n");
}

void unionMap(Map* map1, int work_len1, Map* map2, int work_len2, Map** result, int* result_worklen, int* result_maxlen)
{
    //if you have same keys with different values then values from map1 will be preferable

    free(*result);
    *result_maxlen = work_len1 + work_len2;
    //*result = (Map*) malloc(*result_maxlen*sizeof(Map));
    *result = createMap(*result_maxlen);
    for (int i = 0; i < work_len1; i++)
    {
        (*result)[i] = map1[i];
    }

    *result_worklen = work_len1;

    for (int i = 0; i < work_len2; i++)
    {
        add(result, result_worklen, result_maxlen, map2[i].key, map2[i].value);
    }
}

void intersectionMap(Map* map1, int work_len1, Map* map2, int work_len2, Map** result, int* result_worklen, int* result_maxlen)
{
    //if you have same keys with different values then values from map1 will be preferable

    free(*result);
    if (work_len1 < work_len2)
        *result_maxlen = work_len2;
    else
        *result_maxlen = work_len1;
    *result_worklen = 0;

    //*result = (Map*) malloc(*result_maxlen*sizeof(Map));
    *result = createMap(*result_maxlen);
    int exists = 0;
    for (int i = 0; i < work_len1; i++)
    {
        get(map2, work_len2, map1[i].key, &exists);
        if (exists)
            add(result, result_worklen, result_maxlen, map1[i].key, map1[i].value);
    }
}

void differenceMap(Map* map1, int work_len1, Map* map2, int work_len2, Map** result, int* result_worklen, int* result_maxlen)
{
    //map1 / map2
    free(*result);
    *result_maxlen = work_len1;
    //*result = (Map*) malloc(*result_maxlen*sizeof(Map));
    *result = createMap(*result_maxlen);
    for (int i = 0; i < work_len1; i++)
    {
        (*result)[i] = map1[i];
    }
    *result_worklen = work_len1;
    for (int i = 0 ; i < work_len2; i++)
    {
        del(*result, result_worklen, map2[i].key);
    }
}

void symmetricdifferenceMap(Map* map1, int work_len1, Map* map2, int work_len2, Map** result, int* result_worklen, int* result_maxlen)
{
    // map1^map2 = (map1 \ map2)U(map2 \ map1)

    //map1 \ map2 as left_diff
    int left_diff_worklen = 0;
    int left_diff_maxlen = 1;
    Map* left_diff = createMap(left_diff_maxlen);
    differenceMap(map1, work_len1, map2, work_len2, &left_diff, &left_diff_worklen, &left_diff_maxlen);

    //map2 \ map1 as right_diff
    int right_diff_worklen = 0;
    int right_diff_maxlen = 1;
    Map* right_diff = createMap(right_diff_maxlen);
    differenceMap(map2, work_len2, map1, work_len1, &right_diff, &right_diff_worklen, &right_diff_maxlen);

    //left_diff U right_diff
    free(*result);
    *result_maxlen = left_diff_worklen + right_diff_worklen;
    *result_worklen = 0;
    *result = createMap(*result_maxlen);
    unionMap(left_diff, left_diff_worklen, right_diff, right_diff_worklen, result, result_worklen, result_maxlen);
}
void demonstrateOp(Map* map1, int work_len1, Map* map2, int work_len2, Map** result, int result_worklen)
{
    printf("First map: \n");
    printMap(map1, work_len1);
    printf("Second map: \n");
    printMap(map2, work_len2);
    printf("Result: \n");
    printMap(result, result_worklen);
}

int main()
{
    int map1_maxlen = 5;
    int map1_worklen = 0;
    Map* map1 = createMap(map1_maxlen);
    add(&map1, &map1_worklen, &map1_maxlen, "First_Name", "Giorgio");
    add(&map1, &map1_worklen, &map1_maxlen, "Last_Name", "Giovanna");
    add(&map1, &map1_worklen, &map1_maxlen, "Age", "19");
    add(&map1, &map1_worklen, &map1_maxlen, "Occupation", "Kyiv");
    add(&map1, &map1_worklen, &map1_maxlen, "Married", "0");
    add(&map1, &map1_worklen, &map1_maxlen, "Country", "Ukraine"); //doubles memory

    printMap(map1, map1_worklen);

    int d1 = del(map1, &map1_worklen, "Married");
    int d2 = del(map1, &map1_worklen, "Blank");
    printf("After delete %s: \n", "Married");
    printMap(map1, map1_worklen);

    int exists = 1;
    char* get1 = get(map1, map1_worklen, "Last_Name", &exists);
    printf("Get %s: %s\n\n","Last_Name", get1);

    //union demonstration
    int map2_maxlen = 3;
    int map2_worklen = 0;
    Map* map2 = createMap(map2_maxlen);
    add(&map2, &map2_worklen, &map2_maxlen, "University", "KNU");
    add(&map2, &map2_worklen, &map2_maxlen, "First_Name", "Alice");
    add(&map2, &map2_worklen, &map2_maxlen, "Second_Name", "Jane");

    int union_worklen = 0;
    int union_maxlen = 1;
    Map* union_test = createMap(union_maxlen);
    unionMap(map1, map1_worklen, map2, map2_worklen, &union_test, &union_worklen, &union_maxlen);

    printf("Union demonstration:\n");
    demonstrateOp(map1, map1_worklen, map2, map2_worklen, union_test, union_worklen);

    //intersection demonstration
    int map3_worklen = 0;
    int map3_maxlen = 3;
    Map* map3 = createMap(map3_maxlen);
    add(&map3, &map3_worklen, &map3_maxlen, "Occupation", "Kyiv");
    add(&map3, &map3_worklen, &map3_maxlen, "Age", "20");

    int inter_worklen = 0;
    int inter_maxlen = 1;
    Map* inter_test = createMap(inter_maxlen);
    intersectionMap(map1, map1_worklen, map3, map3_worklen, &inter_test, &inter_worklen, &inter_maxlen);

    printf("Intersection demonstration:\n");
    demonstrateOp(map1, map1_worklen, map3, map3_worklen, inter_test, inter_worklen);

    //difference demonstration
    int map4_worklen = 0;
    int map4_maxlen = 3;
    Map* map4 = createMap(map4_maxlen);
    add(&map4, &map4_worklen, &map4_maxlen, "Occupation", "Dubai");
    add(&map4, &map4_worklen, &map4_maxlen, "Country", "OAE");

    int diff_worklen = 0;
    int diff_maxlen = 1;
    Map* diff_test = createMap(diff_maxlen);
    differenceMap(map1, map1_worklen, map4, map4_worklen, &diff_test, &diff_worklen, &diff_maxlen);

    printf("Difference demonstration:\n");
    demonstrateOp(map1, map1_worklen, map4, map4_worklen, diff_test, diff_worklen);

    //symmetric difference demostration
    int map5_worklen = 0;
    int map5_maxlen = 5;
    Map* map5 = createMap(map5_maxlen);
    add(&map5, &map5_worklen, &map5_maxlen, "Occupation", "Lviv");
    add(&map5, &map5_worklen, &map5_maxlen, "Country", "Ukraine");
    add(&map5, &map5_worklen, &map5_maxlen, "Education", "Lviv Polytechnic");
    add(&map5, &map5_worklen, &map5_maxlen, "Profession", "Gangstar");

    int symdiff_worklen = 0;
    int symdiff_maxlen = 1;
    Map* symdiff = createMap(symdiff_maxlen);
    symmetricdifferenceMap(map1, map1_worklen, map5, map5_worklen, &symdiff, &symdiff_worklen, &symdiff_maxlen);

    printf("Symmetric difference demonstration:\n");
    demonstrateOp(map1, map1_worklen, map5, map5_worklen, symdiff, symdiff_worklen);
    return 0;
}


