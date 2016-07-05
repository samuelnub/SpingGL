#include <iostream>
#include <initializer/windower.h>

//SDL2 has this lil' hitch where it #defines main in its SDL_main.h, so you need to provide the argc and args[] flags in your actual main function to override it, thanks, SDL, ya bitch
int main(int argc, char *args[])
{
	std::cout << "ayy\n";

	try
	{
		Window window;
	}
	catch (const char *error)
	{
		std::cout << "Bumped into an exception!\n" << error << "\n";
	}
	
	std::cout << "lmao\n";
	return 0;
}