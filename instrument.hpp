/** \file The DefaultInstrument Module
 */
#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <vector>

#include "event.hpp"
#include "track.hpp"

/** The default instrument. A simple fixed length note instrument using a
 * a sinusoid weighted by an envelope function.
 */
class DefaultInstrument{
public:

  /** No default constructor. A DefaultInstrument must have an associated 
   * track. 
   */
  DefaultInstrument() = delete;

  /** Cannot be copied. 
   */
  DefaultInstrument(const DefaultInstrument & ) = delete;

  /** Cannot be assigned.
   */
  DefaultInstrument & operator=(const DefaultInstrument & ) = delete;

  /** Construct a Default Instrument using the specified track.
   * \param track the track to read events from. 
   */
  DefaultInstrument(const Track & track);

  /** Returns true of the DefaultInstrument has stopped processing events.
   * This can occu e.g. if all events have been consumed or a EndTrackEvent
   * has been processed.
   */
  bool halted();

  /** Return the sample after the (real) time increment. Samples the 
   * instrument based on its state after processing all events prior 
   * to this time.
   * \param deltaT the time increment in seconds.
   */
  double sample(double deltaT);

  /**
   * Resets the instrument to a default state with a new track
   * \param track The new track to play
   */
  void reset(const Track & track);

private:

	struct NoteData {
		NoteEvent onEvent;
		bool playable;
		intmax_t tickTime;
		NoteData() : onEvent(false, 0, 0), playable(true), tickTime(0) {}
	};
	std::vector<NoteData> notesOn;
	double time;
	bool stopped;
	intmax_t tempo;
	Track currentTrack;
	Track::ConstIteratorType trackIterator;
	Track::ConstIteratorType endOfTrack;

	/** Converts Midi event tick times to the corresponding real time
	 * \param tickTime the Midi event tick time to convert
	 */
	double convertToTime(intmax_t tickTime);

	/** Returns the piecewise function corresponding to a note that was
	 * started at the specified ticktime based on the current time
	 * \param tickTime the Midi tick time that the note started
	 */
	double envelope(intmax_t tickTime);

	/** Returns the frequency for a given number based on the conversion
	* given in the project specification
	* \param noteNum the number of the note being searched for
	*/
	double frequency(int8_t noteNum);

	/** Does the math for a sample from a single note
	* \param note the struct containing all relavant data for this note sample
	*/
	double partialSample(NoteData note);
};


#endif
