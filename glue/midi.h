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


namespace Score
{
	struct NoteEvent
	{
		int Note;
		int Velocity;
		float Time;
	};

	struct Builder
	{
		float BeatsPerBar;
		float BeatUnit;
		float MsPerBeat;
		float Cursor;
		std::vector<NoteEvent> Starts;
		std::vector<NoteEvent> Stops;
		Builder(float InBeatsPerBar=4, float InBeatUnit=4, float BPM=144);
	};

	void AddNote(Builder& Score, int Note, int Velocity, float Size);
	void Advance(Builder& Score, float Size);
	std::vector<int> Compile(Builder& Score);
	void Play(std::vector<int>& CompiledScore, int Channel);
}
