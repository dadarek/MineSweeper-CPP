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

char intToChar(int i)
{
  char intAsChar[2];
  sprintf(intAsChar, "%d", i);
  return intAsChar[0];
}

class Field
{
  char spaces_[10000];
  int rows_;
  int columns_;

  public:
  Field( const char* spaces, int rows, int columns )
    : rows_    ( rows    )
    , columns_ ( columns )
  {
    strcpy( this->spaces_, spaces );
  }

  int rows() const { return rows_; }
  int columns() const { return columns_; }
  int totalSquares() const { return rows_ * columns_; }

  bool hasMine( int square ) const
  {
    return this->spaces_[ square ] == '*';
  }

  bool isFirstColumn( int square ) const
  {
    return square % columns_ == 0;
  }
 
  bool isLastColumn( int square ) const
  {
    return isFirstColumn( square+1 );
  }

  bool isLastRow( int square ) const
  {
    int sameColumnNextRow = square + columns_;
    return sameColumnNextRow >= totalSquares();
  }

  bool isFirstRow( int square ) const
  {
    int sameColumnPrevRow = square - columns_;
    return sameColumnPrevRow < 0;
  }

  bool previousColumnHasMine( int square ) const
  {
    bool result = false;
    if( !isFirstColumn( square ) )
    {
      result = hasMine( square - 1 );
    }
    return result;
  }

  bool nextColumnHasMine( int square ) const
  {
    bool result = false;
    if( !isLastColumn( square ) )
    {
      result = hasMine( square + 1 );
    }
    return result;
  }

  bool rowBelowHasMine( int square ) const
  {
    bool result = false;
    if( !isLastRow( square ) )
    {
      result = hasMine( square + columns_ );
    }
    return result;
  }

  bool rowAboveHasMine( int square ) const
  {
    bool result = false;
    if( !isFirstRow( square ) )
    {
      result = hasMine( square - columns_ );
    }
    return result;
  }

  bool topLeftHasMine( int square ) const
  {

    bool result = false;
    if( !isFirstRow( square ) && !isFirstColumn( square ) )
    {
      result = hasMine( square - columns_ - 1 );
    }
    return result;
  }

  bool topRightHasMine( int square ) const
  {

    bool result = false;
    if( !isFirstRow( square ) && !isLastColumn( square ) )
    {
      result = hasMine( square - columns_ + 1 );
    }
    return result;
  }

  bool bottomLeftHasMine( int square ) const
  {

    bool result = false;
    if( !isLastRow( square ) && !isFirstColumn( square ) )
    {
      result = hasMine( square + columns_ - 1 );
    }
    return result;
  }

  bool bottomRightHasMine( int square ) const
  {

    bool result = false;
    if( !isLastRow( square ) && !isLastColumn( square ) )
    {
      result = hasMine( square + columns_ + 1 );
    }
    return result;
  }

  int countNeighboringMines( int square ) const
  {
    int neighboringMines = 0;
    if( nextColumnHasMine    ( square ) ) ++neighboringMines;
    if( previousColumnHasMine( square ) ) ++neighboringMines;
    if( rowBelowHasMine      ( square ) ) ++neighboringMines;
    if( rowAboveHasMine      ( square ) ) ++neighboringMines;
    if( topLeftHasMine       ( square ) ) ++neighboringMines;
    if( topRightHasMine      ( square ) ) ++neighboringMines;
    if( bottomLeftHasMine    ( square ) ) ++neighboringMines;
    if( bottomRightHasMine   ( square ) ) ++neighboringMines;
    return neighboringMines;
  }
};

void findMines( const Field& field,  char* result)
{
  int currentSquare = 0;
  for(int currentColumn = 0; currentColumn < field.columns(); currentColumn++)
  {
    for(int currentRow = 0; currentRow < field.rows(); currentRow++)
    {
      if(field.hasMine( currentSquare ) )
      {
        result[currentSquare] = '*';
      }
      else
      {
        int neighboringMines = field.countNeighboringMines( currentSquare );
        result[ currentSquare ] = intToChar( neighboringMines );
      }
      ++currentSquare;
    }
  }

  int lastSquare = field.columns() * field.rows() - 1;
  result[ lastSquare+1 ] = '\0';
}

void TestScenario(const char* mines, int rows, int columns, const char* expectedResult)
{
  char result[10000];

  Field field( mines, rows, columns );
  findMines( field, result );
  ASSERT_STREQ( expectedResult, result );
}

void RUN_TESTS()
{
  char result[10000];
  // TODO: How do you get the ASSERT_EQ to print the line of the originating test?
  // Would you need a macro test def?

  TestScenario(".", 1, 1, "0");
  TestScenario("*", 1, 1, "*");

  // COLUMNS
  TestScenario("..", 1, 2, "00");
  TestScenario("**", 1, 2, "**");

  TestScenario(".*", 1, 2, "1*");
  TestScenario("*.", 1, 2, "*1");

  TestScenario("...", 1, 3, "000");
  TestScenario("***", 1, 3, "***");

  TestScenario("*..", 1, 3, "*10");
  TestScenario("..*", 1, 3, "01*");
  TestScenario(".*.", 1, 3, "1*1");

  TestScenario("*.*", 1, 3, "*2*");
  TestScenario("**.", 1, 3, "**1");
  TestScenario(".**", 1, 3, "1**");

  // ROWS
  TestScenario("..", 2, 1, "00");
  TestScenario("**", 2, 1, "**");

  TestScenario(".*", 2, 1, "1*");
  TestScenario("*.", 2, 1, "*1");

  TestScenario("...", 3, 1, "000");
  TestScenario("***", 3, 1, "***");

  TestScenario("*..", 3, 1, "*10");
  TestScenario(".*.", 3, 1, "1*1");
  TestScenario("..*", 3, 1, "01*");

  TestScenario("**.", 3, 1, "**1");
  TestScenario(".**", 3, 1, "1**");
  TestScenario("*.*", 3, 1, "*2*");

  // CORNERS
  TestScenario("....", 2, 2, "0000");
  TestScenario("****", 2, 2, "****");

  TestScenario("*...", 2, 2, "*111");
  TestScenario(".*..", 2, 2, "1*11");
  TestScenario("..*.", 2, 2, "11*1");
  TestScenario("...*", 2, 2, "111*");

  TestScenario("**..", 2, 2, "**22");
  TestScenario(".**.", 2, 2, "2**2");
  TestScenario("..**", 2, 2, "22**");

  // 9 x 9
  TestScenario("****.****", 3, 3, "****8****");

  // EXAMPLES FROM BOOK
  TestScenario("*........*......", 4, 4, "*10022101*101110");
  TestScenario("**.........*....", 3, 5, "**100332001*100");

  // JUST FOR GIGGLES
  // *...*      *222*
  // .*.*. ==== 2*3*2
  // ..*..      23*32
  // .*.*. ==== 2*3*2
  // *...*      *222*
  TestScenario("*...*.*.*...*...*.*.*...*", 5, 5, "*222*2*3*223*322*3*2*222*");

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
