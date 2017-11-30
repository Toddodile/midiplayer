#include "track.hpp"

Track::Track(intmax_t ticks_per_beat)
{
	ticksPerBeat = ticks_per_beat;
}

intmax_t Track::getTicksPerBeat() const
{
	return ticksPerBeat;
}

Track::ConstIteratorType Track::begin() const
{
	return events.begin();
}

Track::ConstIteratorType Track::end() const
{
	return events.end();
}

void Track::addEndEvent(intmax_t tick_time)
{
	events.push_back(MIDIEvent(EndTrackEvent(), tick_time));
}

void Track::addTempoEvent(intmax_t tempo, intmax_t tick_time)
{
	events.push_back(MIDIEvent(TempoEvent(tempo), tick_time));
}

void Track::addNoteEvent(uint8_t code, uint8_t data1, uint8_t data2, intmax_t tick_time)
{
	bool noteOn;
	if (code == 8 || data2 == 0)
	{
		noteOn = false;
	}
	else
	{
		noteOn = true;
	}
	events.push_back(MIDIEvent(NoteEvent(noteOn, data1, data2), tick_time));
}