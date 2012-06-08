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
  Field(const char* spaces, int rows, int columns)
    : rows(rows),
      columns(columns)
  {
    strcpy(this->spaces, spaces);
  }
};

bool hasMine(const Field& field, int square )
{
  return field.spaces[square] == '*';
}
bool isFirstColumn(const Field& field, int square)
{
  return square % field.columns == 0;
}
bool isLastColumn(const Field& field, int square)
{
  return isFirstColumn(field, square+1);
}
bool isLastRow(const Field& field, int square)
{
  int totalSquares = field.rows * field.columns;
  int sameColumnNextRow = square + field.columns;
  return sameColumnNextRow >= totalSquares;
}

char intToChar(int i)
{
  char intAsChar[2];
  sprintf(intAsChar, "%d", i);
  return intAsChar[0];
}

bool previousColumnHasMine(const Field& field, int square)
{
  bool result = false;
  if( !isFirstColumn(field, square) )
  {
    result = hasMine(field, square - 1);
  }
  return result;
}

bool nextColumnHasMine(const Field& field, int square)
{
  bool result = false;

  if( !isLastColumn(field, square) )
  {
    result = hasMine(field, square + 1);
  }
  return result;
}

bool rowBelowHasMine(const Field& field, int square)
{
  bool result = false;

  if( !isLastRow(field, square) )
  {
    result = hasMine(field, square + field.columns);
  }
  return result;
}

int countNeighboringMines(const Field& field, int square)
{
  int neighboringMines = 0;
  if( nextColumnHasMine(field, square) ) ++neighboringMines;
  if( previousColumnHasMine(field, square) ) ++neighboringMines;
  if( rowBelowHasMine(field, square) ) ++neighboringMines;
  return neighboringMines;
}


void findMines( const Field& field,  char* result)
{
  for(int currentColumn = 0; currentColumn < field.columns; currentColumn++)
  {
    for(int currentRow = 0; currentRow < field.rows; currentRow++)
    {
      int currentSquare = ((currentRow+1) * (currentColumn+1)) - 1;

      if(field.spaces[currentSquare] == '*')
      {
        result[currentSquare] = '*';
      }
      else
      {
        int neighboringMines = countNeighboringMines(field, currentSquare);
        result[currentSquare] = intToChar( neighboringMines );
      }
    }
  }

  int lastSquare = field.columns * field.rows - 1;
  result[ lastSquare+1 ] = '\0';
}

void RUN_TESTS()
{
  char result[10000];

  {
    Field field(".", 1, 1);
    findMines(field, result);
    ASSERT_STREQ("0", result);
  }

  {
    Field field("*", 1, 1);
    findMines(field, result);
    ASSERT_STREQ("*", result);
  }

  {
    Field field("..", 1, 2);
    findMines(field, result);
    ASSERT_STREQ("00", result);
  }

  {
    Field field("**", 1, 2);
    findMines(field, result);
    ASSERT_STREQ("**", result);
  }

  {
    Field field(".*", 1, 2);
    findMines(field, result);
    ASSERT_STREQ("1*", result);
  }

  {
    Field field("*.", 1, 2);
    findMines(field, result);
    ASSERT_STREQ("*1", result);
  }

  {
    Field field("...", 1, 3);
    findMines(field, result);
    ASSERT_STREQ("000", result);
  }

  {
    Field field("***", 1, 3);
    findMines(field, result);
    ASSERT_STREQ("***", result);
  }

  {
    Field field("*..", 1, 3);
    findMines(field, result);
    ASSERT_STREQ("*10", result);
  }

  {
    Field field("..*", 1, 3);
    findMines(field, result);
    ASSERT_STREQ("01*", result);
  }

  {
    Field field(".*.", 1, 3);
    findMines(field, result);
    ASSERT_STREQ("1*1", result);
  }

  {
    Field field("*.*", 1, 3);
    findMines(field, result);
    ASSERT_STREQ("*2*", result);
  }

  {
    Field field("**.", 1, 3);
    findMines(field, result);
    ASSERT_STREQ("**1", result);
  }

  {
    Field field(".**", 1, 3);
    findMines(field, result);
    ASSERT_STREQ("1**", result);
  }

  // ROWS

  {
    Field field("..", 2, 1);
    findMines(field, result);
    ASSERT_STREQ("00", result);
  }

  {
    Field field("**", 2, 1);
    findMines(field, result);
    ASSERT_STREQ("**", result);
  }

  {
    Field field(".*", 2, 1);
    findMines(field, result);
    ASSERT_STREQ("1*", result);
  }

  {
    //Field field("*.", 2, 1);
    //findMines(field, result);
    //ASSERT_STREQ("*1", result);
  }

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
