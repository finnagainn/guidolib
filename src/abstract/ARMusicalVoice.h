#ifndef ARMusicalVoice_H
#define ARMusicalVoice_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

//#include <typeinfo>

#include "ObjectList.h"
#include "ARMusicalEvent.h"
// #include "ARPositionTag.h"	
#include "TagList.h"		// is a typedef
#include  "kf_ilist.h"		// for ObjectList
//#include "ARNote.h"

class ARAuto;
class ARMusicalVoiceState;
class ARPositionTag;
class ARVoiceManager;
class ARMeter;
class ARBeamState;
class ARDisplayDuration;
class ARBase;
class ARTuplet;
class ARMusic;
class ARMusicalTag;
class ARChordTag;
class ARShareLocation;
class ARGrace;
class ARNote;
class ARClef;
class ARKey;
class ARCluster;

class GRVoiceManager;
class TimeUnwrap;

typedef KF_IPointerList<ARPositionTag> PositionTagList;
typedef KF_IPointerList<ARPositionTag> StartPositionTagList;

/** \brief The class ARChordGroup is used to find groups within chords that share
	 a stem and also a display-duration. The chordgroups are constructed during 
	 chord-creation.
*/

class ARChordGroup
{
	public:

		ARChordGroup() : dur( DURATION_0 ), startpos( 0 ), endpos( 0 ) { } 

		TYPE_DURATION dur;
		GuidoPos startpos;
		GuidoPos endpos;
};

typedef KF_IPointerList<ARChordGroup> ChordGroupList;


/** \brief The class for GUIDO sequence, list of timepositions of events of a stream.

	Completely represents a GUIDO sequence. It is used by
	the class ARMusic to represent a complete voice within a piece.
*/
class ARMusicalVoice : public ObjectList, public ARMusicalEvent
{
	friend class GRVoiceManager;
	friend class ARMusic;
	
	public:
		typedef enum		CHORD_TYPE {UP_SIMPLE, DOWN_SIMPLE, UP, DOWN, UP_COMPLEX, DOWN_COMPLEX, ERROR} CHORD_TYPE;
		typedef enum		CHORD_ACCIDENTAL {NATURAL, FLAT, SHARP, CAU_NATURAL, CAU_FLAT, CAU_SHARP, NONE} CHORD_ACCIDENTAL;
		// cau_natural, cau_flat, cau_sharp : cautionary accidentals
	
						 ARMusicalVoice();
		virtual			~ARMusicalVoice();

		void			MarkVoice(float from,float length);
		void			MarkVoice(int fromnum,int fromdenom, int lengthnum, int lengthdenom);

		virtual void	initChordNote();
		virtual void	FinishChord();
		virtual void	BeginChord();

		ARNote        * setTrillChord(CHORD_TYPE & param_type, CHORD_ACCIDENTAL & param_accidental);
        void            setClusterChord(ARCluster *inCurrentCluster);
		
		int				removeTag(ARMusicalObject * obj);
		void			SplitEventAtPos(ARMusicalVoiceState & vst, const TYPE_TIMEPOSITION & tp, int tieormerge);
		static void		DispdurToTupletdur(TYPE_DURATION & dur,const TYPE_DURATION & base);
		static void		TupletdurToDispdur(TYPE_DURATION & dur,const TYPE_DURATION & base);
		static bool		DurationIsDisplayable(TYPE_DURATION & dur,int & b_punkt);
		static bool		DurationFitsBase(const TYPE_DURATION & dur,const TYPE_DURATION & base, TYPE_DURATION & newbase);
		void			ConvertToNormalForm();

		virtual GuidoPos getLastEventPosition();
//		virtual void	FreezeState(ARMusicalVoiceState * in);
		virtual void	setPositionTagEndPos(int id, ARMusicalTag * tag, ARMusicalTag * start = NULL);
		virtual void	RemovePositionTag( ARPositionTag * ptag);
		virtual void	AddPositionTag(ARPositionTag *);
		virtual	PositionTagList * createPositionTagList();

		// does Auto-Beaming ...
		virtual void	doAutoStuff1();
		virtual void	doAutoStuff2();
		
		virtual void doAutoTrill();
        virtual void doAutoCluster();

		// expensive Function!
		GuidoPos		getPosAtTimePosition( const TYPE_TIMEPOSITION & timeposition);
		// expensive Function!
		ObjectList *	getARMusicalObjectsAtTimePosition(const TYPE_TIMEPOSITION & timeposition);

