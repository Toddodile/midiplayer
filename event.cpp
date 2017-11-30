#include "event.hpp"

TempoEvent::TempoEvent(intmax_t t)
{
	tempo = t;
}

intmax_t TempoEvent::getTempo() const
{
	return tempo;
}



NoteEvent::NoteEvent(bool on, int8_t notenum, int8_t notevelocity)
{
	noteOn = on;
	if (notenum < 0)
	{
		notenum = 0;
	}
	else if (notenum > 127)
	{
		notenum = 127;
	}
	noteNumber = notenum;
	noteVelocity = notevelocity;
}

bool NoteEvent::getNoteOn()
{
	return noteOn;
}

int8_t NoteEvent::getNoteNumber() const
{
	return noteNumber;
}

int8_t NoteEvent::getNoteVelocity() const
{
	return noteVelocity;
}



MIDIEvent::MIDIEvent(TempoEvent e, intmax_t ticktime)
{
	event.tempoEvent = e;
	tickTime = ticktime;
	eventType = TEMPO;
}

MIDIEvent::MIDIEvent(EndTrackEvent e, intmax_t ticktime)
{
	event.endTrackEvent = e;
	tickTime = ticktime;
	eventType = END;
}

MIDIEvent::MIDIEvent(NoteEvent e, intmax_t ticktime)
{
	event.noteEvent = e;
	tickTime = ticktime;
	eventType = NOTE;
}

bool MIDIEvent::isTempoEvent() const
{
	return eventType == TEMPO;
}

bool MIDIEvent::isEndTrackEvent() const
{
	return eventType == END;
}

bool MIDIEvent::isNoteEvent() const
{
	return eventType == NOTE;
}

intmax_t MIDIEvent::getTickTime() const
{
	return tickTime;
}

TempoEvent MIDIEvent::asTempoEvent() const
{
	return event.tempoEvent;
}

NoteEvent MIDIEvent::asNoteEvent() const
{
	return event.noteEvent;
}