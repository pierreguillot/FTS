REVERSE ENGINEERING the FST audio plugin API
============================================


# starting
We use JUCE-5.4.1 to compile a VST2-compliant plugin host, but without VST-headers.

- edit `JUCE/extras/AudioPluginHost/AudioPluginHost.jucer` and enable VST, by
enaming the `JUCE_PLUGINHOST_VST3` variable:

~~~
JUCE_PLUGINHOST_VST="1"
~~~

- resave the Project `JUCE/Projucer -resave JUCE/extras/AudioPluginHost/AudioPluginHost.jucer`
- and try to build it:

~~~
cd JUCE/extras/AudioPluginHost/Builds/LinuxMakefile
make
~~~

- JUCE will complain about missing `<pluginterfaces/vst2.x/aeffect.h>` (and later
`<pluginterfaces/vst2.x/aeffectx.h>`) headers.
We will just create a single header `fst.h`, and have stub-implementations of `aeffect.h`
and `aeffectx.h` that simply include the real header.

~~~bash
mkdir FST
touch FST/fst.h
for i in aeffect.h aeffectx.h ; do
cat > $i <<EOF
#pragma once
#include "fst.h"
EOF
~~~

- finally, make JUCE use our headers:

~~~bash
ln -s $(pwd)/FST JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst2.x
~~~

# getting our first enum names

building the AudioPluginHost  gets us a lot of errors (about 501).
these errors are interesting, as this is what we want to implement in the `fst.h` headers.
luckily JUCE will attempt to put everything in the `Vst2` namespace, making it easier to find the
interesting broken things.

So lets start, by extracting all the symbols that should be part of the `Vst2` namespace (but are not):

~~~bash
make -k -C JUCE/extras/AudioPluginHost/Builds/LinuxMakefile 2>&1 \
| grep error: \
| grep "is not a member of .Vst2.$" \
| sed -e 's|.* error: ||' | sed -e 's|^.||' -e 's|. .*||' \
| sort -u
~~~

This gives us the name `Erect` and symbols starting with the following prefixes:
- `audioMaster*`
- `eff*`
- `kPlug*`
- `kSpeaker*`
- `kVst*`
- `Vst*`

As a first draft, let's just add all those symbols as enumerations to our `fst.h` header:

~~~C
#define FST_UNKNOWN(x) x
enum {
    FST_UNKNOWN(audioMasterAutomate),
    FST_UNKNOWN(audioMasterBeginEdit),
    // ..
    FST_UNKNOWN(audioMasterWillReplaceOrAccumulate)
};
// ...
~~~

We wrap all the values into the `FST_UNKNOWN` macro, because we don't know their actual values yet.

- *TODO*: FST_UNKNOWN(x) x = 98765 + __LINE__


# some types
if we now re-compile the AudioPluginHost against our new headers, we get a lot less errors (about 198).
It's still quite a lot of errors though...

Look at the output of

~~~
make -k -C JUCE/extras/AudioPluginHost/Builds/LinuxMakefile 2>&1 | less
~~~

There's a number of errors that are `type` related, using these symbols:

~~~
AEffect
VstEvent
VstEvents
VstPlugCategory
VstSpeakerArrangement
VstTimeInfo
~~~


## VstEvent
A quick `rgrep VstEvent` on the JUCE sources leads us to `modules/juce_audio_processors/format_types/juce_VSTMidiEventList.h`, which reveals that
`VstEvent` is a structure with at least two members `type` and `byteSize`.
The later is of a type compatible with `size_t` (a `sizeof()` value is assigned to it),
the former is assigned a value of `kVstMidiType`, and compared to `kVstSysExType`. Most likely these two are in a separate enum `VstEventType` or somesuch.

There's also related types `VstMidiEvent` and `VstMidiSysexEvent` which can be case to `VstEvent`.
The sysex-type features a `sysexDump` member, that is a dynamically allocated array (most likely of some `BYTE` type).

~~~
typedef enum {
  FST_UNKNOWN(kVstMidiType),
  FST_UNKNOWN(kVstSysExType),
} t_fstEventType;

#define FSTEVENT_COMMON t_fstEventType type; int byteSize
typedef struct VstEvent_ {
 FSTEVENT_COMMON;
} VstEvent;

