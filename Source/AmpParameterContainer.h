/*
  ==============================================================================

    AmpParameterContainer.h
    Created: 4 Dec 2016 7:37:46pm
    Author:  Alexander Heemann

  ==============================================================================
*/

#ifndef AMPPARAMETERCONTAINER_H_INCLUDED
#define AMPPARAMETERCONTAINER_H_INCLUDED

#include "ParameterContainer.h"

class AmpParameterContainer: public ParameterContainer
{
public:
    AmpParameterContainer(int ID,
                          OpenSynth& synth,
                          OpenSynthAudioProcessor& processor): ParameterContainer(ID,
                                                                                  synth,
                                                                                  processor)
    {
        gainParameterID = synth.getIDManager().getNewID();
        processor.addParameter(gain = new AudioParameterFloat("amp_gain", "Amp Gain", 0.0, 1.0, 0.3));
    }
    
    ~AmpParameterContainer() {}
    
    const int getGainParameterID() const { return gainParameterID; }
    
    AudioParameterFloat* getGainParameter() const { return gain; }
    
private:
    int gainParameterID = 0;
        
    AudioParameterFloat* gain;
};



#endif  // AMPPARAMETERCONTAINER_H_INCLUDED
