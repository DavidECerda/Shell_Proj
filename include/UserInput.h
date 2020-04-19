#include <string>
#include <vector>
#include <iostream>
#include "StringUtils.h"

// Allow keyboard manipulation of input in real time

using TFuncPointer = std::string(*)(void *type);

enum specialKeys {NONE = 0,
						CHAR = 1,
						TAB = '\t',
						ENTER = '\n',
						CARRIAGE_RETURN = '\r',
						ECS = 27,
						LEFT_BRACKET = '[',
						UP = 'A',
						DOWN = 'B',
						RIGHT = 'C',
						LEFT = 'D',
						BACKSPACE = '\b',
						DEL = 127};



class UserInput{
    protected:
        size_t index;
        size_t pastIndex;
		specialKeys state;
        std::vector <std::string> pastInputs;
		//std::vector <std::string> commands;
        std::string currentInput;
        std::string originalInput;
        bool stillOriginalInput;



		void AddPastInput();
        void DownKey();
        void UpKey();
        void LeftKey();
        void RightKey();
        void BackspaceKey();
        void CharacterKey(const char c);
		void TabKey();
		//void TabHanlder(std::vector <std::string> &commands, std::vector <std::string(*const fcnptr)()> &handlers);
		//void TabHanlder(std::vector <std::string> &commands, std::vector <TFuncPointer> &handlers);
		void EnterKey();
		std::string GetPastInput();

    public:

        UserInput();
		UserInput(std::string &input);
		UserInput(UserInput &oldInput);
        ~UserInput();
        std::string ReadingInput();
		std::string ReturnInput();
        operator std::string();

};