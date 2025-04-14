#include "testlib.h"
#include "./libs/fixed_int.hpp"

auto main(int argc, char *argv[]) -> int {
    registerTestlibCmd(argc, argv);

    auto userLen = ouf.readInt();
    auto ansLen = ans.readInt();

    auto lenCorrect = (userLen == ansLen);
    if (not lenCorrect) {
        if (userLen > ansLen) {
            quitf(_wa, "Output is longer than answer.");
        } else {
            quitf(_wa, "Output is shorter than answer.");
        }
    } else {
        struct EOFError {};
        auto getc = [&](InStream &is) -> char {
            auto ch = is.readChar();
            if (ch == char(EOFC)) {
                throw EOFError{};
            } else {
                return ch;
            }
        };

        auto isLegal = [&](char ch) -> bool {
            return ch == '(' or ch == ')';
        };

        try {
            char userCh, ansCh;

            for (userCh = getc(ouf); not isLegal(userCh); userCh = getc(ouf)) std::cout << int(userCh) << "\n";
            for (ansCh = getc(ans); not isLegal(ansCh); ansCh = getc(ans));

            std::string u, a;
            for (; userCh != char(EOFC) and isLegal(userCh); userCh = ouf.readChar()) u += userCh;
            for (; ansCh != char(EOFC) and isLegal(ansCh); ansCh = ans.readChar()) a += ansCh;

            std::cout << u << "\n" << a << "\n";
            if (u == a) {
                quitf(_ok, "Answer is correct.");
            } else {
                quitp(0.4, "Bracket sequence is not correct.");
            }
        } catch (EOFError &) {
            quitp(0.4, "Bracket sequence is not correct.");
        }
    }
}
