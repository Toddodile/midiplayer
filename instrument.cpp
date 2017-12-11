#define _USE_MATH_DEFINES
#include "instrument.hpp"
#include <cmath>
#include <math.h>

DefaultInstrument::DefaultInstrument(const Track & track)
{
	currentTrack = track;
	stopped = false;
	time = 0;
	trackIterator = track.begin();
	endOfTrack = track.end();
	stopped = (trackIterator == track.end());
	notesOn = std::vector<NoteData>(128);
	tempo = 500000;
}

void DefaultInstrument::reset(const Track & track)
{
	currentTrack = track;
	stopped = false;
	time = 0;
	trackIterator = currentTrack.begin();
	endOfTrack = currentTrack.end();
	stopped = (trackIterator == endOfTrack);
	notesOn = std::vector<NoteData>(128);
	tempo = 500000;
}

bool DefaultInstrument::halted()
{
	return stopped;
}

double DefaultInstrument::sample(double deltaT)
{
	time += deltaT;
	while (trackIterator != endOfTrack && convertToTime((*trackIterator).getTickTime()) <= time)
	{
		if ((*trackIterator).isEndTrackEvent())
		{
			stopped = true;
			break;
		}
		else if ((*trackIterator).isTempoEvent())
		{
			tempo = (*trackIterator).asTempoEvent().getTempo();
		}
		else
		{
			NoteEvent event = (*trackIterator).asNoteEvent();
			if (event.getNoteOn() && notesOn[event.getNoteNumber()].playable)
			{
				notesOn[event.getNoteNumber()].onEvent = event;
				notesOn[event.getNoteNumber()].playable = false;
				notesOn[event.getNoteNumber()].tickTime = (*trackIterator).getTickTime();
			}
			else if (!(event.getNoteOn()))
			{
				notesOn[event.getNoteNumber()].playable = true;
			}
		}
		trackIterator++;
	} // end while
	if (trackIterator == endOfTrack)
	{
		stopped = true;
	}
	double sum = 0;
	for (int i = 0; i < notesOn.size(); i++)
	{
		if (notesOn[i].onEvent.getNoteOn())
		{
			sum += partialSample(notesOn[i]);
		}
	}
	return sum;
}

double DefaultInstrument::convertToTime(intmax_t tickTime)
{
	return ((tickTime * tempo) / (currentTrack.getTicksPerBeat() * 1000000.0));
}

double DefaultInstrument::envelope(intmax_t tickTime)
{
	double noteOnTime = time - convertToTime(tickTime);
	if (noteOnTime >= 0 && noteOnTime <= 0.0625)
	{
		return (noteOnTime / 0.0625);
	}
	if (noteOnTime > 0.0625 && noteOnTime <= 0.1125)
	{
		return (1 - noteOnTime + 0.0625);
	}
	if (noteOnTime > 0.1125 && noteOnTime <= 0.2)
	{
		return 0.8;
	}
	if (noteOnTime > 0.2 && noteOnTime <= 0.25)
	{
		return (0.8 - (0.8 / 0.05) * (noteOnTime - 0.2));
	}
	return 0;
}

double DefaultInstrument::frequency(int8_t noteNum)
{
	double freqs[] = {261.63, 277.18, 293.66, 311.13, 329.63, 349.63,
		              369.994, 392.0, 415.305, 440.0, 466.164, 493.88};
	return (freqs[noteNum % 12] * std::pow(2, (noteNum / 12) - 5));
}

double DefaultInstrument::partialSample(NoteData note)
{
	return (200 * note.onEvent.getNoteVelocity() * envelope(note.tickTime) * std::sin(2 * M_PI *
		    frequency(note.onEvent.getNoteNumber()) * (time - convertToTime(note.tickTime))));
}
