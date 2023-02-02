#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void readFromFile();
void resetListOfCells();
void displayBoard();
void findPathToCell();

const int maxHeight = 50;
const int maxWidth = 50;

const char WALL = '#';
const char FLOOR = '.';
const char INACTIVE = 'X';

struct cell *queue[maxHeight*maxWidth];
int count = 0;

void insertToQueue(struct cell *cellToSave)
{
    queue[count] = cellToSave;
    count++;
}

bool isUnique(struct cell *uniqueCell)
{
    for (int i = 0; i < count; i++)
    {
        if (queue[i] == uniqueCell)
        {
            return true;
        }
    }
    return false;
}

void resetQueue()
{
    for (int i = 0; i < count; i++)
    {
        queue[i] = NULL;
    }
    count = 0;
}

struct cell *popFromQueue()
{
    if (count == 0)
    {
        return NULL;
    }
    struct cell *deletedCell = queue[0];
    for (int i = 0; i < count; i++)
    {
        queue[i] = queue[i + 1];
    }
    count--;
    return deletedCell;
}


struct cell
{
    struct cell *left_cell;
    struct cell *up_cell;
    struct cell *right_cell;
    struct cell *down_cell;
    bool visited;
    int steps;
    char value; // sciana/podloga
} baseCell;

struct cell list_of_cells[maxHeight][maxWidth];

int main()
{

    readFromFile();
    return 0;
}

void readFromFile()
{
    FILE *file;
    file = fopen("test/test.txt", "r");
    while (true)
    {
        int height = 0;
        int width = 0;
        char type;
        fscanf(file, "%d", &width);
        fscanf(file, "%d", &height);

        if (height)
        {
            printf("width: %d height: %d\n", width, height);
            printf("\nMAP:");
            resetListOfCells(height, width);

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width + 1; j++)
                {
                    fscanf(file, "%c", &type);
                    printf("%c", type);
                    if (type == WALL)
                    {
                        list_of_cells[i][j - 1].value = WALL;
                    }
                }
            }

            struct cell *nextCell = popFromQueue();
            while (nextCell)
            {
                findPathToCell(nextCell);
                nextCell = popFromQueue();
            }
            printf("\nSOLUTION:\n");
            displayBoard(height, width);
        }
        else
        {
            break;
        }
    }
}

void resetListOfCells(int height, int width)
{
    baseCell.value = FLOOR;
    baseCell.steps = 0;
    baseCell.right_cell = &list_of_cells[0][1];
    baseCell.down_cell = &list_of_cells[1][0];
    baseCell.up_cell = NULL;
    baseCell.left_cell = NULL;
    list_of_cells[0][0] = baseCell;
    insertToQueue(&list_of_cells[0][0]);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            else if (i > 0 && i < height - 1)
            {
                if (j > 0 && j < width - 1)
                {
                    list_of_cells[i][j].left_cell = &list_of_cells[i][j - 1];
                    list_of_cells[i][j].right_cell = &list_of_cells[i][j + 1];
                    list_of_cells[i][j].up_cell = &list_of_cells[i - 1][j];
                    list_of_cells[i][j].down_cell = &list_of_cells[i + 1][j];
                }
                else if (j == 0)
                {
                    list_of_cells[i][j].left_cell = NULL;
                    list_of_cells[i][j].right_cell = &list_of_cells[i][j + 1];
                    list_of_cells[i][j].up_cell = &list_of_cells[i - 1][j];
                    list_of_cells[i][j].down_cell = &list_of_cells[i + 1][j];
                }
                else if (j == width - 1)
                {
                    list_of_cells[i][j].left_cell = &list_of_cells[i][j - 1];
                    list_of_cells[i][j].right_cell = NULL;
                    list_of_cells[i][j].up_cell = &list_of_cells[i - 1][j];
                    list_of_cells[i][j].down_cell = &list_of_cells[i + 1][j];
                }
            }
            else if (i == 0)
            {
                if (j > 0 && j < width - 1)
                {
                    list_of_cells[i][j].left_cell = &list_of_cells[i][j - 1];
                    list_of_cells[i][j].right_cell = &list_of_cells[i][j + 1];
                }
                else if (j == width - 1)
                {
                    list_of_cells[i][j].left_cell = &list_of_cells[i][j - 1];
                    list_of_cells[i][j].right_cell = NULL;
                }

                list_of_cells[i][j].up_cell = NULL;
                list_of_cells[i][j].down_cell = &list_of_cells[i + 1][j];
            }
            else if (i == height - 1)
            {
                if (j > 0 && j < width - 1)
                {
                    list_of_cells[i][j].left_cell = &list_of_cells[i][j - 1];
                    list_of_cells[i][j].right_cell = &list_of_cells[i][j + 1];
                }
                else if (j == width - 1)
                {
                    list_of_cells[i][j].left_cell = &list_of_cells[i][j - 1];
                    list_of_cells[i][j].right_cell = NULL;
                }
                else if (j == 0)
                {
                    list_of_cells[i][j].left_cell = NULL;
                    list_of_cells[i][j].right_cell = &list_of_cells[i][j + 1];
                }

                list_of_cells[i][j].up_cell = &list_of_cells[i - 1][j];
                list_of_cells[i][j].down_cell = NULL;
            }
            list_of_cells[i][j].value = FLOOR;
            list_of_cells[i][j].steps = 0;
            list_of_cells[i][j].visited = false;
        }
    }
}

