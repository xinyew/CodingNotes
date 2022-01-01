#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
typedef struct List List;
typedef struct List* ListPtr;
struct List {
    int capacity;
    int size;
    int *arr;
};

ListPtr initialiseWithCapacity(int initialCapacity) {
    ListPtr a = malloc(sizeof *a);
    a->capacity = initialCapacity;
    a->arr = malloc(a->capacity * sizeof *(a->arr));
    a->size = 0;
    return a;
}

ListPtr initialise() {
    return initialiseWithCapacity(10);
}

ListPtr initialiseWithArray(int arr[], int length) {
    ListPtr a = initialiseWithCapacity(length);
    for(int i = 0;  i < length; i++)
        a->arr[a->size++] = arr[i];
    return a;
}

ListPtr values(int n, ...) {
    va_list valist;
    va_start(valist, n);
    ListPtr a = initialiseWithCapacity(n);
    for(int i = 0; i < n; i++) {
        a->arr[a->size++] = va_arg(valist, int);
    }
    va_end(valist);
    return a;
}

ListPtr range(int start, int stop, int step) {
    if(step == 0)
        return initialiseWithCapacity(0);
    ListPtr a = initialiseWithCapacity( abs(stop - start) / abs(step) + 1 );
    if(step > 0) {
        for(int i = start; i < stop; i += step)
            a->arr[a->size++] = i;
    }
    else {
        for(int i = start; i > stop; i += step)
            a->arr[a->size++] = i;
    }
    return a;
}

ListPtr slice(ListPtr a, int start, int stop, int step) {
    if(step == 0 || start < 0 || stop < -1 || start >= a->size || stop >= a->size)
        return initialiseWithCapacity(0);
    ListPtr b = initialiseWithCapacity( abs(stop - start) / abs(step) + 1 );
    if(step > 0) {
        for(int i = start; i < stop; i += step)
            b->arr[b->size++] = a->arr[i];
    }
    else {
        for(int i = start; i > stop; i += step)
            b->arr[b->size++] = a->arr[i];
    }
    return b;
}

bool clear(ListPtr a) {
    free(a->arr);
    a->arr = malloc(0);
    a->capacity = 0;
    a->size = 0;
    return true;
}

bool ensureCapacity(ListPtr a, int minCapacity) {
    if(minCapacity > a->capacity) {
        a->capacity += (a->capacity >> 1);
        if(a->capacity < minCapacity)
            a->capacity = minCapacity;
        a->arr = realloc(a->arr, a->capacity * sizeof *(a->arr));
    }
    return true;
}

bool trimToSize(ListPtr a) {
    a->capacity = a->size;
    a->arr = realloc(a->arr, a->capacity * sizeof *(a->arr));
    return true;
}

bool fill(ListPtr a, int value, int n) {
    ensureCapacity(a, n);
    a->size = n;
    for(int i = 0; i < n; i++) {
        a->arr[i] = value;
    }
    return true;
}

bool append(ListPtr a, int n) {
    ensureCapacity(a, a->size + 1);
    a->arr[a->size++] = n;
    return true;
}

bool extendWithArray(ListPtr a, int arr[], int length) {
    ensureCapacity(a, a->size + length);
    for(int i = 0; i < length; i++)
        a->arr[a->size++] = arr[i];
    return true;
}

bool extend(ListPtr a, ListPtr b) {
    extendWithArray(a, b->arr, b->size);
    return true;
}

bool insert(ListPtr a, int index, int n) {
    if(index < 0)
        index = a->size + index;

    if(index > a->size || index < 0)
        return false;

    if(index == a->size) {
        append(a, n);
        return true;
    }

    ensureCapacity(a, a->size + 1);
    for(int i = a->size; i > index; i--) {
        a->arr[i] = a->arr[i-1];
    }
    a->arr[index] = n;
    a->size++;
    return true;
}

ListPtr copy(ListPtr a) {
    ListPtr b = initialiseWithCapacity(a->capacity);
    extendWithArray(b, a->arr, a->size);
    return b;
}