typedef struct VstMidiEvent_ {
 FSTEVENT_COMMON;
 /* FIXXXME: unknown member order */
 FST_UNKNOWN(int) noteLength;
 FST_UNKNOWN(int) noteOffset;
 FST_UNKNOWN(int) detune;
 FST_UNKNOWN(int) noteOffVelocity;
 FST_UNKNOWN(int) deltaFrames;
 char midiData[4];
} VstMidiEvent;
typedef struct VstMidiSysexEvent_ {
 FSTEVENT_COMMON;
 /* FIXXXME: unknown member order */
  char*sysexDump;
  FST_UNKNOWN(int) dumpBytes;
  FST_UNKNOWN(int) deltaFrames;
  FST_UNKNOWN(int) flags;
  FST_UNKNOWN(int) resvd1, resvd2;
} VstMidiSysexEvent;
~~~

`VstEvent`, `VstMidiEvent` and `VstSysexEvent` are removed from our previous enum.


## VstEvents
Since `VstEvents` is supposed to be a type, let's naively set it to `int`.
This reveals, that JUCE really wants a struct with members `numEvents` (`int`)
and `events` (an array with elements to be cast to `*VstEvent` and the like)

~~~
typedef struct VstEvents_ {
  int numEvents;
  VstEvent**events;
} VstEvents;
~~~


## VstSpeakerArrangement
Since `VstSpeakerArrangement` is supposed to be a type, let's naively set it to `int`.
This will yield errors about not being able to access members in a non-class type.

~~~bash
make -C JUCE/extras/AudioPluginHost/Builds/LinuxMakefile 2>&1 \
| grep VstSpeakerArrangement | grep "error:" \
| sed -e 's|.* error: request for member ||' -e 's| in .*||' \
| sort -u
~~~

Gives us three members, `type`, `speakers`, `numChannels`.
To get the types of these members, let's set it experimentally to something stupid, like `void*`.
That reveleals that we need to be able to convert `type` to `int32` (but more likely some enumeration),
`numChannels` to `int`, and that `speakers` really is an array to `VstSpeakerProperties`.

Somewhere in `modules/juce_audio_processors/format_types/juce_VSTCommon.h`, we can see that
the `type` is populated by `int channelSetToVstArrangementType()` which really returns values like
`kSpeakerArrStereo`.

I'd rather assign the `kSpeakerArr*` enumeration a name `t_fstSpeakerArrangementType` and use that for `type`,
but alas, C++ does not allow to implicitely cast `int` to `enum`, so we need to use `int` instead.

~~~C
typedef struct VstSpeakerArrangement_ {
  int type;
  int numChannels;
  VstSpeakerProperties*speakers;
} VstSpeakerArrangement;



## VstSpeakerProperties
resolving `VstSpeakerArrangement`, leaves us with a new type `VstSpeakerProperties`.
We play the above game again, and get:

~~~C
typedef struct VstSpeakerProperties_ {
  FST_UNKNOWN(int) type;
} VstSpeakerProperties;
~~~

## VstPlugCategory
A value of this type is compared to `kPlugCategShell`, so we typedef the enumeration
with the `kPlug*` names to `VstPlugCategory`.

This also adds the `kPlugSurroundFx` value to this new type, if which I am not sure
yet.


## VstTimeInfo
Again playing the game by setting `VstTimeInfo` to `int`, gives us members of the struct.
So far, the types are guessed based on the values they are assigned to (If a floating-point value is assigned,
we use `double`, for integer types we use `int`):

~~~C
typedef struct VstTimeInfo_ {
  FST_UNKNOWN(double) tempo;
  FST_UNKNOWN(int) timeSigNumerator;
  FST_UNKNOWN(int) timeSigDenominator;
  FST_UNKNOWN(double) sampleRate;// = rate;
  FST_UNKNOWN(int) samplePos;
  FST_UNKNOWN(int) flags;// = Vst2::kVstNanosValid
  FST_UNKNOWN(double) nanoSeconds;
  
  FST_UNKNOWN(double) ppqPos; // (double)position.ppqPosition;
  FST_UNKNOWN(double) barStartPos; // (double)ppqPositionOfLastBarStart;
  FST_UNKNOWN(double) cycleStartPos; // (double)ppqLoopStart;
  FST_UNKNOWN(double) cycleEndPos; // (double)ppqLoopEnd;
  FST_UNKNOWN(int) smpteFrameRate; //int32
  FST_UNKNOWN(int) smpteOffset; //int32
} VstTimeInfo;

## AEffect


