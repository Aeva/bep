#pragma once
#include <vector>
#include <string>
#include "errors.h"


namespace Midi
{
	StatusCode Init();
	std::vector<std::string>& GetPorts();
	StatusCode UsePort(int PortNumber);
	int QueryUserForPort();

	void SendNoteOn(int Channel, int Note, int Velocity);
	void SendNoteOff(int Channel, int Note);
	void SendControlChange(int Channel, int Control, int NewValue);
	void SendProgramChange(int Channel, int Program);
	void PlayNote(int Channel, int Note, int Velocity, int Milliseconds);
}