int indexOf(ListPtr a, int value) {
    for(int i = 0; i < a->size; i++) {
        if(a->arr[i] == value)
            return i;
    }
    return -1;
}

int lastIndexOf(ListPtr a, int value) {
    for(int i = a->size - 1; i >= 0; i--) {
        if(a->arr[i] == value)
            return i;
    }
    return -1;
}

int binarySearch(ListPtr a, int value) {
    int lower = 0, upper = a->size - 1, mid = 0;
    while(lower <= upper) {
        mid = (lower + upper) / 2;
        if(a->arr[mid] == value)
            return mid;
        else if(a->arr[mid] < value)
            lower = mid + 1;
        else
            upper = mid - 1;
    }
    return -1;
}

bool contains(ListPtr a, int value) {
    return indexOf(a, value) >= 0;
}

bool isEmpty(ListPtr a) {
    return a->size == 0;
}

bool isEqual(ListPtr a, ListPtr b) {
    if(a->size != b->size)
        return false;
    for(int i = 0; i < a->size; i++) {
        if(a->arr[i] != b->arr[i])
            return false;
    }
    return true;
}

bool pop(ListPtr a, int index) {
    if(index < 0)
        index = a->size + index;

    if(index >= a->size || index < 0)
        return false;

    for(int i = index; i < a->size-1; i++)
        a->arr[i] = a->arr[i+1];
    a->size--;

    return true;
}

bool delete(ListPtr a, int value) {
int index = indexOf(a, value);
if(index == -1)
return false;
return pop(a, index);
}

int get(ListPtr a, int index) {
    if(index < 0)
        index = a->size + index;

    if(index >= a->size || index < 0)
        return -1;

    return a->arr[index];
}

bool set(ListPtr a, int index, int value) {
    if(index < 0)
        index = a->size + index;

    if(index >= a->size || index < 0)
        return false;

    a->arr[index] = value;
    return true;
}

bool reverse(ListPtr a) {
    for(int start = 0, stop = a->size-1; start < stop; start++, stop--) {
        int t = a->arr[start];
        a->arr[start] = a->arr[stop];
        a->arr[stop] = t;
    }
    return true;
}

bool replace(ListPtr a, int oldValue, int newValue) {
    for(int i = 0; i < a->size; i++) {
        if(a->arr[i] == oldValue)
            a->arr[i] = newValue;
    }
    return true;
}

/*Code for sorting begins*/
int comparisonFunctionAscending (const void *a, const void *b) {
    return ( *(int*)a - *(int*)b );
}

int comparisonFunctionDescending (const void *a, const void *b) {
    return ( *(int*)b - *(int*)a );
}

bool sort(ListPtr a) {
    qsort(a->arr, a->size, sizeof(int), comparisonFunctionAscending);
    return true;
}

bool sortReverse(ListPtr a) {
    qsort(a->arr, a->size, sizeof(int), comparisonFunctionDescending);
    return true;
}
/*Code for sorting ends*/

int count(ListPtr a, int value) {
    int c = 0;
    for(int i = 0; i < a->size; i++) {
        if(a->arr[i] == value)
            c++;
    }
    return c;
}

int sum(ListPtr a) {
    int s = 0;
    for(int i = 0; i < a->size; i++)
        s += a->arr[i];
    return s;
}

int len(ListPtr a) {
    return a->size;
}

int cap(ListPtr a) {
    return a->capacity;
}

int* toArray(ListPtr a) {
    int *b = malloc(a->size * sizeof *b);
    for(int i = 0; i < a->size; i++)
        b[i] = a->arr[i];
    return b;
}

char* toString(ListPtr a) {
    int iMax = a->size - 1;
    if(iMax == -1)
        return "[]";
    char *str = malloc( (a->size * 12 + 1) * sizeof *str );
    strcpy(str, "[");
    for(int i = 0; ; i++) {
        char temp[11];
        sprintf(temp, "%d", a->arr[i]);
        strcat(str, temp);
        if(i == iMax) {
            strcat(str, "]");
            return str;
        }
        strcat(str, ", ");
    }
}

