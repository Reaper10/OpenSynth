/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.1.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ModulationMatrix.h"
#include "OscillatorParameterContainer.h"
//[/Headers]

#include "OscillatorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OscillatorComponent::OscillatorComponent (OpenSynthAudioProcessorEditor &editor, OscillatorParameterContainer* parameterContainer, ModulationMatrix* modulationMatrix)
    : editor(editor), parameterContainer(parameterContainer), modulationMatrix(modulationMatrix)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (oscComboBox = new ComboBox ("Osc Combo Box"));
    oscComboBox->setEditableText (false);
    oscComboBox->setJustificationType (Justification::centredLeft);
    oscComboBox->setTextWhenNothingSelected (TRANS("Waveform"));
    oscComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    oscComboBox->addItem (TRANS("Sine"), 1);
    oscComboBox->addItem (TRANS("Sawtooth"), 2);
    oscComboBox->addItem (TRANS("Square"), 3);
    oscComboBox->addItem (TRANS("Triangle"), 4);
    oscComboBox->addListener (this);

    addAndMakeVisible (gainLabel = new Label ("Gain Label",
                                              TRANS("Gain")));
    gainLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    gainLabel->setJustificationType (Justification::centredLeft);
    gainLabel->setEditable (false, false, false);
    gainLabel->setColour (Label::textColourId, Colours::black);
    gainLabel->setColour (TextEditor::textColourId, Colours::black);
    gainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (titleLabel = new Label ("Title Label",
                                               TRANS("Oscillator")));
    titleLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    titleLabel->setJustificationType (Justification::centredLeft);
    titleLabel->setEditable (false, false, false);
    titleLabel->setColour (Label::textColourId, Colours::black);
    titleLabel->setColour (TextEditor::textColourId, Colours::black);
    titleLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc1SemiLabel = new Label ("Osc 1 Semi Label",
                                                  TRANS("Semi")));
    osc1SemiLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    osc1SemiLabel->setJustificationType (Justification::centredLeft);
    osc1SemiLabel->setEditable (false, false, false);
    osc1SemiLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1SemiLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (osc1CentsLabel = new Label ("Osc 1 Cents Label",
                                                   TRANS("Cents")));
    osc1CentsLabel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    osc1CentsLabel->setJustificationType (Justification::centredLeft);
    osc1CentsLabel->setEditable (false, false, false);
    osc1CentsLabel->setColour (TextEditor::textColourId, Colours::black);
    osc1CentsLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (oscGainSlider = new ModulatedComponent (editor, *parameterContainer->getGainParameter(), parameterContainer->getGainParameterID()));
    oscGainSlider->setName ("Osc Gain Slider");

    addAndMakeVisible (oscSemiSlider = new ModulatedComponent (editor, *parameterContainer->getSemiParameter(), parameterContainer->getSemiParameterID()));
    oscSemiSlider->setName ("Osc Semi Slider");

    addAndMakeVisible (oscCentSlider = new ModulatedComponent (editor, *parameterContainer->getCentsParameter(), parameterContainer->getCentsParameterID()));
    oscCentSlider->setName ("Osc Cent Slider");


    //[UserPreSize]
    oscGainSlider->getSlider()->setRange (0, 1, 0);
    oscGainSlider->getSlider()->setSliderStyle (Slider::Rotary);
    oscGainSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    oscGainSlider->setColour (Slider::thumbColourId, Colour (0xff04042a));
    oscGainSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fc30d0d));
    oscGainSlider->setListener(this);

    oscSemiSlider->getSlider()->setRange (0, 1, 0);
    oscSemiSlider->getSlider()->setSliderStyle (Slider::LinearHorizontal);
    oscSemiSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 40, 20);
    oscSemiSlider->setColour (Slider::thumbColourId, Colour (0xff04042a));
    oscSemiSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fc30d0d));
    oscSemiSlider->setListener(this);

    oscCentSlider->getSlider()->setRange (0, 1, 0);
    oscCentSlider->getSlider()->setSliderStyle (Slider::LinearHorizontal);
    oscCentSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 40, 20);
    oscCentSlider->setColour (Slider::trackColourId, Colours::white);
    oscCentSlider->setListener(this);
    //[/UserPreSize]

    setSize (210, 200);


    //[Constructor] You can add your own custom stuff here..
    oscComboBox->setSelectedId(2);

    idToComponent[parameterContainer->getGainParameterID()] = oscGainSlider.get();
    idToComponent[parameterContainer->getSemiParameterID()] = oscSemiSlider.get();
    idToComponent[parameterContainer->getCentsParameterID()] = oscCentSlider.get();
    //[/Constructor]
}