		// This adds a Musical-Object to the voice at the current time-position
		// The musical-object gets a time (relativeTimePosition) and the duration of the voice is adjusted 
		// (+ duration of musical object).
		GuidoPos		AddTail(ARMusicalObject * newMusicalObject);

		// inserts a musical object at the tail without doing time-setting and other stuff. 
		// is called mainly from CopyChord-function.
		GuidoPos		InsertAtTail(ARMusicalObject * newMusicalObject);
		// adjust the duration of a voice by adding a rest-event
		void			adjustDuration(const TYPE_DURATION & newDuration);

		virtual void	print() const;
		virtual std::ostream & operator<< (std::ostream & os) const;
		virtual std::ostream & output(std::ostream & os, bool isauto = true) const;
		virtual void	browse(TimeUnwrap& mapper) const;
		virtual void	browse(TimeUnwrap& mapper, ARMusicalVoiceState& state) const;
		virtual void	browse(TimeUnwrap& mapper, const ARMusicalObject * start, const ARMusicalObject * end=0) const;

		virtual void	resetGRRepresentation();

		// functions for the voicestate...
		virtual			GuidoPos GetHeadPosition(ARMusicalVoiceState & vst) const;
		virtual void	GetPrevEvent(GuidoPos & pos, ARMusicalVoiceState & vst) const;
		virtual			ARMusicalObject * GetNext(GuidoPos & pos, ARMusicalVoiceState & vst) const;
		virtual const	ARMusicalVoiceState * getVoiceState() const		{ return mCurVoiceState; }

		// for automatic-beaming ...
		enum bmstate { BEAMSAUTO, BEAMSOFF, BEAMSEXPLICIT };

		// this enumeration deals with the readmode.
		// CHORDMODE means that when getting the next (or previous) event, we check whether we are
		// within a chord and then overread (and save) all other events within the chord.
		enum _readmode { CHORDMODE, EVENTMODE };

		void			setVoiceNum(int num)				{ voicenum = num;	}
		int				getVoiceNum() const					{ return voicenum;	}
		void			removeAutoTags();
		void			setReadMode(_readmode newreadmode)	{ readmode = newreadmode; }
		_readmode getReadMode() const						{ return readmode; }

	protected:
		ARChordTag *		currentChord;
		ARShareLocation *	currentShareLocation;
		ChordGroupList *	chordgrouplist;
		GuidoPos			posfirstinchord;
		int					numchordvoice;
		ARMusicalVoiceState * chordBeginState;
		void				doAutoKeys();
		int					sum;
		int					pitchsum;
		int					voicenum;

		void doAutoDispatchLyrics();
		void doAutoFermatas();
		void doAutoTies();
		void doAutoCheckStaffStateTags();
		void doAutoDisplayCheck();
		void doAutoBarlines();
		void doAutoEndBar();
		void doAutoBeaming();
		void doAutoGlissando();

		void doMicroTonal();

		void CloseBase			( ARBase * curbase, ARTuplet * autotuplet, GuidoPos lastevpos, GuidoPos FLA);
		void ReplacePositionTag	(ARPositionTag * ptold, ARPositionTag * ptnew, ARMusicalVoiceState & vst, 
										const char * endtagname = 0 );
		void InsertDisplayDurationTag (const TYPE_DURATION & dispdur, int b_punkt, const TYPE_TIMEPOSITION & tp,
										GuidoPos pos, ARMusicalVoiceState & vst, int setptagpos = 1);

		GuidoPos			  lastevposition;
		ARMusicalVoiceState * mCurVoiceState;
		ARMusicalVoiceState * endState;								// the musicalvoice state at the very end.

		// automatic beaming.
	    int beamLookAhead(GuidoPos & pos, TYPE_DURATION & unit);
		bmstate beamstate;

		_readmode readmode;

		PositionTagList *		mPosTagList;
		StartPositionTagList *	mStartPosTagList;
	
	private:
		GuidoPos	CopyChord( ARMusicalVoiceState & vst, TYPE_TIMEPOSITION tp, const TYPE_DURATION & newdur);
		ARClef*		newAutoClef(ARClef* oldclef, const TYPE_TIMEPOSITION& tp);
		ARKey *		newAutoKey(ARKey * oldkey, const TYPE_TIMEPOSITION& tp);	
};

#endif