bool display(ListPtr a) {
    printf("%s \n", toString(a));
    return true;
}









/* Demo.c */
void initialiseDemo() {
    printf("\n\t\t =============== START DEMO - initialise() =============== \n\n\n");

    printf("Initialising List a with initialise() \n\n");
    List *a = initialise();

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Appending 5, 9 \n\n");
    append(a, 5);
    append(a, 9);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - initialise() =============== \n\n\n");
}

void initialiseWithCapacityDemo() {
    printf("\n\t\t =============== START DEMO - initialiseWithCapacity() =============== \n\n\n");

    printf("Initialising List a with initialiseWithCapacity(0) \n\n");
    List *a = initialiseWithCapacity(0);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Appending 5, 4, 15, 9, 19 \n\n");
    append(a, 5);
    append(a, 4);
    append(a, 15);
    append(a, 9);
    append(a, 19);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - initialiseWithCapacity() =============== \n\n\n");
}

void initialiseWithArrayDemo() {
    printf("\n\t\t =============== START DEMO - initialiseWithArray() =============== \n\n\n");

    printf("Initialising int arr[] with numbers from 1 to 18 \n\n");
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

    printf("Initialising List a with arr[] using initialiseWithArray(arr, 18) \n\n");
    List *a = initialiseWithArray(arr, 18);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - initialiseWithArray() =============== \n\n\n");
}

void valuesDemo() {
    printf("\n\t\t =============== START DEMO - values() =============== \n\n\n");

    printf("Initialising List a with values(5, 1, 2, 5, 9, 17) \n\n");
    List *a = values(5, 1, 2, 5, 9, 17);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - values() =============== \n\n\n");
}

void rangeDemo() {
    printf("\n\t\t =============== START DEMO - range() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Initialising List b with range(10, -1, -2) \n\n");
    List *b = range(10, -1, -2);

    printf("\t b = %s \n", toString(b));
    printf("\t Number of elements = %d \n", len(b));
    printf("\t Capacity of a = %d \n\n", cap(b));

    printf("toString( range(5, 51, 5) ) = %s \n", toString( range(5, 51, 5) ));

    printf("\n\n\t\t =============== END DEMO - range() =============== \n\n\n");
}

void sliceDemo() {
    printf("\n\t\t =============== START DEMO - slice() =============== \n\n\n");

    printf("Initialising List a with range(10, 21, 1) \n\n");
    List *a = range(10, 21, 1);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));


    printf("toString( slice(a, 2, 6, 1) ) = %s \n\n", toString( slice(a, 2, 6, 1) ));
    printf("toString( slice(a, 10, -1, -2) ) = %s \n\n", toString( slice(a, 10, -1, -2) ));

    printf("\n\n\t\t =============== END DEMO - slice() =============== \n\n\n");
}

void clearDemo() {
    printf("\n\t\t =============== START DEMO - clear() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 1) \n\n");
    List *a = range(0, 11, 1);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Removing all elements from List a using clear(a) \n\n");
    clear(a);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Appending 5 \n\n");
    append(a, 5);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - clear() =============== \n\n\n");
}

void ensureCapacityDemo() {
    printf("\n\t\t =============== START DEMO - ensureCapacity() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Ensuring a minimum capacity of 2 using ensureCapacity(a, 2) \n\n");
    ensureCapacity(a, 2);
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Ensuring a minimum capacity of 6 using ensureCapacity(a, 6) \n\n");
    ensureCapacity(a, 6);
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Ensuring a minimum capacity of 7 using ensureCapacity(a, 7) \n\n");
    ensureCapacity(a, 7);
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Ensuring a minimum capacity of 20 using ensureCapacity(a, 20) \n\n");
    ensureCapacity(a, 20);
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - ensureCapacity() =============== \n\n\n");
}

