/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()  : level(0.0f)
{
    setSize (800, 600);
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio)) {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (0, 2); });
    } else {
        setAudioChannels (0, 2);
    }
    
    decibelSlider.setValue(Decibels::gainToDecibels(level, -96.0f));
    decibelSlider.setRange(-100, -12);
    decibelSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 20);
    //levelSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(decibelSlider);
    decibelSlider.onValueChange = [this] {
        level = Decibels::decibelsToGain((float)decibelSlider.getValue());
    };
    
    levelLabel.setText("Noise Level in dB", dontSendNotification);
    levelLabel.attachToComponent(&decibelSlider, true);
    addAndMakeVisible(levelLabel);
}

MainComponent::~MainComponent() {
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
    //bufferToFill.clearActiveBufferRegion();
    
    auto levelScale = level * 2.0f;
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
            auto r = random.nextFloat() * levelScale - level;
            buffer[sample] = r;
        }
    }
}

void MainComponent::releaseResources() {
}

//==============================================================================
void MainComponent::paint (Graphics& g) {
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized() {
    decibelSlider.setBounds(120, 10, getWidth()-130, 20);
}

//==============================================================================
double DecibelSlider::getValueFromText (const String& text) {
    auto decibelText = text.upToFirstOccurrenceOf("dB", false, false).trim();
    auto minusInfinitydB = -96.0;
    return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB : decibelText.getDoubleValue();
}

String DecibelSlider::getTextFromValue (double value) {
    return Decibels::toString(value);
}
