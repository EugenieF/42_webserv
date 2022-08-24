#include "../mainTests.hpp"

/**************************************************************/
/*                 	   	   STACK TEST                         */
/**************************************************************/

template <typename T>
void	initSizeStack(T emptyStack, T filledStack, T toInsertStack)
{
	EXPECT_EQ(emptyStack.size(), (size_t)0);
	EXPECT_EQ(filledStack.size(), (size_t)10);
	EXPECT_EQ(toInsertStack.size(), (size_t)5);
}

template <typename T>
void	isEmptyStack(T emptyStack, T filledStack, T toInsertStack)
{
	EXPECT_EQ(emptyStack.empty(), true);
	EXPECT_EQ(filledStack.empty(), false);
	EXPECT_EQ(toInsertStack.empty(), false);
}

template <typename T, typename U>
void	pushInEmptyStack(T emptyStack, U varX)
{
	for (int i = 0; i < 10; i++)
	{
		emptyStack.push(varX[i]);
		EXPECT_EQ(emptyStack.top(), varX[i]);
	}
}

template <typename T, typename U>
void	popOneInFilledStack(T filledStack, U varX)
{
	filledStack.pop();
	EXPECT_EQ(filledStack.top(), varX[8]);
	EXPECT_EQ(filledStack.size(), (size_t)9);
}

template <typename T, typename U>
void	constructorStack(T emptyStack, T filledStack, T toInsertStack, U varX, U varY)
{
	T	newEmptyStack;
	EXPECT_EQ(newEmptyStack.empty(), true);
	EXPECT_EQ(emptyStack == newEmptyStack, true);

	T	copyEmptyStack(emptyStack);
	EXPECT_EQ(copyEmptyStack.empty(), true);
	EXPECT_EQ(emptyStack == copyEmptyStack, true);

	T	copyFilledStack(filledStack);
	EXPECT_EQ(copyFilledStack.size(), (size_t)10);
	EXPECT_EQ(copyFilledStack.top(), varX[9]);
	EXPECT_EQ(filledStack == copyFilledStack, true);

	T	copyToInsertStack(toInsertStack);
	EXPECT_EQ(copyToInsertStack.size(), (size_t)5);
	EXPECT_EQ(copyToInsertStack.top(), varY[4]);
	EXPECT_EQ(toInsertStack == copyToInsertStack, true);
}

template <typename T>
void	operatorEgalStack(T emptyStack, T filledStack, T toInsertStack)
{
	T newStack;

	newStack = filledStack;
	EXPECT_EQ(newStack.empty(), false);
	EXPECT_EQ(newStack == filledStack, true);

	newStack = emptyStack;
	EXPECT_EQ(newStack.empty(), true);
	EXPECT_EQ(newStack == filledStack, false);
	EXPECT_EQ(newStack == emptyStack, true);

	newStack = toInsertStack;
	EXPECT_EQ(newStack.empty(), false);
	EXPECT_EQ(newStack == emptyStack, false);
	EXPECT_EQ(newStack == toInsertStack, true);
}

template <typename T>
void	operatorEqualStack(T emptyStack, T filledStack, T toInsertStack)
{
	T	copyFilledStack(filledStack);

	EXPECT_EQ(emptyStack == emptyStack, true);
	EXPECT_EQ(emptyStack == filledStack, false);
	EXPECT_EQ(emptyStack == toInsertStack, false);
	EXPECT_EQ(filledStack == toInsertStack, false);

	EXPECT_EQ(filledStack == copyFilledStack, true);
	EXPECT_EQ(copyFilledStack == filledStack, true);
	copyFilledStack.pop();
	EXPECT_EQ(filledStack == copyFilledStack, false);
	EXPECT_EQ(copyFilledStack == filledStack, false);
}

template <typename T>
void	operatorNotEqualStack(T emptyStack, T filledStack, T toInsertStack)
{
	T	copyFilledStack(filledStack);

	EXPECT_EQ(emptyStack != emptyStack, false);
	EXPECT_EQ(emptyStack != filledStack, true);
	EXPECT_EQ(emptyStack != toInsertStack, true);
	EXPECT_EQ(filledStack != toInsertStack, true);

	EXPECT_EQ(filledStack != copyFilledStack, false);
	EXPECT_EQ(copyFilledStack != filledStack, false);
	copyFilledStack.pop();
	EXPECT_EQ(filledStack != copyFilledStack, true);
	EXPECT_EQ(copyFilledStack != filledStack, true);
}

