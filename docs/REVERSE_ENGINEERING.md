REVERSE ENGINEERING the FST audio plugin API
============================================

# Part0: used software

our development system used for reverse-engineering is a
Debian GNU/Linux (amd64) box, running Debian testing/unstable.


## general software
Apart from the general system utilities (like `emacs`, `vi`, `sed`, `grep`,...)
found on any ordinary Linux system, we also use:

- Compiler: `gcc` (Debian 8.2.0-20) 8.2.0
- Debugger: `gdb` (Debian 8.2.1-1) 8.2.1
- Memory Debugger: `valgrind`
- Hexeditor: `bless`
- Diff-viewers: `meld`, `vimdiff`, `vbindiff`


## VST related
all VST-related software was accessed on 2019-02-16.


### Frameworks

- JUCE-5.4.1: https://d30pueezughrda.cloudfront.net/juce/juce-5.4.1-linux.zip

### Plugins

- U-he Protoverb: https://u-he.com/products/protoverb/
- InstaLooper: https://www.audioblast.me/instalooper.html
- BowEcho: https://ineardisplay.com/plugins/legacy/
- Danaides: https://ineardisplay.com/plugins/legacy/
- Digits: http://www.extentofthejam.com/
- Hypercyclic: http://www.mucoder.net/en/hypercyclic/
- Tonespace: http://www.mucoder.net/en/tonespace/

### Hosts

- Reaper: https://www.reaper.fm/download.php (reaper5965_linux_x86_64.tar.xz)

# Part1: unknown symbols

## starting
We use JUCE-5.4.1 to compile a VST2-compliant plugin host, but without VST-headers.

- edit `JUCE/extras/AudioPluginHost/AudioPluginHost.jucer` and enable VST, by
renaming the `JUCE_PLUGINHOST_VST3` variable:

~~~
JUCE_PLUGINHOST_VST="1"
~~~

- resave the Project `JUCE/Projucer -resave JUCE/extras/AudioPluginHost/AudioPluginHost.jucer`
- and try to build it:

~~~bash
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
##pragma once
##include "fst.h"
EOF
~~~

- finally, make JUCE use our headers:

~~~bash
ln -s $(pwd)/FST JUCE/modules/juce_audio_processors/format_types/VST3_SDK/pluginterfaces/vst2.x
~~~

## getting our first enum names

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

This gives us the name `ERect` and symbols starting with the following prefixes:
- `audioMaster*`
- `eff*`
- `kPlug*`
- `kSpeaker*`
- `kVst*`
- `Vst*`

As a first draft, let's just add all those symbols as enumerations to our `fst.h` header:

~~~C
##define FST_UNKNOWN(x) x
enum {
    FST_UNKNOWN(audioMasterAutomate),
    FST_UNKNOWN(audioMasterBeginEdit),
    // ..
    FST_UNKNOWN(audioMasterWillReplaceOrAccumulate)
};
// ...
~~~

We wrap all the values into the `FST_UNKNOWN` macro, because we don't know their actual values yet.

We even might go as far as assign some pseudo-random value that is far off to each enum,
in order to not accidentally trigger a real enum:

~~~
FST_UNKNOWN_ENUM(x) x = 98765 + __LINE__
~~~

## some types
if we now re-compile the AudioPluginHost against our new headers, we get a lot less errors (about 198).
It's still quite a lot of errors though...

Look at the output of

~~~bash
make -k -C JUCE/extras/AudioPluginHost/Builds/LinuxMakefile 2>&1 | less
~~~

There's a number of errors that are `type` related, using these symbols:

- `AEffect`
- `VstEvent`
- `VstEvents`
- `VstPlugCategory`
- `VstSpeakerArrangement`
- `VstTimeInfo`


### VstEvent
A quick `rgrep VstEvent` on the JUCE sources leads us to `modules/juce_audio_processors/format_types/juce_VSTMidiEventList.h`, which reveals that
`VstEvent` is a structure with at least two members `type` and `byteSize`.
The later is of a type compatible with `size_t` (a `sizeof()` value is assigned to it),
the former is assigned a value of `kVstMidiType`, and compared to `kVstSysExType`. Most likely these two are in a separate enum `VstEventType` or somesuch.

There's also related types `VstMidiEvent` and `VstMidiSysexEvent` which can be case to `VstEvent`.
The sysex-type features a `sysexDump` member, that is a dynamically allocated array (most likely of some `BYTE` type).

~~~C
typedef enum {
  FST_UNKNOWN_ENUM(kVstMidiType),
  FST_UNKNOWN_ENUM(kVstSysExType),
} t_fstEventType;

##define FSTEVENT_COMMON t_fstEventType type; int byteSize
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


### VstEvents
Since `VstEvents` is supposed to be a type, let's naively set it to `int`.
This reveals, that JUCE really wants a struct with members `numEvents` (`int`)
and `events` (an array with elements to be cast to `*VstEvent` and the like)

~~~C
typedef struct VstEvents_ {
  int numEvents;
  VstEvent**events;
} VstEvents;
~~~


### VstSpeakerArrangement
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
~~~


### VstSpeakerProperties
resolving `VstSpeakerArrangement`, leaves us with a new type `VstSpeakerProperties`.
We play the above game again, and get:

~~~C
typedef struct VstSpeakerProperties_ {
  FST_UNKNOWN(int) type;
} VstSpeakerProperties;
~~~

### VstPlugCategory
A value of this type is compared to `kPlugCategShell`, so we typedef the enumeration
with the `kPlug*` names to `VstPlugCategory`.

This also adds the `kPlugSurroundFx` value to this new type, if which I am not sure
yet.


### VstTimeInfo
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
~~~

### AEffect
rinse & repeat.

we need a few helpers:

- `VSTCALLBACK` this seems to be only a decorator, we use `#define VSTCALLBACK` to ignore it for now
- `audioMasterCallback` ??? (that's probably a pointer to the dispatcher function)

        effect = module->moduleMain ((Vst2::audioMasterCallback) &audioMaster);

- a pointer-sized-int, for now `typedef long t_fstPtrInt;` will do


after that the biggest issue is that the `AEffect` struct contains a few function-pointers, namely
- `dispatcher`
- `getParameter` & `setParameter`
- `process`, `processReplacing` and `processDoubleReplacing`

luckily JUCE maps those functions quite directly, so we get:
~~~
t_fstPtrInt dispatcher(AEffect*, int opcode, int index, t_fstPtrInt ivalue, void* const ptr, float fvalue);
void setParameter(AEffect*, int index, float value);
float getParameter(AEffect*, int index);
void process(AEffect*, float**indata, float**outdata, int sampleframes);
void processReplacing(AEffect*, float**indata, float**outdata, int sampleframes);
void processReplacingDouble(AEffect*, double**indata, double**outdata, int sampleframes);
~~~

And we end up with something like the following:

~~~C
typedef long t_fstPtrInt; /* pointer sized int */
##define VSTCALLBACK

 /* dispatcher(effect, opcode, index, ivalue, ptr, fvalue) */
typedef t_fstPtrInt (t_fstEffectDispatcher)(struct AEffect_*, int, int, t_fstPtrInt, void* const, float);
typedef void (t_fstEffectSetParameter)(struct AEffect_*, int, float);
typedef float (t_fstEffectGetParameter)(struct AEffect_*, int);
typedef void (t_fstEffectProcess)(struct AEffect_*, float**indata, float**outdata, int sampleframes);
typedef void (t_fstEffectProcessInplace)(struct AEffect_*, float**indata, float**outdata, int sampleframes);
typedef void (t_fstEffectProcessInplaceDbl)(struct AEffect_*, double**indata, double**outdata, int sampleframes);

typedef FST_UNKNOWN(t_fstEffectDispatcher*) audioMasterCallback;

typedef struct AEffect_ {
  FST_UNKNOWN(int) magic; /* 0x56737450 */
  FST_UNKNOWN(int) uniqueID;
  FST_UNKNOWN(int) version;

  FST_UNKNOWN(void*) object; // FIXXXME

  t_fstEffectDispatcher* dispatcher; // (AEffect*, Vst2::effClose, 0, 0, 0, 0);
  t_fstEffectGetParameter* getParameter; // float(Aeffect*, int)
  t_fstEffectSetParameter* setParameter; // (Aeffect*, int, float)
  t_fstEffectProcess* process;
  t_fstEffectProcessInplace* processReplacing;
  t_fstEffectProcessInplaceDbl* processDoubleReplacing;

  FST_UNKNOWN(int) numPrograms;
  FST_UNKNOWN(int) numParams;
  FST_UNKNOWN(int) numInputs;
  FST_UNKNOWN(int) numOutputs;
  FST_UNKNOWN(int) flags;
  FST_UNKNOWN(int) initialDelay;

  FST_UNKNOWN(t_fstPtrInt) resvd1;
  FST_UNKNOWN(t_fstPtrInt) resvd2;
} AEffect;
~~~


### VstPinProperties
this is also a type rather than an enum.
play the typedef game again, and we get:

~~~C
typedef struct VstPinProperties_ {
    FST_UNKNOWN(int) arrangementType;
    char*label;
    int flags;
} VstPinProperties;
~~~


### ERect
the last remaining type we missed is `ERect`.

rinse & repeat and we have:

~~~C
typedef struct ERect_ {
  int left;
  int right;
  int top;
  int bottom;
} ERect;
~~~

## conclusion
with the changes in place, we can now compile JUCE's AudioPluginProcessor.
it's still a long way to make it actually work...

# Part 1.1 more symbols

The JUCE wrapper is not entirely symmetric, so only deriving missing
symbols from AudioPluginHost is not enough.
We also need to compile a (minimal) Audio Plugin using JUCE,
to get additional symbols.
Just creating a new *Audio Plugin* project using Projucer will
give us some additional symbols.

Apart from an additional `VstPinProperties.shortLabel`,
we still miss the following types:

~~~
AEffectDispatcherProc
AEffectSetParameterProc
AEffectGetParameterProc
AEffectProcessProc
AEffectProcessDoubleProc
~~~

as well as some constants:
~~~
kVstMaxLabelLen
kVstMaxShortLabelLen
kVstClockValid
kVstPinIsActive
kVstSmpte249fps
kVstSmpteFilm16mm
kVstSmpteFilm35mm
kVstVersion
~~~

The `kVstVersion` is a bit special, as JUCE doesn't use the `Vst2::` namespace on it.
We have to `#define` it, rather than use an `enum` for it...



# Part2: how the plugin interfaces with the host
For now, we have resolved all the names of the API to some more or less random values.
We also have discovered a few complex types (`struct`s), and while we know the names
of the struct-members and have an approximate guess of their types, we have no clue about
the order of the members, and thus the memory layout.
Since those structs are presumably used to pass data between a plugin host and a plugin,
getting them right is very important - at least if we don't want crashes.

Until know, we didn't need to know anything about the structure of the API.
We just blindly provided "reasonable" (dummy) names whenver we encountered an unknown name.

In order to reverse engineer the entire API, we should have a closer look about what we have so far.

- a lot of integer-typed values, presumably grouped in `enum`s
  - `VstPlugCategory`
  - a speaker arrangement type
  - an event type
- a few "Vst" types
  - `VstEvent`, `VstMidiEvent`, `VstSysexEvent`, `VstEvents`
  - `VstSpeakerProperties`, `VstSpeakerArrangement`
  - `VstTimeInfo`
  - `VstPinProperties`
- a generic type
  - `ERect` (just a rectangle with 4 corners)
- the somewhat special `AEffect` struct
  - it is protected with a magic number (that happens to translate to `VstP`)
  - it contains function pointer
  - the function pointer members take a pointer to an AEffect as their first argument

we can also make a few assumptions:
- some of the structs contain a `flags` field, which is presumable an integer type where bits need to be set/unset
- for simplicity of the API, most of the integer types will be `int32`.

Anyhow, the `AEffect` seems to be a central structure. Most likely it means *Audio Effect*,
which is a nice name for any kind of plugin.
So most likely this structure contains an *instance* of a plugin.
There are some specialized "class methods" (yay: OOP in C!),
like a DSP block processing `process` and friends, as well as setters and getters for plugin parameters.
Apart from that there is a generic `dispatcher` function that takes an opcode (an integer)
that describes the actual functionality.
That's a nice trick to keep an API small and stable.

Let's have a quick look at how plugins are instantiated (see the `open` method in
`JUCE/modules/juce_audio_processors/format_types/juce_VSTPluginFormat.cpp`):

- the plugin file ("dll") is opened (using `dlopen()` or the like)
- the function with the name `VSTPluginMain` (or as a fallback `main`) is obtained from the dll
- whenever we want to instantiate a new plugin (`constructEffect`) we call this function,
  and pass it a callback function as an argument.
  the plugin may use this callback the query information from the host
- the `VstPluginMain` function returns a pointer to an instance of `AEffect`
- we can use the `AEffect.dispatcher` function (which has the same signature as the callback function)
  to call most of the plugin's methods (including an `open` and `close`).
  A long list of functions callable via the `dispatcher` can be found in
  `juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp`

In the `juce_VST_Wrapper.cpp` file, we can also find some more Vst2-symbols of the `eff*` category,
all of them being used as the opcode argument for the `AEffect.dispatcher`.

~~~
effEditDraw
effEditMouse
effEditSleep
effEditTop
effString2Parameter
effGetVstVersion
effGetCurrentMidiProgram
effSetTotalSampleToProcess
effGetNumMidiInputChannels
effGetNumMidiOutputChannels
~~~

We also not that the symbols matching `effFlags*` are never used as opcode specifier.
Instead they are used as bitmasks for `AEffect.flags`.

While we are at it, maybe the `audioMaster*` symbols are opcodes for the callback we pass
to the `VstPluginMain` function? Let's check:

~~~bash
rgrep "audioMaster" JUCE \
| sed -e 's|audioMasterCallback|CB|g' \
| egrep "audioMaster[A-Z]"
~~~

Which seems to confirm our idea.
Let's also check whether we have missed some `audioMaster*` symbols:

~~~bash
rgrep "Vst2::audioMaster[A-Z]" JUCE/ \
| sed -e 's|.*Vst2::\(audioMaster[A-Za-z0-9]*\).*|\1|' \
| sort -u
~~~

nope. all of them are already in our `fst.h` file.


## Conclusion
There is only a single symbol that needs to be shared between a plugin and a host:
the name of the main entry function into the plugin, which can be `VSTPluginMain` or `main`.

The host calls this function, and provides a callback function of type

~~~C
typedef t_fstPtrInt (t_fstEffectDispatcher)(AEffect*, int opcode, int, t_fstPtrInt, void* const, float);
~~~

The main function returns an `AEffect*` handle, which in turn has a member `dispatcher` of the same
type `t_fstEffectDispatcher`.
These two functions do the most work when communicating between host and plugin.
The opcodes for the plugin start with `eff*` (but not `effFlags*`),
the opcodes for the host start with `audioMaster*`.


