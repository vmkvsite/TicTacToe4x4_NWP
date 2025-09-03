#include "pch.h"
#include "CppUnitTest.h"
#include "../TicTacToe4x4_GUI/Game.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TicTacToeGUITests
{
    TEST_CLASS(TicTacToeGUIUnitTests)
    {
    public:

        TEST_METHOD(TestValidMove)
        {
            Game game;
            bool result = game.makeMove(1, 1);
            Assert::IsTrue(result);
        }

        TEST_METHOD(TestInvalidMove)
        {
            Game game;
            game.makeMove(1, 1);
            bool result = game.makeMove(1, 1);
            Assert::IsFalse(result);
        }

        TEST_METHOD(TestOutOfBoundsMove)
        {
            Game game;
            bool result = game.makeMove(0, 1);
            Assert::IsFalse(result);
        }

        TEST_METHOD(TestHorizontalWin)
        {
            Game game;
            game.makeMove(1, 1); game.makeMove(2, 1);
            game.makeMove(1, 2); game.makeMove(2, 2);
            game.makeMove(1, 3); game.makeMove(2, 3);
            game.makeMove(1, 4);
            Assert::IsTrue(game.isGameEnded());
            Assert::AreEqual('X', game.getWinner());
        }

        TEST_METHOD(TestVerticalWin)
        {
            Game game;
            game.makeMove(2, 2); game.makeMove(1, 1);
            game.makeMove(2, 3); game.makeMove(2, 1);
            game.makeMove(2, 4); game.makeMove(3, 1);
            game.makeMove(3, 2); game.makeMove(4, 1);
            Assert::IsTrue(game.isGameEnded());
            Assert::AreEqual('O', game.getWinner());
        }

        TEST_METHOD(TestDiagonalWin)
        {
            Game game;
            game.makeMove(1, 1); game.makeMove(1, 2);
            game.makeMove(2, 2); game.makeMove(1, 3);
            game.makeMove(3, 3); game.makeMove(1, 4);
            game.makeMove(4, 4);
            Assert::IsTrue(game.isGameEnded());
            Assert::AreEqual('X', game.getWinner());
        }

        TEST_METHOD(TestAntiDiagonalWin)
        {
            Game game;
            game.makeMove(2, 1); game.makeMove(1, 4);
            game.makeMove(2, 2); game.makeMove(2, 3);
            game.makeMove(2, 4); game.makeMove(3, 2);
            game.makeMove(3, 1); game.makeMove(4, 1);
            Assert::IsTrue(game.isGameEnded());
            Assert::AreEqual('O', game.getWinner());
        }

        TEST_METHOD(TestScoreIncrement)
        {
            Game game;
            game.makeMove(1, 1); game.makeMove(2, 1);
            game.makeMove(1, 2); game.makeMove(2, 2);
            game.makeMove(1, 3); game.makeMove(2, 3);
            game.makeMove(1, 4);
            Assert::AreEqual(1, game.getXWins());
            Assert::AreEqual(0, game.getOWins());
        }

        TEST_METHOD(TestInfiniteModeToggle)
        {
            Game game;
            Assert::IsFalse(game.isInfiniteMode());
            game.toggleInfiniteMode();
            Assert::IsTrue(game.isInfiniteMode());
        }

        TEST_METHOD(TestInfiniteModeRemoveOldest)
        {
            Game game;
            game.toggleInfiniteMode();

            game.makeMove(1, 1); game.makeMove(1, 2);
            game.makeMove(3, 1); game.makeMove(2, 2);
            game.makeMove(2, 3); game.makeMove(3, 2);
            game.makeMove(1, 4); game.makeMove(2, 4);

            game.makeMove(4, 4);

            Assert::AreEqual(' ', game.getSymbolAt(1, 1));
            Assert::AreEqual('X', game.getSymbolAt(4, 4));
        }
    };
}