template <typename T>
void	operatorSupEqualStack(T emptyStack, T filledStack, T toInsertStack)
{
	T	copyFilledStack(filledStack);

	EXPECT_EQ(emptyStack >= emptyStack, true);
	EXPECT_EQ(emptyStack >= filledStack, false);
	EXPECT_EQ(emptyStack >= toInsertStack, false);
	EXPECT_EQ(filledStack >= toInsertStack, false);
	EXPECT_EQ(filledStack >= emptyStack, true);
	EXPECT_EQ(toInsertStack >= emptyStack, true);
	EXPECT_EQ(toInsertStack >= filledStack, true);

	EXPECT_EQ(filledStack >= copyFilledStack, true);
	EXPECT_EQ(copyFilledStack >= filledStack, true);
	copyFilledStack.pop();
	EXPECT_EQ(filledStack >= copyFilledStack, true);
	EXPECT_EQ(copyFilledStack >= filledStack, false);
	filledStack.pop();
	filledStack.pop();
	EXPECT_EQ(filledStack >= copyFilledStack, false);
	EXPECT_EQ(copyFilledStack >= filledStack, true);
}

template <typename T>
void	operatorSupStack(T emptyStack, T filledStack, T toInsertStack)
{
	T	copyFilledStack(filledStack);

	EXPECT_EQ(emptyStack > emptyStack, false);
	EXPECT_EQ(emptyStack > filledStack, false);
	EXPECT_EQ(emptyStack > toInsertStack, false);
	EXPECT_EQ(filledStack > toInsertStack, false);
	EXPECT_EQ(filledStack > emptyStack, true);
	EXPECT_EQ(toInsertStack > emptyStack, true);
	EXPECT_EQ(toInsertStack > filledStack, true);

	EXPECT_EQ(filledStack > copyFilledStack, false);
	EXPECT_EQ(copyFilledStack > filledStack, false);
	copyFilledStack.pop();
	EXPECT_EQ(filledStack > copyFilledStack, true);
	EXPECT_EQ(copyFilledStack > filledStack, false);
	filledStack.pop();
	filledStack.pop();
	EXPECT_EQ(filledStack > copyFilledStack, false);
	EXPECT_EQ(copyFilledStack > filledStack, true);
}

template <typename T>
void	operatorInfEqualStack(T emptyStack, T filledStack, T toInsertStack)
{
	T	copyFilledStack(filledStack);

	EXPECT_EQ(emptyStack <= emptyStack, true);
	EXPECT_EQ(emptyStack <= filledStack, true);
	EXPECT_EQ(emptyStack <= toInsertStack, true);
	EXPECT_EQ(filledStack <= toInsertStack, true);
	EXPECT_EQ(filledStack <= emptyStack, false);
	EXPECT_EQ(toInsertStack <= emptyStack, false);
	EXPECT_EQ(toInsertStack <= filledStack, false);

	EXPECT_EQ(filledStack <= copyFilledStack, true);
	EXPECT_EQ(copyFilledStack <= filledStack, true);
	copyFilledStack.pop();
	EXPECT_EQ(filledStack <= copyFilledStack, false);
	EXPECT_EQ(copyFilledStack <= filledStack, true);
	filledStack.pop();
	filledStack.pop();
	EXPECT_EQ(filledStack <= copyFilledStack, true);
	EXPECT_EQ(copyFilledStack <= filledStack, false);
}

template <typename T>
void	operatorInfStack(T emptyStack, T filledStack, T toInsertStack)
{
	T	copyFilledStack(filledStack);
	T	copyBisFilledStack(filledStack);

	EXPECT_EQ(emptyStack < emptyStack, false);
	EXPECT_EQ(emptyStack < filledStack, true);
	EXPECT_EQ(emptyStack < toInsertStack, true);
	EXPECT_EQ(filledStack < toInsertStack, true);
	EXPECT_EQ(filledStack < emptyStack, false);
	EXPECT_EQ(toInsertStack < emptyStack, false);
	EXPECT_EQ(toInsertStack < filledStack, false);

	EXPECT_EQ(filledStack < copyFilledStack, false);
	EXPECT_EQ(copyFilledStack < filledStack, false);
	copyFilledStack.pop();
	EXPECT_EQ(filledStack < copyFilledStack, false);
	EXPECT_EQ(copyFilledStack < filledStack, true);
	filledStack.pop();
	filledStack.pop();
	EXPECT_EQ(filledStack < copyFilledStack, true);
	EXPECT_EQ(copyFilledStack < filledStack, false);
}

template <typename T, typename U>
void	topStack(T filledStack, T toInsertStack, U varX, U varY)
{
	for (int i = 9; i >= 0; i--)
	{
		EXPECT_EQ(filledStack.top(), varX[i]);
		filledStack.pop();
	}
	for (int i = 4; i >= 0; i--)
	{
		EXPECT_EQ(toInsertStack.top(), varY[i]);
		toInsertStack.pop();
	}
}