void displayBoard(int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (list_of_cells[i][j].value == FLOOR)
            {
                if (list_of_cells[i][j].steps < 10)
                    printf(" ");
                printf(" %d ", list_of_cells[i][j].steps);
            }
            else if (list_of_cells[i][j].value == INACTIVE)
            {
                printf("  0 ");
            }
            else
            {
                printf("  # ");
            }
        }
        printf("\n");
    }
}

int findIndexLowestValue(struct cell *arr[4], int j)
{
    int x = (*arr[0]).steps;
    int y = 0;
    for (int i = 1; i < j; i++)
    {
        if (x > (*arr[i]).steps)
        {
            x = (*arr[i]).steps;
            y = i;
        }
    }
    return y;
}

void findPathToCell(struct cell *processingCell)
{

    struct cell *allNeighbors[4] = {
        (*processingCell).left_cell,
        (*processingCell).right_cell,
        (*processingCell).up_cell,
        (*processingCell).down_cell};

    struct cell *visitedNeighbors[4];
    struct cell *unvisitedNeighbors[4];
    int j = 0;
    int k = 0;
    for (int i = 0; i < 4; i++)
    {
        if (allNeighbors[i])
        {
            if (((*allNeighbors[i]).value == FLOOR) && ((*allNeighbors[i]).visited))
            {
                visitedNeighbors[j] = allNeighbors[i];
                j++;
            }
            else if (((*allNeighbors[i]).value == FLOOR) && !((*allNeighbors[i]).visited))
            {
                unvisitedNeighbors[k] = allNeighbors[i];
                k++;
            }
        }
    }
    if (j == 0)
    {
        (*processingCell).steps = 0;
        (*processingCell).visited = true;
        for (int i = 0; i < k; i++)
        {
            if (!isUnique(unvisitedNeighbors[i]))
            {
                insertToQueue(unvisitedNeighbors[i]);
            }
        }
    }
    else
    {
        (*processingCell).visited = true;
        int indexOfLowest = findIndexLowestValue(visitedNeighbors, j);
        (*processingCell).steps = (*visitedNeighbors[indexOfLowest]).steps + 1;
        for (int i = 0; i < k; i++)
        {
            if (!isUnique(unvisitedNeighbors[i]))
            {
                insertToQueue(unvisitedNeighbors[i]);
            }
        }
    }
}
