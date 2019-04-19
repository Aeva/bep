#include "glue/midi.h"


int main()
{
	StatusCode InitStatus = Midi::Init();
	int SelectedPort = Midi::QueryUserForPort();
	StatusCode PortStatus = Midi::UsePort(SelectedPort);
	Midi::PlayNote(0, 60, 90, 1000); // Middle C
	Midi::PlayNote(0, 69, 90, 1000); // A 440
}