# Part3: AEffect
So let's try our code against some real plugins/hosts.
We start with some freely available plugin from a proper commercial plugin vendor,
that has been compiled for linux.
There are not many, but [u-he](https://u-he.com/) provides the `Protoverb` as binaries
for Windows, macOS and Linux (hooray!), free of charge (hooray again!).
*u-he* sell commercial plugins as well, so i figure they have a proper license from Steinberg
that allows them to do this. (I'm a bit afraid that many of the FLOSS VST2 plugins simply
violate Steinberg's terms of use. So I prefer to use plugins from commercial manufacturers for now.
Just for the record: by no means I'm trying to reverse engineer any of the plugin's trade secrets.)

Another bonus of `Protoverb` is that it is made *without* JUCE.
While JUCE is certainly a great toolkit, having plugins outside
that ecosystem will hopefully provide better coverage of edge cases...

Anyhow: we can try to load the `Protoverb` plugin in the JUCE AudioPluginHost that we
just compiled with our broken `fst.h` header.

Using <kbd>Ctrl</kbd>+<kbd>p</kbd> -> `Options` -> `Scan for new or updated VST plug-ins`,
we select the folder where we extracted the `Protoverb.64.so` file into and hit <kbd>Scan</kbd>.

Not very surprisingly it fails to load the plugin.

The error is:
> Creating VST instance: Protoverb.64
> *** Unhandled VST Callback: 1

This errors is printed in `juce_audio_processors/format_types/juce_VSTPluginFormat.cpp`,
when dispatching the callback opcodes.
Adding a little `printf` around the `JUCE_VST_WRAPPER_INVOKE_MAIN` it becomes clear that
the plugin is calling the callback from within the `VstPluginMain` function.
And it refuses to instantiate if we cannot handle this request.

Looking at the `audioMaster` opcodes, i only see `audioMasterVersion`
that looks like if it could be queried at the very beginning.
JUCE returns a hardcoded value of `2400` for this opcode,
and it seems like this is the VST-version (the plugin would need to make sure
that the host is expecting a compatible VST version before it instantiates itself).
Most likely the `kVstVersion` define uses the same value.

So we now have our first known enum. Yipee!

~~~C
#define FST_ENUM_EXP(x, y) x = y
// ...
   FST_ENUM_EXP(audioMasterVersion, 1),
~~~

Recompiling, and scanning for the plugin again, leads to...a crash.
But before that, we get a bit of information (obviously the plugin is quite verbose when
initializing).
The interesting parts are

> *** Unhandled VST Callback: 33

and the crash itself, which happens *after* the plugin returned from `VstMainPlugin`.

Starting `gdb` to get a backtrace:

~~~sh
$ gdb --args JUCE/extras/AudioPluginHost/Builds/LinuxMakefile/build/AudioPluginHost
[...]
(gdb) run
[...]
Segmentation fault (core dumped)
(gdb) bt
#0  0x00007fffed6c9db2 in ?? () from /home/zmoelnig/src/iem/FST/tmp/vst/Protoverb/Protoverb.64.so
#1  0x00005555556b4d95 in juce::VSTPluginInstance::create (newModule=..., initialSampleRate=44100, initialBlockSize=512)
    at ../../../../modules/juce_audio_processors/format_types/juce_VSTPluginFormat.cpp:1146
[...]
~~~

the relevant line in `juce_VSTPluginFormat.cpp` calls

~~~
newEffect->dispatcher (newEffect, Vst2::effIdentify, 0, 0, 0, 0);
~~~

So we are accessing the `dispatcher` member of the `AEffect` struct.
Since we don't really know the memory layour of `AEffect` yet, the pointer most likely points to garbage.

Anyhow, in order to discover the layout of `AEffect` we need to examine it more closely.

For this we write a super-simplistic `FstHost` application,
that can load plugins for testing.
(I originally intended to just use the JUCE framework for this;
but we will experience lots of crashes, and will have to recompile *often*.
JUCE is rather big and it turns out that it takes just too long...)

~~~C
#include <stdio.h>
#include <dlfcn.h>
#include "fst.h"
typedef AEffect* (t_fstMain)(t_fstEffectDispatcher*);
t_fstPtrInt dispatcher (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  printf("FstHost::dispatcher(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, ivalue, ptr, fvalue);
  return 0;
}
t_fstMain* load_plugin(const char* filename) {
  void*handle = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
  void*vstfun = 0;
  if(!handle)return 0;
  if(!vstfun)vstfun=dlsym(handle, "VSTPluginMain");
  if(!vstfun)vstfun=dlsym(handle, "main");
  if(!vstfun)dlclose(handle);
  printf("loaded %s as %p: %p\n", filename, handle, vstfun);
  return (t_fstMain*)vstfun;
}
int test_plugin(const char*filename) {
  t_fstMain*vstmain = load_plugin(filename);
  if(!vstmain)return printf("'%s' was not loaded\n");
  AEffect*effect = vstmain(&dispatcher);
  if(!effect)return printf("unable to instantiate plugin from '%s'\n", filename);
  return 0;
}
int main(int argc, const char*argv[]) {
  for(int i=1; i<argc; i++) test_plugin(argv[i]);
  return 0;
}
~~~

After building with `g++ -IFST FstHost.cpp -o FstHost -ldl -lrt` we can now try to open
plugins with `./FstHost path/to/plugin.so`.

Running it on a real plugin ("Protoverb.64.so") just returns
> FstHost::dispatcher((nil), 1, 0, 0, (nil), 0.000000);
> unable to instantiate plugin from 'tmp/vst/Protoverb/Protoverb.64.so'

So we apparently *must* handle the `audioMasterVersion` opcode in our local dispatcher
(just like JUCE does); let's extend our local dispatcher to do that:

~~~
t_fstPtrInt dispatcher (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  printf("FstHost::dispatcher(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, ivalue, ptr, fvalue);
  switch(opcode) {
  case audioMasterVersion: return 2400;
  default:  break;
  }
  return 0;
}
~~~

And now we are getting somewhere!

Let's examine the `AEffect` data we get from the VSTPluginMain-function.
Setting a breakpoint in `gdb` right after the call to `vstmain`, and running
`print *effect` gives us:

~~~gdb
$1 = {magic = 1450406992, uniqueID = 0, version = -141927152, object = 0x7ffff78a5d70, dispatcher = 0x7ffff78a5d60,
  getParameter = 0x7ffff78a5d50, setParameter = 0x500000001, process = 0x200000002, processReplacing = 0x31,
  processDoubleReplacing = 0x0, numPrograms = 0, numParams = 0, numInputs = 16, numOutputs = 0, flags = 0,
  initialDelay = 1065353216, resvd1 = 93824992479632, resvd2 = 0}
~~~

This looks bad enough.
Not all looks bad though: `magic` has a value of `1450406992`, which really is `0x56737450` in hex,
which happens to be the magic number `VstP`.
The rest however is absymal: negative version numbers, unique IDs that are `0` (how unique can you get),
a function pointer (to `processReplacing`) that is `0x31` which is definitely invalid.

So let's take a closer look at the actual data.
For this, we just dump the memory where `AEffect*` points to into a file
(using `dumpdata(filename, effect, 160);` with the function below, just after calling `VSTPluginMain`)
and then use some hexeditor on it.

~~~
#include <string>
void dumpdata(const char*basename, const void*data, size_t length) {
  const char*ptr = (const char*)data;
  std::string filename = std::string(basename);
  filename+=".bin";
  FILE*f = fopen(filename.c_str(), "w");
  for(size_t i=0; i<length; i++) {
    fprintf(f, "%c", *ptr++);
  }
  fclose(f);
}
~~~

Inspecting the binary dumps with `bless`, we see that there are quite a lot of
8- (and some additional 4-) byte sequences that are always zero.
namely: bytes @04-07 (addresses in hex-notation), @40-57 and @68-6F.

~~~
00000000  50 74 73 56 00 00 00 00  10 ad 87 f7 ff 7f 00 00  |PtsV............|
00000010  70 ad 87 f7 ff 7f 00 00  60 ad 87 f7 ff 7f 00 00  |p.......`.......|
00000020  50 ad 87 f7 ff 7f 00 00  01 00 00 00 05 00 00 00  |P...............|
00000030  02 00 00 00 02 00 00 00  31 00 00 00 00 00 00 00  |........1.......|
00000040  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
00000050  10 00 00 00 00 00 00 00  00 00 00 00 00 00 80 3f  |...............?|
00000060  c0 fb 58 55 55 55 00 00  00 00 00 00 00 00 00 00  |..XUUU..........|
00000070  56 50 68 75 01 00 00 00  80 ad 87 f7 ff 7f 00 00  |VPhu............|
00000080  90 ad 87 f7 ff 7f 00 00  00 00 00 00 00 00 00 00  |................|
00000090  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
~~~

If we run this repeatedly on the same plugin file, we can collect multiple dumps
(don't forget to rename them, else they will be overwritten).
Comparing those multiple dumps, we can see that many bytes stay the same, but a few
are changing for each run.

Comparing the changing values with the addresses of heap-allocated memory and functions
(e.g. the DLL-handle returned by `dlopen()` and the function pointer returned by `dlsym()`)
it becomes clear, that we have six function pointers
(e.g. `?? ad 87 f7 ff 7f 00 00` at @8, @10, @18, @20, @78, @80) and
one heap allocated object (e.g. `c0 fb 58 55 55 55 00 00` at @60).
On my amd64 system, pointers take 8 byte and aligned at 8 bytes (one halfline in the hexdump).

We don't know which functions are stored in which pointer,
but it cannot be coincidence, that our `AEffect` struct has exactly 6 function pointers
(`dispatcher`, `getParameter`, `setParameter`, `process`, `processReplacing`, `processDoubleReplacing`).
We are also a bit lucky, because we dont really know whether each function ptr must actually be filled
with a valid function (it might be allowed to set them to `0x0`).
The `Protoverb` plugin seems to implement all of them!

The heap allocated object (@60) might match the `void*object` member.

Comparing the hexdumps of multiple different plugins, we notice that @70
there are always 4 printable characters (here `VPhu`), although they are different
for each plugin.
Additionally, we notice that the `Protoverb` plugin (being pretty verbose on the stderr)
said something like:
> setUniqueID (1969770582)

This number is `0x75685056` in hex. If we compare that to the bytes @70,
we cannot help but note that we have discovered the location of the `uniqueID` member.

If we start long enough at the remaining byte sequences, we see some other patterns
as well.
esp. at positions @28-37, there seem to be four (little endian) 4-byte integer values
(`1,5,2,2` for the *ProtoVerb*). These could map to `numPrograms`, `numParams`,
`numInputs` and `numOutputs`.
These assumption is hardened by looking at other plugins like *Digits*, *tonespace* and *hypercyclic*
that are synth-only plugins (no input) and indeed have `0` at @34-37.
I don't know how many plugins really implement *programs*,
but it doesn't sound absurd if all plugins we inspect have only a single program,
while most have numerous parameters.
So for now, let's assume that @28-37 are indeed  `numPrograms`, `numParams`, `numInputs`
and `numOutputs`, each taking 4 bytes.

The remaining non-zero bytesequences are
- @38-3f
- @5c-5f: always `00 00 80 3f`, which - weirdly enough - is the `1.f` in single-precision floating point (little endian)
- @74-77

the bits at position @38-3f are:

| plugin      | @38-3f (binary)                                                            | decimal |
|-------------|----------------------------------------------------------------------------|---------|
| ProtoEffect | `00110001 00000000 00000000 00000000  00000000 00000000 00000000 00000000` | 049 0   |
| InstaLooper | `00010001 00000000 00000000 00000000  00000000 00000000 00000000 00000000` | 017 0   |
| Digits      | `00110000 00000001 00000000 00000000  00000000 00000000 00000000 00000000` | 304 0   |
| BowEcho     | `00110001 00000010 00000000 00000000  00000000 00000000 00000000 00000000` | 561 0   |
| Danaides    | `00110001 00000010 00000000 00000000  00000000 00000000 00000000 00000000` | 561 0   |
| hypercyclic | `00110001 00000011 00000000 00000000  00000000 00000000 00000000 00000000` | 817 0   |
| tonespace   | `00110001 00000011 00000000 00000000  00000000 00000000 00000000 00000000` | 817 0   |

whereas the bits at position @74-77 are:

| plugin      | @74-77 (binary)                       | decimal |
|-------------|---------------------------------------|---------|
| ProtoEffect | `00000001 00000000 00000000 00000000` | 1       |
| InstaLooper | `00000001 00000000 00000000 00000000` | 1       |
| Digits      | `00000001 00000000 00000000 00000000` | 1       |
| BowEcho     | `01101110 00000000 00000000 00000000` | 110     |
| Danaides    | `01100110 00000000 00000000 00000000` | 102     |
| hypercyclic | `11000010 10010110 00000000 00000000` | 38594   |
| tonespace   | `11000011 10111010 00000000 00000000` | 47811   |
|             |                                       |         |


We still have the following fields to find:
- `flags`
- `initialDelay`
- `version`
- `resvd1`, `resvd2`

The `reserved` fields are most likely set to `0`
(all fields reserved for future use in structs, usually don't ever get used).
We know that JUCE (ab?)uses one of them to store a pointer, so they must be 8-bytes (on our 64bit system),
but we don't have any clue yet about their exact position in memory.

Searching for the `initialDelay` in the JUCE-code, it seems that this value is *in samples*,
which should make it an `int` value.
`flags` is certainly `int` (of whatever size, but the more the merrier),
and `version` most likely as well.

Comparing the values we see at positions @38-3f for various plugins seem to indicate a bitfield
(rather big changes in the decimal representations, but when looking at the binary representations
there are not so many changes at all),so for now we assume that this is the `flags` field.
Whether its 32bit or 64bit we don't really know.
However, the size of the flags field will most likely be constant across various architectures
(32bit resp. 64bit), so it probably won't be a "pointer sized int".

The `version` might be encoded in the data at @74-77. This field is (likely)
a free-form field, where the plugin can put anything (with a possible restriction that
it should be "better" (newer) plugins should sort after older ones).
The actual versioning scheme would be left to the plugin authors,
which might explain the wildly varying version numbers we have
(with *hyperbolic* and *tonespace* coming from the same author;
and *BowEcho* and *Danaides* coming from another author;
but all 4 plugins being based on JUCE (which nudges the user into setting a proper version).
Using a version number of `1` as *I don't care* is also not something unheard of...).

Collecting all the info we have so far, we end up with something like the following
(using `pad` variables to account for unknown 0-memory).
There's also a `float1` dummy member (that always holds the value `1.f`),
and the `initialDelay` is set to the seemingly const-value at @50 (`16` in decimal,
which is a plausible value for a sample delay;
btw, loading the *Protoverb* plugin into *REAPER*,
will also print something about a 16samples delay to the stderr).

~~~C
typedef struct AEffect_ {
  t_fstInt32 magic; /* @0 0x56737450, aka 'VstP' */
  char pad1[4]; // always 0
  t_fstEffectDispatcher* dispatcher; // ???
  t_fstEffectProcess* process; // ???
  t_fstEffectGetParameter* getParameter; // ???
  t_fstEffectSetParameter* setParameter; // ???

  t_fstInt32 numPrograms;
  t_fstInt32 numParams;
  t_fstInt32 numInputs;
  t_fstInt32 numOutputs;

  t_fstPtrInt flags; // size unclear
  t_fstPtrInt resvd1; //??
  t_fstPtrInt resvd2; //??
  t_fstInt32 initialDelay; //??

  char pad2[8]; //?
  float float1; //?
  void* object; // FIXXXME
  char pad3[8] pad3; //??

  t_fstInt32 uniqueID; // @112
  t_fstInt32 version;

  t_fstEffectProcessInplace* processReplacing; //?
  t_fstEffectProcessInplaceDbl* processDoubleReplacing; //?

} AEffect;
~~~

If we run the same tests on 32bit plugins, we notice that we got the alignment wrong
in a number of places.

Here's a hexdump of the 1st 96 bytes of a 32bit `AEffect`:

~~~
00000000  50 74 73 56 50 91 8d f7  30 92 8d f7 f0 91 8d f7  |PtsVP...0.......|
00000010  c0 91 8d f7 01 00 00 00  05 00 00 00 02 00 00 00  |................|
00000020  02 00 00 00 31 00 00 00  00 00 00 00 00 00 00 00  |....1...........|
00000030  10 00 00 00 00 00 00 00  00 00 00 00 00 00 80 3f  |...............?|
00000040  c0 ee 56 56 00 00 00 00  56 50 68 75 01 00 00 00  |..VV....VPhu....|
00000050  80 92 8d f7 c0 92 8d f7  00 00 00 00 00 00 00 00  |................|
~~~

As we can see, there is no zero padding after the initial 4-byte `magic`.
Also the number of zeros around the `object` member seems to be off.

Using `pointer sized int` instead of `int32` helps a bit:

~~~
typedef struct AEffect_ {
  t_fstPtrInt magic;
  t_fstEffectDispatcher* dispatcher;  //??
  t_fstEffectProcess* process; //?
  t_fstEffectGetParameter* getParameter; //??
  t_fstEffectSetParameter* setParameter; //??

  t_fstInt32 numPrograms;
  t_fstInt32 numParams;
  t_fstInt32 numInputs;
  t_fstInt32 numOutputs;

  t_fstPtrInt flags;
  t_fstPtrInt resvd1; //??
  t_fstPtrInt resvd2; //??
  t_fstInt32 initialDelay; //??

  char pad2[8]; //??
  float float1; //??
  void* object;
  t_fstPtrInt pad3; //??
  t_fstInt32 uniqueID;
  t_fstInt32 version;

  t_fstEffectProcessInplace* processReplacing; //??
  t_fstEffectProcessInplaceDbl* processDoubleReplacing; //??
} AEffect;
~~~


### conf. REAPER
Printing the contents `AEffect` for the *Protoverb* plugin in gdb, gives something like:

~~~
{magic = 1450406992,
 dispatcher = 0xf78d9150,
 process = 0xf78d9230,
 getParameter = 0xf78d91f0,
 setParameter = 0xf78d91c0,
 numPrograms = 1,
 numParams = 5,
 numInputs = 2,
 numOutputs = 2,
 flags = 49,
 resvd1 = 0,
 resvd2 = 0,
 initialDelay = 16,
 pad2 = "\000\000\000\000\000\000\000",
 myfloat = 1,
 object = 0x5656eec0,
 pad3 = 0,
 uniqueID = 1969770582,
 version = 1,
 processReplacing = 0xf78d9280,
 processDoubleReplacing = 0xf78d92c0
}
~~~

Opening the same plugin in *REAPER* we can also learn a few things:

- 2 in, 2 out
- 1 built-in program named "initialize"
- 5 parameters
  - "main: Output" (100.)
  - "main: Active #FDN" (1.)
  - "FDN: Feedback" (50.)
  - "FDN: Dry" (100.)
  - "FDN: Wet" (30.)
- `stderr`
  - setUniqueID (1969770582)
  - HOST `REAPER` (so the plugin queries the host for the host-name)
  - Starting REVISION 4105
  - sending latency to host... 16
  - GUI: 1200 x 600
  - GUI:  640 x 575
  - CONNECTIONS_PER_PARAMETER: 8


So at least we have the number of inputs, outputs, programs and parameters right, as well as the uniquID.


## flags

We have the following flags to assign:
- `effFlagsHasEditor`
- `effFlagsIsSynth`
- `effFlagsCanDoubleReplacing`
- `effFlagsCanReplacing`
- `effFlagsNoSoundInStop`
- `effFlagsProgramChunks`

While the bitfields at @38 have the following values (displayed as little-endian)
| plugin      | flags               |
|-------------|---------------------|
| ProtoEffect | `00000000 00110001` |
| InstaLooper | `00000000 00010001` |
| Digits      | `00000001 00110000` |
| BowEcho     | `00000010 00110001` |
| Danaides    | `00000010 00110001` |
| hypercyclic | `00000011 00110001` |
| tonespace   | `00000011 00110001` |

Things we know from running the plugins through REAPER:
- all effects except *Digits* have a GUI
- *Protoverb*, *InstaLooper*, *BowEcho*, *Danaides* are all effects (with input and output)
- *Digits*, *hypercyclic*, *tonespace* are all instruments (aka synths; no input)
Things we know from looking at the source code of JUCE-5.4.1
- all JUCE plugins have `effFlagsHasEditor|effFlagsCanReplacing|effFlagsProgramChunks`
- we don't know which JUCE version was used to compile our JUCE plugins!


Comparing this with our binary flag values, we can conclude:

| flag              | value  |
|-------------------|--------|
| effFlagsHasEditor | `1<<1` |
| effFlagsIsSynth   | `1<<9` |


It's a it strange that we have `processReplacing` and `processDoubleReplacing` functions
(according to our hacked up `AEffect` definition) for each plugin,
although there are no more flags that are all set to `true` for each plugin.
This might be a problem with our `AEffect` struct or with the plugins
(or the information about replacing-functions might not be redundant.)

Another observation is that flag `1<<10` is set to `true` for all JUCE-plugins,
and to `false` for the rest.

I don't really know what either `effFlagsNoSoundInStop` nor `effFlagsProgramChunks`
actually mean.
The former is most likely related to time-based plugins (e.g. a reverb; if you pause playback
and reset to some other playback position, then you probably don't want the reverb of the
pre-pause input in your output)


# getting some opcodes
to find out the actual values of the opcodes, we just send various opcodes to the putative `dispatcher`
function of the `AEffect` and see what happens.
This is also a good test to see whether we have the address of the `dispatcher` correct.
I've added a `char[512]` as `ptr` to the dispatcher, because I've seen this in the JUCE's *juce_VSTPluginFormat.cpp*
(`fillInPluginDescription`).

~~~C
for(int i=0; i<256; i++) {
  char buffer[512] = { 0 };
  t_fstPtrInt res = effect->dispatcher (effect, i, 0, 0, buffer, 0);
  const char*str = (const char*)res;
  printf("%d[%d=0x%X]: %.*s\n", i, res, res, 32, str);
  if(*buffer)printf("\t'%.*s'\n", 512, buffer);
}
~~~

Running this on the `Protoverb` looper, crashes with opecode `1`.
For now we just ignore this, and don't run crashing opcodes, with something like:

~~~C
for(int i=0; i<256; i++) {
  bool skip = false;
  switch(i) {
    default: break;
    case 1:
      skip = true;
  }
  if(true)continue;
  t_fstPtrInt res = effect->dispatcher (effect, i, 0, 0, buffer, 0);
  //...
}
~~~

It turns out that the following opcodes '1`, `3`, `4`, `6`, `7`, `8`, `13`,
`14`, `22`, `23`, `24`, `25`, `26`, `29`, `33`, `34`, `35`, `45`, `47`, `48`,
`49`, `51`, `58`, `63`, `69`, `71`, `72' and `73` all crash
(most likely these expect a properly initialized structure at `ptr`).

Additional crashers
- JUCE-based: `3`, `42`, `44`
- Digits: `3`


The rest behaves nicely.

Just looking at our printout (ignoring the `stderr` generated by the plugin itself),
doesn't reveal very much.
The most important thing is probably, that opcode `5` gives us really something interesting in the `buffer`, namely the string
> initialize

...which is the name of the (only) *program* as displayed in REAPER.

Since this sounds promising, we run the code on another plugin (*tonespace*), which has plenty of programs, the first
(according to REAPER) is *EDU-C Major Scale*.
Our code returns (with opcode `5`):
> EDU-C Major Scale

Hooray, we found `effGetProgramName` or `effProgramNameIndexed`
(probably the former, as setting the `index` (and/or `ivalue` resp. `fvalue`)
doesn't make any difference.)

We also notice, that the `t_fstPtrInt` returned by `AEffect.dispatcher` is always `0`.


To proceed, we take a closer look at what else is printed when we run our host with some plugins.
As said before, the `Protoverb` is very verbose (on stderr),
giving us hints on what a certain opcode might be supposed to do.

E.g.

| plugin    | opcode | printout                                                                                 |
|-----------|--------|------------------------------------------------------------------------------------------|
| Protoverb | 10     | AM_AudioMan::reset()                                                                     |
| Protoverb | 12     | AM_VST_base::resume() / AM_AudioMan::reset()                                             |
| Protoverb | 15     | closed editor.                                                                           |
| Protoverb | 59     | plugin doesn't use key, returns false (VST) or jumps to default key handler (WindowProc) |
| Digits    | 11     | Block size 2.0000                                                                        |

it also calls back to the host:


| plugin      | when | opcode | notes                                                                            |
|-------------|------|--------|----------------------------------------------------------------------------------|
| Protoverb   | main | 33     | after that, the VST-version is printed; in REAPER it also prints "HOST 'REAPER'" |
| Protoverb   | main | 13     | right before, it prints "Protoverb VST telling unknown about 16 samples latency" |
|             |      |        | (although no arguments are given to the callback)                                |
| Protoverb   | 12   | 6      | just before the "resume()" comment is issued.                                    |
|-------------|------|--------|----------------------------------------------------------------------------------|
| BowEcho     | 12   | 23     | `(..., 0,     0, NULL, 0.f)` (same for *Danaides*)                               |
| BowEcho     | 12   | 6      | `(..., 0,     1, NULL, 0.f)` (same for *Danaides*)                               |
|-------------|------|--------|----------------------------------------------------------------------------------|
| Digits      | 12   | 6      | `(..., 0,     1, NULL, 0.f)`                                                     |
|-------------|------|--------|----------------------------------------------------------------------------------|
| hypercyclic | 0    | 13     | `(..., 0,     0, NULL, 0.f)`                                                     |
| hypercyclic | 0    | 42     | `(..., 0,     0, NULL, 0.f)`                                                     |
| hypercyclic | 0    | 0      | `(..., 0,     i, NULL, f)` for i in range(numParams) and f=[0.f .. 1.f]          |
| hypercyclic | 0    | 13     | `(..., 0,     0, NULL, 0.f)`                                                     |
| hypercyclic | 0    | 42     | `(..., 0,     0, NULL, 0.f)`                                                     |
| hypercyclic | 0    | 0      | `(..., 0,     i, NULL, f)` for i in range(numParams) and f=[0.f .. 1.f]          |
| hypercyclic | 2    | 0      | `(..., 0,     i, NULL, f)` for i in range(numParams) and f=[0.f .. 1.f]          |
| hypercyclic | 12   | 23     | `(..., 0,     0, NULL, 0.f)`                                                     |
| hypercyclic | 12   | 7      | `(..., 0, 65024, NULL, 0.f)`                                                     |
| hypercyclic | 12   | 6      | `(..., 0,     1, NULL, 0.f)`                                                     |
|-------------|------|--------|----------------------------------------------------------------------------------|
| tonespace   |      |        | almost the same as *hypercyclic*, but with an additional final callback          |
|             |      |        | to `FstHost::dispatcher(eff, 0, 5, 0, NULL, 0.)` whenever we iterated over params|



I guess that host-opcode `33` is meant to query the name of the host application.
In `juce_VSTPluginFormat.cpp` this is handled with
the `audioMasterGetProductString` and `audioMasterGetVendorString` opcodes,
which both return `1` and write a string into the `ptr`.
The string has a maximum length of `min(kVstMaxProductStrLen, kVstMaxVendorStrLen)`,
so these two symbols are actually constants, not enums.
We don't know the actual maximum string lengths, so I tried to compile with `-fsanitize=address -fsanitize=undefined`
and write as many bytes into the return string as possible.
This gave me a maximum string length of `197782` bytes. impressive, but i'm not sure i can trust these values...

The host-opcode `0` seems to be used to tell the host the current values for all the parameters.
In `juce_VSTPluginFormat.cpp::handleCallback()` this is handled in the `audioMasterAutomate` opcode.



# Part: a plugin

~~~C
/* compile with: 'g++ -shared -IFST -g -O0 FstPlugin.cpp -o FstPlugin.so' */
#include <stddef.h>
#include <cstdio>
#include "fst.h"
static t_fstEffectDispatcher*dispatch = 0;
static t_fstPtrInt dispatcher(AEffect*eff, t_fstInt32 opcode, int index, t_fstPtrInt ivalue, void* const ptr, float fvalue) {
  printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)\n", eff, opcode, index, ivalue, ptr, fvalue);
  return 0;
}

static void setParameter(AEffect*eff, int index, float value) {
  printf("FstClient::setParameter(%p)[%d] -> %f\n", eff, index, value);
}
static float getParameter(AEffect*eff, int index) {
  printf("FstClient::getParameter(%p)[%d]\n", eff, index);
  return 0.5;
}
static void process(AEffect*eff, float**indata, float**outdata, int sampleframes) {
  printf("FstClient::process(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
}
static void processReplacing(AEffect*eff, float**indata, float**outdata, int sampleframes) {
  printf("FstClient::process'(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
}
static void processDoubleReplacing(AEffect*eff, double**indata, double**outdata, int sampleframes) {
  printf("FstClient::process2(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
}
extern "C"
AEffect*VSTPluginMain(t_fstEffectDispatcher*dispatch4host) {
  dispatch = dispatch4host;
  printf("FstPlugin::main(%p)\n", dispatch4host);
  AEffect* eff = new AEffect;

  eff->magic = 0x56737450;
  eff->dispatcher = dispatcher;
  eff->process = process;
  eff->getParameter = getParameter;
  eff->setParameter = setParameter;

  eff->numPrograms = 1;
  eff->numParams = 3;
  eff->numInputs = 2;
  eff->numOutputs = 2;
  eff->float1 = 1.;
  eff->object = eff;
  eff->uniqueID = 123456;
  eff->version = 666;

  eff->processReplacing = processReplacing;
  eff->processDoubleReplacing = processDoubleReplacing;
  return eff;
}
~~~

This simple plugin allows us to query the host for whatever opcodes the host understands:

~~~C
for(size_t i = 0; i<64; i++) {
  char buf[512] = {0};
  t_fstPtrInt res = dispatch(0, i, 0, 0, buf, 0);
  if(*buf)
    printf("\t'%.*s'\n", 512, buf);
  if(res)
    printf("\treturned %d\n", res);
}
~~~

With REAPER, this returns:

| op     | result     | buf                               |
| ------ | ---------- | ------------                      |
| *0*    | 1          |                                   |
| *1*    | 2400       |                                   |
| 6      | 1          |                                   |
| 8      | 1          |                                   |
| 10     | 1200000    |                                   |
| 11     | 65536      |                                   |
| 12     | 1          |                                   |
| 13     | 1          |                                   |
| 23     | 1          |                                   |
| *32*   | 1          | Cockos                            |
| *33*   | 1          | REAPER                            |
| *34*   | 5965       |                                   |
| 42     | 1          |                                   |
| 43     | 1          |                                   |
| 44     | 1          |                                   |
| 48     | 1          | ~/Documents/REAPER Media/test.RPP |
|        |            |                                   |

This table confirms that host-opcode `33` is `audioMasterGetProductString`,
and we learn that host-opcode `32` is `audioMasterGetVendorString`.
The number `5969` happens to be the version of REAPER (`5.965`), so
host-opcode `34` is likely `audioMasterGetVendorVersion`.

The opcode `48` returns the currently opened REAPER session file.
The `audioMasterGetDirectory` might match, but REAPER returns a
file rather than a diretory.

TODO: 10, 11

# AEffect initialisation from host

## startup

After the `VSTPluginMain` function has returned to the host,
REAPER calls our plugin a couple or times, with various opcodes:

> FstClient::dispatcher(0x3098d20, 0, 0, 0, (nil), 0.000000)
> FstClient::dispatcher(0x3098d20, 2, 0, 0, (nil), 0.000000)
> FstClient::dispatcher(0x3098d20, 5, 0, 0, 0x7fffe1b4a2f0, 0.000000)
> [...]
> FstClient::dispatcher(0x3098d20, 10, 0, 0, (nil), 44100.000000)
> FstClient::dispatcher(0x3098d20, 11, 0, 512, (nil), 0.000000)
> FstClient::dispatcher(0x3098d20, 12, 0, 1, (nil), 0.000000)
> FstClient::dispatcher(0x3098d20, 35, 0, 0, (nil), 0.000000)
> FstClient::dispatcher(0x3098d20, 45, 0, 0, 0x7fffe1b4a530, 0.000000)
> FstClient::dispatcher(0x3098d20, 47, 0, 0, 0x7fffe1b4a530, 0.000000)
> FstClient::dispatcher(0x3098d20, 51, 0, 0, 0xab4617, 0.000000)
> [...]
> FstClient::dispatcher(0x3098d20, 58, 0, 0, (nil), 0.000000)
> FstClient::dispatcher(0x3098d20, 71, 0, 0, (nil), 0.000000)

Opcode `10` looks like the host samplerate (`effSetSampleRate`),
and `11` looks like the blocksize (`effSetBlockSize`).

Opcode `5` is what we already suspect to be `effGetProgramName`,
which we can now confirm by implementing it.
If we do return a nice string for opcode 5,
REAPER will then call opcode `2` with a varying index and then calls opcode `5` again
(so supposedly it tries to change the program and query the new program name.
In theory it should use `effGetProgramNameIndexed` instead...

To find out what the pointers store, we can just try to print them out
(making sure to only print the first N bytes, in order to not flood the terminal).
For most of the opcodes this doesn't print anything (so the `ptr` points to a properly
zero-initialised memory region), only for opcode `51` we get something:

- `hasCockosExtensions`
- `hasCockosNoScrollUI`
- `receiveVstEvents`
- `receiveVstMidiEvent`
- `sendVstEvents`
- `sendVstMidiEvent`
- `wantsChannelCountNotifications`

This looks very much like a generic interface to either query a property by name
(could be `effCanDo`) or to allow vendor-specific programs (could be `effVendorSpecific`).
Looking up how JUCE handles the `effCanDo` in *juce_VST_Wrapper.cpp*, we see that
it really takes the `ptr` argument as string and compares it to values like
`bypass`, `sendVstEvents` and `hasCockosExtensions`. Bingo!


## inserting the plugin

If we add our new plugin to a REAPER track, we see printout like the following
(btw: for now i've muted the output for opcodes `53` and `3` as they are spamming
the console):

> FstClient::setParameter(0x27967d0)[0] -> 0.000000
> FstClient::setParameter(0x27967d0)[0] -> 0.000000
> FstClient::dispatcher(0x27967d0, 8, 0, 0, 0x7ffeca15b580, 0.000000)
> FstClient::dispatcher(0x27967d0, 7, 0, 0, 0x7ffeca15b480, 0.000000)
> FstClient::dispatcher(0x27967d0, 6, 0, 0, 0x7ffeca15b480, 0.000000)
> FstClient::setParameter(0x27967d0)[1] -> 279445823603548880896.000000
> FstClient::setParameter(0x27967d0)[1] -> 0.000000
> FstClient::dispatcher(0x27967d0, 8, 1, 0, 0x7ffeca15b580, 0.000000)
> FstClient::dispatcher(0x27967d0, 7, 1, 0, 0x7ffeca15b480, 0.000000)
> FstClient::dispatcher(0x27967d0, 6, 1, 0, 0x7ffeca15b480, 0.000000)
> FstClient::setParameter(0x27967d0)[2] -> 279445823603548880896.000000
> FstClient::setParameter(0x27967d0)[2] -> 0.000000
> FstClient::dispatcher(0x27967d0, 8, 2, 0, 0x7ffeca15b580, 0.000000)
> FstClient::dispatcher(0x27967d0, 7, 2, 0, 0x7ffeca15b480, 0.000000)
> FstClient::dispatcher(0x27967d0, 6, 2, 0, 0x7ffeca15b480, 0.000000)

Since the opcodes `6`, `7` and `8` are the only ones that use the index
(with an index that goes until `numParams-1`), they are likely to be
parameter related. Conincidientally we have 3 parameter-related opcodes:
`effGetParamLabel`, `effGetParamDisplay` and `effGetParamName`.

If we respond to these opcodes with some strings (by copying them into `object`) and
display the generic FX-GUI in reaper, we notice that our strings appear besides the
three parameter sliders (`8` to the left, and `7` followed by `6` to the right).
For other plugins, these strings read like "Gain" (on the left) and "+0.00 dB" (on the right).
We can pretty safely assume that *Name* refers to the human readable name of the parameter (e.g. "Gain"),
so *Display* would be a nice string rendering of the value itself (e.g. "+0.00")
and *Label* would be the units (e.g. "dB").

Playing around with the sliders and watching the calls to `setParameter`,
we see that whenever we move the slider, both the setter- and the getter-function are called.
However, we get bogus values, e.g.:

> FstClient::setParameter(0x2a88690)[0] -> -27706476332321908694659177423680045056.000000

This looks like the `setParameter` callback is not called correctly.
Remember, that we used arbitrary positions for the function pointers in the `AEffect` structure
(we were just lucky that the `dispatcher` function was at the correct place).
This means that the host might call the `setParameter` function without providing
any explicit `value` argument at all )e.g. because the host things this is really the getter).
So let's play with those, and just revert the order of the two functions:

~~~C
typedef struct AEffect_ {
  t_fstInt32 magic;
  t_fstEffectDispatcher* dispatcher;
  t_fstEffectProcess* process;
  t_fstEffectSetParameter* setParameter;
  t_fstEffectGetParameter* getParameter;
  // ...
}
~~~

And indeed, this seems to have done the trick!

When we move a slider, both parameter getter and setter are called as well as the opcodes `6`, `7` & `8`.
About 0.5 seconds after the last parameter change happened, the host calls opcode `5` (which we already believe
to be `effGetProgramName`).

# Part: more enums

## eff[...]Program[...]
If we set the `numPrograms` to some identifiably value (e.g. 5) and open the plugin with REAPER,
we can see that it does something like, the following when opening up the generic GUI:

~~~
FstClient::dispatcher(0x3339ff0, 2, 0, 0, (nil), 0.000000)
FstClient::dispatcher(0x3339ff0, 5, 0, 0, 0x7ffc1b29af60, 0.000000)
FstClient::dispatcher(0x3339ff0, 2, 0, 1, (nil), 0.000000)
FstClient::dispatcher(0x3339ff0, 5, 0, 0, 0x7ffc1b29af60, 0.000000)
FstClient::dispatcher(0x3339ff0, 2, 0, 2, (nil), 0.000000)
FstClient::dispatcher(0x3339ff0, 5, 0, 0, 0x7ffc1b29af60, 0.000000)
FstClient::dispatcher(0x3339ff0, 2, 0, 3, (nil), 0.000000)
FstClient::dispatcher(0x3339ff0, 5, 0, 0, 0x7ffc1b29af60, 0.000000)
FstClient::dispatcher(0x3339ff0, 2, 0, 4, (nil), 0.000000)
FstClient::dispatcher(0x3339ff0, 5, 0, 0, 0x7ffc1b29af60, 0.000000)
FstClient::dispatcher(0x3339ff0, 2, 0, 0, (nil), 0.000000)
~~~

If we write a different string into the `ptr` for each call to opcode `5`,
we can see that the generic GUI displays our strings as the program names.
So it really seems that REAPER makes each program current and then
queries the current program name in order to retrieve all program names.
Which confirms that `effGetProgramName` is `5` and introduces
`effSetProgram` as `2`.

We can now easily find the opcode for the corresponding `effSetProgramName`,
by passing a unique string (`"program#%02d" % opcode`) for the opcodes 0..10.
If we call `effGetProgramName` afterwards, we see that the program name is now
*program#04*, so `effSetProgramName` is `4`.

We have `effSetProgram`, `effSetProgramName` and `effGetProgramName` as `2`, `4` and `5` resp.,
so probably `effGetProgram` is `3`.
This can easily be confirmed by a little test program that first changes the program
to a known value (using a plugin that actually has that many programs!) via `effSetProgram`,
and then calling opcode `3` to get that value back (as returned from the dispatcher).


## effEdit*

The *Protoverb* plugin prints information:

| opcode      | printout                 | opcodeName |
|-------------|--------------------------|------------|
| 13          | "AM_VST_Editor::getRect 1200 x 600" | effEditGetRect? |
| 14          | "AM_VST_Editor::open"    | effEditOpen? |
| 15          | "closed editor."         | effEditClose? |

Opcode `13` does not crash if we provide it a ptr to some memory,
and it will write some address into that provided memory.
It will also return that same address (just what JUCE does for the `effEditGetRect` opcode.)
If we read this address as `ERect` we get a garbage rectangle (e.g. `0x78643800+0+0`).
The memory looks like

> 00 00 00 00 58 02 B0 04  00 00 00 00 00 00 00 00

Given that a GUI rectangle will most likely not have a zero width or height,
it might be, that the `ERect` members are really only 2-bytes wide (`short`),
which would then translate to the numbers `0, 0, 600, 1200`,
which happens to align nicely with the the printout of *Protoverb*

~~~C
typedef struct ERect_ {
  short left;
  short top;
  short right;
  short bottom;
} ERect;
~~~

## effGetProgramNameIndexed

the following little helper gives us information, what a plugin returns for the various opcodes:

~~~C
  for(size_t opcode=16; opcode<64; opcode++) {
    char buffer[200] = { 0 };
    t_fstPtrInt result = effect->dispatcher(effect, opcode, 0, 0, buffer, 0.f);
    if(result || *buffer)
      printf("tested %d\n", opcode);
    if(result)
      printf("\t%llu 0x%llX\n", result, result);
    if(*buffer) {
      printf("\tbuffer '%.*s'\n", 512, buffer);
      hexprint(buffer, 16);
    }
  }
~~~

I get:

> tested 22	 | 1316373862 0x4E764566
> tested 23  |  791 0x317 |  buffer '�  ��U'
> tested 24  |  1 0x1
> tested 25  |  1 0x1
> tested 26  |  1 0x1
> tested 29  |  1 0x1     |  buffer 'initialize'
> tested 33  |  1 0x1     |  buffer 'Protoverb-In0'
> tested 34  |  1 0x1     |  buffer 'Protoverb-Out1'
> tested 35  |  1 0x1
> tested 45  |  1 0x1     |  buffer 'Protoverb'
> tested 47  |  1 0x1     |  buffer 'u-he'
> tested 48  |  1 0x1     |  buffer 'Protoverb 1.0.0'
> tested 49  |  65536 0x10000
> tested 51  |  18446744073709551615 0xFFFFFFFFFFFFFFFF
> tested 58  |  2400 0x960
> tested 63  |  18446744073709551615 0xFFFFFFFFFFFFFFFF

The "initialize" string we already know as Protoverbs sole program name.
The only missing opcode for retrieving program names is `effGetProgramNameIndexed`.

We can confirm this assumption with a plugin that has many programs (e.g. *tonespace*)
and the following snippet:

~~~C
  for(int i=0; i<effect->numPrograms; i++) {
    char buffer[200] = { 0 };
    effect->dispatcher(effect, 29, i, 0, buffer, 0.f);
    if(*buffer) {
      printf("\tbuffer#%d '%.*s'\n", i, 512, buffer);
    }
  }
~~~

## effectName, productString, vendorString, vendorVersion

In the [C-snippet above](#effgetprogramnameindexed), we also get nice strings for
opcodes `45`, `47`, and `48`, that look like they would be good matches for
`effGetEffectName` (45), `effGetProductString` (48) and `effGetVendorString` (47).

The value returned by 49, looks suspiciously like the version:

| plugin      | AEffect.version | returned by `49` |
|-------------|-----------------|------------------|
| BowEcho     | 110             | 110              |
| Danaides    | 102             | 102              |
| Digits      | 1               |                  |
| InstaLooper | 1               | 1                |
| Protoverb   | 1               | 0x10000          |
| hypercyclic | 150             | 150              |
| tonespace   | 250             | 250              |

So we assign `effGetVendorVersion` the value of `49`
