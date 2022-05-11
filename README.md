# group13-hw-JUCE-FourierSerious
Group 13 repository for the JUCE homework of CMLS 2022

# Group FourierSerious - Components:
- Manuel Alejandro Jaramillo Rodríguez
- Marcello Grati
- Maria Gracia Fernandez
- Silvia Pasin
- Natasa Popovic

# Project description:
This is an implementation of a synthesiser based on subtractive synthesis. This type of synthesis starts from a waveform at which a filter is applied. The type of waveform can be chosen by the user (between sine, square, sawtooth, triangle and white noise) and its frequency is controlled by a MIDI input. The user can also choose between three types of filter (lowpass, highpass and bandpass) and can modify some parameters of it. 

# Instructions:
In order to use the plugin, you need to:
1. download the code
2. open the file "SubSynth.jucer" in Projucer
3. open it with an IDE (for example Xcode, Visual Studio..)
4. build the project inside the IDE
5. use the plugin in a music software

Alternatively, for MacOS users just download the .zip file containing the image file. 
Once opened, drag and drop the plugin in the Components directory and start to use it in your favorite DAW.

# Interface:

![GUI](https://github.com/polimi-cmls-22/group13-hw-JUCE-FourierSerious/blob/main/GUI.jpg?raw=true)

The GUI interface allows the user to control some parameters of the synthesiser. At the top left we find a selector through which we can choose the type of waveform between sine, square, sawtooth, triangle and white noise. Below this, we can choose the type of filter that we want to apply to the signal, among the lowpass, the highpass and the bandpass. For each filter we can set the cutoff frequency and the resonance with the sliders. In the right part, there are the sliders that allows to control the attack, delay, sustain and release parameters of the ADSR envelope applied to the amplitude of the signal and to the filter. At the bottom, there is a waveform viewer that allows to see the temporal evolution of the signal.