void trimToSizeDemo() {
    printf("\n\t\t =============== START DEMO - trimToSize() =============== \n\n\n");

    printf("Initialising List a with initialise() \n\n");
    List *a = initialise();

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Initialising int arr[] with numbers from 1 to 6 \n\n");
    int arr[] = {1, 2, 3, 4, 5, 6};

    printf("Extending List a with arr[] using extendWithArray(a, arr, 6) \n\n");
    extendWithArray(a, arr, 6);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Trimming Unused Space in List a using trimToSize(a) \n\n");
    trimToSize(a);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - trimToSize() =============== \n\n\n");
}

void fillDemo() {
    printf("\n\t\t =============== START DEMO - fill() =============== \n\n\n");

    printf("Initialising List a with initialise() \n\n");
    List *a = initialise();

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Filling List a with 20 occurences of 5 using fill(a, 5, 20) \n\n");
    fill(a, 5, 20);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Filling List a with 7 occurences of 19 using fill(a, 19, 7) \n\n");
    fill(a, 19, 7);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - fill() =============== \n\n\n");
}

void appendDemo() {
    printf("\n\t\t =============== START DEMO - append() =============== \n\n\n");

    printf("Initialising List a with initialiseWithCapacity(0) \n\n");
    List *a = initialiseWithCapacity(0);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Appending 5, 4, 15, 9, 19 using append(a, n) \n\n");
    append(a, 5);
    append(a, 4);
    append(a, 15);
    append(a, 9);
    append(a, 19);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - append() =============== \n\n\n");
}

void extendWithArrayDemo() {
    printf("\n\t\t =============== START DEMO - extendWithArray() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Initialising int arr[] with numbers from 1 to 6 \n\n");
    int arr[] = {1, 2, 3, 4, 5, 6};

    printf("Extending List a with arr[] using extendWithArray(a, arr, 6) \n\n");
    extendWithArray(a, arr, 6);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - extendWithArray() =============== \n\n\n");
}

void extendDemo() {
    printf("\n\t\t =============== START DEMO - extend() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Initialising List b with range(1, 11, 2) \n\n");
    List *b = range(1, 11, 2);

    printf("\t b = %s \n", toString(b));
    printf("\t Number of elements = %d \n", len(b));
    printf("\t Capacity of b = %d \n\n", cap(b));

    printf("Extending List a with List b using extend(a, b) \n\n");
    extend(a, b);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Extending List a using extend(a, range(90, 96, 2)) \n\n");
    extend(a, range(90, 96, 2));

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - extend() =============== \n\n\n");
}

void insertDemo() {
    printf("\n\t\t =============== START DEMO - insert() =============== \n\n\n");

    printf("Initialising List a with range(0, 10, 1) \n\n");
    List *a = range(0, 10, 1);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Inserting 99 at 0th index using insert(a, 0, 99) \n\n");
    insert(a, 0, 99);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Inserting 88 at the end using insert(a, len(a), 88) \n\n");
    insert(a, len(a), 88);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Inserting 55 at 5th index using insert(a, 5, 55) \n\n");
    insert(a, 5, 55);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Inserting 22 at the last index using insert(a, -1, 22) \n\n");
    insert(a, -1, 22);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - insert() =============== \n\n\n");
}

