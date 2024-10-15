#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>


// metoda do drukowania tablicy
void printArray(int** array, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%3d", array[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// metoda do drukowania sciezki
void printPathArray(int** array, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (array[i][j] == 0) {
                printf("  0");
            }
            else {
                printf("  X");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// dla zadanej wartosci target ze zbioru {-1, 0, 1} i zadanej orientacji isVertical (pion, poziom) 
// oraz dla zadanej komorki na pozycji [row][col] zwraca wartosc true jesli istnieje sciezka z tej komorki
bool hasPathFromCell(int** array, int** visited, int size, int row, int col, int target, bool isVertical) {
    // sprawdzamy czy jestesmy w zakresie tablicy
    if (row >= size || row < 0 || col >= size || col < 0 ) {
        return false;
    }

    // nie odwiedzamy juz komorki ktora byla odwiedzona aby nie wpasc w petle
    if (visited[row][col] != 0) { 
        return false;
    }

    // sprawdzamy czy wartość w komórce jest taka jaka oczekujemy
    if (array[row][col] != target) {
        return false;
    }

    // oznaczamy komorke jako odwiedzona
    visited[row][col] = 1;

    // jesli idąc wertykalnie doszlismy do ostatniego wiersza tablicy to konczymy
    if (row == size - 1 && isVertical) {
        printf("Vertical path for %d exist:\n", target);
        // tablica visited wskazuje sciezke jaka przeszedl algorytm
        printPathArray(visited, size);
        return true;
    }

    // jesli idąc horyzontalnie doszlismy do ostatniej kolumny tablicy
    if (col == size - 1 && !isVertical) {
        printf("Horizontal path for %d exist:\n", target);
        // tablica visited wskazuje sciezke jak przeszedl algorytm
        printPathArray(visited, size);
        return true;
    }

    // rekurencyjnie wywolujemy metode dla kazdego "sąsiada", priorytetem sa dla nas komorki bliskie wektorowi [1,1]
    if (hasPathFromCell(array, visited, size, row + 1, col + 1, target, isVertical) ||
        hasPathFromCell(array, visited, size, row, col + 1, target, isVertical) ||
        hasPathFromCell(array, visited, size, row + 1, col, target, isVertical) ||
        hasPathFromCell(array, visited, size, row + 1, col - 1, target, isVertical) ||
        hasPathFromCell(array, visited, size, row - 1, col + 1, target, isVertical) ||
        hasPathFromCell(array, visited, size, row, col - 1, target, isVertical) ||
        hasPathFromCell(array, visited, size, row - 1, col, target, isVertical) ||
        hasPathFromCell(array, visited, size, row - 1, col - 1, target, isVertical)) {
        return true;
    }

    // jezeli zaden z "sąsiadów" nie jest "odpowiedni" to sie cofamy i zwracamy false
    visited[row][col] = 0;
    return false;
}

// dla zadanej wartosci target ze zbioru {-1, 0, 1} i orientacji isVertical (poziom=0, pion=1) zwraca wartosc true jesli istnieje sciezka
bool hasPath(int** array, int size, int target, bool isVertical) {
    // przechodzimy przez 1. wiersz lub 1. kolumne w zaleznosci od isVertical
    for (int i = 0; i < size; i++) {
        // dwuwymiarowa tablica odwiedzonych komorek, konieczna aby sie nie zapetlic
        int** visited = (int**)malloc(size * sizeof(int*));
        for (int i = 0; i < size; i++) {
            visited[i] = (int*)malloc(size * sizeof(int));
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                visited[i][j] = 0;
            }
        }

        // w zaleznosci od isVertical ustalamy czy to bedzie 1. wiersz czy 1. kolumna
        int row = isVertical ? 0 : i;
        int col = isVertical ? i : 0;
        // zwracamy true od razu jak znajdziemy sciezke, bo zgodnie z umowa nie zwiekszy sie i tak liczba punktow
        if (hasPathFromCell(array, visited, size, row, col, target, isVertical))
            return true;
    }
    
    // jesli nie znalezlismy sciezki przechodzac przez caly 1. wiersz lub 1. kolumne to zwracamy false
    return false;
}

// dla kazdej wartosci ze zbioru {-1, 0, 1} i kazdej orientacji isVertical (poziom=0, pion=1) sumujemy laczny wynik, i go zwracamy
int getResult(int** array, int size) {
    int result = 0;
    for (int target = -1; target <= 1; target++) {
        for (int isVertical = 0; isVertical <= 1; isVertical++) {
            // jesli istnieje sciezka to zwiekszamy wynik
            if (hasPath(array, size, target, isVertical)) {
                result++;
            }
        }
    }
    return result;
}

int main()
{
    // wczytanie wymiaru tablicy
    int size;
    printf("Please enter the size of the square array: ");
    scanf_s("%d", &size);

    // sprawdzenie czy wartosc spelnia warunek zadania
    if (size <= 3) {
        printf("You must enter a natural number greater than 3!\n");
        return 1;
    }

    // alokacja pamieci na tablice o rozmiarze size*size
    int** array = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        array[i] = (int*)malloc(size * sizeof(int));
    }

    // wypelnienie tablicy losowymi wartosciami ze zbioru {-1, 0, 1}
    srand(time(0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            array[i][j] = rand() % 3 - 1;
        }
    }

    // wydrukowanie wyniku
    printf("Input array:\n");
    printArray(array, size);
    int result = getResult(array, size);
    printf("Total number of points: %d\n", result);

    _getch();
    return 0;
}
