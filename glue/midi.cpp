#include "midi.h"
#include <unistd.h>
#include <iostream>
#include <rtmidi/RtMidi.h>


int CurrentPort = -1;
RtMidiOut* MidiOut = nullptr;
std::vector<std::string> Ports;


StatusCode Midi::Init()
{
	if (MidiOut != nullptr)
	{
		std::cout << "Invalid call to initialize Midi interface.\n";
		return StatusCode::FAIL;
	}
	try
	{
		MidiOut = new RtMidiOut();
	}
	catch (RtMidiError &error)
	{
		error.printMessage();
		return StatusCode::FAIL;
	}
	int PortCount = MidiOut->getPortCount();
	if (PortCount == 0)
	{
		std::cout << "No MIDI output ports found.\n";
		return StatusCode::FAIL;
	}
	Ports.reserve(PortCount);
	for (int Port = 0; Port < PortCount; ++Port)
	{
		Ports.push_back(MidiOut->getPortName(Port));
	}
	return StatusCode::PASS;
}


StatusCode Midi::UsePort(int PortNumber)
{
	if (MidiOut != nullptr && CurrentPort == -1 && PortNumber > 0 && PortNumber < Ports.size())
	{
		try
		{
			MidiOut->openPort(PortNumber);
			CurrentPort = PortNumber;
			return StatusCode::PASS;
		}
		catch (RtMidiError &error)
		{
			error.printMessage();
		}
	}
	return StatusCode::FAIL;
}


std::vector<std::string>& GetPorts()
{
	return Ports;
}


int Midi::QueryUserForPort()
{
	int PortCount = Ports.size();
retry:
	std::cout << "Found the following MIDI output ports:\n";
	for (uint Port = 0; Port < PortCount; ++Port)
	{
		std::cout << " - (" << Port << ")\t" << Ports[Port] << "\n";
	}
	std::cout << "Which port should I use?\n > ";
	int Selected;
	std::cin >> Selected;
	if (Selected <0 && Selected >= PortCount)
	{
		std::cout << "\nInvalid port number: " << Selected << "\n\n";
		goto retry;
	}
	std::cout << "\nUsing port (" << Selected << ") " << Ports[Selected] << "\n";
	return Selected;
}


enum class Status : int
{
	NoteOff = 0x8,
	NoteOn = 0x9,
	PolyphonicPressure = 0xA,
	ControlChange = 0xB,
	ProgramChange = 0xC,
	ChannelPressure = 0xD,
	PitchBlend = 0xE,
	System = 0xF
};


const uint MessageMask = 0xF0;
const uint ChannelMask = 0x0F;
const uint DataMask = 0x7F;


template<Status Message, int DataByteCount>
inline void SendMessage(int Channel, int* Data)
{
	const int BufferSize = DataByteCount + 1;
	unsigned char Buffer[BufferSize];
	int StatusByte = ((static_cast<int>(Message) << 4) & MessageMask) | (Channel & ChannelMask);
	Buffer[0] = static_cast<unsigned char>(StatusByte);
	std::cout << "Sending Message: " << static_cast<int>(Buffer[0]);
	for (int i=0; i<DataByteCount; ++i)
	{
		Buffer[i+1] = static_cast<unsigned char>(Data[i] & DataMask);
		std::cout << " " << static_cast<int>(Buffer[i+1]);
	}
	std::cout << "\n";
	MidiOut->sendMessage(const_cast<const unsigned char*>(Buffer), BufferSize);

}


void Midi::SendNoteOn(int Channel, int Note, int Velocity)
{
	int Data[2] = { Note, Velocity };
	SendMessage<Status::NoteOn, 2>(Channel, Data);
}


void Midi::SendNoteOff(int Channel, int Note)
{
	int Data[2] = { Note, 0 };
	SendMessage<Status::NoteOn, 2>(Channel, Data);
}


void Midi::SendControlChange(int Channel, int Control, int NewValue)
{
	int Data[2] = { Control, NewValue };
	SendMessage<Status::ControlChange, 2>(Channel, Data);
}


void Midi::SendProgramChange(int Channel, int Program)
{
	int Data[1] = { Program };
	SendMessage<Status::ProgramChange, 1>(Channel, Data);
}


void Midi::PlayNote(int Channel, int Note, int Velocity, int Milliseconds)
{
	Midi::SendNoteOn(Channel, Note, Velocity);
	usleep(Milliseconds * 1000);
	Midi::SendNoteOff(Channel, Note);
}

