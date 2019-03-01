#include "audiomanager.h"
#include "src/mainwindow.h"
#include "src/vst2audioplugin.h"
#include "src/common.h"
#include "sdk/portaudio.h"
AudioManager::AudioManager(MasterTrack *mTrack) : masterTrack(mTrack)
{

}

int AudioManager::exportAudio(QString filePath)
{

    initializeIO();
    initializePlugins();

    while (beginExporting())
    {
    }
    num_samples = samples.size();
    qDebug() << "Finished plugin processing";
    writeHeader(filePath);
    endExporting();
    return 1;
}

void AudioManager::initializePlugins()
{
    for (int i = 0; i < masterTrack->midiTracks().size(); ++i)
    {
        auto track = masterTrack->midiTracks().at(i).get();
        auto plugin = track->plugin();
        if (plugin->canProcess() && !track->muted())
        {
            plugin->exportAudioInit();
        }
    }
}
// when isPlaying returns 0, that means all plugins have finished processing audio
// Slight bug in that last bit of audio is not saved
int AudioManager::beginExporting()
{
    int isPlaying = 0;
    for (int i = 0; i < masterTrack->midiTracks().size(); ++i)
    {
        auto track = masterTrack->midiTracks().at(i).get();
        auto plugin = track->plugin();
        if (plugin->canProcess() && !track->muted())
        {
            isPlaying +=  plugin->exportAudioBegin(output,g_blocksize);
            for (int j = 0; j < g_blocksize; ++j)
            {
                output_storage[0][j] += output[0][j] * g_volume;
                output_storage[1][j] += output[1][j] * g_volume;
            }
            AudioEngine::silenceChannel(output,num_outputs,g_blocksize);
        }

    }
    for (int j = 0; j < g_blocksize; ++j)
    {
        samples.push_back((int)(output_storage[0][j] * 8388608.0 )  & 0xffffff00);
        samples.push_back((int)(output_storage[1][j] * 8388608.0 )  & 0xffffff00);
    }
    AudioEngine::silenceChannel(output_storage,num_outputs,g_blocksize);
    return isPlaying;
}

void AudioManager::endExporting()
{
    for (int i = 0; i < num_outputs; ++i)
    {
        free(output_storage[i]);
        free(output[i]);
    }
    for (int i = 0; i < masterTrack->midiTracks().size(); ++i)
    {
        auto plugin = masterTrack->midiTracks().at(i)->plugin();
        plugin->exportAudioEnd();
    }
    free(output_storage);
    free(output);
}
//https://stackoverflow.com/questions/31024177/export-buffer-to-wav-in-c
void AudioManager::writeHeader(QString filePath)
{
    std::ofstream stream;
    stream.open(filePath.toLocal8Bit().data(), std::ios::binary);
    int32_t value;

    stream.write("RIFF", 4);                    // RIFF chunk
    value = 36 + (num_samples * num_channels * bits_per_sample / 8);
    stream.write((char*)&value,4);               // RIFF chunk size in bytes
    stream.write("WAVE", 4);                    // WAVE chunk
    stream.write("fmt ", 4);                    // fmt chunk
    value = 16;
    stream.write((char*)&value, 4);                     // size of fmt chunk
    value = 1;
    stream.write((char*)&value, 2);                      // Format = PCM
    value = 2;
    stream.write((char*)&value, 2);                       // # of Channels
    value = g_sampleRate;
    stream.write((char*)&value, 4);               // Sample Rate
    value = g_sampleRate* num_channels * bits_per_sample / 8;
    stream.write((char*)&value, 4);      // Byte rate
    value = bits_per_sample / 8 * num_channels;
    stream.write((char*)&value, 2);               // Frame size
    value = bits_per_sample;
    stream.write((char*)&value, 2);                       // Bits per sample
    stream.write("data", 4);                   // data chunk
    value = num_samples * num_channels * bits_per_sample / 8;
    stream.write((char*)&value, 4);     // data chunk size in bytes

    for (int i = 0; i < num_samples; i++)
    {
        stream.write((char*)&samples.at(i),bits_per_sample/8);

    }
    stream.close();


}

void AudioManager::initializeIO() {

    output = (float**)malloc(sizeof(float**) * num_outputs);
    for(int channel = 0; channel < num_outputs; channel++)
    {

        output[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
    }

    output_storage = (float**)malloc(sizeof(float**) * num_outputs);
    for(int channel = 0; channel < num_outputs; channel++)
    {

        output_storage[channel] = (float*)malloc(sizeof(float*) * g_blocksize);
    }
}
