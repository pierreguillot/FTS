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

# getting errors

building the AudioPluginHost  gets us a lot of errors.
these errors are interesting, as this is what we want to implement in the `fst.h` headers.
luckily JUCE will attempt to put everything in the `Vst2` namespace, making it easier to find the
interesting broken things.

So lets start, by extracting all the symbols that should be part of the `Vst2` namespace (but are not):

~~~
make 2>&1 | grep error: | grep "is not a member of .Vst2.$"| sed -e 's|.* error: ||' | sed -e 's|^.||' -e 's|. .*||' | sort -u
~~~