# MidiInter



Strauss is the beginning of what will hopefully one day be a Digital Audio Workstation that focuses
on efficient composer workflow through the integration of both notation software such as Musescore as well as traditional DAWS such as Abelton. This DAW does not aim to support many of the features of live recording that exist in most DAWs, but to instead create an efficient and practical workflow for composers who either compose with notation software or directly into the piano roll.

Strauss is  "basic feature" complete and is fully capable of producing music with.

![midiinter_2018-08-04_14-06-05](https://user-images.githubusercontent.com/1783601/43680436-a784b32c-97ef-11e8-959e-e2b187a42ee4.png)




### Current features
* Complete Piano roll functionality
* Velocity editing
* CC recording and editing
* Load midi files
* VST2 instrument playback
* VST2 effect chaining (reverb, compression, etc)
* Midi device recording
* Export audio as .WAV file
* saving and loading project


### Using Strauss
You must first build Portaudio debug and release versions and put them in ./Debug and ./Release folders of your working dir.
see http://portaudio.com/docs/v19-doxydocs/compile_windows.html.
Then build protobuf as a dynamic linked library and put the produced protobuf.lib files in the respective ./Release and ./Debug folders.
(Build using MSVC2017)
Next get the VST2 SDK - download the VST3 SDK and you will find the VST2 version in there. Copy the contents to a folder called
./SDK in your WD.

Some short-cuts used to navigate:
* Alt - swich between piano roll and plugin selector views.
* B   - Toggle draw mode in the Control Change view.
* C   - switch between piano roll and Control Change View.
