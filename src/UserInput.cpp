#include "UserInput.h"


///--------------------------------------------------------------------------
/// @brief	The class handles retrieval of user input one character at time
/// 		and performing manipulations based on a key press like a unix shell.
///			Right and left key press move the cursor, up and down retrieve previous
///			inputs, backspace deletes, and enter returns the input to the callng program.
///			Tab key needs to given a list of commands that the user wants to auto fill.
/// 		
/// 
/// Assumptions: 
/// 
/// Limitations: 
///
/// @param
///---------------------------------------------------------------------------


// Constructor for UserInput with no parameters. Initializes index and pastIndex to zero. 
UserInput::UserInput(){
    index = 0;
    pastIndex = 0;
    state = specialKeys::NONE;
    stillOriginalInput = true;
}

// Constructor for UserInput fro a string. Index initialized to size of input string.
UserInput::UserInput(std::string &input){
	originalInput = input;
    index = input.size();
    pastIndex = 0;
    state = specialKeys::NONE;
    stillOriginalInput = true;
}

UserInput::UserInput(UserInput &oldInput){
	originalInput = oldInput.originalInput;
	index = oldInput.index;
	pastIndex = oldInput.pastIndex;
	state = oldInput.state;
	stillOriginalInput = oldInput.stillOriginalInput;
}

// Destructor
UserInput::~UserInput(){

}

// Main function of UserInput class, reads incoming user input 
//and calls corresponding function to handle special cases for user input.

std::string UserInput::ReadingInput(){
	char c,d,e;
	
	while (state != specialKeys::ENTER){
		std::cin.get(c);

		if(c == specialKeys::ECS){
			std::cin.get(d);

			if (d == specialKeys::LEFT_BRACKET){
				std::cin.get(e);
				switch(e) {
					case specialKeys::LEFT : LeftKey(); break;
					case specialKeys::RIGHT : RightKey(); break;
					case specialKeys::UP : UpKey(); break;
					case specialKeys::DOWN : DownKey(); break;
					default : break;
				}
			}
			continue;

		}
		else{
			switch(c) {
				case specialKeys::TAB : TabKey(); break;
				case specialKeys::BACKSPACE : BackspaceKey(); break;
				case specialKeys::DEL : BackspaceKey(); break;
				case specialKeys::ENTER : EnterKey(); break;
				case specialKeys::CARRIAGE_RETURN : EnterKey(); break;
				default : CharacterKey(c); break;
			}

		}
	}

	state = specialKeys::NONE;
	return ReturnInput();
}

// Function to add past inputs into the past input vector.
void UserInput::AddPastInput(){
    pastInputs.push_back(currentInput);
	pastIndex = pastInputs.size()-1;
	return;
}

// Function to retrieve past inputs based on state.
// If the state is UP key, it will get the next older previous input
// If the statis is DOWN key, it will get the next newer previous input
std::string UserInput::GetPastInput(){
	std::string tempInput;

	if (state == specialKeys::UP){
		tempInput = pastInputs[pastIndex--];
	}
	else if (state == specialKeys::DOWN){
		tempInput = pastInputs[++pastIndex];
	}
	else{
		tempInput = currentInput;
	}

	return tempInput;
}

//Operator over load to convert UserInput to string
UserInput::operator std::string(){
	return currentInput;
}

// Returns current input and resets the input to an empty string
std::string UserInput::ReturnInput(){
	std::string temp = currentInput;
	currentInput = "";
	originalInput = "";
	return temp;
}


// Function that handles the UP key user input
// If the past index is not at 0 (i.e there are more past inputs)
	// It will erase the current input and print the retrieved past input.
	// It also will store the original input, if we are on the original input.
// If the pastIndex is at 0 and there have been previous past inputs 
	// It will get the past input, but will not decrement the past input index.
void UserInput::UpKey(){

	state = specialKeys::UP;

    if(pastIndex != 0){
        int i = currentInput.size();
        for(int j = index; j < i; j++){
            std::cout<<" ";
        }
        while(i>0){
            i--;
            std::cout<<"\b \b";
        }
        
        if (stillOriginalInput){
            originalInput = currentInput;
            stillOriginalInput = false;
        }
		currentInput = GetPastInput();
		index = currentInput.size();
        std::cout<<currentInput;
    }

    else if(pastIndex == 0 && pastInputs.size() != 0){
        int i = currentInput.size();
        for(int j = index; j < i; j++){
            std::cout<<" ";
        }

        while(i>0){
            i--;
            std::cout<<"\b \b";
        }

        if (stillOriginalInput){
            originalInput = currentInput;
            stillOriginalInput = false;
        }
        currentInput = pastInputs[0];
        index = currentInput.size();
        std::cout<<currentInput;
    }
	
    return;
}