void copyDemo() {
    printf("\n\t\t =============== START DEMO - copy() =============== \n\n\n");

    printf("Initialising List a with range(0, 10, 2) \n\n");
    List *a = range(0, 10, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Initialising List b with copy(a) \n\n");
    List *b = copy(a);

    printf("\t b = %s \n", toString(b));
    printf("\t Number of elements = %d \n", len(b));
    printf("\t Capacity of b = %d \n", cap(b));

    printf("\n\n\t\t =============== END DEMO - copy() =============== \n\n\n");
}

void indexOfDemo() {
    printf("\n\t\t =============== START DEMO - indexOf() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Searching for the first occurrence of numbers from 0 to 10 using indexOf(a, i) \n\n");
    for(int i = 0; i < 11; i++) {
        int index = indexOf(a, i);
        if(index < 0)
            printf("\t %d not found in a \n", i);
        else
            printf("\t %d found at index %d \n", i, index);
    }

    printf("\n\n\t\t =============== END DEMO - indexOf() =============== \n\n\n");
}

void lastIndexOfDemo() {
    printf("\n\t\t =============== START DEMO - lastIndexOf() =============== \n\n\n");

    printf("Initialising int arr[] with {0, 1, 2, 2, 3, 4, 5, 6, 2, 7, 8, 9} \n\n");
    int arr[] = {0, 1, 2, 2, 3, 4, 5, 6, 2, 7, 8, 9};

    printf("Initialising List a with arr[] using initialiseWithArray(arr, 12) \n\n");
    List *a = initialiseWithArray(arr, 12);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Searching for the last occurrence of 2, 10 using lastIndexOf(a, i) \n\n");
    printf("\t Last index of 2 = %d \n", lastIndexOf(a, 2));
    printf("\t Last index of 10 = %d \n", lastIndexOf(a, 10));

    printf("\n\n\t\t =============== END DEMO - lastIndexOf() =============== \n\n\n");
}

void binarySearchDemo() {
    printf("\n\t\t =============== START DEMO - binarySearch() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Searching for the index of numbers from 0 to 10 using binarySearch(a, i) \n\n");
    for(int i = 0; i < 11; i++) {
        int index = binarySearch(a, i);
        if(index < 0)
            printf("\t %d not found in a \n", i);
        else
            printf("\t %d found at index %d \n", i, index);
    }

    printf("\n\n\t\t =============== END DEMO - binarySearch() =============== \n\n\n");
}

void containsDemo() {
    printf("\n\t\t =============== START DEMO - contains() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Checking for the presence of numbers from 0 to 10 using contains(a, i) \n\n");
    for(int i = 0; i < 11; i++) {
        if(contains(a, i))
            printf("\t a contains %d \n", i);
        else
            printf("\t a doesn't contain %d \n", i);
    }

    printf("\n\n\t\t =============== END DEMO - contains() =============== \n\n\n");
}

void isEmptyDemo() {
    printf("\n\t\t =============== START DEMO - isEmpty() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 1) \n\n");
    List *a = range(0, 11, 1);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Checking if a is empty using isEmpty(a) \n\n");
    if(isEmpty(a))
        printf("\t a is empty \n\n");
    else
        printf("\t a is not empty \n\n");

    printf("Removing all elements from List a using clear(a) \n\n");
    clear(a);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Checking if a is empty using isEmpty(a) \n\n");
    if(isEmpty(a))
        printf("\t a is empty \n");
    else
        printf("\t a is not empty \n");

    printf("\n\n\t\t =============== END DEMO - isEmpty() =============== \n\n\n");
}

void isEqualDemo() {
    printf("\n\t\t =============== START DEMO - isEqual() =============== \n\n\n");

    printf("Initialising List a with range(0, 10, 2) \n\n");
    List *a = range(0, 10, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Initialising List b with copy(a) \n\n");
    List *b = copy(a);

    printf("\t b = %s \n", toString(b));
    printf("\t Number of elements = %d \n", len(b));
    printf("\t Capacity of b = %d \n\n", cap(b));

    printf("Checking if List a is equal to List b using isEqual(a, b) \n\n");
    if(isEqual(a, b))
        printf("\t a is equal to b \n\n");
    else
        printf("\t a is not equal to b \n\n");

    printf("Appending 0 to List b using append(b, 0) \n\n");
    append(b, 0);

    printf("\t b = %s \n", toString(b));
    printf("\t Number of elements = %d \n", len(b));
    printf("\t Capacity of b = %d \n\n", cap(b));

    printf("Checking if List a is equal to List b using isEqual(a, b) \n\n");
    if(isEqual(a, b))
        printf("\t a is equal to b \n\n");
    else
        printf("\t a is not equal to b \n\n");

    printf("Appending 0 to List a using append(a, 0) \n\n");
    append(a, 0);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Checking if List a is equal to List b using isEqual(a, b) \n\n");
    if(isEqual(a, b))
        printf("\t a is equal to b \n");
    else
        printf("\t a is not equal to b \n");

    printf("\n\n\t\t =============== END DEMO - isEqual() =============== \n\n\n");
}

void popDemo() {
    printf("\n\t\t =============== START DEMO - pop() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 1) \n\n");
    List *a = range(0, 11, 1);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Removing first element of a using pop(a, 0) \n\n");
    pop(a, 0);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Removing last element of a using pop(a, -1) \n\n");
    pop(a, -1);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Trying to remove value at index 10 from a using pop(a, 10) \n\n");
    if(pop(a, 10))
        printf("\t a = %s \n\n", toString(a));
    else
        printf("\t index %d not found in a \n\n", 10);

    printf("Trying to remove value at index 2 from a using pop(a, 2) \n\n");
    if(pop(a, 2))
        printf("\t a = %s \n\n", toString(a));
    else
        printf("\t index %d not found in a \n\n", 10);

    printf("Removing value at 2nd last index using pop(a, -2) \n\n");
    pop(a, -2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - pop() =============== \n\n\n");
}

void deleteDemo() {
    printf("\n\t\t =============== START DEMO - delete() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Trying to delete numbers from 0 to 10 using delete(a, i) \n\n");
    for(int i = 0; i < 11; i++) {
        if(delete(a, i))
            printf("\t %d deleted from a \n", i);
        else
            printf("\t a doesn't contain %d \n", i);
    }
    printf("\n");

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - delete() =============== \n\n\n");
}

void getDemo() {
    printf("\n\t\t =============== START DEMO - get() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Trying to print the values at indexes 0 to 10 using get(a, i) \n\n");
    for(int i = 0; i < 11; i++) {
        printf("\t Value at index %d = %d \n", i, get(a, i));
    }

    printf("\n\n\t\t =============== END DEMO - get() =============== \n\n\n");
}

void setDemo() {
    printf("\n\t\t =============== START DEMO - set() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Trying to double the value of all elements using set(a, i, get(a, i) * 2) \n\n");
    for(int i = 0; i < 11; i++) {
        if(set(a, i, get(a, i) * 2))
            printf("\t Doubling value at index %d successful \n", i);
        else
            printf("\t Index %d not found \n", i);
    }
    printf("\n");

    printf("a after doubling \n\n");
    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - set() =============== \n\n\n");
}

void reverseDemo() {
    printf("\n\t\t =============== START DEMO - reverse() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 1) \n\n");
    List *a = range(0, 11, 1);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Reversing the order of elements using reverse(a) \n\n");
    reverse(a);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - reverse() =============== \n\n\n");
}

void replaceDemo() {
    printf("\n\t\t =============== START DEMO - replace() =============== \n\n\n");

    printf("Initialising int arr[] with {0, 1, 2, 3, 4, 5, 6, 1, 2, 2, 3, 4, 2, 6, 2} \n\n");
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 1, 2, 2, 3, 4, 2, 6, 2};

    printf("Initialising List a with arr[] using initialiseWithArray(arr, 15) \n\n");
    List *a = initialiseWithArray(arr, 15);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Replacing all occurrences of 2 with 99 using replace(a, 2, 99) \n\n");
    replace(a, 2, 99);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - replace() =============== \n\n\n");
}

void sortDemo() {
    printf("\n\t\t =============== START DEMO - sort() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Extending List a using extend(a, range(1, 11, 2)) \n\n");
    extend(a, range(1, 11, 2));

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Sorting the elements of a in increasing order using sort(a) \n\n");
    sort(a);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - sort() =============== \n\n\n");
}

void sortReverseDemo() {
    printf("\n\t\t =============== START DEMO - sortReverse() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Extending List a using extend(a, range(1, 11, 2)) \n\n");
    extend(a, range(1, 11, 2));

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Sorting the elements of a in increasing order using sortReverse(a) \n\n");
    sortReverse(a);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - sortReverse() =============== \n\n\n");
}

void countDemo() {
    printf("\n\t\t =============== START DEMO - count() =============== \n\n\n");

    printf("Initialising int arr[] with {1, 2, 2, 3, 3, 3, 5, 5, 5, 5} \n\n");
    int arr[] = {1, 2, 2, 3, 3, 3, 5, 5, 5, 5};

    printf("Initialising List a with arr[] using initialiseWithArray(arr, 10) \n\n");
    List *a = initialiseWithArray(arr, 10);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Counting the frequency of numbers from 0 to 5 using count(a, i) \n\n");
    for(int i = 0; i <= 5; i++)
        printf("\t %d is present %d times \n", i, count(a, i));

    printf("\n\n\t\t =============== END DEMO - count() =============== \n\n\n");
}

void sumDemo() {
    printf("\n\t\t =============== START DEMO - sum() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Calculating sum of all elements of a using sum(a) \n\n");
    printf("\t Sum(a) = %d \n", sum(a));

    printf("\n\n\t\t =============== END DEMO - sum() =============== \n\n\n");
}

void lenDemo() {
    printf("\n\t\t =============== START DEMO - len() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Appending 5, 9 \n\n");
    append(a, 5);
    append(a, 9);

    printf("\t a = %s \n", toString(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Finding new length of a using len(a) \n\n");
    printf("\t Len(a) = %d \n", len(a));

    printf("\n\n\t\t =============== END DEMO - len() =============== \n\n\n");
}

void capDemo() {
    printf("\n\t\t =============== START DEMO - cap() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Appending 5 \n\n");
    append(a, 5);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n\n", len(a));

    printf("Finding new capacity of a using cap(a) \n\n");
    printf("\t Cap(a) = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - cap() =============== \n\n\n");
}

void toArrayDemo() {
    printf("\n\t\t =============== START DEMO - toArray() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("\t a = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Initialising int arr[] with elements of List a using toArray(a) \n\n");
    int *arr = toArray(a);
    int arrLength = len(a);

    printf("\t arr[] = ");
    for(int i = 0; i < arrLength; i++)
        printf("%d ", arr[i]);
    printf("\n");

    printf("\n\n\t\t =============== END DEMO - toArray() =============== \n\n\n");
}

void toStringDemo() {
    printf("\n\t\t =============== START DEMO - toString() =============== \n\n\n");

    printf("Initialising List a with range(1000000000, 1000000009, 1) \n\n");
    List *a = range(1000000000, 1000000009, 1);

    printf("\t toString(a) = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n\n", cap(a));

    printf("Appending 1111111111 \n\n");
    append(a, 1111111111);

    printf("\t toString(a) = %s \n", toString(a));
    printf("\t Number of elements = %d \n", len(a));
    printf("\t Capacity of a = %d \n", cap(a));

    printf("\n\n\t\t =============== END DEMO - toString() =============== \n\n\n");
}

void displayDemo() {
    printf("\n\t\t =============== START DEMO - display() =============== \n\n\n");

    printf("Initialising List a with range(0, 11, 2) \n\n");
    List *a = range(0, 11, 2);

    printf("Displaying the elements of a using display(a) \n\n");
    printf("\t ");
    display(a);
    printf("\n");

    printf("Appending 5 \n\n");
    append(a, 5);

    printf("Displaying the elements of a using display(a) \n\n");
    printf("\t ");
    display(a);

    printf("\n\n\t\t =============== END DEMO - display() =============== \n\n\n");
}

int main(void) {

    initialiseDemo();

    initialiseWithCapacityDemo();

    initialiseWithArrayDemo();

    valuesDemo();

    rangeDemo();

    sliceDemo();

    clearDemo();

    ensureCapacityDemo();

    trimToSizeDemo();

    fillDemo();

    appendDemo();

    extendWithArrayDemo();

    extendDemo();

    insertDemo();

    copyDemo();

    indexOfDemo();

    lastIndexOfDemo();

    binarySearchDemo();

    containsDemo();

    isEmptyDemo();

    isEqualDemo();

    popDemo();

    deleteDemo();

    getDemo();

    setDemo();

    reverseDemo();

    replaceDemo();

    sortDemo();

    sortReverseDemo();

    countDemo();

    sumDemo();

    lenDemo();

    capDemo();

    toArrayDemo();

    toStringDemo();

    displayDemo();

    return 0;
}