OscillatorComponent::~OscillatorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    oscComboBox = nullptr;
    gainLabel = nullptr;
    titleLabel = nullptr;
    osc1SemiLabel = nullptr;
    osc1CentsLabel = nullptr;
    oscGainSlider = nullptr;
    oscSemiSlider = nullptr;
    oscCentSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OscillatorComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    {
        int x = 0, y = 53, width = 155, height = 51;
        Colour fillColour = Colour (0xff39bccf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 0, y = 100, width = 155, height = 51;
        Colour fillColour = Colour (0xff10d2ee);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    //[/UserPaint]
}

void OscillatorComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    oscComboBox->setBounds (8, 24, 104, 24);
    gainLabel->setBounds (168, 72, 40, 24);
    titleLabel->setBounds (24, 0, 79, 24);
    osc1SemiLabel->setBounds (112, 56, 40, 24);
    osc1CentsLabel->setBounds (112, 104, 48, 24);
    oscGainSlider->setBounds (160, 8, 48, 64);
    oscSemiSlider->setBounds (8, 56, 112, 64);
    oscCentSlider->setBounds (8, 104, 112, 64);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OscillatorComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == oscComboBox)
    {
        //[UserComboBoxCode_oscComboBox] -- add your combo box handling code here..
        parameterContainer->setWaveform(waveformForId(oscComboBox->getSelectedId()));
        //[/UserComboBoxCode_oscComboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void OscillatorComponent::itemDropped(const int sourceID, const int destinationID)
{
    modulationMatrix->connect(sourceID, destinationID);
    idToComponent[destinationID]->update();
}

Waveform OscillatorComponent::waveformForId(int waveformId)
{
    switch (waveformId) {
        case 1:
            return WaveformSine;
        case 2:
            return WaveformSawtooth;
        case 3:
            return WaveformSquare;
        case 4:
            return WaveformTriangle;
        default:
            return WaveformSine;
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OscillatorComponent" componentName=""
                 parentClasses="public Component, public DragAndDropListener"
                 constructorParams="OpenSynthAudioProcessorEditor &amp;editor, OscillatorParameterContainer* parameterContainer, ModulationMatrix* modulationMatrix"
                 variableInitialisers="editor(editor), parameterContainer(parameterContainer), modulationMatrix(modulationMatrix)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="210" initialHeight="200">
  <BACKGROUND backgroundColour="ffffffff">
    <RECT pos="0 53 155 51" fill="solid: ff39bccf" hasStroke="0"/>
    <RECT pos="0 100 155 51" fill="solid: ff10d2ee" hasStroke="0"/>
  </BACKGROUND>
  <COMBOBOX name="Osc Combo Box" id="b7704cccd5f9a10e" memberName="oscComboBox"
            virtualName="" explicitFocusOrder="0" pos="8 24 104 24" editable="0"
            layout="33" items="Sine&#10;Sawtooth&#10;Square&#10;Triangle"
            textWhenNonSelected="Waveform" textWhenNoItems="(no choices)"/>
  <LABEL name="Gain Label" id="d01ee9ac72b6cecf" memberName="gainLabel"
         virtualName="" explicitFocusOrder="0" pos="168 72 40 24" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Gain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="Title Label" id="5be71bd636933011" memberName="titleLabel"
         virtualName="" explicitFocusOrder="0" pos="24 0 79 24" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Oscillator" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="Osc 1 Semi Label" id="a15acc2dcec0c75d" memberName="osc1SemiLabel"
         virtualName="" explicitFocusOrder="0" pos="112 56 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Semi" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="Osc 1 Cents Label" id="4a616d42488b077f" memberName="osc1CentsLabel"
         virtualName="" explicitFocusOrder="0" pos="112 104 48 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Cents" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="Osc Gain Slider" id="55ba71ccbe11a612" memberName="oscGainSlider"
                    virtualName="ModulatedComponent" explicitFocusOrder="0" pos="160 8 48 64"
                    class="Component" params="editor, *parameterContainer-&gt;getGainParameter(), parameterContainer-&gt;getGainParameterID()"/>
  <GENERICCOMPONENT name="Osc Semi Slider" id="429ec19e3dd99c8a" memberName="oscSemiSlider"
                    virtualName="ModulatedComponent" explicitFocusOrder="0" pos="8 56 112 64"
                    class="Component" params="editor, *parameterContainer-&gt;getSemiParameter(), parameterContainer-&gt;getSemiParameterID()"/>
  <GENERICCOMPONENT name="Osc Cent Slider" id="d183e4205188de14" memberName="oscCentSlider"
                    virtualName="ModulatedComponent" explicitFocusOrder="0" pos="8 104 112 64"
                    class="Component" params="editor, *parameterContainer-&gt;getCentsParameter(), parameterContainer-&gt;getCentsParameterID()"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
