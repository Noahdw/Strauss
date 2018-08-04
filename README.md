# MidiInter



MidiInter is the beginning of what will hopefully one day be a Digital Audio Workstation that focuses
on composer workflow through the integration of both notation software such as Musescore as well as traditional DAWS such as Abelton. This DAW does not aim to support many of the features of live recording that exist in most DAWs, but to instead create an efficient and practical workflow for composers who either compose with notation software or directly into the piano roll.

Excuse the terrible name, it is temporary until I can think of something better.
MidiInter is nearly "basic feature" complete and merely needs project saving/loading to reach that point. Once there, it will be fully capable of creating music.

![midiinter_2018-08-04_14-06-05](https://user-images.githubusercontent.com/1783601/43680436-a784b32c-97ef-11e8-959e-e2b187a42ee4.png)




### Current features
* Complete Piano roll functionality
* Velocity editing
* CC recording and editing
* Load midi files
* VST2 instrument playback
* VST2 effect chaining (reverb, compression, etc)
* Midi device recording
* Export audio was .WAV file



### Using MidiInter
You must first build Portaudio debug and release versions and put them in ./Debug and ./Release folders of your working dir.
see http://portaudio.com/docs/v19-doxydocs/compile_windows.html.
It should hopefully work after that.

Some short-cuts used to navigate:
* Alt - swich between piano roll and plugin selector views.
* B   - Toggle draw mode in the Control Change view.
* C   - switch between piano roll and Control Change View.
