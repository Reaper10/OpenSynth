/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "LFOComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LFOComponent::LFOComponent (LFOParameterContainer& parameterContainer)
    : parameterContainer(parameterContainer)
{
    //[Constructor_pre] You can add your own custom stuff here..
    addAndMakeVisible (frequencySlider = new ParameterSlider (*parameterContainer.getFrequencyParameter(), ParameterIDLFO1Frequency));
    //[/Constructor_pre]

    addAndMakeVisible (lfoTitleLabel = new Label ("LFO Title Label",
                                                  TRANS("LFO")));
    lfoTitleLabel->setFont (Font (18.00f, Font::plain).withTypefaceStyle ("Regular"));
    lfoTitleLabel->setJustificationType (Justification::centredLeft);
    lfoTitleLabel->setEditable (false, false, false);
    lfoTitleLabel->setColour (Label::backgroundColourId, Colour (0xffd05555));
    lfoTitleLabel->setColour (TextEditor::textColourId, Colours::black);
    lfoTitleLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    
    frequencySlider->setRange (0, 1, 0);
    frequencySlider->setSliderStyle (Slider::Rotary);
    frequencySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Frequency\n")));
    label->setFont (Font (13.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colours::black);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (waveformComboBox = new ComboBox ("Waveform ComboBox"));
    waveformComboBox->setEditableText (false);
    waveformComboBox->setJustificationType (Justification::centredLeft);
    waveformComboBox->setTextWhenNothingSelected (TRANS("Waveform"));
    waveformComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    waveformComboBox->addItem (TRANS("Sine"), 1);
    waveformComboBox->addItem (TRANS("Saw"), 2);
    waveformComboBox->addItem (TRANS("Square"), 3);
    waveformComboBox->addItem (TRANS("Triangle"), 4);
    waveformComboBox->addListener (this);

    addAndMakeVisible (modulationPlug = new ModulationPlug());
    modulationPlug->setName ("Modulation Plug");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (180, 100);


    //[Constructor] You can add your own custom stuff here..
    waveformComboBox->setSelectedId(1);
    modulationPlug->setID(parameterContainer.getID());
    //[/Constructor]
}

LFOComponent::~LFOComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lfoTitleLabel = nullptr;
    frequencySlider = nullptr;
    label = nullptr;
    waveformComboBox = nullptr;
    modulationPlug = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LFOComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LFOComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    lfoTitleLabel->setBounds (0, 0, 184, 24);
    frequencySlider->setBounds (1, 26, 55, 56);
    label->setBounds (0, 72, 72, 24);
    waveformComboBox->setBounds (80, 64, 88, 24);
    modulationPlug->setBounds (144, 32, 23, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LFOComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == waveformComboBox)
    {
        //[UserComboBoxCode_waveformComboBox] -- add your combo box handling code here..
        switch (waveformComboBox->getSelectedId() - 1)
        {
            case WaveformSine:
                parameterContainer.setWaveform(WaveformSine);
                break;
            case WaveformSawtooth:
                parameterContainer.setWaveform(WaveformSawtooth);
                break;
            case WaveformSquare:
                parameterContainer.setWaveform(WaveformSquare);
                break;
            case WaveformTriangle:
                parameterContainer.setWaveform(WaveformTriangle);
                break;
            default:
                break;
        }
        //[/UserComboBoxCode_waveformComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LFOComponent" componentName=""
                 parentClasses="public Component" constructorParams="LFOParameterContainer&amp; parameterContainer"
                 variableInitialisers="parameterContainer(parameterContainer)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="180" initialHeight="100">
  <BACKGROUND backgroundColour="ffffffff"/>
  <LABEL name="LFO Title Label" id="d57d9b5342a6dbf8" memberName="lfoTitleLabel"
         virtualName="" explicitFocusOrder="0" pos="0 0 184 24" bkgCol="ffd05555"
         edTextCol="ff000000" edBkgCol="0" labelText="LFO" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="18" kerning="0" bold="0" italic="0" justification="33"/>
  <SLIDER name="LFO Frequency Slider" id="11528b7f3ccd952" memberName="frequencySlider"
          virtualName="ParameterSlider" explicitFocusOrder="0" pos="1 26 55 56"
          min="0" max="1" int="0" style="Rotary" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="0"/>
  <LABEL name="new label" id="d745b67132d73127" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="0 72 72 24" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="Frequency&#10;" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="13" kerning="0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="Waveform ComboBox" id="38e78ad41cb58384" memberName="waveformComboBox"
            virtualName="" explicitFocusOrder="0" pos="80 64 88 24" editable="0"
            layout="33" items="Sine&#10;Saw&#10;Square&#10;Triangle" textWhenNonSelected="Waveform"
            textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="Modulation Plug" id="27df8c89727bce37" memberName="modulationPlug"
                    virtualName="ModulationPlug" explicitFocusOrder="0" pos="144 32 23 24"
                    class="Component" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]