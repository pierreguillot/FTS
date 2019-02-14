/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
int main (int argc, char* argv[]) {
     KnownPluginList knownPluginList;
     VSTPluginFormat formatToScan;
     FileSearchPath paths;
    // ..your code goes here!

    PluginDirectoryScanner scanner(knownPluginList, formatToScan, paths, true, File(), true);
    return 0;
}