// Function to handle a down key input
// If there have been inputs or the past input index is not zero
	//It will erase the current input, retrieve a newer preivous input, and print it.
//If the past input index is at the last element of the vector
	// It will erase the current input, retrieve the original input, and print it.
	// Then it will set the original input flag to true.
void UserInput::DownKey(){
	state = specialKeys::DOWN;

    if(!(pastIndex == 0 && pastInputs.size() == 0)){
        if(pastIndex < pastInputs.size()-1){
            int i = currentInput.size();
            for(int j = index; j < i; j++){
                std::cout<<" ";
            }
            while(i>0){
                i--;
                std::cout<<"\b \b";
            }
            currentInput = GetPastInput();
			index = currentInput.size();
            std::cout<<currentInput;
        }
        else if (pastIndex == pastInputs.size()-1){
            if (currentInput != originalInput){
                int i = currentInput.size();
                for(int j = index; j < i; j++){
                    std::cout<<" ";
                }
                while(i>0){
                    i--;
                    std::cout<<"\b \b";
                }
                currentInput = originalInput;
                index = currentInput.size();

                std::cout<<currentInput;
            }
            stillOriginalInput = true;
        }
    }

    return;
}

// Function to handle a right key press.
// Simply moves cursor forward one while,
// not at the end of input and increments index
void UserInput::RightKey(){
	state = specialKeys::RIGHT;

    if (index < currentInput.size()){
		std::cout<<currentInput[index++];
	}
	
    return;
}

// Function to handle a left key press.
// Simply moves cursor back one while,
// not at the beginning of input and 
// increments index
void UserInput::LeftKey() {
	state = specialKeys::LEFT;

    if(index != 0){
		index --;
		std::cout<<"\b";
    }

	
    return;
}

// Function to handle tab key press
// Still needs implementation
void UserInput::TabKey(){
	
	state = specialKeys::TAB;
	/*
	std::string temp = currentInput;

	for (auto command : commands){
		bool match = true;

		for(int i = 0; i < std::min(command.length(),temp.length()); i ++){

			if (temp[i] != command[i]){
				match = false;
				break;
			}

		}

		if(match){
			temp = command;
			break;
		}
	}

	int i = currentInput.size();
	for(int j = index; j < i; j++){
		std::cout<<" ";
	}
	while(i>0){
		i--;
		std::cout<<"\b \b";
	}


	currentInput = temp;
	index = currentInput.size();
	std::cout<<currentInput;
	*/
	return;
}

// Function to handle enter key press.
// Resets index, prints new line, adds input to past input.
void UserInput::EnterKey(){
	state = specialKeys::ENTER;
	index = 0;
	//currentInput = StringUtils::Strip(currentInput);
	if (currentInput.empty()){
		std::cout<<"\n";
	}
	else{
		AddPastInput();
		std::cout<<'\n';
	}
	
	return;

}

// Function to handle a backspace key press.
// If the input is not empty, it will erase
// one letter and move the index back one.
void UserInput::BackspaceKey(){
	state = specialKeys::BACKSPACE;

    if(!currentInput.empty()){

        if(index == currentInput.size()){
            currentInput.pop_back();
            index --;
            std::cout<<"\b \b";
        }else if (index != 0){
            std::cout<<"\b \b";
            std::cout<<currentInput.substr(index)<<" \b\b";
            for(unsigned int i = index ; i < currentInput.size() - 1; i ++){
                std::cout<<"\b";
            }
            currentInput.erase(--index,1);
        }
	}
    return;
    
}

// Function to handle a character key
// It will print the character and save it to input
// where the index for the current input is.
void UserInput::CharacterKey(const char c){
	state = specialKeys::CHAR;

    if(index == currentInput.size() || (index == 0 && currentInput.size()==0)){
				currentInput += c;
				index ++;
				std::cout<<c;
	}

    else{
        try{
            auto pos = currentInput.begin()+(index);
            std::cout<<c<<currentInput.substr(index);
            for(unsigned int i = index ; i < currentInput.size(); i ++){
                std::cout<<"\b";
            }
            currentInput.insert(pos,c);
            index++;
        }
        catch(const std::out_of_range& oor){
            std::cerr <<"Out of Range error: "<<oor.what()<<'\n';
        }
    }

    return;
}
