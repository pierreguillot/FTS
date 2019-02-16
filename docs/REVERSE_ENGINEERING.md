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
t_fstPtrInt dispatcher(AEffect*, int opcode, int index, t_fstPtrInt value, void* const ptr, float opt);
void setParameter(AEffect*, int index, float value);
float getParameter(AEffect*, int index);
void process(AEffect*, float**indata, float**outdata, int sampleframes);
void processReplacing(AEffect*, float**indata, float**outdata, int sampleframes);
void processReplacingDouble(AEffect*, double**indata, double**outdata, int sampleframes);
~~~

And we end up with something like:

~~~C
typedef long t_fstPtrInt; /* pointer sized int */
##define VSTCALLBACK

 /* dispatcher(effect, opcode, index, value, ptr, opt) */
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


# Part3: assigning values to enums
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
t_fstPtrInt dispatcher (AEffect* effect, int opcode, int index, t_fstPtrInt value, void*ptr, float opt) {
  printf("FstHost::dispatcher(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, value, ptr, opt);
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
t_fstPtrInt dispatcher (AEffect* effect, int opcode, int index, t_fstPtrInt value, void*ptr, float opt) {
  printf("FstHost::dispatcher(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, value, ptr, opt);
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

If we run this repeatedly on the same plugin file, we can collect multiple dumps
(don't forget to rename them, else they will be overwritten).
Comparing those multiple dumps, we can see that many bytes stay the same, but a few
are changing for each run.

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

Comparing the changing values with the addresses of heap-allocated memory and functions
(e.g. the DLL-handle returned by `dlopen()` and the function pointer returned by `dlsym()`)
it becomes clear, that we have 6 function pointers
(e.g. `?? ad 87 f7 ff 7f 00 00` at @8, @10, @18, @20, @78, @80) and
1 heap allocated object (e.g. `c0 fb 58 55 55 55 00 00` at @60).
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
Additionally, we notice that the `Protoverb` object (being pretty verbose on the stderr)
said something like:
> setUniqueID (1969770582)

This number is `0x75685056` in hex. If we compare that to the bytes @70,
we cannot help but note that we have discovered the location of the `uniqueID` member.


InstaLooper(1.2)|Protoverb(1.0/4105)
00-03: magic-number
04-07: 0
08-0F: a pointer [dispatch/process/getParam/setParam]
10-17: a pointer [dispatch/process/getParam/setParam]
18-1F: a pointer [dispatch/process/getParam/setParam]
20-27: a pointer [dispatch/process/getParam/setParam]
28-2F: 01.00.00.00.18.00.00.00|01.00.00.00.05.00.00.00 [numPrograms, numParams?]
30-37: 02.00.00.00.02.00.00.00 [numInputs, numOutputs?]
38-3F: 11.0000|31.0000 [flags?]
40-47: 0
48-4F: 0
50-57: 0
58-5F: 0000.80.f3 == 00.00.00.00 + (float)1.
60-67: pointer to (&effect-48) [object?]
68-6F: 0
70-73: unique-id
74-77: 01.00.00.00 [version?]
78-7F: a pointer [processRepl/processDblRepl]
80-87: a pointer [processRepl/processDblRepl]
88-8F: 0

Digits:30-37: 0/2 (no in, 2 out?)
Digits:38-3F: 30.01.00000
Digits:74-77: 01.00.00.00

BowEcho:30-37: 2/2
BowEcho:38-3F: 31.02.0000
BowEcho:74-77: 6E.00.00.00

Danaides:30-37: 2/2
Danaides:38-3F: 31.02.0000
Danaides:74-77: 66.00.00.00

hypercyclic:30-37: 0/2
hypercyclic:38-3F: 31.03.0000
hypercyclic:74-77: 96.00.00.00

tonespace:30-37: 0/2
tonespace:38-3F: 31.03.0000
tonespace:74-77: FA.00.00.00
