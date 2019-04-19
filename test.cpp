#include "glue/midi.h"


int main()
{
	StatusCode InitStatus = Midi::Init();
	int SelectedPort = Midi::QueryUserForPort();
	StatusCode PortStatus = Midi::UsePort(SelectedPort);
	/*
	Midi::PlayNote(0, 60, 90, 1000); // Middle C
	Midi::PlayNote(0, 69, 90, 1000); // A 440
	*/
	Score::Builder Song = Score::Builder(4, 4, 144);

	Score::AddNote(Song, 69, 70, 4); // A4, velocity, quarter note
	Score::Advance(Song, 4);
	Score::AddNote(Song, 76, 85, 4); // E5, velocity, quarter note
	Score::Advance(Song, 4);
	Score::AddNote(Song, 72, 80, 4); // A4, velocity, quarter note
	Score::Advance(Song, 4);
	Score::AddNote(Song, 81, 80, 4); // C5, velocity, quarter note
	Score::AddNote(Song, 76, 90, 4); // E5, velocity, quarter note
	std::vector<int> Compiled = Score::Compile(Song);
	Score::Play(Compiled, 0);
}

