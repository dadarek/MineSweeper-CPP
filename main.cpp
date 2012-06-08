#include <stdio.h> // C functions
#include <iostream> // C++ functions

int __TOTAL_PASSES__ = 0;
int __TOTAL_FAILURES__ = 0;

#define ASSERT_STREQ(expected, actual) \
{ \
  const char* e = expected; \
  const char* a = actual; \
  if( strcmp(e, a) != 0 ) \
  { \
    __TOTAL_FAILURES__++; \
    printf("\033[0;31m"); \
    printf("******ASSERTION FAILED******\n"); \
    printf("In file       : %s:%d\n", __FILE__, __LINE__); \
    printf("Expected      : %s\n", e); \
    printf("Actual        : %s\n", a); \
    printf("Comparing     : %s\n", #expected); \
    printf("Against       : %s\n\n", #actual); \
    printf("\033[0m"); \
  } \
  else \
  { \
    __TOTAL_PASSES__++; \
    printf("\033[0;32m"); \
    printf("TEST PASSED\n"); \
    printf("In file       : %s:%d\n", __FILE__, __LINE__); \
    printf("Expected      : %s\n", e); \
    printf("Actual        : %s\n", a); \
    printf("Comparing     : %s\n", #expected); \
    printf("Against       : %s\n\n", #actual); \
    printf("\033[0m"); \
  } \
}  

using namespace std;

struct Field
{
  char spaces[10000];
  int rows;
  int columns;
  Field(int rows, int columns)
    : rows(rows),
      columns(columns)
  {}
};

bool hasMine(const char* field, int square )
{
  return field[square] == '*';
}
bool isFirstColumn(int square, int columns)
{
  return square % columns == 0;
}
bool isLastColumn(int square, int columns)
{
  return isFirstColumn(square+1, columns);
}
bool isLastRow(int square, int rows, int columns)
{
  int totalSquares = rows * columns;
  int sameColumnNextRow = square + columns;
  return sameColumnNextRow >= totalSquares;
}

char intToChar(int i)
{
  char intAsChar[2];
  sprintf(intAsChar, "%d", i);
  return intAsChar[0];
}

bool previousColumnHasMine(int currentSquare, const char* field, int rows, int columns)
{
  bool result = false;
  if( !isFirstColumn(currentSquare, columns) )
  {
    result = hasMine(field, currentSquare - 1);
  }
  return result;
}

bool nextColumnHasMine(int currentSquare, const char* field, int rows, int columns)
{
  bool result = false;

  if( !isLastColumn(currentSquare, columns) )
  {
    result = hasMine(field, currentSquare + 1);
  }
  return result;
}

bool rowBelowHasMine(int currentSquare, const char* field, int rows, int columns)
{
  bool result = false;


  if( !isLastRow(currentSquare, rows, columns) )
  {
    result = hasMine(field, currentSquare + columns);
  }
  return result;
}

int countNeighboringMines(int currentSquare, const char* field, int rows, int columns)
{
  int neighboringMines = 0;
  if( nextColumnHasMine(currentSquare, field, rows, columns) ) ++neighboringMines;
  if( previousColumnHasMine(currentSquare, field, rows, columns) ) ++neighboringMines;
  if( rowBelowHasMine(currentSquare, field, rows, columns) ) ++neighboringMines;
  return neighboringMines;
}


void findMines( const char* field, int rows, int columns, char* result)
{
  for(int currentColumn = 0; currentColumn < columns; currentColumn++)
  {
    for(int currentRow = 0; currentRow < rows; currentRow++)
    {
      int currentSquare = ((currentRow+1) * (currentColumn+1)) - 1;

      if(field[currentSquare] == '*')
      {
        result[currentSquare] = '*';
      }
      else
      {
        int neighboringMines = countNeighboringMines(currentSquare, field, rows, columns);
        result[currentSquare] = intToChar( neighboringMines );
      }
    }
  }

  int lastSquare = columns * rows - 1;
  result[ lastSquare+1 ] = '\0';
}

void RUN_TESTS()
{
  char field[10000];
  char result[10000];

  strcpy(field, ".");
  findMines(field, 1, 1, result);
  ASSERT_STREQ("0", result);

  strcpy(field, "*");
  findMines(field, 1, 1, result);
  ASSERT_STREQ("*", result);

  strcpy(field, "..");
  findMines(field, 1, 2, result);
  ASSERT_STREQ("00", result);

  strcpy(field, "**");
  findMines(field, 1, 2, result);
  ASSERT_STREQ("**", result);

  strcpy(field, ".*");
  findMines(field, 1, 2, result);
  ASSERT_STREQ("1*", result);

  strcpy(field, "*.");
  findMines(field, 1, 2, result);
  ASSERT_STREQ("*1", result);

  strcpy(field, "...");
  findMines(field, 1, 3, result);
  ASSERT_STREQ("000", result);

  strcpy(field, "***");
  findMines(field, 1, 3, result);
  ASSERT_STREQ("***", result);

  strcpy(field, "*..");
  findMines(field, 1, 3, result);
  ASSERT_STREQ("*10", result);

  strcpy(field, "..*");
  findMines(field, 1, 3, result);
  ASSERT_STREQ("01*", result);

  strcpy(field, ".*.");
  findMines(field, 1, 3, result);
  ASSERT_STREQ("1*1", result);

  strcpy(field, "*.*");
  findMines(field, 1, 3, result);
  ASSERT_STREQ("*2*", result);

  strcpy(field, "**.");
  findMines(field, 1, 3, result);
  ASSERT_STREQ("**1", result);

  strcpy(field, ".**");
  findMines(field, 1, 3, result);
  ASSERT_STREQ("1**", result);

  // ROWS

  strcpy(field, "..");
  findMines(field, 2, 1, result);
  ASSERT_STREQ("00", result);

  strcpy(field, "**");
  findMines(field, 2, 1, result);
  ASSERT_STREQ("**", result);

  strcpy(field, ".*");
  findMines(field, 2, 1, result);
  ASSERT_STREQ("1*", result);

  //strcpy(field, "*.");
  //findMines(field, 2, 1, result);
  //ASSERT_STREQ("*1", result);


  if(__TOTAL_FAILURES__ > 0)
    printf("\033[0;31m");
  else
    printf("\033[0;32m");

  printf("******TEST SUMMARY******\n");
  printf("Passes   : %d\n", __TOTAL_PASSES__);
  printf("Failures : %d\n", __TOTAL_FAILURES__);
  printf("-------------\n");
  printf("Total    : %d\n", __TOTAL_FAILURES__ + __TOTAL_PASSES__);
  printf("\033[0m");
}

void C_READ()
{
  for(;;)
  {
    long p, q;
    printf("C Reading...\n");
    if( EOF == scanf("%ld %ld", &p, &q ) )
      break;

    printf( "You typed: [%ld, %ld]\n", p, q );
  }
}

void CPP_READ()
{
  for(;;)
  {
    long long p, q;
    cout << "C++ Reading..." << endl;
    if( !( cin >> p >> q ) )
      break;

    cout << "You typed: [" << p << ", " << q << "]" << endl;
  }

  cout << "Done reading." << endl;
}

int main()
{
  RUN_TESTS();
  return 0;
}
