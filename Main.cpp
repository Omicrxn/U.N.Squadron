#include "Application.h"
#include "Globals.h"
#include "MemLeaks.h"

enum class main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = nullptr;

int main()
{
	ReportMemoryLeaks();

	int mainReturn = EXIT_FAILURE;
	main_states state = main_states::MAIN_CREATION;
	
	while (state != main_states::MAIN_EXIT)
	{
		switch (state)
		{
		case main_states::MAIN_CREATION:
		{
			LOG("Application Creation --------------\n");
			App = new Application();
			state = main_states::MAIN_START;
		}	break;

		case main_states::MAIN_START:
		{
			LOG("Application Start --------------\n");
			if (App->Init() == false)
			{
				LOG("Application Init exits with error -----\n");
				state = main_states::MAIN_EXIT;
			}
			else
			{
				state = main_states::MAIN_UPDATE;
			}
		}	break;

		case main_states::MAIN_UPDATE:
		{
			update_status status = App->Update();

			if (status == update_status::UPDATE_ERROR)
			{
				LOG("Application Update exits with error -----\n");
				state = main_states::MAIN_EXIT;
			}
			else if (status == update_status::UPDATE_STOP)
			{
				state = main_states::MAIN_FINISH;
			}
		}	break;

		case main_states::MAIN_FINISH:
		{
			LOG("Cleaning Game -----\n");
			if (App->CleanUp() == false)//if the cleanup gives an error the game is finished with errors
			{
				LOG("Game CleanUp returning with some errors -----");
			}
			else {
				mainReturn = EXIT_SUCCESS;
				state = main_states::MAIN_EXIT;
			}
		}	break;

		}
	}

	LOG("\nBye :)\n");

	delete App;
	return mainReturn;
}