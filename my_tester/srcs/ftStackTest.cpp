#include "mainTests.hpp"

/**************************************************************/
/*                     RUN FT::STACK TEST                     */
/**************************************************************/

TEST_F(ftStackTest, initSizeStack)
{
	std::cout << std::endl << BLUE_B;
	std::cout << "                                      ";
	std::cout << "           [ FT::STACK ] " << RESET << std::endl;

	initSizeStack(intEmptyStack, intFilledStack, intToInsertStack);
	initSizeStack(floatEmptyStack, floatFilledStack, floatToInsertStack);
	initSizeStack(doubleEmptyStack, doubleFilledStack, doubleToInsertStack);
	initSizeStack(stringEmptyStack, stringFilledStack, stringToInsertStack);

	initSizeStack(intListEmptyStack, intListFilledStack, intListToInsertStack);
	initSizeStack(stringListEmptyStack, stringListFilledStack, stringListToInsertStack);

	initSizeStack(intDequeEmptyStack, intDequeFilledStack, intDequeToInsertStack);
	initSizeStack(stringDequeEmptyStack, stringDequeFilledStack, stringDequeToInsertStack);
}
